#include "request.hpp"

extern fd_set		R_SET; 
extern fd_set		W_SET;
extern int 			highsock;

void Request::split_resp(char *buffer)
{
	std::string s(buffer);
	int i = 0;
	//SCRIPT_NAME
	int n = s.find("'\n'");
	if (n != (int)std::string::npos)
	{
		n = n + 3;
		i = n;
		m_url.append(&buffer[n], strlen(buffer) - n);
	}
	else
	{
		m_url.append(buffer, strlen(buffer));
	}
}

int Request::forking()
{
	int pid, res, status;
	int pp[2];
	res = 0;
	if (getcwd(curr_dir, 200) == NULL)
		return (-1);
	dir_cgi = std::string(curr_dir) + "/";
	path = dir_cgi + m_url;
	_head_req.PATH_TRANSLATED = path;
	_head_req.PATH_INFO = _head_req.REQUEST_URI;
	_head_req.CONTENT_LENGTH = std::to_string(m_body.size());

	if (!(path_exists(path)))
		return 127;

	struct stat buf;
	int ret;
	if ((ret = stat(path.c_str(), &buf)) < 0)
		std::cout << ERR_STAT << strerror(errno) << std::endl;
	else
	{
		const char chars[] = "rwxrwxrwx";
		char mode[10];
  		for (size_t i = 0; i < 9; i++)
    		mode[i] = (buf.st_mode & (1 << (8-i))) ? chars[i] : '-';
		mode[9] = '\0';
  		//std::cout << "mode: " << mode << std::endl;
		if (mode[2] != 'x' || mode[5] != 'x' || mode[8] != 'x')
			return (127);
		free(mode);
		//free(chars);
	}
	std::string _headers = _head_req.get_meta();
	char **env = ft_split(_headers.c_str(), '&');
	if (pipe(pp))
		perror(PIPE_ERR);
	pid = fork();
	char **argv = NULL;
	std::string cgi_output(dir_cgi);
	cgi_output = cgi_output + OUTPUT_CGI + std::to_string(m_client);
	int fd;
	if ((fd = open(cgi_output.c_str(), O_RDWR | O_CREAT, 0666)) < 0)
		std::cout << strerror(errno) << std::endl;
	if (pid == 0)
	{
		close(pp[1]);
   		dup2(pp[0], 0);
		dup2(fd, 1);
		res = execve(path.c_str(), argv, env);
		if (res != 0)
		{
			exit(127);
			return 0;
		}
		return 2;
	}
	else if (pid > 0)
	{
		close(pp[0]);
		write(pp[1], m_body.c_str(), m_body.size());
		if (waitpid(pid, &status, 0) == -1)
			perror(WAIT_ERR);
		close(pp[1]);
		close(fd);
	}
	else
		perror(FORK_ERR);
	std::ifstream f_cgi(cgi_output);
	std::string str_cgi((std::istreambuf_iterator<char>(f_cgi)), std::istreambuf_iterator<char>());
	std::string code = _head_req.getStringtoParse(str_cgi.c_str(), STATUS_STR);
	m_errorCode = std::stoi(code);
	_head_resp.CONTENT_TYPE = _head_req.getStringtoParse(str_cgi.c_str(), CONTENT_T_STR);
	int n = str_cgi.find(ENDCHARS); //peut etre rajouter un \n
	if (n != (int)std::string::npos)
	{
    	n = n + std::string(ENDCHARS).size();
		m_body.clear();
		m_body = str_cgi.substr(n, str_cgi.size() - n);
	}
	f_cgi.close();
	unlink(cgi_output.c_str());
	int k = 0;
	while (env[k])
	{
		free(env[k]);
		k++;
	}
	free(env);
	//free(dir_cgi);
	//dir_cgi = NULL;
	//free(path);
	//path = NULL;
	_status = SEND; // A SUPPRIMER QUAND REGLE
	return 0;
}

void Request::exec_cgi(){

	//std::cout << _body_size << std::endl;
	//std::cout << _loc._body_size << std::endl;
	if (_body_size > _loc._body_size)
	{
		m_errorCode = 413;
		_status = SEND;
		return;
	}
	/*if (_head_req.REQUEST_METHOD == POST)
	{
		post(); // on recupere les infos dans le body
	}*/
	m_url = _loc._cgi_root + _loc._cgi_file;
	_head_req.SERVER_NAME = _conf._server_name;
	_head_req.SCRIPT_NAME = _loc._cgi_file;
	pid_ret = forking();
	sleep(5);
}

void Request::post() {
	//std::cout << _body_size << std::endl;
	//std::cout << _loc._body_size << std::endl;
	//std::cout << "----" << std::endl;
	if (_body_size > _loc._body_size)
	{
		m_errorCode = 413;
		_status = SEND;
		return;
	}
	/*if (is_cgi)
		return;*/
	if (m_body.size() == 0)
		m_errorCode = 200;
	else
		m_errorCode = 201;
	
	m_path = POST_HTML;
	file_fd = open(m_path.c_str(), O_RDONLY);
	setFileToSet(true);
	_status = READ_FILE;
}

void Request::put() {

	if (_body_size > _loc._body_size)
	{
		m_errorCode = 413;
		_status = SEND;
		return;
	}
	m_path = _loc._uploaded_files_root + m_url;
	if (path_exists(m_path))
		m_errorCode = 200;
	else
		m_errorCode = 201; //created

	file_fd = open(m_path.c_str(), O_WRONLY);
	setFileToSet(true);
	_status = WRITE_FILE;
}

void Request::delete_m()
{
	if (path_exists(m_path))
		m_errorCode = 200;
	else
		m_errorCode = 204;
	unlink(m_path.c_str());
	_status = SEND;
}

void Request::get() {
	// Open the document in the local file system

	file_fd = open(m_path.c_str(), O_RDONLY);
	//std::cout << file_fd << std::endl;
	struct stat buf;
	fstat(file_fd, &buf);
	close(file_fd);
	if (buf.st_mode & S_IFDIR)
		m_path = m_path + "/" + m_index;
	file_fd = open(m_path.c_str(), O_RDONLY);
	if (path_exists(m_path))
	{
		//m_url = "";
		setFileToSet(true);
		
		//std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
		//std::cout << str << std::endl;
		//split_resp((char *)str.c_str());
		m_errorCode = 200;
		_status = READ_FILE;
		return;
	}
	else
	{
		notFound();
	}
}

int		Request::read_file() 
{
	int					ret = 0;
	char				buf[4096];
	std::string			result;
	while ((ret = read(file_fd, buf, 4095)) > 0)
	{
		buf[ret] = '\0';
		result += buf;
	}
	//std::cout << m_url << std::endl;
	split_resp((char *)result.c_str());
	close(file_fd);
	setFileToSet(false);
	_status = SEND;
	//std::cout << ret << std::endl;
	return ret;
}

int		Request::write_file() {
	int ret = 0;

	if (_head_req.REQUEST_METHOD == PUT) 
		ret = write(file_fd, m_body.c_str(), _body_size - 1); //get msg from body, limit if above content-lenght

	close(file_fd);
	setFileToSet(false);
	_status = SEND;
	return ret;
}

void	Request::setFileToSet(bool state)
{
	if (file_fd != -1)
	{
		if (state)
		{
			FD_SET(file_fd, &R_SET);
			FD_SET(file_fd, &W_SET);
			if (file_fd > highsock)
				highsock = file_fd;
			//_status = READ_FILE;
		}
		else
		{
			FD_CLR(file_fd, &R_SET);
			FD_CLR(file_fd, &W_SET);
			close(file_fd);
			if (file_fd == highsock) {
				while (!(FD_ISSET(highsock, &R_SET)) && !(FD_ISSET(highsock, &W_SET))) 
					highsock -= 1;
			}
		}
	}
}
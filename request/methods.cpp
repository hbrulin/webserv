#include "request.hpp"

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
		m_url.append(&buffer[n], ft_strlen(buffer) - n);
	}
	else
	{
		m_url.append(buffer, ft_strlen(buffer));
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
		{
			return (127);
		}
		//free(mode);
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
	return 0;
}

void Request::exec_cgi(){

	if (_body_size > _loc._body_size)
	{
		m_errorCode = 413;
		return;
	}
	m_url = _loc._cgi_root + _loc._cgi_file;
	_head_req.SERVER_NAME = _conf._server_name;
	_head_req.SCRIPT_NAME = _loc._cgi_file;
	pid_ret = forking();
	sleep(5);
}

void Request::post() {
	if (_body_size > _loc._body_size)
	{
		m_errorCode = 413;
		return;
	}

	if (m_body.size() == 0)
	{
		m_path = "www/post_0.html";
		m_errorCode = 200;
		if (read_file() == -1)
			internalError();
	}
	else
	{
		m_path = POST_HTML;
		m_errorCode = 201;
		if (read_file() == -1)
			internalError();
	}
}

void Request::put() {
	int fd;

	if (_body_size > _loc._body_size)
	{
		m_errorCode = 413;
		return;
	}
	m_path = _loc._uploaded_files_root + m_url;
	if (path_exists(m_path))
		m_errorCode = 200;
	else
	{
		m_errorCode = 201; //created
		_head_resp.LOCATION = m_path;
	}

	fd = open(m_path.c_str(), O_WRONLY | O_CREAT | O_TRUNC);
	if(write(fd, m_body.c_str(), _body_size) == -1)//get msg from body, limit if above content-lenght
		internalError();
	close(fd);
}

void Request::delete_m()
{
	if (path_exists(m_path))
		m_errorCode = 200;
	else
		m_errorCode = 204;
	unlink(m_path.c_str());
}

void Request::get() {

	int fd = open(m_path.c_str(), O_RDONLY);
	struct stat buf;
	fstat(fd, &buf);
	close(fd);
	if (buf.st_mode & S_IFDIR)
	{
	/*	std::string b = m_path + "/" + m_index;
		if (path_exists(b) && !_loc._index.empty())
			m_path = b;
		else if (_loc._autoindex == true)
			m_path = _loc.get_autoindex();
		else
			m_path = b;*/
		if (_loc._autoindex == true && _loc._index.empty())
			m_path = _loc.get_autoindex();
		else
			m_path = m_path + "/" + m_index;
	}

	if (path_exists(m_path))
	{
		m_errorCode = 200;
		if (read_file() == -1)
			internalError();
		return;
	}
	else
		notFound();
}


int		Request::read_file() 
{
	int 				fd = open(m_path.c_str(), O_RDONLY);
	int					ret = 0;
	char				buf[4096];
	m_url = "";
	while ((ret = read(fd, buf, 4095)) > 0)
	{
		buf[ret] = '\0';
		m_url += buf;
	}
	//std::cout << m_url << std::endl;
	//split_resp((char *)result.c_str());
	close(fd);
	//std::cout << ret << std::endl;
	return ret;
}
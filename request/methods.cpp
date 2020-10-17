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
	std::ostringstream oss;
	if (getcwd(curr_dir, 200) == NULL)
		return (-1);
	if ((dir_cgi = ft_strjoin(curr_dir, "/")) == NULL) // leak
		return (-1);
	if ((path = ft_strjoin(dir_cgi, m_url.c_str())) == NULL)
		return (-1);
	_head_req.PATH_TRANSLATED = path;
	_head_req.PATH_INFO = _head_req.REQUEST_URI;
	_head_req.CONTENT_LENGTH = std::to_string(m_body.size());
	std::ifstream f(path);
	if (!f.good())
	{
		return 127;
	}
	struct stat buf;
	int ret;
	if ((ret = stat((const char *)path, &buf)) < 0)
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
	}
	std::string _headers = _head_req.get_meta();
	char **env = ft_split(_headers.c_str(), '&');
	char **av;
	av = (char **)malloc(sizeof(char *) * 3);
	av[0] = ft_strdup(path);
	if (pipe(pp))
		perror(PIPE_ERR);
	pid = fork();
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
		res = execve(av[0], NULL, env);
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
		//int boucle = 1;
		// while (boucle)
		// {
		// 	int ret = wait(&status);
		// 	if (WIFSIGNALED(status))
		// 	{
		// 		if (WTERMSIG(status) == 2)
		// 			return -3;
		// 		else if (WTERMSIG(status) == 3)
		// 			return -2;
		// 	}
		// 	else
		// 		return WEXITSTATUS(status);
		// 	if (ret == pid)
		// 		boucle = 0;
		// }
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
	remove(cgi_output.c_str());
	return 0;
}

void Request::exec_cgi(){
	if (_head_req.REQUEST_METHOD == POST)
	{
		post(); // on recupere les infos dans le body
	}
	m_url = _loc._cgi_root + _loc._cgi_file;
	_head_req.SERVER_NAME = _conf._server_name;
	_head_req.SCRIPT_NAME = _loc._cgi_file;
	pid_ret = forking();
	

}

void Request::post() {
	if (_head_req.CONTENT_LENGTH == "" && _head_req.TRANSFER_ENCODING == "")
	{
			m_errorCode = 411;
			return;
	}
	if (is_cgi)
		return;
	if (m_body.size() == 0)
	{
		std::ifstream f(POST_HTML);
		std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
		m_path = POST_HTML;
		m_url = str;
		m_errorCode = 200;
		f.close();
	}
	else if (m_body.size() > _loc._body_size)
	{
		std::ifstream f(POST_HTML);
		std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
		m_path = POST_HTML;
		m_url = str;
		m_errorCode = 413;
		f.close();
	}
	else
	{
		std::ifstream f(POST_HTML);
		std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
		m_path = POST_HTML;
		m_url = str;
		m_errorCode = 201;
		f.close();
	}
}

void Request::put() {
	/*std::cout << m_body.size() << std::endl;
	std::cout << m_chunk_size << std::endl << std::endl;*/
	unsigned int n;
	if (_head_req.TRANSFER_ENCODING == CHUNKED_STR)
		n = m_chunk_size;
	else if (_head_req.CONTENT_LENGTH.empty() == 0)
		n = (unsigned int)stoi(_head_req.CONTENT_LENGTH);
	else 
	{
		m_errorCode = 411;
		n = 0;
		return;
	}
	//std::cout << n << "!!!!!!\n";
	if ((_head_req.CONTENT_LENGTH.empty() == 0 && (unsigned int)stoi(_head_req.CONTENT_LENGTH)) > _loc._body_size)
	{
		m_errorCode = 413;
		return;
	}
	m_path = _loc._uploaded_files_root + m_url;
	std::ifstream f(m_path);
	if (f.good())
		m_errorCode = 200;
	else
		m_errorCode = 201; //created
	f.close();
	//std::cout << _loc._uploaded_files_root << "\n";
	std::ofstream ff(m_path);
	if (ff.good())
		ff << m_body.substr(0, n) << std::endl; //get msg from body, limit if above content-lenght
	else
		m_errorCode = 456;
	ff.close();
}

void Request::delete_m()
{
	//m_path = _conf._root + m_url;
	std::ifstream f(m_path);
	if (f.good())
		m_errorCode = 200;
	else
		m_errorCode = 204; //created
	f.close();
	unlink(m_path.c_str());
}

void Request::get() {
	// Open the document in the local file system

	int fd = open(m_path.c_str(), O_RDONLY);
	struct stat buf;
	fstat(fd, &buf);
	close(fd);
	if (buf.st_mode & S_IFDIR)
		m_path = m_path + YOUPI_BAD;

	std::ifstream f(m_path);
	if (f.good())
	{
		m_url = "";
		std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
		split_resp((char *)str.c_str());
		m_errorCode = 200;
		f.close();
		return;
		/*if (!isAllowed(m_path) || m_errorCode == 405)
		{
			f.close();
			if (_loc._root.find("fr") != std::string::npos || _loc._root.find("en") != std::string::npos || _loc._root.find("es") != std::string::npos || _loc._root.find("de") != std::string::npos)
				_loc._root = _loc._root.substr(0, _loc._root.size() - 3);
			m_path = _loc._root + ERROR_FOLDER + NOT_ALLOWED;
			std::ifstream f(m_path);
			std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
			m_url = str;
			m_errorCode = 405;
			f.close();
		}*/
		/*else if (!isAuthorized(m_header))
		{
			f.close();
			std::ifstream f(_loc._root + m_unauthorized);
			std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
			m_path = _loc._root + m_unauthorized;
			m_url = str;
			m_errorCode = 401;
			f.close();
		}
		//else
		{
			//std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
			//m_url = str;
			m_errorCode = 200;
			f.close();
			return;
		}*/
	}
	else
	{
		f.close();
		notFound();
	}
}

#include "request.hpp"

void Request::split_resp(char *buffer)
{
	std::string s(buffer);
	int i = 0;
	//SCRIPT_NAME
	int n = s.find("'\n'");
	if (n != (int)std::string::npos)
	{
		m_header.append(buffer, n);
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
	_head_req.PATH_INFO = path;
	std::cout << "path cgi: " << path << std::endl;
	std::ifstream f(path);
	if (!f.good())
	{
		return 127;
	}
	struct stat buf;
	int ret;
	if ((ret = stat((const char *)path, &buf)) < 0)
		std::cout << "erreur stat" << strerror(errno) << std::endl;
	else 
	{
		const char chars[] = "rwxrwxrwx";
		char mode[10];
  		for (size_t i = 0; i < 9; i++) 
		{
    		mode[i] = (buf.st_mode & (1 << (8-i))) ? chars[i] : '-';
		}
		mode[9] = '\0';
  		//std::cout << "mode: " << mode << std::endl;
		if (mode[2] != 'x' || mode[5] != 'x' || mode[8] != 'x')
		{
			return (127);
		}
	}

	
	std::string _headers = _head_req.get_meta();
	std::string s_env = content_env.append(_headers);
	std::cout << s_env << std::endl;
	char **env = ft_split(s_env.c_str(), '&');
	if (pipe(pp))
		perror("pipe");
	pid = fork();
	if (pid == 0)
	{
		close(pp[1]);
   		dup2(pp[0], 0);
		dup2(m_client, 1);
		std::cout << _head_resp.getBuffer(200, path, _loc._methods); // A revoir car renvoie mauvais header si ne fonctionne pas
		res = execve(path, NULL, env);
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
		int boucle = 1;
		while (boucle)
		{
			int ret = wait(&status);
			if (WIFSIGNALED(status))
			{
				if (WTERMSIG(status) == 2)
					return -3;
				else if (WTERMSIG(status) == 3)
					return -2;
			}
			else
				return WEXITSTATUS(status);
			if (ret == pid)
				boucle = 0;
		}
	}
	else
	{
		perror("fork");
	}
	return 0;
}
void Request::get_post_content()
{
	int n;
	std::string s(m_buffer);
	n = s.find("\r\n\r"); //peut etre rajouter un \n
	if (n != (int)std::string::npos)
	{
    	n = n + std::string("\r\n\r\n").size();
		content_env = s.substr(n, s.size() - n);
	}
	else
	{
		n = s.find("\n\n");
		if (n != (int)std::string::npos)
		{
        	n = n + std::string("\n\n").size();
			content_env = s.substr(n, s.size() - n);
		}
	}
	m_output = "";
}


void Request::exec_cgi(){
	//_loc._cgi_file = "test.php";
	if (strstr(m_buffer, "POST") != NULL)
	{
		post(); // on recupere les infos dans le body
	}
	m_url = _loc._cgi_root + _loc._cgi_file;
	//std::cout << "cgi url" << m_url << std::endl;
	_head_req.CONTENT_LENGTH = std::to_string(content_env.size());
	_head_req.SERVER_NAME = _conf._server_name;
	_head_req.SCRIPT_NAME = _loc._cgi_file;
	pid_ret = forking();
	//std::cout << "pid ret" << pid_ret << std::endl;
}

void Request::post() {
	if (m_body.empty())
	{
		m_errorCode = 405;
		m_url = "";
		return;
	}
	if (_head_req.CONTENT_LENGTH == "" && _head_req.TRANSFER_ENCODING == NULL)
	{
			std::ifstream f(_loc._root + m_length_required);
			std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
			m_path = _loc._root + m_length_required;
			m_url = str;
			m_errorCode = 411;
			return;
	}
	get_post_content();
	//std::ifstream f(_loc._root + "post.html");
	//std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
	//m_path = _loc._root + "post.html";
	//m_url = str;
			//m_errorCode = 400;
	//f.close();
}

void Request::put() {
	//_conf._body_size = 20; //JUSTE POUR TESTER - EN ATTENTE FIX
	
	if (_head_req.CONTENT_LENGTH.empty() && _head_req.TRANSFER_ENCODING == NULL)
	{
			std::ifstream f(_loc._root + m_length_required);
			std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
			m_path = _loc._root + m_length_required;
			m_url = str;
			m_errorCode = 411;
			return;
	}
	else if (((unsigned int)stoi(_head_req.CONTENT_LENGTH)) > _loc._body_size)
	{
		m_errorCode = 413;
		//std::cout << _head_req.CONTENT_LENGTH << "\n";
		//std::cout << _conf._body_size << "\n";
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
	int n = stoi(_head_req.CONTENT_LENGTH);
	std::ofstream ff(m_path);
	if (ff.good())
		ff << m_body.substr(0, n) << std::endl; //get msg from body, limit if above content-lenght
	else
		m_errorCode = 456;
	ff.close();
}

/*void Request::patch()
{
	//open file and check existance
	//check_content
	std::string content = m_url.substr(m_url.find('{'), m_url.find('}'));
	while ()
}*/

void Request::delete_m()
{
	/* a tester sur nginx pour voir les erreurs et tout*/
	//m_path = _conf._root + m_url;
	std::ifstream f(m_path);
	if (f.good())
		m_errorCode = 200;
	else
		m_errorCode = 204; //created
	f.close();
	unlink(m_path.c_str());
	/*
	DELETE /echo/delete/json HTTP/1.1
	Authorization: Bearer mt0dgHmLJMVQhvjpNXDyA83vA_PxH23Y
	Accept: application/json
	Content-Type: application/json
	Content-Length: 19
	Host: reqbin.com
	*/
	/*
	HTTP/1.1 200 OK
	Content-Length: 19
	Content-Type: application/json

	{"success":"true"}
	*/
}

void Request::get() {
	/*if (m_url == m_index)
		m_path = _head_req.contentNego (_conf._root, m_url);
	else
	{
		m_path = _conf._root + m_url;
	}*/

	/*if (m_path == "not_acceptable")
	{
			std::ifstream f(_conf._root + m_not_acceptable);
			std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
			m_path = _conf._root + m_not_acceptable;
			m_url = str;
			m_errorCode = 406;
			return;
	}*/

	// Open the document in the local file system
	std::ifstream f(m_path);

	// Check if it opened and if it did, take content
	if (f.good())
	{
		std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
		split_resp((char *)str.c_str());
		m_header = m_url;
		if (m_errorCode == 400)
		{
			f.close();
			std::ifstream f(_loc._root + m_bad_request);
			std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
			m_path = _loc._root + m_bad_request;
			m_url = str;
			//m_errorCode = 400;
			f.close();
		}
//		else if (_head_req.SERVER_PROTOCOL != "HTTP/1.1" && m_errorCode != 405)
//		{
//			f.close();
//			std::ifstream f(_conf._root + m_not_supported);
//			std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
//			m_path = _conf._root + m_not_supported;
//			m_url = str;
//			m_errorCode = 505;
//			f.close();
//		}
		else if (!isAllowed(m_path) || m_errorCode == 405)
		{
			f.close();
			std::ifstream f(_loc._root + m_not_allowed);
			std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
			m_path = _loc._root + m_not_allowed;
			m_url = str;
			m_errorCode = 405;
			f.close();
		}
		else if (!isAuthorized(m_header))
		{
			f.close();
			std::ifstream f(_loc._root + m_unauthorized);
			std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
			m_path = _loc._root + m_unauthorized;
			m_url = str;
			m_errorCode = 401;
			f.close();
		}
		else
		{
			//std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
			//m_url = str;
			m_errorCode = 200;
			f.close();
		}
	}
	else
	{
			
		f.close();
		std::cout << "loc root" << _loc._root << std::endl;
		std::ifstream f("www/404.html");
		std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
		m_url = str;
		m_errorCode = 404;
		std::cout << "url" << m_url << std::endl;
		//error code par défaut à 404, à revoir pour autres erreurs, genre manque de header par exemple
		f.close();
	}
}

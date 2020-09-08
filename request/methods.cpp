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
	if ((dir_cgi = ft_strjoin(dir_cgi, _conf._cgi_root.c_str())) == NULL)
		return (-1);
	if ((path = ft_strjoin(dir_cgi, m_url.c_str())) == NULL)
		return (-1);
	_head_req.PATH_TRANSLATED = path;
	std::cout << "path: " << path << std::endl;
	std::string _headers = _head_req.get_meta(_conf);
	//if (content_env != NULL)
	std::string s_env = content_env.append(_headers);
	//std::cout << s_env << std::endl;
	char **env = ft_split(s_env.c_str(), '&');
	if (pipe(pp))
		perror("pipe");
	pid = fork();
	if (pid == 0)
	{
		close(pp[1]);
   		dup2(pp[0], 0);
		dup2(m_client, 1);
		std::cout << _head_resp.getBuffer(200, path, _conf._methods);
		res = execve(path, NULL, env);
		if (res != 0)
		{
			m_output = "HTTP/1.1 400 Bad Request\r\n"
        	"Content-length: 97\r\n"
        	"Content-Type: text/html\r\n\r\n"
        	"<!doctype html><html><head><title>CGI Error</title></head><body><h1>CGI Error.</h1></body></html>\r\n";
			send(m_client, m_output.c_str(), m_output.size() + 1, 0);
			exit(127);
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
					return -1;
				else if (WTERMSIG(status) == 3)
					return -1;
			}
			else
				return -1;
			if (ret == pid)
				boucle = 0;
		}
	}
	else
	{
		perror("fork");
	}
	return res;
}

void Request::post() {
	if (_head_req.CONTENT_LENGTH == "" && _head_req.TRANSFER_ENCODING == NULL)
	{
			std::ifstream f(_conf._root + m_length_required);
			std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
			m_path = _conf._root + m_length_required;
			m_url = str;
			m_errorCode = 411;
			return;
	}
	int n;
		std::string s(m_buffer);
    	//std::string referer;
		n = s.find("\r\n\r");
		if (n != (int)std::string::npos)
		{
        	n = n + std::string("\r\n\r\n").size();
		//	std::cout << "size" << s.size() << std::endl;
		//	std::cout << "n" << n << std::endl;
			content_env = s.substr(n, s.size() - n);
		//	std::cout << "content env" << content_env << std::endl;
			_head_req.CONTENT_LENGTH = std::to_string(content_env.size());
		}
		m_output = "";
		//std::cout << "before forking" << std::endl;
		forking();
}

void Request::put() {
	//_conf._body_size = 20; //JUSTE POUR TESTER - EN ATTENTE FIX
	
	if (_head_req.CONTENT_LENGTH.empty() && _head_req.TRANSFER_ENCODING == NULL)
	{
			std::ifstream f(_conf._root + m_length_required);
			std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
			m_path = _conf._root + m_length_required;
			m_url = str;
			m_errorCode = 411;
			return;
	}
	else if (stoi(_head_req.CONTENT_LENGTH) > _conf._body_size)
	{
		m_errorCode = 413;
		std::cout << _head_req.CONTENT_LENGTH << "\n";
		std::cout << _conf._body_size << "\n";
		return;
	}
	std::ifstream f(_loc._uploaded_files_root + m_url);
	if (f.good())
		m_errorCode = 200;
	else
		m_errorCode = 201; //created
	f.close();
	std::cout << _loc._uploaded_files_root << "\n";
	int n = stoi(_head_req.CONTENT_LENGTH);
	std::ofstream ff(_loc._uploaded_files_root + m_url);
	if (ff.good())
		ff << m_body.substr(0, n) << std::endl; //get msg from body, limit if above content-lenght
	else
		std::cout << "error" << std::endl;
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
			std::ifstream f(_conf._root + m_bad_request);
			std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
			m_path = _conf._root + m_bad_request;
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
			std::ifstream f(_conf._root + m_not_allowed);
			std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
			m_path = _conf._root + m_not_allowed;
			m_url = str;
			m_errorCode = 405;
			f.close();
		}
		else if (!isAuthorized(m_header))
		{
			f.close();
			std::ifstream f(_conf._root + m_unauthorized);
			std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
			m_path = _conf._root + m_unauthorized;
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
		std::ifstream f(_conf._root + m_not_found);
		std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
		m_url = str;
		//error code par défaut à 404, à revoir pour autres erreurs, genre manque de header par exemple
		f.close();
	}
}

#include "request.hpp"
#include <iostream>
#include <unistd.h>



int Request::isAcceptable()
{
	//Check Langage
	int i = 0;
	while (_head_req.ACCEPT_LANGUAGE[i] != NULL)
	{
		if (strstr(_head_req.ACCEPT_LANGUAGE[i], _head_resp.CONTENT_LANGUAGE.c_str()) != NULL)
			return 1;
		i++;
	}
	return 0;
}

void Request::parse() {
	
	/*parse word by word*/
	std::istringstream iss(m_buffer);
	std::vector<std::string> parsed((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());

	//NEED TO PARSE HEADERS TOO?

	// If the GET request is valid, try and get the name
	if (!(parsed[0] == "GET" || parsed[0] == "POST" || parsed[0] == "HEAD" || parsed[0] == "TRACE" || parsed[0] == "PATCH" || parsed[0] == "DELETE" || parsed[0] == "OPTION" || parsed[0] == "CONNECT" || parsed[0] == "PUT"))
	{
		m_errorCode = 400;
	}
	if (parsed.size() >= 3 && (parsed[0] == "GET" || parsed[0] == "POST" || parsed[0] == "HEAD"))
	{
		m_content = parsed[1];
		_head_req.REQUEST_METHOD = parsed[0];
		_head_req.SERVER_PROTOCOL = parsed[2];
		char **tab = ft_split(_head_req.getStringtoParse(m_buffer, "Authorization: ").c_str(), ' ');
		if (tab != NULL && tab[0] != NULL)
				_head_req.AUTH_TYPE = tab[0];
		_head_req.CONTENT_TYPE = _head_req.getStringtoParse(m_buffer, "Content-Type: ");
		_head_req.QUERY_STRING = _head_req.getMetatoParse((char *)m_content.c_str(), "?", (char *)" #");
		_head_req.getScriptName((char *)m_content.c_str());
		_head_req.SERVER_NAME = _head_req.getMetatoParse((char *)m_content.c_str(), "://", ":/?#");
		if (_head_req.getMetatoParse((char*)m_content.c_str(), _head_req.SERVER_NAME + ":", "?/#") != "")
			_head_req.SERVER_PORT = _head_req.getMetatoParse((char*)m_content.c_str(), _head_req.SERVER_NAME + ":", "?/#") != "";
		//_head_req.SERVER_PROTOCOL = _head_req.getMetatoParse(m_content, "", "://");
		if (m_content == "/") //GET / HTTP/1.1
		{
			m_content = m_index;
		}
		//std::cout << m_content << std::endl;
	}
	// HEADERS
	_head_req.REFERER = _head_req.getReferer(m_buffer);
	_head_req.USER_AGENT = _head_req.getUserAgent(m_buffer); 
	if (_head_req.getStringtoParse(m_buffer, "Accept-Language: ") != "")
		_head_req.ACCEPT_LANGUAGE = ft_split(_head_req.getStringtoParse(m_buffer, "Accept-Language: ").c_str(), ',');
	if (_head_req.getStringtoParse(m_buffer, "Accept-Charset: ") != "")
		_head_req.ACCEPT_CHARSET = ft_split(_head_req.getStringtoParse(m_buffer, "Accept-Charset: ").c_str(), ',');
	if (_head_req.getStringtoParse(m_buffer, "Transfer-Encoding: ") != "")
		_head_req.TRANSFER_ENCODING = ft_split(_head_req.getStringtoParse(m_buffer, "Transfer-Encoding: ").c_str(), ',');
	_head_req.DATE = _head_req.getStringtoParse(m_buffer, "Date: ");
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
	if ((path = ft_strjoin(dir_cgi, m_content.c_str())) == NULL)
		return (-1);
	_head_req.PATH_TRANSLATED = path;
	std::string _headers = _head_req.get_meta(_conf);
	std::string s_env = _headers.append(content_env);
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

void Request::handle() {
	//How do we determine the request isn’t for a document it can simply deliver, and create a CGI process?
	
	//IF NOT ACCEPTABLE

	// Open the document in the local file system
	std::ifstream f(_conf._root + m_content); 
	std::string path = "www/" + m_content;
	if (strstr(m_buffer, "POST") != NULL) // .cgi != NULL
	{
		for (int i = 0; i < (int)strlen(m_buffer); i++)
		{
			if (m_buffer[i] == '\r' && m_buffer[i - 1] == '\n' && m_buffer[i - 2] == '\r')
			{
				i = i + 2;
				char *tmp= new char[strlen(m_buffer) - i];
				memset(tmp, sizeof(tmp), 0);
				for (int j = 0; j < (int)strlen(m_buffer) - i; j++)
				{
					if (ft_isprint(m_buffer[i + j]))
						tmp[j] = m_buffer[i + j];
					else
						break;	
				}
				memset(content_env, sizeof(content_env), 0);
				content_env = tmp;
				_head_req.CONTENT_LENGTH = std::to_string(ft_strlen(content_env));
				break;
			}
		}
		m_output = "";
		forking();
		f.close();
		return ;
	}
	// Check if it opened and if it did, take content
	if (f.good())
	{
		if (m_errorCode == 400)
		{
			f.close();
			std::ifstream f(_conf._root + m_bad_request); 
			std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
			m_content = str;
			m_errorCode = 400;
			f.close();
		}
		else if (!isAcceptable())
		{
			f.close();
			std::ifstream f(_conf._root + m_not_acceptable); 
			std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
			m_content = str;
			m_errorCode = 406;
			f.close();
		}
		else 
		{
			std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
			m_content = str;
			m_errorCode = 200;
			f.close();
		}
	}
	else //DEFINIR COMMENT GERER ERREURS ICI
	{
		f.close();
		std::ifstream f(_conf._root + m_not_found);  
		std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
		m_content = str;
		//error code par défaut à 404, à revoir pour autres erreurs, genre manque de header par exemple
		f.close();
	}
	// Write the document back to the client, with HEADERS - define how to deal with them
	std::ostringstream oss;
	oss << _head_resp.getBuffer(m_errorCode, path.c_str(), _conf._methods);
	if (_head_req.REQUEST_METHOD != "HEAD")
		oss << m_content;
	m_output = oss.str();
}


int Request::send_to_client() {
	if (send(m_client, m_output.c_str(), m_output.size() + 1, 0) <= 0)
		return - 1;
	return 0;
}
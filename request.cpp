#include "request.hpp"
#include <iostream>
#include <unistd.h>
#include "libft.h"


void Request::parse() {
	
	/*parse word by word*/
	std::istringstream iss(m_buffer);
	std::vector<std::string> parsed((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());

	//NEED TO PARSE HEADERS TOO?

	// If the GET request is valid, try and get the name
	if (parsed.size() >= 3 && parsed[0] == "GET")
	{
		m_content = parsed[1];

		if (m_content == "/") //GET / HTTP/1.1
		{
			m_content = m_index;
		}
		//std::cout << m_content << std::endl;
	}

}

int Request::forking()
{
	int pid, res, status;
	int pp[2];
	std::ostringstream oss;
	char *env[] = {content_env, NULL};
	if (pipe(pp))
		perror("pipe");
	pid = fork();
	if (pid == 0)
	{
		close(pp[1]);
   		dup2(pp[0], 0);
		dup2(m_client, 1);
   		//res = execve("usr/bin/php", argv, env);
		//char *env[] = {(char *)m_content.c_str(), NULL};
		res = execve("/Users/Othilie/webserver/webserv/cgi-bin/test.cgi", NULL, env);
		if (res != 0)
		{
			m_output = "HTTP/1.1 200 OK\r\n"
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
	
	// Open the document in the local file system
	std::ifstream f("www" + m_content); //DEFINI PAR PATH, META_VARIABLE?
	if (strstr(m_buffer, "POST") != NULL) // .cgi != NULL
	{
		std::stringstream iss;
		std::string s(m_buffer);
		iss << m_buffer;
		for (int i = 0; i < strlen(m_buffer); i++)
		{
			if (m_buffer[i] == '\r' && m_buffer[i - 1] == '\n')
			{
				i = i + 2;
				char *tmp= new char[strlen(m_buffer) - i];
				for (int j = 0; j < strlen(m_buffer) - i; j++)
				{
					tmp[j] = m_buffer[i + j];
				}
				content_env = tmp;
				break;
			}
		}
		//std::string str((std::istreambuf_iterator<char>(iss), std::istreambuf_iterator<char>());
		//m_content = ;
		m_output = "";
		forking();
		f.close();
		return ;
	}
	// Check if it opened and if it did, take content
	if (f.good())
	{
		std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
		m_content = str;
		m_errorCode = 200;
		f.close();
	}
	else //DEFINIR COMMENT GERER ERREURS ICI
	{
		f.close();
		std::ifstream f("www" + m_not_found);  
		std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
		m_content = str;
		//error code par défaut à 404, à revoir pour autres erreurs, genre manque de header par exemple
		f.close();
	}

	// Write the document back to the client, with HEADERS - define how to deal with them
	std::ostringstream oss;
	oss << "HTTP/1.1 " << m_errorCode << " OK\r\n";
	oss << "Cache-Control: no-cache, private\r\n";
	oss << "Content-Type: text/html\r\n";
	oss << "Content-Length: " << m_content.size() << "\r\n";
	oss << "\r\n";
	oss << m_content;

	m_output = oss.str();
}


void Request::send_to_client() {
	send(m_client, m_output.c_str(), m_output.size() + 1, 0);
}
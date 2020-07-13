#ifndef REQUEST_HPP
# define REQUEST_HPP

#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <sstream>
#include <fstream>
#include <vector>
#include "../parser/Config.hpp"
#include "../request/libft.h"
#include "../headers/head_req.hpp"
#include "../headers/head_resp.hpp"
#include "../headers/meta_var.hpp"

/*Faire deux classes : une pour reception, une pour envoi?*/

class Request 
{
	private:
	Request(){};
	Config _conf;
	//HEADERS
	Head_req _head_req;
	Head_resp _head_resp;
	//meta_var _meta_var;
	char *m_buffer;
	char *content_env; //env que l'on recup dans requete POST pour les cgi
	int m_client;
	char curr_dir[200];
	char *dir_cgi;
	char *path;
	//GET
	//POST
	//FILES
	//META_DATA
	//std::string _server_name;

	/*default outputs - à intégrer dans structures ci-dessus?? 
	config selon file .config pour index?*/
	std::string m_content;
	std::string m_not_found;
	std::string m_not_acceptable;
	std::string m_index;
	int m_errorCode;
	//output
	std::string m_output; //peut-être pas nécessaire, réutiliser m_content?
	//memset m_content et m_output
	public:
	Request(char *buffer, int fd, Config conf, int port) 
	{
		_conf = conf;
		m_buffer = buffer;
		m_client = fd;
		m_not_found = "404.html";
		m_not_acceptable = "406.html";
		m_index = "index.html";
		m_errorCode = 404; //define other error codes
		_head_req.SERVER_PORT = std::to_string(port);
	};
	void parse();
	void handle();
	int send_to_client();
	int forking();
	int isAcceptable();
};


#endif
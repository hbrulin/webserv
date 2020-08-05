#ifndef REQUEST_HPP
# define REQUEST_HPP

#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <sstream>
#include <fstream>
#include <iostream>
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
	std::string content_env; //env que l'on recup dans requete POST pour les cgi
	int m_client;
	char curr_dir[200];
	char *dir_cgi;
	char *path;
	//int index_auth;
	//GET
	//POST
	//FILES
	//META_DATA
	//std::string _server_name;

	/*default outputs - à intégrer dans structures ci-dessus??
	config selon file .config pour index?*/
	std::string m_content;
	std::string m_header;
	std::string m_not_found;
	std::string m_not_acceptable;
	std::string m_not_allowed;
	std::string m_bad_request;
	std::string m_unauthorized;
	std::string m_not_supported;
	std::string m_index;
	std::string m_path;
	int m_errorCode;
	//output
	std::string m_output; //peut-être pas nécessaire, réutiliser m_content?
	//memset m_content et m_output
	public:
	Request(char *buffer, int fd, Config conf, int port)
	{
		_conf = conf;
		memset((char *) &m_buffer, 0, sizeof(m_buffer));
		m_buffer = buffer;
		m_client = fd;
		m_not_found = "404.html";
		m_not_allowed = "405.html";
		m_not_acceptable = "406.html";
		m_bad_request = "400.html";
		m_unauthorized = "401.html";
		m_not_supported = "505.html";
		m_index = "index.html";
		m_errorCode = 404; //define other error codes
		_head_req.SERVER_PORT = std::to_string(port);

	};
	void parse();
	void handle();
	int send_to_client();
	void post();
	void get();
	void put();
	void delete_m();
	int forking();
	//int isAcceptable();
	int isAuthorized(std::string str);
	void split_resp(char *buffer);
	int isAllowed(std::string path);
	bool check_if_method_is_allowed(std::string);
};


#endif

#ifndef REQUEST_HPP
# define REQUEST_HPP

#include <string>
//#include <sys/types.h>
#include <sys/socket.h>
//#include <sstream>
//#include <fstream>
//#include <iostream>
//#include <vector>
#include "../parser/Config.hpp"
#include "../parser/Location.hpp"
#include "../utils/libft.h"
#include "../headers/head_req.hpp"
#include "../headers/head_resp.hpp"
//#include <algorithm>
//#include <sys/stat.h>
#include <fcntl.h>
//#include <sys/types.h>
//#include <dirent.h>
#include "../utils/definitions.hpp"


class Request
{
	public:
	Request(){};
	Config _conf;
	Location _loc;
	//HEADERS
	Head_req _head_req;
	Head_resp _head_resp;
	//meta_var _meta_var;
	char *m_buffer;
	std::string content_env; //env que l'on recup dans requete POST pour les cgi
	int m_client;
	unsigned long s_addr;
	char curr_dir[200];
	char *dir_cgi;
	//std::string cgi_output;
	char *path;

	//std::string m_method;
	std::string m_url;
	std::string m_body;
	std::string m_headers;
	std::string m_header;
	std::string m_not_found;
	std::string m_index;
	std::string m_path;
	int m_errorCode;
	int pid_ret;
	bool is_cgi;
	unsigned int m_chunk_size;

	//output
	std::string m_output; 
	//memset m_content et m_output
	public:
	Request(std::string headers, std::string body, int fd, Config conf, int port, unsigned long addr);
	void parse();
	void getBody();
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
	void get_post_content();
	void get_query_string();
	void exec_cgi();
	
};


#endif

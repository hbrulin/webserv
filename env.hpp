#ifndef ENV_HPP
# define ENV_HPP

#include <string> 

class Env {

	protected:
		std::string AUTH_TYPE;
		std::string CONTENT_LENGTH;
		std::string CONTENT_TYPE;
		std::string GATEWAY_INTERFACE; 
		std::string PATH_INFO;
		std::string PATH_TRANSLATED; 
		std::string QUERY_STRING;
		std::string REMOTE_ADDR;
		std::string REMOTE_IDENT; 
		std::string REMOTE_USER;
		std::string REQUEST_METHOD; 
		std::string REQUEST_URI;
		std::string SCRIPT_NAME;
		std::string SERVER_NAME;
		std::string SERVER_PORT;
		std::string SERVER_PROTOCOL; 
		std::string SERVER_SOFTWARE;

	public:
		Env();
		//parse_config(int config_file); //fd du file
};


#endif
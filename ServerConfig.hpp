#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include <string>
#include <list>
#include <iostream>
#include <map>
#include "env.hpp"

#define ALPHACHAR "ABCEDFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789" // Degueulasse


/*
** this class should serve to parse a config struct listed in Data
** The architecture isn't definitive yet
*/

class ServerConfig
{
	/*
	** maybe a map of functions
	*/
	typedef void (ServerConfig::*Func)(std::string s);
   	//typedef std::map<std::string, FUNCPTR> FuncMap;
	typedef std::map<std::string, Func> fmap;
	//typedef bool (ServerConfig::*)(std::string) fparse;

	public:
	ServerConfig() ; // initialise with default

	bool setConfig(std::string s); // real constructor

	ServerConfig(const ServerConfig& serverConfig);
	void operator = (const ServerConfig& serverConfig);

	//Parse_method
	void initiate_map(fmap&);



	void print_data(); // utilitary, for testing only

	// getters:

	private:
	//paths
	/*
	** not sure if host and listen and stuff should be strings or int
	** Should i check if ips are valid here, or elsewhere?
	*/

	std::string _server_name;
	std::string _root; //-> root path of the server like www/
	std::string _errors; // ->root of default errors directory
	unsigned int _client_body_size;// --> don't know what it is

	//routing
	unsigned int _listen; // the listen port
	std::string _host; // local host or something else

	std::list<std::string> _accepted_method; // list of accepted method
	bool _allow_directory_listing;
	std::string _default_directory_answer_file; // ????

	bool		_allow_uploaded;
	std::string	_uploaded_files_root;

	std::string _cgi_extension;
	std::string _cgi_root;

	void parse_root(std::string b);
	void parse_client_body_size(std::string b);
	void parse_errors(std::string b);
	void parse_listen(std::string);
	void parse_host(std::string);
	void parse_method(std::string);
	void parse_directory(std::string);
	void parse_allow_directory_listing(std::string);
	void parse_default_directory_answer_file(std::string);
	void parse_allow_uploaded(std::string);
	void parse_uploaded_files_root(std::string);
	void parse_server_name(std::string);


};

#endif

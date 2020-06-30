#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include <string>
#include <list>
#include <iostream>
#include <map>
#include "env.hpp"

#define ALPHACHAR "ABCEDFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789" // Degueulasse

void parse_root(std::string b);

class ServerConfig
{
	/*
	** maybe a map of functions
	*/
	typedef void (*FUNC)(std::string s);
   	//typedef std::map<std::string, FUNCPTR> FuncMap;
	typedef std::map<std::string, void (*)(std::string s)> fmap;
	//typedef bool (ServerConfig::*)(std::string) fparse;

	public:
	ServerConfig() ; // initialise with default

	bool setConfig(std::string s); // real constructor

	ServerConfig(const ServerConfig& serverConfig);
	void operator = (const ServerConfig& serverConfig);

	//Parse_method
	void initiate_map(fmap&);


	// getters:

	private:
	//paths
	std::string _root; //-> root path of the server like www/
	std::string _errors; // ->root of default errors directory
	unsigned int _client_body_size;// --> don't know what it is

	//routing
	std::string _serverName; // SERVER_NAME, typing it should be equivalent to host i guess
	unsigned int _listen; // the listen port
	unsigned int _host; // local host or something else

	std::list<std::string> _accepted_method; // list of accepted method
	bool _directory_listing;
	std::string _default_directory_answer_file; // ????

	bool		_allow_uploaded;
	std::string	_uploaded_files_root;


};

#endif

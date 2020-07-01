#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include <string>
#include <list>
#include <iostream>
#include <map>
#include "env.hpp"
#include "Config.hpp"

#define ALPHACHAR "ABCEDFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789" // Degueulasse


/*
** this class should serve to parse a config struct listed in Data
** The architecture isn't definitive yet
*/
class Config;

class ConfigParser
{
	/*
	** maybe a map of functions
	*/
	typedef void (ConfigParser::*Func)(std::string s);
   	//typedef std::map<std::string, FUNCPTR> FuncMap;
	typedef std::map<std::string, Func> fmap;
	//typedef bool (ConfigParser::*)(std::string) fparse;

	public:
	ConfigParser() ; // initialise with default

	bool setConfig(Config* config, std::string s); // real constructor

	ConfigParser(const ConfigParser& configParser);
	void operator = (const ConfigParser& configParser);
	~ConfigParser();

	//Parse_method
	void initiate_map();



	void print_data(Config* config = NULL); // utilitary, for testing only

	// getters:

	private:

	Config*	_config;

	fmap _map;

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

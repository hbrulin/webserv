#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include <string>
#include <list>
#include <iostream>
#include <map>
#include "Config.hpp"

#define ALPHACHAR "ABCEDFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789" // Degueulasse
#define END_INSTRUCTION_CHAR " \t\n\r;}"
#define WHITESPACE " \t\n\r "
#define NUMBER_OF_PARAMETERS 13

/*
** this class should serve to parse a config struct listed in Data
** The architecture isn't definitive yet
*/
struct Config;

class ConfigParser
{
	/*
	** maybe do do a sum check to verificate if everythng is parse
	** A voir pour le nombre de parametre a checker: ex: Allow_uploaded == 0 ...
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

	void print_data(Config* config); // utilitary, for testing only

	private:

	Config*	_config;

	fmap _map;

	void parse_root(std::string b);
	void parse_body_size(std::string b);
	void parse_errors(std::string b);
	void parse_listen(std::string);
	void parse_host(std::string);
	void parse_method(std::string);
	void parse_directory(std::string);
	void parse_directory_listing(std::string);
	void parse_default_directory_answer_file(std::string);
	void parse_send_files(std::string);
	void parse_files_root(std::string);
	void parse_server_name(std::string);
	void parse_cgi_root(std::string);
	void parse_cgi_type(std::string);

	void remove_whitespace(std::string&);

};

#endif

#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include <string>
#include <list>
#include <iostream>
#include <map>
#include "Config.hpp"
#include "../utils/definitions.hpp"

struct Config;

class ConfigParser
{
	typedef void (ConfigParser::*Func)(std::string s);
	typedef std::map<std::string, Func> fmap;

	public:
	ConfigParser() ; // initialise with default

	bool setConfig(Config* config, std::string& s); // real constructor

	ConfigParser(const ConfigParser& configParser);
	void operator = (const ConfigParser& configParser);
	~ConfigParser();

	void initiate_map();

	void print_data(Config* config); // utilitary, for testing only

	private:

	Config*	_config;

	fmap _map;

	void parse_root(std::string b);
	void parse_listen(std::string);
	void parse_host(std::string);
	void parse_method(std::string);
	void parse_directory(std::string);
	void parse_server_name(std::string);

	void parse_location(std::string);

	void parse_cgi_root(std::string b);
	void parse_cgi_type(std::string b);
	void parse_body_size(std::string b);

	void parse_errors(std::string);

	void parse_index(std::string);
	void parse_autoindex(std::string b);

	void remove_whitespace(std::string&);

};

#endif

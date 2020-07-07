#ifndef DATA_HPP
#define DATA_HPP

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "Config.hpp"
#include "ConfigParser.hpp"

#define DEFAULT_CONFIG_PATH "./config/default.conf"

/*
** Initialiser as Data(av[1])
** check validity
** if null, go find default config
** Will throw uncatched exception if does not work properly
** will basically read until find server {
** list.push(ServerConfig( [config file from server { to next } ]));
**
** Errors yet not handled or tested:
** empty file
** missing key
** invalid numerical key like invalid listen port
** invalid ip
** if more than one server i need to check listen port and host port of each
** invalid method name ???
*/

class Data
{
	public:
	/*
	** Unique constructor to be used
	*/

	Data(const char* file_name = NULL);
	Data(const Data& data);
	void operator = (const Data& data);
	~Data();

	// GETTERS
	unsigned int getSize(); // return _serverList.size()
	const std::vector<Config>& getConfigList(); // return _serverList
	const Config& operator [] (unsigned int); // getter for list[]


	private:
	Data();

	std::vector<Config> _configList;
	ConfigParser _configParser;
	std::string _cgi_extension;
	std::string _cgi_root;
};

#endif
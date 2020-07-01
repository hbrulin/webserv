#ifndef DATA_HPP
#define DATA_HPP

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "env.hpp"
#include "Config.hpp"
#include "ConfigParser.hpp"

#define DEFAULT_CONFIG_PATH "./config_file.conf"

/*
** Initialiser as Data(av[1])
** check validity
** if null, go find default config
** Will throw uncatched exception if does not work properly
** will basically read until find server {
** list.push(ServerConfig( [config file from server { to next } ]));
**
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
	const std::vector<Config>& getServerList(); // return _serverList
	const Config& operator [] (unsigned int); // getter for list[]


	private:
	Data();

	std::vector<Config> _serverList;
	ConfigParser _configParser;
	std::string _cgi_extension;
	std::string _cgi_root;
};

#endif

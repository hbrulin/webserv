#ifndef DATA_HPP
#define DATA_HPP

#include <string>
#include <list>
#include <iostream>
#include <fstream>
#include "env.hpp"
#include "ServerConfig.hpp"

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

	// GETTERS
	unsigned int getSize(); // return _serverList.size()
	std::list<ServerConfig> getServerList(); // return _serverList
	ServerConfig& operator [] (unsigned int); // getter for list[]




	private:
	Data();

	std::list<ServerConfig> _serverList;
	std::string _cgi_extension;
	std::string _cgi_root;
};

#endif

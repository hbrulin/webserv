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
** Supress a server if it have the same port as another
**
** Creer un parser de metadata
*/

/*
** POUR LES METHODES ET LE TYPE DE CGI:
** if faut renseigner les methodes ou le type de cgi dans Data.cpp dans les tableaux
** de char* correspondant, et le terminer par NULL
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
	std::vector<Config>& getConfigList(); // return _serverList
	const Config& operator [] (unsigned int); // getter for list[]

	static const char* _SUPPORTED_CGI[];
	static const char* _SUPPORTED_METHOD[];

	private:
	//Data() {};

	std::vector<Config> _configList;
	ConfigParser _configParser;
	std::string _cgi_extension;
	std::string _cgi_root;

	void check_validity();
	void check_multiple_ports();
};

#endif

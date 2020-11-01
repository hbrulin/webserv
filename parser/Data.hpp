#ifndef DATA_HPP
#define DATA_HPP

#include <string>
#include <vector>
#include <iostream>
#include "Config.hpp"
#include "ConfigParser.hpp"
#include "../utils/definitions.hpp"
#include "../utils/libft.h"

class Data
{
	public:
	Data(const char* file_name = NULL);
	Data(const Data& data);
	void operator = (const Data& data);
	~Data();

	// GETTERS
	unsigned int getSize(); // return _serverList.size()
	std::vector<Config>& getConfigList(); // return _serverList
	const Config& operator [] (unsigned int); // getter for list[]

	void print();

	static const char* _SUPPORTED_CGI[];
	static const char* _SUPPORTED_METHOD[];

	private:
	Data() {};

	std::vector<Config> _configList;
	ConfigParser _configParser;
	std::string _cgi_extension;
	std::string _cgi_root;

	void check_validity();
	void check_multiple_ports();
};

#endif

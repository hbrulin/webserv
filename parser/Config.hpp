#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <vector>
#include <iostream>
#include <fstream>
#include <sys/stat.h>

#include "Locations.hpp"
#include "../utils/definitions.hpp"

struct Config
{
	//paths
	/*
	** not sure if host and listen and stuff should be strings or int
	** Should i check if ips are valid here, or elsewhere?
	*/
	Config();
	Config(const Config&);

	void set_blank();
	void set_default_locations();
	void set_default_errors();

	std::string _server_name;
	std::string _root; //-> root path of the server like www/
	std::string _errors; // ->root of default errors directory
	std::map<int,std::string> _error;


	//routing
	unsigned int 				_listen; // the listen port
	std::vector<unsigned int>	_ports; // vec containing all the ports passed
	std::string 				_host; // local host or something else


	//bool 		_directory_listing;
	//std::string _default_directory_answer_file; // ????

	//bool		_send_files;
	//std::string	_files_root;

	std::string 				_cgi_type;
	std::string 				_cgi_root;

	unsigned int 						_body_size;// --> don't know what it is
	///std::vector<std::string> 	_methods; // list of accepted method

//	Location _cgi;

	Locations _locations;

	char	**_auth_path;
	char	**_auth_realm;

};

#endif

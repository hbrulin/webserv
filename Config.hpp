#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <vector>

struct Config
{
	//paths
	/*
	** not sure if host and listen and stuff should be strings or int
	** Should i check if ips are valid here, or elsewhere?
	*/

	std::string _server_name;
	std::string _root; //-> root path of the server like www/
	std::string _errors; // ->root of default errors directory
	unsigned int _client_body_size;// --> don't know what it is

	//routing
	unsigned int _listen; // the listen port
	std::string _host; // local host or something else

	std::vector<std::string> _accepted_method; // list of accepted method
	bool _allow_directory_listing;
	std::string _default_directory_answer_file; // ????

	bool		_allow_uploaded;
	std::string	_uploaded_files_root;

	std::string _cgi_extension;
	std::string _cgi_root;
};

#endif

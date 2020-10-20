#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <sys/stat.h>
#include "../utils/definitions.hpp"


class Location
{

	typedef void (Location::*Func)(std::string s);
	typedef std::map<std::string, Func> fmap;

	public:
	Location();
	~Location();
	Location(const Location&);
	void operator = (const Location&);

	void parse(std::string);
	std::string get_name();

	std::string		get_path(); // path = _root + _name
	std::string		get_file_path(std::string); // to search files inside location
	bool			check_allowed_method(std::string method, std::string request_uri);
	bool			check_allowed_cgi(std::string ext);
	bool			upload(std::string file);

	std::string		get_index();
	std::string		get_index_path();
	unsigned int	get_body_size();
	std::string		get_cgi_root();
	std::string		get_listing();


	//Location get_location(std::string);

	void print();

	public:
	std::string _name; // name of the directory
	std::string _root; // Preceding the location name
	std::string	_mode; // mode of searching files

	std::string _index;
	std::string _try_files;
	std::string _allow;

	// Config file:

	std::vector<std::string>	_methods;
	bool 						_send_files;
	bool						_directory_listing;
	std::string 				_uploaded_files_root;
	std::string					_directory_answer_file;

	unsigned int				_body_size;

	std::string					_cgi_type;
	std::string					_cgi_root;
	std::string					_cgi_file;
	std::string					_cgi_method;

	std::string 				_errors;
	std::map<int,std::string> _error;


	fmap _map;

	void initiate_map();

	void parse_root(std::string);
	void parse_try_files(std::string);
	void parse_index(std::string);
	void parse_allow(std::string);
	void parse_method(std::string);

	void parse_cgi_root(std::string b);
	void parse_cgi_type(std::string b);
	void parse_directory_listing(std::string b);
	void parse_default_directory_answer_file(std::string b);
	void parse_send_files(std::string b);
	void parse_uploaded_files_root(std::string b);
	void parse_body_size(std::string b);
	void parse_cgi_file(std::string);
	void parse_errors(std::string);
	void parse_error(std::string);


	void remove_whitespace(std::string&);
	bool check_mode();

	void set_default_errors(std::map<int,std::string>);
	std::string get_error_path(int code);

	void check_path_validity();
	void check_methods_validity();


	static const char* _SUPPORTED_CGI[];
	static const char* _SUPPORTED_METHOD[];
};

#endif

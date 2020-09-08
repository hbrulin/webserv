#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <string>
#include <iostream>
#include <vector>
#include <map>

#define LOCEXCEPTION "Bad location block: "
#define END_INSTRUCTION_CHAR " \t\n\r;}"
#define ALPHACHAR "ABCEDFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"


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
	bool			check_allowed_method(std::string method);
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


	void remove_whitespace(std::string&);
	bool check_mode();

	void check_path_validity();
	void check_methods_validity();


	static const char* _SUPPORTED_CGI[];
	static const char* _SUPPORTED_METHOD[];
};

#endif

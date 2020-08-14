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

	Location get_location(std::string);

	void print();

	private:
	std::string _name; // name of the directory
	std::string _root; // Preceding the location name
	std::string	_mode; // mode of searching files

	std::string _index;
	std::string _try_files;
	std::string _allow;

	std::vector<std::string> _methods;

	fmap _map;

	void initiate_map();

	void parse_root(std::string);
	void parse_try_files(std::string);
	void parse_index(std::string);
	void parse_allow(std::string);


	void remove_whitespace(std::string&);
	bool check_mode();
};

#endif

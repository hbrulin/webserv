#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <string>
#include <iostream>

class Location
{
	public:
	Location();
	~Location();
	Location(const Location&);
	void operator = (const Location&);

	std::string		get_path(); // path = _root + _name
	std::string		get_file_path(std::string file_name); // to search files inside location
	

	private:
	std::string _name; // name of the directory
	std::string _root; // Preceding the location name
	std::string	_mode; // mode of searching files
};

#endif

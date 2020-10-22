#ifndef LOCATIONS_HPP
#define LOCATIONS_HPP

#include "Location.hpp"
#include <vector>
#include "../utils/definitions.hpp"

class Locations
{
	public:
	Locations();
	~Locations();
	Locations(const Locations&);
	void operator = (const Locations&);

	unsigned long size();

	// access
	Location& get_loc(std::string name);
	Location& operator [] (unsigned long i);
	void push_back (Location loc);

	std::string get_path(std::string name);

	Location& get_loc_by_url(std::string url);

	void check_path_validity();

	void 	print();

	public:
	std::vector<Location> 	_v;
	Location 				_blank; // blank location returned if none was found
};

#endif

/*
site:80/1/8/a
site:80/a

location "/1/" {blal}
location "/1/8"
location "/a" {}
url
*/

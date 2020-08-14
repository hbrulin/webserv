#ifndef LOCATIONS_HPP
#define LOCATIONS_HPP

#include "Location.hpp"
#include <vector>

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


	private:
	std::vector<Location> 	_v;
	Location 				_blank;
};

#endif

#include "Locations.hpp"

Locations::Locations()
{

}

Locations::~Locations() {}

Locations::Locations(const Locations& locs)
{
	_v = locs._v;
}

void Locations::operator = (const Locations& locs)
{
	_v = locs._v;
}

unsigned long Locations::size()
{
	return (_v.size());
}

Location& Locations::operator [] (unsigned long i)
{
	return (_v[i]);
}

Location& Locations::get_loc(std::string name)
{
	for (unsigned long i = 0; i < _v.size(); i++)
	{
		if (name == _v[i].get_name())
			return (_v[i]);
	}
	return (_blank);
}

std::string Locations::get_path(std::string name)
{
	for (unsigned long i = 0; i < _v.size(); i++)
	{
		if (name == _v[i].get_name())
			return (_v[i].get_path());
	}
	return ("");
}

void Locations::push_back(Location loc)
{
	_v.push_back(loc);
}

#include "Location.hpp"

Location::Location()
{
	_name = "";
	_root = "";
	_mode = "";
}

Location::~Location()
{}

Location::Location(const Location& location)
{
	_name = location._name;
	_root = location._root;
	_mode = location._mode;
}

void operator = (const Location& location)
{
	_name = location._name;
	_root = location._root;
	_mode = location._mode;
}

Location::Location(std::string b)
{
	// parse mode
	// parse name
	// parse root
}

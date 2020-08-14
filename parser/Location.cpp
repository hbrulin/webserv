#include "Location.hpp"

Location::Location()
{
	_name = "";
	_root = "";
	_mode = "";
	_try_files = "";
	_index = "";
	_allow = "";
	initiate_map();
}

void Location::initiate_map()
{
	_map["root"] = &Location::parse_root;
	_map["try_files"] = &Location::parse_try_files;
	_map["index"] = &Location::parse_index;
	_map["allow"] = &Location::parse_allow;
}

Location::~Location()
{}

Location::Location(const Location& location)
{
	_name = location._name;
	_root = location._root;
	_mode = location._mode;
	initiate_map();
}

void Location::operator = (const Location& location)
{
	_name = location._name;
	_root = location._root;
	_mode = location._mode;
	initiate_map();
}

void Location::parse(std::string b)
{
	// parse mode
	// parse name
	// parse root

	_root = "";
	if (b.find('/') == b.npos)
		throw(std::logic_error("Bad location block: Missing name"));
	_mode = b.substr(0, b.find('/'));
	remove_whitespace(_mode);
	if (!check_mode())
		throw(std::logic_error("Bad location block: Unrecognized mode: '" + _mode + "'"));
	b = b.substr(b.find('/'));

	if (b.find('{') == b.npos)
		throw(std::logic_error("Bad location block: Missing token '{' after name"));

	_name = b.substr(0, b.find('{'));
	_name = _name.substr(1); // { inclus si substr(1, find({)) ?????
	remove_whitespace(_name);

	b = b.substr(b.find('{') + 1);

//	std::cout << "Mode: " << _mode << std::endl;
//	std::cout << "Name: " << _name << std::endl;

	std::string key = b;
	std::string value = "";
	remove_whitespace(key);
	if (key.empty())
		return ;

	while (b.size())
	{
		if (b.find_first_not_of(END_INSTRUCTION_CHAR) == b.npos)
			break ;
		if (b.find(';') == b.npos)
			throw (std::logic_error("Bad location block: Missing end instruction token ';'"));
		b = b.substr(b.find_first_not_of(END_INSTRUCTION_CHAR));
		key = b.substr(0, b.find_first_of(END_INSTRUCTION_CHAR));
		remove_whitespace(key);
//		std::cout << "key: " << key << std::endl;
		b = b.substr(key.size());
		value = b.substr(0, b.find(';'));
		remove_whitespace(value);
		b = b.substr(b.find(';') + 1);

		fmap::iterator it;
		it = _map.find(key);
		if (it == _map.end())
			throw(std::logic_error("Bad location block: Unrecognized key: " + key));
		(this->*(_map[key]))(value);
	}
}

// Parsers lnked to the fmap like in configParser
void Location::parse_root(std::string b)
{
	remove_whitespace(b);
	_root = b;
}

void Location::parse_try_files(std::string b)
{
	remove_whitespace(b);
	_try_files = b;
}

void Location::parse_index(std::string b)
{
	remove_whitespace(b);
	_index = b;
}

void Location::parse_allow(std::string b)
{
	remove_whitespace(b);
	_allow = b;
}

void Location::remove_whitespace(std::string& s)
{
	//s.erase(std::remove_if(s.begin(), s.end(), ::isspace ), s.end());
	std::string::iterator start = s.begin();
	while (start != s.end() && ::isspace(*start))
		start++;

	std::string::iterator end = s.end();
	do
		end--;
	while (std::distance(start, end) > 0 && std::isspace(*end));
	s = std::string(start, end + 1);
}

// ACCESS

std::string Location::get_path()
{
	return (_root + _name);
}


bool Location::check_mode()
{
	if (!_mode.empty() && _mode != "=" && _mode != "~" && _mode != "~*" && _mode != "^~")
		return (false);
	return (true);
}

void Location::print()
{
 	std::cout << "name: " << _name << std::endl << "root: " << _root << std::endl <<
	"_mode: " << _mode << std::endl << "index: " << _index << std::endl <<
	"try files: " << _try_files << std::endl << "allow: " << _allow << std::endl;;
}

std::string Location::get_name()
{
	return (_name);
}

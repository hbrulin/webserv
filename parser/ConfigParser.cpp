#include "ConfigParser.hpp"

#include <algorithm>

ConfigParser::ConfigParser()
{
	_config = NULL;
	initiate_map();
}

ConfigParser::ConfigParser(const ConfigParser& configParser)
{
	_config = configParser._config;
	initiate_map();
}

ConfigParser::~ConfigParser()
{

}

void ConfigParser::operator = (const ConfigParser& configParser)
{
	_config = configParser._config;
	initiate_map();
}

bool ConfigParser::setConfig(Config* config, std::string& s)
{
	std::string key;
	std::string value;
	int i;

	_config = config;
	//_config->set_default_errors();
	while (s.size() > 0 && s.compare(0, 1, "}") != 0)
	{
		i = 0;
		if (s.find_first_not_of(END_INSTRUCTION_CHAR) == s.npos)
			break ;
		s = s.substr(s.find_first_not_of(END_INSTRUCTION_CHAR));
		i = s.find_first_of(END_INSTRUCTION_CHAR);
		if (i == (int)s.npos)
			break ;
		key = s.substr(0, i);
		remove_whitespace(key);
		s = s.substr(i);

		if (key != "location")
			value = s.substr(0, s.find(';'));
		else
			value = s.substr(0, s.find('}'));

		//value.erase(std::remove_if(value.begin(), value.end(), ::isspace ), value.end());
		//std::cout << "Key " << key << " Value: " << value << std::endl;
		if (_map[key] == NULL)
			throw (std::logic_error("Parsing error: Unknown option: " + key));

		try
		{
			(this->*(_map[key]))(value);
		}
		catch (std::logic_error& e)
		{
			if (key == "location")
				std::cout << e.what() << std::endl;
			throw (std::logic_error("Parsing error: Unknown value: '" + value + "' for option: " + key));
			return (false);
		}
		if (key != "location")
			s = s.substr(s.find(';') + 1);
		else
			s = s.substr(s.find('}') + 1);
	}

	_config->set_blank();
	_config->set_default_locations();

//	_config->_locations.check_path_validity(); -> a travailler pour voir si on check les paths
	return (true);
}

void ConfigParser::initiate_map()
{
	//map["root"] = &ConfigParser::parse_root;
	/*
	** Si on doit ajouter un parametre, il faut rajouter une clé ici avec sa methode
	** correspondente que l'on crée en dessous et rajouter un attribut a config.hpp
	*/
	_map["root"] = &ConfigParser::parse_root;
	_map["errors"] = &ConfigParser::parse_errors;
	_map["body_size"] = &ConfigParser::parse_body_size;
	_map["server_name"] = &ConfigParser::parse_server_name;
	_map["listen"] = &ConfigParser::parse_listen;
	_map["methods"] = &ConfigParser::parse_method;
	_map["cgi_root"] = &ConfigParser::parse_cgi_root;
	_map["cgi_type"] = &ConfigParser::parse_cgi_type;
	_map["location"] = &ConfigParser::parse_location;
}

void ConfigParser::parse_root(std::string b)
{
	remove_whitespace(b);
	_config->_root = b;
}

void ConfigParser::parse_body_size(std::string b)
{
	remove_whitespace(b);
	int body = stoi(b);
	if (body < 0)
		throw(std::logic_error("Body size cannot be negative"));
	_config->_body_size = body;
}
/* OBSOLETE
void ConfigParser::parse_errors(std::string b)
{
	remove_whitespace(b);
	_config->_errors = b;
}
*/
void ConfigParser::parse_listen(std::string b)
{
	remove_whitespace(b);

	if (b.empty())
		return ;
	if (b.compare(0, 9, "localhost") == 0)
	{
		_config->_host = LOCALHOST;
		b = b.substr(9);
		if (b.find(':') != b.npos)
		{
			//_config->_host = b.substr(0, b.find(':'));
			b = b.substr(b.find(':') + 1);
			if (b.size())
			{
				_config->_listen = stoi(b);
				_config->_ports.push_back(stoi(b));
			}
		}
	}
	else if (b.find('.') != b.npos) // contains an ip
	{
		if (b.find(':') != b.npos)
		{
			_config->_host = b.substr(0, b.find(':'));
			b = b.substr(b.find(':') + 1);
			if (b.size())
			{
				_config->_listen = stoi(b);
				_config->_ports.push_back(stoi(b));
			}
		}
		else
			_config->_host = b;
	}
	else
	{
		// maybe check if number
		_config->_listen = stoi(b);
		_config->_ports.push_back(stoi(b));
	}
}


// fonction obsolete
void ConfigParser::parse_host(std::string b)
{
	//remove_whitespace(b);
	// check if is ip;
	std::string localhost = "localhost";


	_config->_host = b;
}

void ConfigParser::parse_server_name(std::string b)
{
	remove_whitespace(b);
	_config->_server_name = b;
}


void ConfigParser::parse_location(std::string b)
{
	Location loc;
	loc.parse(b);
	_config->_locations.push_back(loc);
	//b = "void";
	// Parse mode if or not
	// parse

}

void ConfigParser::parse_cgi_root(std::string b)
{
	remove_whitespace(b);
	_config->_cgi_root = b;
}

void ConfigParser::parse_cgi_type(std::string b)
{
	remove_whitespace(b);
	std::transform(b.begin(), b.end(), b.begin(), ::tolower);
	_config->_cgi_type = b;
}


void ConfigParser::parse_method(std::string b)
{
	std::string s = "";

	while (b.size())
	{
		s = b.substr(
		b.find_first_of(ALPHACHAR),
		b.find_first_of(END_INSTRUCTION_CHAR, b.find_first_of(ALPHACHAR)));
		//std::cout << s << std::endl;
		remove_whitespace(s);
		std::transform(s.begin(), s.end(),s.begin(), ::toupper); // convert to lower case
		//_config->_methods.push_back(s);

		if (b.find_first_of(END_INSTRUCTION_CHAR, b.find_first_of(ALPHACHAR)) == std::string::npos)
			break;
		b = b.substr(b.find_first_of(END_INSTRUCTION_CHAR, b.find_first_of(ALPHACHAR)));
	}
}

void ConfigParser::parse_errors(std::string b)
{
	std::string code;
	std::string path;

	while (b.size())
	{
		code = "";
		path = "";
		code = b.substr(
		b.find_first_of("0123456789"),
		b.find_first_of(END_INSTRUCTION_CHAR, b.find_first_of("0123456789")));
		//std::cout << s << std::endl;
		remove_whitespace(code);

		if (b.find_first_of(END_INSTRUCTION_CHAR, b.find_first_of("0123456789")) == std::string::npos)
			break;
		b = b.substr(b.find_first_of(END_INSTRUCTION_CHAR, b.find_first_of(ALPHACHAR)));
		//std::cout << "Code: " << code;
		path = b.substr(
		b.find_first_of(ALPHACHAR),
		b.find_first_of(END_INSTRUCTION_CHAR, b.find_first_of(ALPHACHAR)));
		//std::cout << " Path: " << path << std::endl;

		_config->_errors[std::stoi(code)] = path;
		if (b.find_first_of(END_INSTRUCTION_CHAR, b.find_first_of("0123456789")) == std::string::npos)
			break;
		b = b.substr(b.find_first_of(END_INSTRUCTION_CHAR, b.find_first_of(ALPHACHAR)));

		//std::cout << "b: " << b << std::endl;
	}
}

void ConfigParser::remove_whitespace(std::string& s)
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


void ConfigParser::print_data(Config* config)
{
//	if (!config)
//		config = _config;
	std::cout << "-----------ServerInfo-----------\n";
	std::cout << "server_name " << config->_server_name << "\nlisten: " <<
	config->_listen << "\nhost: " << config->_host << "\nroot: " << config->_root << std::endl;
	config->_locations.print();
	std::cout << "Error pages:\n";
	for (std::map<int,std::string>::iterator it = config->_errors.begin(); it != config->_errors.end(); it++)
		std::cout << it->first << ": " << it->second << std::endl;
}

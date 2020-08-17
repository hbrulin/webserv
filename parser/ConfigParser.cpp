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
	//something like this
	std::string key;
	std::string value;
	int i;
	int parsing_sum = 0;

	_config = config;
	//std::cout << s << std::endl;
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
			//std::cout << "1\n";
			parsing_sum++;
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

	/*if (parsing_sum != NUMBER_OF_PARAMETERS)
		throw (std::logic_error("One parameter is missing")); // A voir pour retravailler*/

/*	_config->_locations.get_loc_by_url("");
	_config->_locations.get_loc_by_url("/bonjour/aurevoir/salut/");
	_config->_locations.get_loc_by_url("/");*/
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
	//_map["host"] = &ConfigParser::parse_host;
	_map["methods"] = &ConfigParser::parse_method;
	//_map["directory_listing"] = &ConfigParser::parse_directory_listing;
	//_map["default_directory_answer_file"] = &ConfigParser::parse_default_directory_answer_file;
	//_map["send_files"] = &ConfigParser::parse_send_files;
	//_map["uploaded_files_root"] = &ConfigParser::parse_files_root;
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
	_config->_body_size = stoi(b);
}

void ConfigParser::parse_errors(std::string b)
{
	remove_whitespace(b);
	_config->_errors = b;
}

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
		_config->_methods.push_back(s);

		if (b.find_first_of(END_INSTRUCTION_CHAR, b.find_first_of(ALPHACHAR)) == std::string::npos)
			break;
		b = b.substr(b.find_first_of(END_INSTRUCTION_CHAR, b.find_first_of(ALPHACHAR)));
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
	config->_listen << "\nhost: " << config->_host << "\nroot: " << config->_root << "\nerrors: " << config->_errors << std::endl;
	config->_locations.print();
	std::cout << std::endl;
}

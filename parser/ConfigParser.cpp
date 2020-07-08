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

bool ConfigParser::setConfig(Config* config, std::string s)
{
	//something like this
	std::string key;
	std::string value;
	int i;
	int parsing_sum = 0;

	_config = config;

	//std::cout << s << std::endl;
	while (s.size() > 0 && s != "}")
	{
		i = 0;
//		s.find_first_of(ALPHACHAR);
		s = s.substr(s.find_first_of(ALPHACHAR));

		i = s.find_first_of(END_INSTRUCTION_CHAR);
		key = s.substr(0, i);
		remove_whitespace(key);
		s = s.substr(i);

		value = s.substr(0, s.find(';'));

		//value.erase(std::remove_if(value.begin(), value.end(), ::isspace ), value.end());
		//std::cout << "Key " << key << " Value: " << value << std::endl;
		if (_map[key] == NULL)
			throw (std::logic_error("Parsing error: Unknown option: " + key));

		try
		{
			(this->*(_map[key]))(value);
			parsing_sum++;
		}
		catch (...)
		{
			throw (std::logic_error("Parsing error: Unknown value: " + value + " for option: " + key));
			return (false);
		}

		s = s.substr(s.find(';') + 1);
	}

	/*if (parsing_sum != NUMBER_OF_PARAMETERS)
		throw (std::logic_error("One parameter is missing")); // A voir pour retravailler*/
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
	_map["client_body_size"] = &ConfigParser::parse_client_body_size;
	_map["server_name"] = &ConfigParser::parse_server_name;
	_map["listen"] = &ConfigParser::parse_listen;
	_map["host"] = &ConfigParser::parse_host;
	_map["method"] = &ConfigParser::parse_method;
	_map["directory_listing"] = &ConfigParser::parse_allow_directory_listing;
	_map["default_directory_answer_file"] = &ConfigParser::parse_default_directory_answer_file;
	_map["uploaded_files"] = &ConfigParser::parse_allow_uploaded;
	_map["uploaded_files_root"] = &ConfigParser::parse_uploaded_files_root;
	_map["cgi_root"] = &ConfigParser::parse_cgi_root;
	_map["cgi_type"] = &ConfigParser::parse_cgi_type;
}

void ConfigParser::parse_root(std::string b)
{
	remove_whitespace(b);
	_config->_root = b;
}

void ConfigParser::parse_errors(std::string b)
{
	remove_whitespace(b);
	_config->_errors = b;
}

void ConfigParser::parse_client_body_size(std::string b)
{
	remove_whitespace(b);
	_config->_client_body_size = stoi(b);
}

void ConfigParser::parse_listen(std::string b)
{
	remove_whitespace(b);
	_config->_listen = stoi(b);
	//std::cout << b << std::endl;
	_config->_ports.push_back(stoi(b));
}


void ConfigParser::parse_host(std::string b)
{
	remove_whitespace(b);
	if (b == "localhost")
		b = LOCALHOST;
	_config->_host = b;
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
		std::transform(s.begin(), s.end(),s.begin(), ::tolower); // convert to lower case
		_config->_accepted_method.push_back(s);

		if (b.find_first_of(END_INSTRUCTION_CHAR, b.find_first_of(ALPHACHAR)) == std::string::npos)
			break;
		b = b.substr(b.find_first_of(END_INSTRUCTION_CHAR, b.find_first_of(ALPHACHAR)));
	}
}

void ConfigParser::parse_allow_directory_listing(std::string b)
{
	remove_whitespace(b);
	if (b == "yes")
		_config->_allow_directory_listing = true;
	else if (b == "no")
		_config->_allow_directory_listing = false;
	else
		throw(std::logic_error("Allow_directory_listing: Must be yes or no"));
}

void ConfigParser::parse_default_directory_answer_file(std::string b)
{
	remove_whitespace(b);
	_config->_default_directory_answer_file = b;
}

void ConfigParser::parse_allow_uploaded(std::string b)
{
	remove_whitespace(b);
	if (b == "yes")
		_config->_allow_uploaded = true;
	else if (b == "no")
		_config->_allow_uploaded = false;
	else
		throw(std::logic_error("Allow_uploaded: Must be yes or no"));
}

void ConfigParser::parse_uploaded_files_root(std::string b)
{
	remove_whitespace(b);
	_config->_uploaded_files_root = b;
}

void ConfigParser::parse_server_name(std::string b)
{
	remove_whitespace(b);
	_config->_server_name = b;
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
	config->_listen << "\nhost: " << config->_host << "\nroot: " << config->_root << "\nerrors: " << config->_errors
	<< "\nclient body stuff: " << config->_client_body_size
	<< "\nAllow_uploaded: " << config->_allow_uploaded << "\nuploaded_files_root: " << config->_uploaded_files_root
	<< "\nAllow_directory_listing: " << config->_allow_directory_listing <<
	"\ndefault_directory_answer_file: " << config->_default_directory_answer_file
	<< "\nAccepted Methods:";
	for (std::vector<std::string>::size_type i = 0; i < config->_accepted_method.size(); i++)
		std::cout << " " << config->_accepted_method[i];

	std::cout << "\nCGI_ROOT: " << _config->_cgi_root << "\nCGI_TYPE: " << _config->_cgi_type;

	std::cout << std::endl;
}

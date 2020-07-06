#include "ConfigParser.hpp"

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
		s = s.substr(s.find_first_of(ALPHACHAR));
		i = s.find_first_of(END_INSTRUCTION_CHAR);
		key = s.substr(0, i);
		key.erase(std::remove_if(key.begin(), key.end(), ::isspace ), key.end());
		s = s.substr(i);

		value = s.substr(0, s.find(';'));
		value.erase(std::remove_if(value.begin(), value.end(), ::isspace ), value.end());
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

	print_data();
	if (parsing_sum != NUMBER_OF_PARAMETERS)
		throw (std::logic_error("One parameter is missing"));
	return (true);
}

void ConfigParser::initiate_map()
{
	//map["root"] = &ConfigParser::parse_root;
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
	_config->_root = b;
	//std::cout << _root << std::endl;
	//std::cout << "root here: " << _root << std::endl;
	//return (true);
}

void ConfigParser::parse_errors(std::string b)
{
	_config->_errors = b;
}

void ConfigParser::parse_client_body_size(std::string b)
{
	_config->_client_body_size = stoi(b);
}

void ConfigParser::parse_listen(std::string b)
{
	_config->_listen = stoi(b);
}


void ConfigParser::parse_host(std::string b)
{
	_config->_host = b;
}

void ConfigParser::parse_method(std::string b)
{
	std::string s = "";
	//_config->_accepted_method.push_back("test");
	while (b.size() > 0)
	{
		if (b.size() && isalpha(b[0]))
		{
			_config->_accepted_method.push_back("");
			while (b.size() && isalpha(b[0]))
			{
				_config->_accepted_method.back() += b[0];
				b = b.substr(1);
			}
		}
		//if (s.size() > 0 && !isalpha(s[0]))
		//{
			while (b.size() && !isalpha(b[0]))
				b = b.substr(1);
	}
}

void ConfigParser::parse_allow_directory_listing(std::string b)
{
	if (b == "yes")
		_config->_allow_directory_listing = true;
	else if (b == "no")
		_config->_allow_directory_listing = false;
	else
		throw(std::logic_error("Allow_directory_listing: Must be yes or no"));
}

void ConfigParser::parse_default_directory_answer_file(std::string b)
{
	_config->_default_directory_answer_file = b;
}

void ConfigParser::parse_allow_uploaded(std::string b)
{
	if (b == "yes")
		_config->_allow_uploaded = true;
	else if (b == "no")
		_config->_allow_uploaded = false;
	else
		throw(std::logic_error("Allow_uploaded: Must be yes or no"));
}

void ConfigParser::parse_uploaded_files_root(std::string b)
{
	_config->_uploaded_files_root = b;
}

void ConfigParser::parse_server_name(std::string b)
{
	_config->_server_name = b;
}

void ConfigParser::parse_cgi_root(std::string b)
{
		_config->_cgi_root = b;
}

void ConfigParser::parse_cgi_type(std::string b)
{
		_config->_cgi_type = b;
}

void ConfigParser::print_data(Config* config)
{
	if (!config)
		config = _config;
	std::cout << "server_name " << config->_server_name << "\nlisten: " <<
	_config->_listen << "\nhost: " << config->_host << "\nroot: " << config->_root << "\nerrors: " << config->_errors
	<< "\nclient body stuff: " << config->_client_body_size
	<< "\nAllow_uploaded: " << config->_allow_uploaded << "\nuploaded_files_root: " << config->_uploaded_files_root
	<< "\nAllow_directory_listing: " << config->_allow_directory_listing <<
	"\ndefault_directory_answer_file: " << config->_default_directory_answer_file
	<< "\nAccepted Methods:";
	for (int i = 0; i < config->_accepted_method.size(); i++)
		std::cout << " " << config->_accepted_method[i];

	std::cout << "\nCGI_ROOT: " << _config->_cgi_root << "\nCGI_TYPE: " << _config->_cgi_type;

	std::cout << std::endl;

}

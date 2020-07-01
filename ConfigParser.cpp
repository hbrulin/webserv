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

	_config = config;

	while (s.size() > 0 && s != "}")
	{
		i = 0;
		s = s.substr(s.find_first_of(ALPHACHAR));
		i = s.find(" ");
		key = s.substr(0, i);
		s = s.substr(i);
		s = s.substr(s.find_first_of(ALPHACHAR));
		value = s.substr(0, s.find(';'));
		std::cout << "key: " << key << std::endl;
		std::cout << "value: " << value << std::endl;
		//map[s.substr(0, i)](s.substr(i, s.find(';')));
		if (_map[key] == NULL)
			throw (std::logic_error("Parsing error: Unknown option: " + key));
			//Func f = it->second;
		//	(this->*it->second)(value);
		try
		{
			(this->*(_map[key]))(value);
		}
		catch (...)
		{
			throw (std::logic_error("Parsing error: Unknown value: " + value + " for option: " + key));
			return (false);
		}
			//*f(value);
			//(this->(*(it->second)))(value);
		//_root = "lol";
		s = s.substr(s.find(';') + 1);
		//std::cout << "s here: " << s << std::endl;
		//s += s.find(';');
	}

	print_data();
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
	std::string s;
	_config->_accepted_method.push_back("test");
	/*while (b.size() > 1)
	{
		s = b.substr(0, b.find(','));
		_accepted_method.push_back(b);
		if (b.find(','))
			b = b.substr(b.find(',') + 1);
		else
			b = b.substr(b.)
		std::cout << b << std::endl;
	}*/
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

void ConfigParser::print_data(Config* config)
{
	if (!config)
		config = _config;
	std::cout << "server_name " << config->_server_name << " listen: " <<
	_config->_listen << " host: " << config->_host << " root: " << config->_root << " errors: " << config->_errors
	<< " client body stuff: " << config->_client_body_size << " accepted method: " << config->_accepted_method.front()
	<< " Allow_uploaded: " << config->_allow_uploaded << " uploaded_files_root: " << config->_uploaded_files_root
	<< " Allow_directory_listing: " << config->_allow_directory_listing <<
	" default_directory_answer_file: " << config->_default_directory_answer_file << std::endl;
}

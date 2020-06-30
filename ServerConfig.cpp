#include "ServerConfig.hpp"

ServerConfig::ServerConfig()
{
	_root = "";
	_errors = "";
	_client_body_size = 0;
	_server_name = "";
	_listen = 0;
	_host = "";
	_allow_directory_listing = 0;
	_allow_uploaded = 0;
	_uploaded_files_root = "";
	// config = NULL;
}

ServerConfig::ServerConfig(const ServerConfig& serverConfig)
{
	_root = serverConfig._root;
	_errors = serverConfig._errors;
	_client_body_size = serverConfig._client_body_size;
	_server_name = serverConfig._server_name;
	_listen = serverConfig._listen;
	_host = serverConfig._host;
	_allow_directory_listing = serverConfig._allow_directory_listing;
	_allow_uploaded = serverConfig._allow_uploaded;
	_uploaded_files_root = serverConfig._uploaded_files_root;
}

bool ServerConfig::setConfig(std::string s)
{
	//something like this
	fmap map;
	initiate_map(map);
	std::string key;
	std::string value;
	int i;

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
		if (map[key] == NULL)
			throw (std::runtime_error("Parsing error: Unknown option: " + key));
			//Func f = it->second;
		//	(this->*it->second)(value);
		try
		{
			(this->*(map[key]))(value);
		}
		catch (...)
		{
			throw (std::runtime_error("Parsing error: Unknown value: " + value + " for option: " + key));
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

void ServerConfig::initiate_map(fmap& map)
{
	//map["root"] = &ServerConfig::parse_root;
	map["root"] = &ServerConfig::parse_root;
	map["errors"] = &ServerConfig::parse_errors;
	map["client_body_size"] = &ServerConfig::parse_client_body_size;
	map["server_name"] = &ServerConfig::parse_server_name;
	map["listen"] = &ServerConfig::parse_listen;
	map["host"] = &ServerConfig::parse_host;
	map["method"] = &ServerConfig::parse_method;
	map["directory_listing"] = &ServerConfig::parse_allow_directory_listing;
	map["default_directory_answer_file"] = &ServerConfig::parse_default_directory_answer_file;
	map["uploaded_files"] = &ServerConfig::parse_allow_uploaded;
	map["uploaded_files_root"] = &ServerConfig::parse_uploaded_files_root;
}

void ServerConfig::parse_root(std::string b)
{
	_root = b;
	//std::cout << _root << std::endl;
	//std::cout << "root here: " << _root << std::endl;
	//return (true);
}

void ServerConfig::parse_errors(std::string b)
{
	_errors = b;
}

void ServerConfig::parse_client_body_size(std::string b)
{
	_client_body_size = stoi(b);
}

void ServerConfig::parse_listen(std::string b)
{
	_listen = stoi(b);
}


void ServerConfig::parse_host(std::string b)
{
	_host = b;
}

void ServerConfig::parse_method(std::string b)
{
	std::string s;
	_accepted_method.push_back("test");
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

void ServerConfig::parse_allow_directory_listing(std::string b)
{
	if (b == "yes")
		_allow_directory_listing = true;
	else if (b == "no")
		_allow_directory_listing = false;
	else
		throw(std::runtime_error("Allow_directory_listing: Must be yes or no"));
}

void ServerConfig::parse_default_directory_answer_file(std::string b)
{
	_default_directory_answer_file = b;
}
void ServerConfig::parse_allow_uploaded(std::string b)
{
	if (b == "yes")
		_allow_uploaded = true;
	else if (b == "no")
		_allow_uploaded = false;
	else
		throw(std::runtime_error("Allow_uploaded: Must be yes or no"));
}

void ServerConfig::parse_uploaded_files_root(std::string b)
{
	_uploaded_files_root = b;
}

void ServerConfig::parse_server_name(std::string b)
{
	_server_name = b;
}

void ServerConfig::print_data()
{
	std::cout << "server_name " << _server_name << " listen: " <<
	_listen << " host: " << _host << " root: " << _root << " errors: " << _errors
	<< " client body stuff: " << _client_body_size << " accepted method: " << _accepted_method.front()
	<< " Allow_uploaded: " << _allow_uploaded << " uploaded_files_root: " << _uploaded_files_root
	<< " Allow_directory_listing: " << _allow_directory_listing <<
	" default_directory_answer_file: " << _default_directory_answer_file << std::endl;
}

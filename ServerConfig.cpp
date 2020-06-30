#include "ServerConfig.hpp"

ServerConfig::ServerConfig()
{
	_root = "";
	_errors = "";
	_client_body_size = 0;
	_serverName = "";
	_listen = 0;
	_host = 0;
	_directory_listing = 0;
	_uploaded_files_root = "";
}

ServerConfig::ServerConfig(const ServerConfig& serverConfig)
{
	_root = serverConfig._root;
	_errors = serverConfig._errors;
	_client_body_size = serverConfig._client_body_size;
	_serverName = serverConfig._serverName;
	_listen = serverConfig._listen;
	_host = serverConfig._host;
	_directory_listing = serverConfig._directory_listing;
	_uploaded_files_root = serverConfig._uploaded_files_root;
}
bool ServerConfig::setConfig(std::string s)
{
	//something like this
	fmap map;
	initiate_map(map);

	while (s.size() > 0 && s != "}")
	{
		int i = 0;
		s = s.substr(s.find_first_of(ALPHACHAR));
		i = s.find(" ");
		std::string key = s.substr(0, i);
		s = s.substr(i);
		s = s.substr(s.find_first_of(ALPHACHAR));
		std::string value = s.substr(0, s.find(';'));
		std::cout << "key: " << key << std::endl;
		std::cout << "value: " << value << std::endl;
		//map[s.substr(0, i)](s.substr(i, s.find(';')));
	//	fmap::const_iterator it;
	//	if (key == "root")
	//		((*it).second)(value);
		_root = "lol";
		s = s.substr(s.find(';') + 1);
		//std::cout << "s here: " << s << std::endl;
		//s += s.find(';');
	}


	return (true);
}

void ServerConfig::initiate_map(fmap& map)
{
	map["root"] = &parse_root;
}

void parse_root(std::string b)
{
	//_root = b.substr(0, b.find(';'));
	//std::cout << _root << std::endl;
	std::cout << "lol\n";
	//return (true);
}

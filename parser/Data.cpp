#include "Data.hpp"

#define MAX_PORTS 65535


const char* Data::_SUPPORTED_CGI[] = {"php", "other", "bla", NULL};
const char* Data::_SUPPORTED_METHOD[] = {"GET", "POST", "HEAD", "DELETE", "PUT", NULL};

static bool block_is_closed(std::string s)
{
	int closed = 0;

	for (unsigned long i = 0; i < s.size(); i++)
	{
		if (s[i] == '{')
			closed += 1;
		else if (s[i] == '}')
			closed -= 1;
	}
	if (!closed)
		return (1);
	else
		return (0);
}

Data::Data(const char* file_name)
{
	std::string path;
	std::string b;
	std::string s = "";

	std::string::size_type	i = 0;

	if (file_name == NULL)
		path = DEFAULT_CONFIG_PATH;
	else
		path = file_name;

	std::ifstream file(path);

	if (!file)
		throw (std::logic_error("File path invalid: " + path));

	while (std::getline(file, b)) // first run that take off comments
	{
		s += b.substr(0, b.find('#'));
	//	s += "\n"; // maybe needed to count lines
	}

	if (!block_is_closed(s)) // mettre un check si close
		throw (std::logic_error("Parsing error: missing \'}\'"));

	while (s.size() > 0 && (i = s.find_first_not_of(END_INSTRUCTION_CHAR, i)) != s.npos)
	{
		if (s.size() && s.substr(s.find_first_not_of(END_INSTRUCTION_CHAR, i), 6) != "server")
			throw (std::logic_error("Parsing error: Unknown token: " + s.substr(0, 6)));
		s = s.substr(i + 6);
		if (s.find_first_not_of(WHITESPACE) == s.npos)
			throw (std::logic_error("Parsing_error: Missing token \'{\' after server"));
		s = s.substr(s.find_first_not_of(WHITESPACE));

		if (s[0] != '{')
			throw (std::logic_error("Parsing error: unknown token after server"));

		s = s.substr(s.find('{') + 1);
		//std::cout << s << std::endl;
		if (s.find_first_of(ALPHACHAR) == s.npos)
			throw(std::logic_error("Parsing error: server content empty"));
		if (s.find_first_of(';') == s.npos)
			throw(std::logic_error("Parsing error: Missing \';\' in config content"));

		//s = s.substr(s.find('}'));


		_configList.push_back(Config());
		_configParser.setConfig(&_configList.back(), s);
	}

	check_multiple_ports();
	check_validity();
//	std::cout << _configList.back()._listen << std::endl;
//	std::cout << _configList.front()._listen << std::endl;
//	std::cout << _configList.size() << std::endl;
	/*for (std::vector<Config>::size_type i = 0; i < _configList.size(); i++)
		_configParser.print_data(&_configList[i]);*/

}



Data::Data(const Data& data)
{
	_configList = data._configList;
}

unsigned int Data::getSize()
{
	return (_configList.size());
}

std::vector<Config>& Data::getConfigList()
{
	return (_configList);
}

const Config& Data::operator [] (unsigned int i)
{
	if (i > _configList.size())
		throw (std::out_of_range("Index out of range"));
	return (_configList[i]);
}

Data::~Data()
{

}

void Data::operator = (const Data& data)
{
	_configList = data._configList;
}

/// CHECK ERRORS

static bool path_exists(std::string& s)
{
	struct stat buffer;
	return (stat (s.c_str(), &buffer) == 0);
}

static void check_path_validity(Config& config)
{
	std::string error = "Error on server: " + config._server_name + ": path ";

	if (!path_exists(config._root))
		throw (std::logic_error(error + config._root + " is invalid"));
	if (!path_exists(config._errors))
		throw (std::logic_error(error + config._errors + " is invalid"));
	//if (!path_exists(config._cgi_root))
	//	throw (std::logic_error(error + config._cgi_root + " is invalid"));
	//if (config._directory_listing && !path_exists(config._default_directory_answer_file))
	//	throw (std::logic_error(error + config._default_directory_answer_file + " is invalid"));
	//if (config._send_files && !path_exists(config._files_root))
	//	throw (std::logic_error(error + config._files_root + " is invalid"));
}

static void check_rooting_validity(Config& config)
{
	std::string error = "Error on server: " + config._server_name + ": ";
	int byte;
	int j = 0;

	if (config._listen < 0 || config._listen > MAX_PORTS)
		throw (std::logic_error(error + std::to_string(config._listen) + " port is invalid"));
	//check host
	for (int i = 0; i < 4; i++)
	{
		try
		{
			byte = ft_atoi_base(&config._host[j], "0123456789");
			//byte = std::atoi(&config._host[j]);
			//std::cout << byte << std::endl;
		}
		catch (...)
		{
			throw (std::logic_error(error + "Host " + config._host + " is an invalid ip"));
		}
		if (byte < 0 || byte > 127 || (byte == 0 && config._host[j] != '0'))
			throw (std::logic_error(error + "Host " + config._host + " is an invalid ip"));
		j = config._host.find_first_of('.', j) + 1;
	}
}
/*
static void check_methods_validity(Config& config)
{
	std::string error = "Error on server: " + config._server_name + ": ";
	int j = 0;

	for (std::vector<std::string>::size_type i = 0; i < config._methods.size(); i++)
	{
		j = 0;
		while (1)
		{
			if (Data::_SUPPORTED_METHOD[j] == NULL)
				throw(std::logic_error(error + "Unsupported Method: " + config._methods[i]));
			if (config._methods[i] == Data::_SUPPORTED_METHOD[j])
				break ;
			j++;
		}
	}
	j = 0;
	while (1)
	{
		if (Data::_SUPPORTED_CGI[j] == NULL)
			throw(std::logic_error(error + "Unsupported cgi type: " + config._cgi_type));
		if (config._cgi_type == Data::_SUPPORTED_CGI[j])
			break ;
		j++;
	}
}*/
/*
static void check_server_doubles(std::vector<Config>& configList)
{
	for (std::vector<Config>::size_type i = 0; i < configList.size() - 1; i++)
	{
		for (std::vector<Config>::size_type j = i + 1; j < configList.size(); j++)
			if (configList[i]._listen == configList[j]._listen &&
			configList[i]._host == configList[j]._host) // doublon
				configList.erase(configList.begin() + j);
	}
}
*/
void Data::check_validity()
{
	/*
	** Fonction pour checker si pas de doublons sur les serveurs
	** Si les paths sont valides
	** si les methods et le type de cgi est supporté
	** si les ports et l'host sont valides
	** ??? plus ???
	*/
	// Check path
	for (std::vector<Config>::size_type i = 0; i < _configList.size(); i++)
	{
		try
		{
			check_path_validity(_configList[i]);
		//	check_methods_validity(_configList[i]);
			check_rooting_validity(_configList[i]);
		}
		catch (std::logic_error& e)
		{
			std::cerr << e.what() << std::endl;
			exit(1);
		}
	}
	// check si il y a des doublons sur les servers
//	check_server_doubles(_configList);
//	for (std::vector<Config>::size_type i = 0; i < _configList.size(); i++)
//		_configParser.print_data(&_configList[i]);
}

void Data::check_multiple_ports()
{
	std::vector<Config>::size_type size = _configList.size();

	for (std::vector<Config>::size_type i = 0; i < size; i++)
	{
//		for (unsigned long k = 0; k < _configList[i]._ports.size(); k++)
//			std::cout << "ports: " << _configList[i]._ports[k] << std::endl;
//		std::cout << "1\n";
		if (_configList[i]._ports.size() > 1)
		{
			std::vector<Config>::size_type j = 0;
			while (j < _configList[i]._ports.size() - 1)
			{
				Config b(_configList[i]);
				//std::cout << _configList[i]._ports.size() << "j " << j << std::endl;
				b._listen = _configList[i]._ports[j];
				_configList.push_back(b);
				j++;
			}
		}
	}
}

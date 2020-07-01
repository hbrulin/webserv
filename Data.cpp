#include "Data.hpp"

Data::Data(const char* file_name)
{
	std::string path;
	std::string b;
	std::string s = "";

	int			i = 0;

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
	}

	while (s.size() > 0 && (i = s.find_first_of("server", i)) != s.npos)
	{
		if (s[i + 6] != 32 && s[i + 6] != '{')
			throw (std::logic_error("Parsing error: unknown token after server"));
		s = s.substr(i);
		b = s.substr(s.find('{') + 1, s.find('}') - s.find('{'));
		s = s.substr(s.find('}'));

		/*
		** try (create erver config)
		** catch throw exception
		*/
			//ServerConfig a;
			//a.setConfig(b);
			_serverList.push_back(Config());
			_configParser.setConfig(&_serverList.back(), b);
		//std::cout << b << "||ENDOFB||" << std::endl;
	}

	//std::cout << s << std::endl;
}

Data::Data(const Data& data)
{
	_serverList = data._serverList;
}

unsigned int Data::getSize()
{
	return (_serverList.size());
}

const std::vector<Config>& Data::getServerList()
{
	return (_serverList);
}

const Config& Data::operator [] (unsigned int i)
{
	if (i > _serverList.size())
		throw (std::out_of_range("Index out of range"));
	return (_serverList[i]);
}

Data::~Data()
{

}

void Data::operator = (const Data& data)
{
	_serverList = data._serverList;
}


int main(int ac, char **av)
{
	Data data(av[1]);
}

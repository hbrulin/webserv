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
	//	s += "\n"; // maybe needed to count lines
	}

	while (s.size() > 0 && (i = s.find_first_of("server", i)) != s.npos)
	{
		s = s.substr(i + 6);
		s = s.substr(s.find_first_not_of(WHITESPACE));

		if (s[0] != '{')
			throw (std::logic_error("Parsing error: unknown token after server"));
		if (s.find_first_of('}') == s.npos)
			throw (std::logic_error("Parsing error: missing \'}\'"));

		b = s.substr(s.find('{') + 1, s.find('}') - s.find('{'));
		if (b.find_first_of(ALPHACHAR) == b.npos)
			throw(std::logic_error("Parsing error: server content empty"));
		if (b.find_first_of(';') == b.npos)
			throw(std::logic_error("Parsing error: Missing \';\' in config content"));

		s = s.substr(s.find('}'));


		_serverList.push_back(Config());
		_configParser.setConfig(&_serverList.back(), b);
	}
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
	try
	{
		Data data(av[1]);
		std::cout << "here: " << data[0]._server_name << std::endl;
		std::cout << "here: " << data[1]._server_name << std::endl;

	}
	catch (std::logic_error& e)
	{
		std::cout << e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "Error while configuring, abort ...\n";
	}

}

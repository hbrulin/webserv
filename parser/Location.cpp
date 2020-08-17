#include "Location.hpp"

const char* Location::_SUPPORTED_CGI[] = {"php", "other", NULL};
const char* Location::_SUPPORTED_METHOD[] = {"GET", "POST", "HEAD", "DELETE", "PUT", NULL};

Location::Location()
{
	_name = "";
	_root = "";
	_mode = "";
	_try_files = "";
	_index = "";
	_allow = "";
	_body_size = 0;
	_uploaded_files_root = "";
	_send_files = false;
	_cgi_type = "";
	_cgi_root = "";
	_directory_listing = false;
	_directory_answer_file = "";

	_methods.push_back("GET");
	initiate_map();
}

void Location::initiate_map()
{
	_map["root"] = &Location::parse_root;
	_map["try_files"] = &Location::parse_try_files;
	_map["index"] = &Location::parse_index;
	_map["allow"] = &Location::parse_allow;
	_map["methods"] = &Location::parse_method;
	_map["cgi_root"] = &Location::parse_cgi_root;
	_map["cgi_type"] = &Location::parse_cgi_type;
	_map["allow_directory_listing"] = &Location::parse_directory_listing;
	_map["directory_answer_file"] = &Location::parse_default_directory_answer_file;
	_map["body_size"] = &Location::parse_body_size;
	_map["send_files"] = &Location::parse_send_files;
	_map["uploaded_files_root"] = &Location::parse_uploaded_files_root;
}

Location::~Location()
{}

Location::Location(const Location& l)
{
	_name = l._name;
	_root = l._root;
	_mode = l._mode;
	_try_files = l._try_files;
	_index = l._index;
	_allow = l._allow;
	_methods = l._methods;
	_cgi_root = l._cgi_root;
	_cgi_type = l._cgi_type;
	_body_size = l._body_size;
	_send_files = l._send_files;
	_uploaded_files_root = l._uploaded_files_root;
	_directory_listing = l._directory_listing;
	_directory_answer_file = l._directory_answer_file;
	initiate_map();
}

void Location::operator = (const Location& l)
{
	_name = l._name;
	_root = l._root;
	_mode = l._mode;
	_try_files = l._try_files;
	_index = l._index;
	_allow = l._allow;
	_methods = l._methods;
	_cgi_root = l._cgi_root;
	_cgi_type = l._cgi_type;
	_body_size = l._body_size;
	_send_files = l._send_files;
	_uploaded_files_root = l._uploaded_files_root;
	_directory_listing = l._directory_listing;
	_directory_answer_file = l._directory_answer_file;
	initiate_map();
}

void Location::parse(std::string b)
{
	// parse mode
	// parse name
	// parse root

	_root = "";
	if (b.find('/') == b.npos)
		throw(std::logic_error("Bad location block: Missing name"));
	_mode = b.substr(0, b.find('/'));
	remove_whitespace(_mode);
	if (!check_mode())
		throw(std::logic_error("Bad location block: Unrecognized mode: '" + _mode + "'"));
	b = b.substr(b.find('/'));

	if (b.find('{') == b.npos)
		throw(std::logic_error("Bad location block: Missing token '{' after name"));

	_name = b.substr(0, b.find('{'));
	_name = _name.substr(1); // { inclus si substr(1, find({)) ?????
	remove_whitespace(_name);

	//if (!_name.empty()) // pas sur de ca
	_name = "/" + _name;

	b = b.substr(b.find('{') + 1);

//	std::cout << "Mode: " << _mode << std::endl;
//	std::cout << "Name: " << _name << std::endl;

	std::string key = b;
	std::string value = "";
	remove_whitespace(key);
	if (key.empty())
		return ;

	while (b.size())
	{
		if (b.find_first_not_of(END_INSTRUCTION_CHAR) == b.npos)
			break ;
		if (b.find(';') == b.npos)
			throw (std::logic_error("Bad location block: Missing end instruction token ';'"));
		b = b.substr(b.find_first_not_of(END_INSTRUCTION_CHAR));
		key = b.substr(0, b.find_first_of(END_INSTRUCTION_CHAR));
		remove_whitespace(key);
//		std::cout << "key: " << key << std::endl;
		b = b.substr(key.size());
		value = b.substr(0, b.find(';'));
		remove_whitespace(value);
		b = b.substr(b.find(';') + 1);

		fmap::iterator it;
		it = _map.find(key);
		if (it == _map.end())
			throw(std::logic_error("Bad location block: Unrecognized key: " + key));
		(this->*(_map[key]))(value);
	}
	check_methods_validity();
	//print();
}

// Parsers lnked to the fmap like in Location
void Location::parse_root(std::string b)
{
	remove_whitespace(b);
	_root = b;
}

void Location::parse_try_files(std::string b)
{
	remove_whitespace(b);
	_try_files = b;
}

void Location::parse_index(std::string b)
{
	remove_whitespace(b);
	_index = b;
}

void Location::parse_allow(std::string b)
{
	remove_whitespace(b);
	_allow = b;
}

void Location::parse_method(std::string b)
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
		_methods.push_back(s);

		if (b.find_first_of(END_INSTRUCTION_CHAR, b.find_first_of(ALPHACHAR)) == std::string::npos)
			break;
		b = b.substr(b.find_first_of(END_INSTRUCTION_CHAR, b.find_first_of(ALPHACHAR)));
	}
}

void Location::remove_whitespace(std::string& s)
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

// ACCESS

std::string Location::get_path()
{
	return (_root + _name);
}


bool Location::check_mode()
{
	if (!_mode.empty() && _mode != "=" && _mode != "~" && _mode != "~*" && _mode != "^~")
		return (false);
	return (true);
}

void Location::print()
{
	std::cout << "\n-----------\n";
 	std::cout << "name: " << _name << std::endl << "root: " << _root << std::endl <<
	"_mode: " << _mode << std::endl << "index: " << _index << std::endl <<
	"try files: " << _try_files << std::endl << "allow: " << _allow << std::endl;
	std::cout << "Methods:";
	for (unsigned long i = 0; i < _methods.size(); i++)
	{
		std::cout << " " << _methods[i];
	}

	std::cout << "\nclient body stuff: " << _body_size
	<< "\nAllow_uploaded: " << _send_files << "\nuploaded_files_root: " << _uploaded_files_root
	<< "\nAllow_directory_listing: " << _directory_listing <<
	"\ndefault_directory_answer_file: " << _directory_answer_file;

	std::cout << "\nCGI_ROOT: " << _cgi_root << "\nCGI_TYPE: " << _cgi_type;
	std::cout << std::endl;
}

std::string Location::get_name()
{
	return (_name);
}

///////////////////

void Location::parse_cgi_root(std::string b)
{
	remove_whitespace(b);
	_cgi_root = b;
}

void Location::parse_cgi_type(std::string b)
{
	remove_whitespace(b);
	std::transform(b.begin(), b.end(), b.begin(), ::tolower);
	_cgi_type = b;
}

void Location::parse_directory_listing(std::string b)
{
	remove_whitespace(b);
	if (b == "yes")
		_directory_listing = true;
	else if (b == "no")
		_directory_listing = false;
	else
		throw(std::logic_error("Bad Location block: Allow_directory_listing: Must be yes or no"));
}

void Location::parse_default_directory_answer_file(std::string b)
{
	remove_whitespace(b);
	_directory_answer_file = b;
}

void Location::parse_send_files(std::string b)
{
	remove_whitespace(b);
	if (b == "yes")
		_send_files = true;
	else if (b == "no")
		_send_files = false;
	else
		throw(std::logic_error("Bad Location block: Allow_uploaded: Must be yes or no"));
}

void Location::parse_uploaded_files_root(std::string b)
{
	remove_whitespace(b);
	_uploaded_files_root = b;
}

void Location::parse_body_size(std::string b)
{
	remove_whitespace(b);
	_body_size = stoi(b);
}


// checks :

void Location::check_methods_validity()
{
	int j = 0;

	for (std::vector<std::string>::size_type i = 0; i < _methods.size(); i++)
	{
		j = 0;
		while (1)
		{
			if (Location::_SUPPORTED_METHOD[j] == NULL)
				throw(std::logic_error("Bad location Block: Unsupported Method: " + _methods[i]));
			if (_methods[i] == Location::_SUPPORTED_METHOD[j])
				break ;
			j++;
		}
	}
	j = 0;
	while (1 && !_cgi_type.empty())
	{
		if (Location::_SUPPORTED_CGI[j] == NULL)
			throw(std::logic_error("Bad location Block: Unsupported cgi type: " + _cgi_type));
		if (_cgi_type == Location::_SUPPORTED_CGI[j])
			break ;
		j++;
	}
}

/*
void check_path_validity()
{
	if (!path_exists(root))
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
*/

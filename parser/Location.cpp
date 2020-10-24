#include "Location.hpp"

const char* Location::_SUPPORTED_CGI[] = {"php", "bla", NULL};
const char* Location::_SUPPORTED_METHOD[] = {"GET", "POST", "HEAD", "DELETE", "PUT", NULL};

// Faire une loc par defaut

static bool path_exists(std::string& s)
{
	struct stat buffer;
	return (stat (s.c_str(), &buffer) == 0);
}

void Location::check_path_validity()
{
	std::string error = "Error on location: " + _name + ": path ";

/*	if (!path_exists(_root))
		throw (std::logic_error(error + _root + " is invalid"));*/
	//if (!path_exists(_errors))
	//	throw (std::logic_error(error + _errors + " is invalid"));

	for (std::map<int,std::string>::iterator it = _errors.begin(); it != _errors.end(); it++)
	{
		if (!path_exists(it->second))
			throw (std::logic_error(error + it->second + " is invalid"));
	}
	//if (!path_exists(config._cgi_root))
	//	throw (std::logic_error(error + config._cgi_root + " is invalid"));
	//if (config.__autoindex && !path_exists(config._default_directory_answer_file))
	//	throw (std::logic_error(error + config._default_directory_answer_file + " is invalid"));
	//if (config._send_files && !path_exists(config._files_root))
	//	throw (std::logic_error(error + config._files_root + " is invalid"));
}

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
	_cgi_file = "";
	_autoindex = false;
	//_errors = ""; OBSOLETE

	//_methods.push_back("GET");
	//set_default_errors();
	//_methods.push_back("HEAD");
//	set_default_errors();
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
	_map["cgi_file"] = &Location::parse_cgi_file;
	_map["autoindex"] = &Location::parse_autoindex;
	_map["body_size"] = &Location::parse_body_size;
	_map["send_files"] = &Location::parse_send_files;
	_map["uploaded_files_root"] = &Location::parse_uploaded_files_root;
	_map["errors"] = &Location::parse_errors;
	//_map["error"] = &Location::parse_error;
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
	_cgi_file = l._cgi_file;
	_body_size = l._body_size;
	_send_files = l._send_files;
	_uploaded_files_root = l._uploaded_files_root;
	_autoindex = l._autoindex;
	_errors = l._errors;
	//_error = l._error;
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
	_cgi_file = l._cgi_file;
	_body_size = l._body_size;
	_send_files = l._send_files;
	_uploaded_files_root = l._uploaded_files_root;
	_autoindex = l._autoindex;
	_errors = l._errors;
	//_error = l._error;
	initiate_map();
}

void Location::parse(std::string b)
{
	// parse mode
	// parse name
	// parse root
	unsigned long name_end;
	unsigned long arg_end = b.find('{');


	_root = "";
	if ((b.find('/') == b.npos || b.find('/') > arg_end) && (b.find('.') == b.npos || b.find('.') > arg_end))
		throw(std::logic_error("Bad location block: Missing name"));


	if (b.find('/') != b.npos && b.find('/') < arg_end)
		name_end = b.find('/');
	else
		name_end = b.find('.');
	_mode = b.substr(0, name_end);
	//std::cout << "MODE: " << _mode << std::endl;
	remove_whitespace(_mode);

	if (!check_mode())
		throw(std::logic_error("Bad location block: Unrecognized mode: '" + _mode + "'"));
	b = b.substr(name_end);

	if (b.find('{') == b.npos)
		throw(std::logic_error("Bad location block: Missing token '{' after name"));

	_name = b.substr(0, b.find('{'));
	//_name = _name.substr(1); // { inclus si substr(1, find({)) ?????
	remove_whitespace(_name);
	if (_name[_name.size() - 1] != '/' && _name[0] != '.')
		_name.push_back('/');

	b = b.substr(b.find('{') + 1);

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
	return (_root);
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
	<< "\nAllow_autoindex: " << _autoindex;

	std::cout << "\nCGI_ROOT: " << _cgi_root << "\nCGI_TYPE: " << _cgi_type
	<< "\nCGI_FILE: " << std::endl;;
	for (std::map<int,std::string>::iterator it = _errors.begin(); it != _errors.end(); it++)
		std::cout << it->first << ": " << it->second << std::endl;

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

void Location::parse_autoindex(std::string b)
{
	remove_whitespace(b);
	if (b == "yes")
		_autoindex = true;
	else if (b == "no")
		_autoindex = false;
	else
		throw(std::logic_error("Bad Location block: Allow_autoindex: Must be yes or no"));
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
	int body = stoi(b);
	if (body < 0)
		throw(std::logic_error("Bad Location block: Body size cannot be negative"));
	_body_size = body;
}

void Location::parse_cgi_file(std::string b)
{
	remove_whitespace(b);
	//if (b.find_first_of('.') == b.npos || b.find_first_of('.') != b.find_last_of('.'))
	//	throw(std::logic_error("Bad Location block: 'invalid cgi file:'must contains '.''"));
	_cgi_file = b;
}

/*
void Location::parse_errors(std::string b)
{
	remove_whitespace(b);
	_errors = b;
}
*/
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
/*	while (1 && !_cgi_type.empty())
	{
		if (Location::_SUPPORTED_CGI[j] == NULL)
			throw(std::logic_error("Bad location Block: Unsupported cgi type: " + _cgi_type));
		if (_cgi_type == Location::_SUPPORTED_CGI[j])
			break ;
		j++;
	}*/
}

bool Location::check_allowed_method(std::string method, std::string request_uri)
{
	//std::cout << _methods.size() <<  std::endl;
	for (unsigned long i = 0; i < _methods.size(); i++)
	{
		//std::cout << _methods[i] << std::endl;
		if (_methods[i] == method)
			return (true);
	}
	_cgi_method = "POST";
	if (_cgi_method == method && request_uri.find(_cgi_type) != std::string::npos)
		return (true);
	return (false);
}

std::string Location::get_index()
{
	return (_index);
}

std::string Location::get_index_path()
{
	return (get_path() + _index);
}

unsigned int Location::get_body_size()
{
	return (_body_size);
}

std::string		Location::get_cgi_root()
{
	return (_cgi_root);
}

std::string		Location::get_listing()
{
	if (!_autoindex)
		return ("Directory listing not allowed");
	// return a string with all files in location
	return ("directory listing on");
}

void Location::parse_errors(std::string b)
{
	std::string code = "";
	std::string path = "";

	while (b.size())
	{
		code = b.substr(
		b.find_first_of("0123456789"),
		b.find_first_of(END_INSTRUCTION_CHAR, b.find_first_of("0123456789")));
		//std::cout << s << std::endl;
		remove_whitespace(code);

		if (b.find_first_of(END_INSTRUCTION_CHAR, b.find_first_of("0123456789")) == std::string::npos)
			break;
		b = b.substr(b.find_first_of(END_INSTRUCTION_CHAR, b.find_first_of(ALPHACHAR)));
		//std::cout << "Code: " << code;
		path = b.substr(
		b.find_first_of(ALPHACHAR),
		b.find_first_of(END_INSTRUCTION_CHAR, b.find_first_of(ALPHACHAR)));
		//std::cout << " Path: " << path << std::endl;


		_errors[std::stoi(code)] = path;
		if (b.find_first_of(END_INSTRUCTION_CHAR, b.find_first_of("0123456789")) == std::string::npos)
			break;
		b = b.substr(b.find_first_of(END_INSTRUCTION_CHAR, b.find_first_of(ALPHACHAR)));

		//std::cout << "b: " << b << std::endl;
	}
}


void Location::set_default_errors(std::map<int,std::string> errors)
{
	for (std::map<int,std::string>::iterator it = errors.begin(); it != errors.end(); it++)
	{
		if (_errors.find(it->first) == _errors.end() ||
		(_errors.find(it->first) != _errors.end() && _errors.find(it->first)->second.empty()))
			_errors[it->first] = it->second;
	}
}

std::string Location::get_error_path(int code)
{
	if (_errors.find(code) != _errors.end())
		return (_errors[code]);
	return ("");
}

std::string Location::get_autoindex()
{
	std::string path = _root; // not sure

	std::string list = "<!DOCTYPE html>\n\t<head>\n\t\t<title" + path
	+ "</title>\n\t</head>\n\t\t<body><p>";

	if (_autoindex == false)
	{
		return (_errors[404]);
	}

	//if (path + partial_path doesn't exists return 404)

	struct dirent *files;
	DIR *current = opendir(path.c_str());
	if (current == NULL)
		return (_errors[404]);
	while ((files = readdir(current)) != NULL)
	{
		list.append("\t\t<a href =\"");
		list.append(std::string(files->d_name));
		list.append("\">");
		list.append("-" + std::string(files->d_name));
		list.append("</a>\n");
	}
	list.append("\t</p>\n\t\t</body>\n\t</html>\n");
	closedir(current);

	int fd = open((path + "list.html").c_str(), O_CREAT | O_WRONLY, S_IRWXO);

	write(fd, list.c_str(), list.size());// << std::endl;
	close(fd);
	return ((path + "list.hmtl"));
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
	//if (config.autoindex && !path_exists(config._default_directory_answer_file))
	//	throw (std::logic_error(error + config._default_directory_answer_file + " is invalid"));
	//if (config._send_files && !path_exists(config._files_root))
	//	throw (std::logic_error(error + config._files_root + " is invalid"));
}
*/

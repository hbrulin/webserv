#include "Config.hpp"
#include "../utils/definitions.hpp"

Config::Config()
{
	_server_name = "";
	//_errors = DEFAULT_ERROR_PAGES; -> OBSOLETE
	_listen = 80;
	_root = "";
	_host = "0.0.0.0";
	_body_size = 120000000;
	set_default_errors();
	//_ports.push_back(80);
}


Config::Config(const Config& b)
{
	_server_name = b._server_name;

	_listen = b._listen;
	_host = b._host;

	_root = b._root;

	_ports = b._ports;
	_body_size = b._body_size;
	_locations = Locations(b._locations);

	// a retirer des qu'on peut:
	_cgi_type = b._cgi_type;
	_cgi_root = b._cgi_root;
	//_errors= b._error;
	_errors = b._errors;

}

void Config::operator = (const Config& b)
{
	_server_name = b._server_name;

	_listen = b._listen;
	_host = b._host;

	_root = b._root;

	_ports = b._ports;
	_body_size = b._body_size;
	_locations = Locations(b._locations);

	// a retirer des qu'on peut:
	_cgi_type = b._cgi_type;
	_cgi_root = b._cgi_root;
	//_error = b._error;
	_errors = b._errors;
}

void Config::set_blank()
{
	_locations._blank._body_size = _body_size;
	_locations._blank._errors = _errors;
	//_locations._blank._error = _error;
	_locations._blank._root = _root;
	_locations._blank._cgi_root = _cgi_root;
	_locations._blank._cgi_type = _cgi_type;
}

void Config::set_default_locations()
{
	for (unsigned long i = 0; i < _locations.size(); i++)
	{
	//	if (_locations[i]._errors.empty())
	//		_locations[i]._errors = _errors;
		if (_locations[i]._body_size == 0)
			_locations[i]._body_size = _body_size;
		_locations[i].set_default_errors(_errors);
	}
}

void Config::set_default_errors()
{
	if (_errors.find(405) == _errors.end())
		_errors[405] = NOT_ALLOWED;
	if (_errors.find(406) == _errors.end())
		_errors[406] = NOT_ACCEPTABLE;
	if (_errors.find(400) == _errors.end())
		_errors[400] = BAD_REQUEST;
	if (_errors.find(401) == _errors.end())
		_errors[401] = UNAUTHORIZED;
	if (_errors.find(505) == _errors.end())
		_errors[505] = NOT_SUPPORTED;
	if (_errors.find(404) == _errors.end())
		_errors[404] = DEF_ERR_PAGE;
}



/* mettre dans les loc:
body size, errors, (cgi type?)
*/

#include "Config.hpp"

Config::Config()
{
	_server_name = "";
	_errors = DEFAULT_ERROR_PAGES;
	_listen = 80;
	_root = "";
	_host = "0.0.0.0";
	_body_size = 120000000;
	//_ports.push_back(80);
}


Config::Config(const Config& b)
{
	_server_name = b._server_name;
	_errors = b._errors;
	_listen = b._listen;
	_host = b._host;

	_root = b._root;

	_ports = b._ports;
	_body_size = b._body_size;
	_locations = Locations(b._locations);

	// a retirer des qu'on peut:
	_cgi_type = b._cgi_type;
	_cgi_root = b._cgi_root;

}

void Config::set_blank()
{
	_locations._blank._body_size = _body_size;
	_locations._blank._errors = _errors;
	_locations._blank._root = _root;
	_locations._blank._cgi_root = _cgi_root;
	_locations._blank._cgi_type = _cgi_type;
}

void Config::set_default_locations()
{
	for (unsigned long i = 0; i < _locations.size(); i++)
	{
		if (_locations[i]._errors.empty())
			_locations[i]._errors = _errors;
		if (_locations[i]._body_size == 0)
			_locations[i]._body_size = _body_size;
	}
}



/* mettre dans les loc:
body size, errors, (cgi type?)
*/

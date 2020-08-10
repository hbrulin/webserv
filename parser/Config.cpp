#include "Config.hpp"

Config::Config()
{
	_server_name = "";
	_errors = DEFAULT_ERROR_PAGES;
	_listen = 80;
	_host = "0.0.0.0";
}

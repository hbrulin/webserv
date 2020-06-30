#include "env.hpp"

Env::Env() {
	//set default to something
	AUTH_TYPE = "default";
	CONTENT_LENGTH = "default";
	CONTENT_TYPE = "default";
	GATEWAY_INTERFACE = "default";
	PATH_INFO = "default";
	PATH_TRANSLATED = "default";
	QUERY_STRING = "default";
	REMOTE_ADDR = "default";
	REMOTE_IDENT = "default";
	REMOTE_USER = "default";
	REQUEST_METHOD = "default";
	REQUEST_URI = "default";
	SCRIPT_NAME = "default";
	SERVER_NAME = "default";
	SERVER_PORT = "default";
	SERVER_PROTOCOL = "default";
	SERVER_SOFTWARE = "default";
}

/*Env::parse_config(int config_file) {

}*/
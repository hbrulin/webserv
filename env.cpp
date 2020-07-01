#include "env.hpp"

/*
**	AUTH_TYPE=""; #identification type if applicable
**	 ?????
**	CONTENT_LENGHT=""; # size of input data if put or post method are used
**		if not specified if should be until EOF for us i guess
**	CONTENT_TYPE=""; # Internet media type of input data if put or post are used
**	GATEWAY_INTERFACE=""; # CGI/version
**	PATH_INFO=""; # path suffix apprended to URL after program name and a slash
**		# -> basically name of the program like /cgi-bin/or CGI_ROOT/
**	PATH_TRANSLATED=""; # full path as supposed by server, if PATH_INFO is present
**			# ???? guess it's CGI_ROOT + PATH_INFO
**	QUERY_STRING=""; #QUERY_STRING: the part of URL after ? character.
**		# The query string may be composed of *name=value pairs
**		# separated with ampersands (such as var1=val1&var2=val2...)
**		# when used to submit form data transferred via GET method as
**		# defined by HTML application/x-www-form-urlencoded.
**		# used to past specific vaiables to script i guess
**	REMOTE_ADDR=""; # IP address of the client (decimal)
**	REMOTE_IDENT=""; # client Ident i guess
**	REMOTE_USER=""; # Used for cetrian AUTH_TYPE ????
**	REQUEST_METHOD=""; # name of HTTP METHOD -> GET or POST
**	REQUEST_URL=""; # the all URL i guess -> CGI_ROOT + PATH_INFO + QUERY_STRING + SCRIPT_NAME
**	SCRIPT_NAME=""; # relative path of program like /cgi-bin/script.cgi
**	SERVER_NAME=""; # host name of the server, like local host
**	SERVER_PORT=""; # TCP port (decimal) -> 8080
**	SERVER_PROTOCOL=""; # HTTP/VERSION -> HTTP/1.1
**	SERVER_SOFTWARE=""; # name version of HTTP server ->webserv/1.1
*/

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

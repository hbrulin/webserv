#ifndef DEFINITIONS_HPP
# define DEFINITIONS_HPP

//parse config
#define LOCALHOST "127.0.0.1"
#define DEFAULT_ERROR_PAGES "www/"
#define ALPHACHAR "ABCEDFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"
#define END_INSTRUCTION_CHAR " \t\n\r;}"
#define WHITESPACE " \t\n\r "
#define NUMBER_OF_PARAMETERS 13
#define DEFAULT_CONFIG_PATH "./config/default.conf"
#define LOCEXCEPTION "Bad location block: "

//ENDL
#define ENDCHARS "\r\n\r\n"
#define ENDCHARS_BOD "0\r\n\r\n"

//sizes
#define INT_MIN_STR "-2147483648"
#define BUFFER_SIZE 100000000

//METHODS
#define POST "POST"
#define PUT "PUT"
#define GET "GET"
#define HEAD "HEAD"
#define DELETE "DELETE"

//ERRORS PAGES AND CODES
#define NOT_ALLOWED "405.html"
#define NOT_ACCEPTABLE "406.html"
#define BAD_REQUEST "400.html"
#define UNAUTHORIZED "401.html"
#define NOT_SUPPORTED "505.html"
#define DEFAULT_CODE 200
#define ERR_STAT "erreur stat\n"
#define PIPE_ERR "pipe error\n"
#define WAIT_ERR "wait error\n"
#define FORK_ERR "fork error\n"

//META_VAR
#define DEF_PROTOCOL "HTTP/1.1"

//HEADERS
#define CHUNKED "chunked"
#define CONTENT_T "Content-Type: "
#define CONTENT_L "Content-Length: "
#define HOST_STR "Host: "

//HEADERS CGI
#define STATUS "Status: "

//FOLDERS AND FILES
#define YOUPIBANANE "YoupiBanane/"
#define UPLOADED "uploaded"
#define OUTPUT_CGI "/cgi-bin/cgi_output_"

//EXTENSIONS
#define PHP ".php"

#endif
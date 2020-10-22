#ifndef DEFINITIONS_HPP
# define DEFINITIONS_HPP

typedef enum	e_status
{
	PARSE,
	READ_FILE, //-> il faut creer nouvelles fts
	WRITE_FILE, 
	DONE // (si error ou si tout est ok)
}				t_status;


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

//sizes and formats
#define INT_MIN_STR "-2147483648"
#define BUFFER_SIZE 1000000
#define TIME_FORMAT "%Y-%m-%d %H:%M"
#define GMT " GMT"

//METHODS
#define POST "POST"
#define PUT "PUT"
#define GET "GET"
#define HEAD "HEAD"
#define DELETE "DELETE"

//ERRORS PAGES AND CODES
#define NOT_ALLOWED "www/error_pages/405.html"
#define NOT_ACCEPTABLE "www/error_pages/406.html"
#define BAD_REQUEST "www/error_pages/400.html"
#define UNAUTHORIZED "www/error_pages/401.html"
#define NOT_SUPPORTED "www/error_pages/505.html"
#define PARTIAL_CONTENT "www/error_pages/206.html"
#define AUTHORIZATION_REQUIRED "www/error_pages/401.html"
#define VERSION_NOT_SUPPORTED "www/error_pages/505.html"
#define LENGTH_REQUIRED "www/error_pages/411.html"
#define DEFAULT_CODE 200
#define ERR_STAT "erreur stat\n"
#define PIPE_ERR "pipe error\n"
#define WAIT_ERR "wait error\n"
#define FORK_ERR "fork error\n"

//META_VAR
#define DEF_PROTOCOL "HTTP/1.1"

//HEADERS
#define CHUNKED_STR "chunked"
#define CONTENT_T_STR "Content-Type: "
#define CONTENT_L_STR "Content-Length: "
#define HOST_STR "Host: "
#define CACHE_STR "Cache-Control: no-cache, private\r\n"
#define CONTENT_LANG_STR "Content-Langage: "
#define CONTENT_LOC_STR "Content-Location: "
#define USER_AGENT_STR "User-Agent: "
#define REFERER_STR "Referer: "
#define ACCEPT_LAN_STR "Accept-Langage: "
#define TRANSFER_EN_STR "Transfer-Encoding: "
#define AUTH_STR "Authorization: "
#define ACCEPT_EN_STR "Accept-Encoding: "
#define ACCEPT_CHAR_STR "Accept-Charset: "

//HEADERS RESP
#define DATE_STR "Date: "
#define SERVER_STR "Server: "
#define LAST_MOD_STR "Last-Modified: "
#define RETRY_STR "Retry-after: "
#define ALLOW_STR "Allow: "
#define SERVER_METHODS_STR "Server methods: "
#define SECRET_STR "http-X-Secret-Header-For-Test: "

//HEADERS CGI
#define STATUS_STR "Status: "

//FOLDERS AND FILES
#define CONTENT_NEGO_AVAILABLE "www/"
#define OUTPUT_CGI "/cgi-bin/cgi_output_"
#define POST_HTML "www/post.html"
#define ERROR_FOLDER "error_pages/"
#define DEF_ERR_PAGE "www/error_pages/404.html"

//EXTENSIONS
#define PHP ".php"
#define CGI ".cgi"

#endif

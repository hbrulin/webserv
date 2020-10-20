#ifndef HEAD_REQ_HPP
#define HEAD_REQ_HPP

#include <iostream>
//#include <sys/stat.h>
//#include <sys/time.h>
//#include <vector>
#include <string>
//#include <iterator>
#include "../utils/libft.h"
#include "../parser/Config.hpp"
#include "../parser/Location.hpp"
#include <sstream>
//#include <fstream>
//#include <iostream>
//#include <cctype>
#include "../utils/definitions.hpp"

class Head_req
{
    public:
    Head_req() : HOST(LOCALHOST) { 
    }
    // response
    //request 
    //to check if ok ??? How to implement
    char        **ACCEPT_CHARSET;
    std::vector<std::string> ACCEPT_LANGUAGE;
    //char **AUTHORIZATION;
    std::string HOST; //OK + Add PORT according to config_file -> déjà dans la requete
    std::string REFERER; // How to get that > Link ???
    std::string USER_AGENT; // OS, version, application
    std::string REMOTE_ADDR;
	std::string DATE; // HH:MM:SS GMT
	std::string TRANSFER_ENCODING;
	std::string ACCEPT_ENCODING;
     std::string X_headers;

    //META
    std::string str;
    std::string AUTH_TYPE;
    std::string CONTENT_TYPE;
    std::string CONTENT_LENGTH;
    std::string REQUEST_METHOD;
    std::string QUERY_STRING;
    std::string SERVER_PROTOCOL;
    std::string SCRIPT_NAME;
    std::string PATH_INFO;
    std::string PATH_TRANSLATED;
    std::string REQUEST_URI;
    std::string SERVER_PORT;
    std::string SERVER_NAME;
    // std::string SERVER_SOFTWARE;
    //functions
    std::string getDate();
    std::string getReferer(std::string s);
    std::string getUserAgent(std::string s);
	std::string getTransferEncoding(std::string s);
    void        getRemAddr();
    std::string getAcceptLangage(std::string s);
    std::string getStringtoParse(std::string s, std::string toParse);
    std::string getMetatoParse(std::string s, std::string toParse, std::string Sep);
    std::string get_meta();
    void        getScriptName(std::string s);
	std::string contentNego(std::string root);
	void		parse(std::string s, std::string url);
    std::string getXtoparse(std::string s, std::string toParse);
    struct stat st;
};

#endif
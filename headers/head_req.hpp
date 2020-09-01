#ifndef HEAD_REQ_HPP
#define HEAD_REQ_HPP

#include <iostream>
#include <sys/stat.h>
#include <sys/time.h>
#include <vector>
#include <string>
#include <iterator>
#include "../libft/libft.h"
#include "../parser/Config.hpp"
#include <sstream>
#include <fstream>
#include <iostream>

class Head_req
{
    public:
    Head_req() : HOST("127.0.0.1") { //CHANGER SELON CONFIG
        // char str[3] = "da";
        //ACCEPT_LANGUAGE = NULL;
        // char accp_char_def[2] = "*";
        // ACCEPT_CHARSET[0] = accp_char_def; 
    }
    // response
    //request 
    //to check if ok ??? How to implement
    char        **ACCEPT_CHARSET;
    std::vector<std::string> ACCEPT_LANGUAGE;
    char **AUTHORIZATION;
    std::string HOST; //OK + Add PORT according to config_file -> déjà dans la requete
    std::string REFERER; // How to get that > Link ???
    std::string USER_AGENT; // OS, version, application
	std::string DATE; // HH:MM:SS GMT
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
    std::string SERVER_NAME; //déjà dans le listener
    char **TRANSFER_ENCODING;
    // std::string SERVER_SOFTWARE;
    //functions
    std::string getDate();
    std::string getReferer(char *string);
    std::string getUserAgent(char *string);
    std::string getAcceptLangage(char *string);
    std::string getStringtoParse(char *m_buffer, std::string toParse);
    std::string getMetatoParse(char *m_buffer, std::string toParse, std::string);
    std::string get_meta(Config _Config);
    void        getScriptName(char *m_buffer);
	std::string contentNego(std::string root, std::string content);
	void		parse(std::vector<std::string> parsed, char *m_buffer, std::string url);
    struct stat st;
};

#endif
#ifndef HEADER_HPP
#define HEADER_HPP

#include <iostream>
#include <sys/stat.h>
#include <sys/time.h>

class Header
{
    public:
    Header() : LOCATION("default"), DATE("default"), LAST_MODIFIED("default"), HOST("127.0.0.1"), SERVER("webserver") {}
    // response
    std::string ALLOW; // kind of request method allowed
    std::string CONTENT_LANGUAGE;
    std::string CONTENT_LENGTH;
    std::string CONTENT_LOCATION;
    std::string CONTENT_TYPE;
    std::string LOCATION;
    std::string LAST_MODIFIED;
    std::string RETRY_AFTER; // 2 lignes date + delay-second
    std::string SERVER; //OK
    std::string TRANSFER_ENCODING;
    std::string WWW_AUTHENTICATE;
    //request 
    //to check if ok ??? How to implement
    std::string ACCEPT_CHARSET;
    std::string ACCEPT_LANGUAGE;
    std::string AUTHORIZATION;
    std::string HOST; //OK + Add PORT according to config_file
    std::string REFERER; // How to get that > Link ???
    std::string USER_AGENT; // OS, version, application
    //Both
	std::string DATE; // HH:MM:SS GMT
    std::string getLastModified(const char *path);
    std::string getDate();
    struct stat st;
};

#endif
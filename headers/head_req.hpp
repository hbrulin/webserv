#ifndef HEAD_REQ_HPP
#define HEAD_REQ_HPP

#include <iostream>
#include <sys/stat.h>
#include <sys/time.h>

class Head_req
{
    public:
    Head_req() : ACCEPT_CHARSET("default"), ACCEPT_LANGUAGE("default"), AUTHORIZATION("default"), HOST("127.0.0.1"), USER_AGENT("webserver") {}
    // response
    //request 
    //to check if ok ??? How to implement
    std::string ACCEPT_CHARSET;
    std::string ACCEPT_LANGUAGE;
    std::string AUTHORIZATION;
    std::string HOST; //OK + Add PORT according to config_file
    std::string REFERER; // How to get that > Link ???
    std::string USER_AGENT; // OS, version, application
	std::string DATE; // HH:MM:SS GMT
    std::string getDate();
    struct stat st;
};

#endif
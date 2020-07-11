#ifndef HEAD_RESP_HPP
#define HEAD_RESP_HPP

#include <iostream>
#include <sys/stat.h>
#include <sys/time.h>
#include <sstream>

class Head_resp
{
    public:
    Head_resp() : LOCATION("default"), DATE("default"), LAST_MODIFIED("default"), SERVER("webserver"), RETRY_AFTER("02:00") {}
    // response
    std::string ALLOW; // kind of request method allowed
    std::string CONTENT_LANGUAGE;
    std::string CONTENT_LENGTH; //OK
    std::string CONTENT_LOCATION;
    std::string CONTENT_TYPE;
    std::string LOCATION; 
    std::string LAST_MODIFIED; //OK
    std::string SERVER; //OK
    // SI 503
    std::string RETRY_AFTER; // 2 lignes date + delay-second
    std::string TRANSFER_ENCODING;
    std::string WWW_AUTHENTICATE;
	std::string DATE; // HH:MM:SS GMT //OK
    std::string getLastModified(const char *path);
    std::string getDate();
    std::string getBuffer(int code, int length, const char *path);
    struct stat st;
};

#endif
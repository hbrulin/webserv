#ifndef HEAD_RESP_HPP
#define HEAD_RESP_HPP

#include <iostream>
#include <sys/stat.h>
#include <sys/time.h>

class Head_resp
{
    public:
    Head_resp() : LOCATION("default"), DATE("default"), LAST_MODIFIED("default"), SERVER("webserver") {}
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
    //Both
	std::string DATE; // HH:MM:SS GMT
    std::string getLastModified(const char *path);
    std::string getDate();
    std::string getBuffer();
    struct stat st;
};

#endif
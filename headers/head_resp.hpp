#ifndef HEAD_RESP_HPP
#define HEAD_RESP_HPP

#include <iostream>
#include <sys/stat.h>
#include <sys/time.h>
#include <sstream>
#include <vector>
#include "../utils/returnCodes.hpp"
#include "../utils/definitions.hpp"

class Head_resp
{
    public:
    Head_resp() : CONTENT_LANGUAGE("fr"), LAST_MODIFIED("default"), SERVER("webserver"), RETRY_AFTER("02:00"),  DATE("default") {}
    // response
    std::string ALLOW;
    std::string CONTENT_LANGUAGE;
    std::string CONTENT_LENGTH; //OK
    std::string CONTENT_LOCATION;
    std::string CONTENT_TYPE;
    std::string LAST_MODIFIED; //OK
    std::string SERVER; // OK
    // SI 503
    std::string RETRY_AFTER; // 2 lignes date + delay-second
    std::string TRANSFER_ENCODING;
    char **WWW_AUTHENTICATE;
	std::string DATE; // HH:MM:SS GMT //OK
    public:
    std::string getLastModified(const char *path);
    std::string getDate();
    std::string getBuffer(int code, const char *path, std::vector<std::string> methods, std::string _method);
    std::string getContentLength(const char *path);
    std::string getBuffer_cgi(int code, std::string m_body);
    struct stat st;
};

#endif
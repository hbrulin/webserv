#include "head_resp.hpp"

//Header::~Header() {}

std::string Head_resp::getLastModified(const char *path) {
    if (lstat((const char *)path, &st) == 0)
	{
		char time[200];
		struct tm *tm =  localtime(&st.st_mtime);	
		if (strftime(time, sizeof(time), "%Y-%m-%d %H:%M", tm) != 0)
			LAST_MODIFIED = std::string(time) + "GMT";	
	}
    return LAST_MODIFIED;
}

std::string Head_resp::getDate()
{
		struct timeval tv;
		if (gettimeofday(&tv, NULL) == 0)
		{
			//std::cout << tv.tz_minuteswest << std::endl;
			time_t t = tv.tv_sec;
			struct tm *tm =  localtime(&t);
			char time[200];
			if (strftime(time, sizeof(time), "%Y-%m-%d %H:%M:%S", tm) != 0)
				DATE = std::string(time) + " GMT";
		}
        return DATE;
}

std::string Head_resp::getBuffer(int code, int length, const char *fichier)
{
    std::ostringstream oss;
    oss << "Content-Type: text/html" << "\r\n";
    oss << "Content-Length: " << length << "\r\n";
    oss << "Date: " << this->getDate() << "\r\n";
    oss << "Server: " << SERVER << "\r\n";
    oss << "Last Modified: " << this->getLastModified(fichier) << "\r\n";
    if (code == 503) // Not available
        oss << "Retry-after: " << RETRY_AFTER << "\r\n";
    oss << "\r\n";
    return oss.str();
}
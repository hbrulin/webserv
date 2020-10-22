#ifndef REQUEST_HPP
# define REQUEST_HPP

#include <string>
#include <sys/wait.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <string.h>
#include "../parser/Config.hpp"
#include "../parser/Location.hpp"
#include "../utils/libft.h"
#include "../headers/head_req.hpp"
#include "../headers/head_resp.hpp"
#include <fcntl.h>
#include "../utils/definitions.hpp"

class Request
{

/*Attributes*/
	public:

	/*Status*/
	int _status;

	/*Conf*/
	Config _conf;
	Location _loc;
	std::string m_index;

	/*Client*/
	int m_client;
	unsigned long s_addr;

	/*For cgi*/
	char curr_dir[200];
	std::string dir_cgi;
	std::string path;
	std::string content_env; //env que l'on recup dans requete POST pour les cgi
	int pid_ret;
	bool is_cgi;

	/*Headers*/
	std::string m_headers;
	Head_req _head_req;
	Head_resp _head_resp;

	/*file fds*/
	int file_fd;

	/*Request*/
	std::string m_path;
	std::string m_url;
	std::string m_body;
	unsigned int _body_size;

	/*Output*/
	int m_errorCode;
	std::string m_output;
	bool bytes_left;
	bool first_send;


/*Methods*/

	private:
    Request(const Request &copy) {(void)copy;}

	public:
	Request() {}
	~Request() {}
	Request &operator=(const Request &copy);
	Request(std::string headers, std::string body, int fd, Config conf, int port, unsigned long addr);
	
	/*parsing*/
	void parse();
	int isGoodRequest();
	int forbiddenChars(std::string s);
	void getBody();

	/*file fds*/
	void	setFileToSet(bool state);
	//void	setFileToWrite(bool state);
	int		read_file();
	int		write_file();


	/*methods*/
	void handle();
	void post();
	int forking();
	void get_post_content();
	void get_query_string();
	void exec_cgi();
	void get();
	void put();
	void delete_m();

	/*response*/
	void split_resp(char *buffer);
	int send_to_client();

	/*errors*/
	int preChecks();
	//int isAcceptable();
	int isAuthorized(std::string str);
	//int isAllowed(std::string path);
	void notFound();
	void badRequest();
	int internalError();
	
};


#endif

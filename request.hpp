#ifndef REQUEST_HPP
# define REQUEST_HPP

class Request {

	private:
	Request();
	char *m_buffer;
	int m_client;
	//HEADERS

	public:
	Request(char *buffer, int sock);

	protected:
	void parse();
	void handle();
	void send_to_client();
};


#endif
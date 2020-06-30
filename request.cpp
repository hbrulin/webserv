#include "request.hpp"

void Request::parse() {
	
	/*parse word by word*/
	std::istringstream iss(m_buffer);
	std::vector<std::string> parsed((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());

	//NEED TO PARSE HEADERS TOO?

	// If the GET request is valid, try and get the name
	if (parsed.size() >= 3 && parsed[0] == "GET")
	{
		m_content = parsed[1];

		//This should really be if it _ends_ in a slash?
		if (m_content == "/") //GET / HTTP/1.1
		{
			m_content = m_index;
		}
	}

}

void Request::handle() {
	//How do we determine the request isn’t for a document it can simply deliver, and create a CGI process?
	
	// Open the document in the local file system
	std::ifstream f(".\\www" + m_content); //DEFINI PAR PATH, META_VARIABLE?

	// Check if it opened and if it did, take content
	if (f.good())
	{
		std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
		m_content = str;
		m_errorCode = 200;
	}
	else
		m_content = m_not_found; //error code par défaut à 404, à revoir pour autres erreurs, genre manque de header par exemple
	f.close();

	// Write the document back to the client, with HEADERS - define how to deal with them
	std::ostringstream oss;
	oss << "HTTP/1.1 " << m_errorCode << " OK\r\n";
	oss << "Cache-Control: no-cache, private\r\n";
	oss << "Content-Type: text/html\r\n";
	oss << "Content-Length: " << m_content.size() << "\r\n";
	oss << "\r\n";
	oss << m_content;

	m_output = oss.str();
}


void Request::send_to_client() {
	send(m_client, m_output.c_str(), m_output.size() + 1, 0);
}
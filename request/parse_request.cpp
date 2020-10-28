#include "request.hpp"


 void		Request::getBody() {
 	std::istringstream f(m_body);
 	std::string buf;
 	std::string total;
 	bool flag = 0;
 	while (std::getline(f, buf))
 	{
 		if (!flag)
 			_body_size += ft_atoi_base(buf, "0123456789abcdef");
 			//_body_size += strtol(buf.c_str(), NULL, 16);
 		else
 			total += buf.substr(0, buf.size() - 1);
 		flag = !flag;
 	}
 	m_body = total;
}

/*void		Request::getBody() {
	std::string		tmp;
	//bool flag = 0;
	size_t pos;
	unsigned int nb = 0;
	std::string total;
	while (!m_body.empty())
	{
		pos = m_body.find("\r\n");
		tmp = m_body.substr(0, pos);
		nb = ft_atoi_base(tmp, "0123456789abcdef");
		_body_size += nb;
		total += m_body.substr(pos + 2, (size_t)nb);
		m_body = m_body.substr(pos + 2 + nb + 2);
	}
	m_body = total;
	std::cout << m_body.size() << std::endl;
	std::cout << _body_size << std::endl;
}*/

std::vector<std::string> Request::getParsed(std::string s) {
	std::string copy = s;
	size_t pos = 0;
	std::vector<std::string> parsed;

	while ((pos = copy.find(" ")) != std::string::npos) {
		parsed.push_back(copy.substr(0, pos));
		copy.erase(0, pos + 1);
	}	
	parsed.push_back(copy);
	return(parsed);
}


int Request::isGoodRequest()
{
	if (m_headers.empty())
		return 1;

	std::string copy = m_headers;
	size_t pos = 0;
	std::string buf;
	int line = 0;
	std::vector<std::string> parsed;

	while ((pos = copy.find("\r\n")) != std::string::npos) 
	{
		buf = copy.substr(0, pos);
		parsed = getParsed(buf);
		if (line == 0)
		{
			if (parsed.size() != 3)
				return 1;
			if (parsed[0] != GET && parsed[0] != POST && parsed[0] != HEAD && parsed[0] != PUT && parsed[0] != DELETE)
				return 1;
			if (ft_strstr(parsed[1].c_str(), "/") == NULL || forbiddenChars(parsed[1]))
				return 1;
			line++;
			m_url = parsed[1];
			_head_req.REQUEST_METHOD = parsed[0];
			_head_req.REQUEST_URI = parsed[1];
			_head_req.SERVER_PROTOCOL = parsed[2];
		}
		else
		{
			if (parsed.size() < 2)
				return 1;
		}
		copy.erase(0, pos + 2);
	}
	parsed = getParsed(copy);
	if (parsed.size() < 2)
		return 1;
	return 0;
}



void Request::parse()
{
	if (!isGoodRequest())
	{
		_head_req.parse(m_headers.c_str(), m_url);
		_loc = _conf._locations.get_loc_by_url(m_url);
		m_index = _loc._index;

		if (preChecks())
			return;

		if (m_url.find("?") != std::string::npos)
			m_url.replace(m_url.find("?"),m_url.size(), "");

		/*build the right path*/
		_loc._name.pop_back();
		if (m_url == "/" || _loc._name == m_url)
			m_url = m_index;
		else if (ft_strstr(m_url.c_str(), _loc._name.c_str()) != NULL)
			m_url.erase(0, _loc._name.size());

		if (!_loc._uploaded_files_root.empty() && ft_strstr(m_url.c_str(), _loc._uploaded_files_root.c_str()) != NULL)
		{
			m_path = m_url;
			return;
		}
		_head_req.CONTENT_LANGUAGE = "fr";
		if (ft_strstr(CONTENT_NEGO_AVAILABLE, _loc._root.c_str()) != NULL)
		{
			_loc._root = _head_req.contentNego(_loc._root);
		}
		_head_resp.CONTENT_LANGUAGE = _head_req.CONTENT_LANGUAGE;
		m_path = _loc._root + m_url;

		/*parse body*/
		if (_head_req.REQUEST_METHOD == PUT || _head_req.REQUEST_METHOD == POST)
		{
			if (_head_req.TRANSFER_ENCODING == CHUNKED_STR)
				getBody();
			else if (_head_req.CONTENT_LENGTH.empty() == 0)
				_body_size = (unsigned int)stoi(_head_req.CONTENT_LENGTH);
			else
			{
				m_errorCode = 411;
				m_path = _loc._errors[m_errorCode];
				if (read_file() == -1)
					internalError();
				return;
			}
		}
	}
	else
	{
		_loc = _conf._locations._blank;
		//_loc.print();
		badRequest();
		return;
	}
}
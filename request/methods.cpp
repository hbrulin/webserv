#include "request.hpp"

void Request::exec_cgi(){

	if (_body_size > _loc._body_size)
	{
		m_errorCode = 413;
		return;
	}
	m_url = _loc._cgi_root + _loc._cgi_file;
	_head_req.SERVER_NAME = _conf._server_name;
	_head_req.SCRIPT_NAME = _loc._cgi_file;
	pid_ret = forking();
	if (handle_cgi_output() > 0)
		pid_ret = 127;
	sleep(5);
}

void Request::post() {
	if (_body_size > _loc._body_size)
	{
		m_errorCode = 413;
		return;
	}

	if (m_body.size() == 0)
	{
		m_path = "www/post_0.html";
		m_errorCode = 200;
		if (read_file() == -1)
			internalError();
	}
	else
	{
		m_path = POST_HTML;
		m_errorCode = 201;
		if (read_file() == -1)
			internalError();
	}
}

void Request::put() {
	int fd;

	if (_body_size > _loc._body_size)
	{
		m_errorCode = 413;
		return;
	}
	m_path = _loc._uploaded_files_root + m_url;
	if (path_exists(m_path))
	{
		unlink(m_path.c_str());
		m_errorCode = 200;
	}
	else
	{
		m_errorCode = 201;
		_head_resp.LOCATION = m_path;
	}

	fd = open(m_path.c_str(), O_WRONLY | O_CREAT, 0666);
	if (fd != -1)
	{
		if(write(fd, m_body.c_str(), _body_size) == -1)
			internalError();
		close(fd);
	}
}

void Request::delete_m()
{
	if (path_exists(m_path))
		m_errorCode = 200;
	else
		m_errorCode = 204;
	unlink(m_path.c_str());
}

void Request::get() {

	struct stat buf = {};
	stat(m_path.c_str(), &buf);
	if (buf.st_mode & S_IFDIR)
	{
		std::string b = m_path + "/" + _loc._index;
		if (_loc._autoindex == true && (_loc._index.empty() || !path_exists(b)))
			m_path = _loc.get_autoindex();
		else
			m_path = b;
	}

	if (path_exists(m_path))
	{
		m_errorCode = 200;
		if (read_file() == -1)
			internalError();
		return;
	}
	else
		notFound();
}


int		Request::read_file()
{
	int 				fd = open(m_path.c_str(), O_RDONLY);
	int					ret = 0;
	char				buf[4096];
	m_url = "";
	if (fd != -1)
	{
		while ((ret = read(fd, buf, 4095)) > 0)
		{
			buf[ret] = '\0';
			m_url += buf;
		}
		close(fd);
	}
	return ret;
}

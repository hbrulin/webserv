#include "request.hpp"

int Request::handle_cgi_output() {

	int 				fd = open(cgi_output.c_str(), O_RDONLY);
	int					ret = 0;
	char				buf[4096];
	std::string 		tmp;

	while ((ret = read(fd, buf, 4095)) > 0)
	{
		buf[ret] = '\0';
		tmp += buf;
	}
	close(fd);
	unlink(cgi_output.c_str());
	if (ret == -1)
		return 1;

	int n = tmp.find("\r\n");
	std::string toParse = tmp.substr(0, n);
	tmp = tmp.substr(n + 2);
	std::string code = _head_req.getStringtoParse(toParse.c_str(), STATUS_STR);
	m_errorCode = std::stoi(code);
	n = tmp.find("\r\n");
	toParse = tmp.substr(0, n);
	_head_resp.CONTENT_TYPE = _head_req.getStringtoParse(toParse.c_str(), CONTENT_T_STR);

	n = tmp.find(ENDCHARS);
	if (n != (int)std::string::npos)
	{
    	n = n + std::string(ENDCHARS).size();
		m_body.clear();
		m_body = tmp.substr(n, tmp.size() - n);
	}
	return 0;
}


int Request::forking()
{
	int pid, res, status;
	int pp[2];
	res = 0;
	if (getcwd(curr_dir, 200) == NULL)
		return (127);
	dir_cgi = std::string(curr_dir) + "/";
	path = dir_cgi + m_url;

	_head_req.PATH_TRANSLATED = path;
	_head_req.PATH_INFO = _head_req.REQUEST_URI;
	_head_req.CONTENT_LENGTH = std::to_string(m_body.size());

	if (!(path_exists(path)))
		return 127;

	struct stat buf;
	int ret;
	if ((ret = stat(path.c_str(), &buf)) < 0)
		std::cout << ERR_STAT << strerror(errno) << std::endl;
	else
	{
		const char chars[] = "rwxrwxrwx";
		char mode[10];
  		for (size_t i = 0; i < 9; i++)
    		mode[i] = (buf.st_mode & (1 << (8-i))) ? chars[i] : '-';
		mode[9] = '\0';
		if (mode[2] != 'x' || mode[5] != 'x' || mode[8] != 'x')
			return (127);
	}

	std::string _headers = _head_req.get_meta();
	char **env = ft_split(_headers.c_str(), '&');
	char **argv = NULL;
	cgi_output = dir_cgi;
	cgi_output = cgi_output + OUTPUT_CGI + std::to_string(m_client);
	int fd;
	if ((fd = open(cgi_output.c_str(), O_RDWR | O_CREAT, 0666)) < 0)
		std::cout << strerror(errno) << std::endl;

	if (pipe(pp))
		perror(PIPE_ERR);
	pid = fork();

	if (pid == 0)
	{
		close(pp[1]);
   		dup2(pp[0], 0);
		dup2(fd, 1);
		res = execve(path.c_str(), argv, env);
		if (res != 0)
		{
			exit(127);
			return 0;
		}
		return 2;
	}
	else if (pid > 0)
	{
		close(pp[0]);
		if (write(pp[1], m_body.c_str(), m_body.size()) == -1)
		{
			ft_tabdel((void**)env);
			close(fd);
			return (1);
		}
		if (waitpid(pid, &status, 0) == -1)
			perror(WAIT_ERR);
		close(pp[1]);
		close(fd);
	}
	else
	{
		perror(FORK_ERR);
		close(fd);
		ft_tabdel((void**)env);
		return 127;
	}
	ft_tabdel((void**)env);
	return 0;	
}
int	Request::isAuthorized(char *path)
{
	if (_config._auth_path != NULL)
	{
		int i =0;
		while (_config._auth_path[i] && strcmp(_config._auth_path[i], path)) {i++;}
		if (_config._auth_path[i] != NULL)
		{
			index_auth = i;
            _head_req.WWW_AUTHENTICATE = "Basic" + "realm=" + _auth_realm[i];
		}

	}
	
}
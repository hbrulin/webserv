#include "parser/Data.hpp"
#include "listener/listener.hpp"
#include "../utils/definitions.hpp"

int main (int ac, char **av) {
	(void)ac;
	ft_memset((char *) &R_SET, 0, sizeof(R_SET));
	ft_memset((char *) &W_SET, 0, sizeof(W_SET));
	try
	{
		Data data(av[1]);
		int size = data.getSize();
		Listener server(data.getConfigList(), size);
		//std::cout << "HERE\n" << data.getConfigList()[0]._locations[2].get_autoindex();
		server.init();
		server.run();
	}
	catch (std::logic_error& e)
	{
		std::cerr << e.what() << std::endl;
		return -1;
	}

	return 0;
}

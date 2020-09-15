#include "parser/Data.hpp"
#include "listener/listener.hpp"

int main (int ac, char **av) {
	(void)ac;
	try
	{
		Data data(av[1]);
		int size = data.getSize();
		//std::cout << "-1\n";

		Location locb = data.getConfigList()[0]._locations.get_loc_by_url("/bonjour/lol.html");
		std::cout << "loc found: ";
		locb.print();
		std::cout << locb._cgi_file << std::endl;
		//std::cout << "HERE" << data.getConfigList()[0]._locations.size() << std::endl;
		//std::cout << data.getConfigList()[0]._locations._blank.check_allowed_method("GET") << std::endl;
		/*Location locb = data.getConfigList()[0]._locations.get_loc_by_url("");
		locb.print();
		std::cout << "HERE: " << locb.check_allowed_method("POST") << std::endl;*/
		Listener server(data.getConfigList(), size);
		//std::cout << "0\n";
		server.init();
		//std::cout << "1\n";
		server.run();
		//std::cout << "2\n";
		//server.clean(); //jamais utilisé vu que ctrl-C?
		//free
	}
	catch (std::logic_error& e)
	{
		std::cerr << e.what() << std::endl;
		return -1;
	}

	return 0;
}

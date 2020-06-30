#include "env.hpp"
#include "listener.hpp"

int main (void) {
	/*Your program should have a config file in argument or use a default path. -> IMPLEMENT + check for extra args */
	Env env;
	Listener server;

	//if config file, open file and use env.parse_config();
	
	server.init();
	server.run();
	server.clean(); //jamais utilisé vu que ctrl-C?

	return 0;
}
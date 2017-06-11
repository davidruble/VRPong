#include "rpc/server.h"

int main(int argc, char* argv[])
{
	rpc::server srv(rpc::constants::DEFAULT_PORT);

	srv.run();
	return 0;
}
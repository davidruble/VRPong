#include "rpc/server.h"
#include <iostream>
#include <string>
using namespace std;

// test RPC to ensure we can communicate with the server
string test(string str)
{
	cout << "In server test function. Input string is: " << str << endl;
	return "Server response!";
}

int main(int argc, char* argv[])
{
	cout << "Starting server..." << endl;
	rpc::server srv(rpc::constants::DEFAULT_PORT);
	cout << "Waiting for RPC calls..." << endl;

	srv.bind("test", &test);

	srv.run();
	return 0;
}
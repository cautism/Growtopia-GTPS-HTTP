#include <iostream>
#include <string>
#include "Core/server.hh"

using namespace std;

int main()
{
	/*Void format is [HTTP SERVER IP] [HTTP SERVER PORT] [SERVER DATA IP] [SERVER DATA PORT]*/
	web_server::run_Server("0.0.0.0", 80, "1.1.1.1", 17091);
}

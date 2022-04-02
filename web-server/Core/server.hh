#define FMT_HEADER_ONLY
#include <fmt/include/core.h>

#include "../sandbird/sandbird.h"
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "shlwapi.lib")

#include <Windows.h>
#include <string>
#include <thread>
#include <vector>
#include <stdint.h>
#include <stdio.h>
#include <map>
#include <chrono>
#include <fstream>
#include <sstream>

using namespace std::chrono;

namespace web_server {
	int server_Handler(sb_Event* event);
	void run_Server(std::string http_server_ip, int http_server_port, std::string gtps_host, int gtps_server_port);
}
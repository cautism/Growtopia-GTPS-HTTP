#include "server.hh"
#include "Other.hh"

sb_Options options;
sb_Server* http_server;

int web_server::server_Handler(sb_Event* event) {
	if (event->type == SB_EV_REQUEST) {
		std::string client_ip = event->address;
		std::string client_method_type = event->method;
		const char* client_requested_path = event->path;

		/*growtopia/server_data.php handling*/
		if (strstr(client_requested_path, "/growtopia/server_data.php") != nullptr && client_method_type == "POST") 
		{
			try 
			{
				logger::printLogs(fmt::format("Client {} sent request to path {} with method {}.", client_ip, client_requested_path, client_method_type));

				long long current_time = GetCurrentTimeInternalSeconds();

				sb_send_status(event->stream, 200, "OK");
				sb_send_header(event->stream, "Content-Type", "text/plain");

				char reply[1024];
				std::string info = "server|" + gtps_ip + "\nport|" + std::to_string(gtps_port) + "\ntype|1\n#maint|`4Oh no! `oWait 0 seconds before logging in again!\nbeta_server|%s\nbeta_port|1945\nbeta_type|1\nmeta|ni.com\nRTENDMARKERBS1001\n";
				if (cooldown.find(client_ip) == cooldown.end()) {
					goto ok;
				}
				else if (cooldown.at(client_ip) > current_time) {
					info = "server|" + gtps_ip + "\nport|" + std::to_string(gtps_port) + "\ntype|1\nmaint|`4Oh no! `oWait " + std::to_string(cooldown.at(client_ip) - current_time) + " seconds before logging in again!\nbeta_server|%s\nbeta_port|1945\nbeta_type|1\nmeta|ni.com\nRTENDMARKERBS1001\n";
					memcpy(reply, info.c_str(), info.size() + 1);
					sb_writef(event->stream, format(reply, http_ip.c_str(), std::to_string(http_port).c_str(), http_ip.c_str()).c_str());
				}
				else {
				ok:
					if (cooldown.find(client_ip) == cooldown.end()) {
						cooldown.insert({ client_ip, (GetCurrentTimeInternalSeconds() + (6)) });
					}
					else {
						cooldown.at(client_ip) = (GetCurrentTimeInternalSeconds() + (6));
					}
					memcpy(reply, info.c_str(), info.size() + 1);
					sb_writef(event->stream, format(reply, http_ip.c_str(), std::to_string(http_port).c_str(), http_ip.c_str()).c_str());
				}
			}
			catch (std::exception& error) 
			{
				logger::printError(fmt::format("Error occured {}", error.what()));
			}
		}
		/*growtopia/server_data.php handling*/

		/*cache handling*/
		if ((strstr(client_requested_path, "/game/") != NULL && client_method_type == "GET" || strstr(client_requested_path, "/social/") != NULL && client_method_type == "GET" || strstr(client_requested_path, "/interface/") != NULL && client_method_type == "GET" || strstr(client_requested_path, "/audio/") != NULL) && client_method_type == "GET")
		{
			uint32_t size = 0;
			const char* path = client_requested_path + 1;
			uint8_t* content = read_file(path, &size);
			if (content) {
				sb_send_status(event->stream, 200, "OK");
				sb_send_header(event->stream, "Content-Type", "application/x-www-form-urlencoded");
				sb_send_header(event->stream, "Content-Length", format("%d", size).c_str());
				sb_send_header(event->stream, "beserver", "06");
				sb_send_header(event->stream, "Connection", "keep-alive");
				sb_send_header(event->stream, "Accept-Ranges", "bytes");
				sb_write(event->stream, content, size);
			}
		}
		/*cache handling*/

		/*Default address path handling*/
		if (strstr(client_requested_path, "/render/") != NULL && client_method_type == "GET") 
		{
			std::string path = client_requested_path + 1;
			std::ifstream read_map(path);
			if (!read_map.is_open()) {
				return SB_RES_CLOSE;
			}
			std::stringstream sstr;
			while (read_map >> sstr.rdbuf());
			std::string test = sstr.str();
			sb_send_status(event->stream, 200, "OK");
			sb_send_header(event->stream, "Content-Type", "text/html");
			sb_writef(event->stream, test.c_str());
		}
		else
		{
			logger::printLogs(fmt::format("Client {} sent request to path {} with method {}.", client_ip, client_requested_path, client_method_type));

			long long current_times = GetCurrentTimeInternalSeconds();
			if (http_cooldown.find(client_ip) == http_cooldown.end()) {
				goto oks;
			}
			else if (http_cooldown.at(client_ip) > current_times) {
				sb_send_status(event->stream, 301, "hi?");
				sb_send_header(event->stream, "Location", "https://shopee.co.id/");
			}
			else {
			oks:
				if (http_cooldown.find(client_ip) == http_cooldown.end()) {
					http_cooldown.insert({ client_ip, (GetCurrentTimeInternalSeconds() + (6)) });
				}
				else {
					http_cooldown.at(client_ip) = (GetCurrentTimeInternalSeconds() + (6));
				}
			}
		}
		/*Default address path handling*/
	}
	return SB_RES_OK;
}

void web_server::run_Server(std::string http_server_ip, int http_server_port, std::string gtps_host, int gtps_server_port) {
	options.handler = server_Handler;
	options.host = http_server_ip.c_str();
	options.port = std::to_string(http_server_port).c_str();
	http_server = sb_new_server(&options);

	gtps_ip = gtps_host, gtps_port = gtps_server_port;

	if (!http_server) 
	{
		logger::printError(fmt::format("Failed to start HTTP Web Server maybe host {} with port {} is in use?", http_server_ip, std::to_string(http_server_port)));
	}
	else 
	{
		logger::printLogs(fmt::format("HTTP Web Server is running with details {}:{}", http_server_ip, std::to_string(http_server_port)));
	}

	while (true) {
		sb_poll_server(http_server, 10);
		util_sleep(1);
	}
	sb_close_server(http_server);
}
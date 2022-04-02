#include <string>
#include <map>
#include <chrono>
#include <cstdarg>

#define FMT_HEADER_ONLY
#include <fmt/include/core.h>
#include "../Logger/Logger.hh"

using namespace std::chrono;

std::map<const std::string, long long> cooldown = {

};

std::map<const std::string, long long> http_cooldown = {

};

std::string http_ip = "", gtps_ip = "";
int http_port = 0, gtps_port = 0;

inline long long GetCurrentTimeInternalSeconds() {
	return (duration_cast<seconds>(system_clock::now().time_since_epoch())).count();
}

std::string format(const char* msg, ...) {
	char fmt[1024] = { 0 };
	va_list va;
	va_start(va, msg);
	vsnprintf(fmt, 1024, msg, va);
	va_end(va);
	return std::string(fmt);
}

uint8_t* read_file(const char* file, uint32_t* size) {
	FILE* fp;
	fopen_s(&fp, file, "rb");
	if (!fp) {
		std::string file_name = file;
		logger::printError(fmt::format("Failed to read file {} returned 404 error.", file_name));
		return nullptr;
	}
	fseek(fp, 0, SEEK_END);
	const uint32_t fsize = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	uint8_t* data = static_cast<uint8_t*>(malloc(fsize));
	if (data) {
		memset(data, 0, fsize);
		fread(data, fsize, 1, fp);
		fclose(fp);
		if (size)
			*size = fsize;
		return data;
	}
	return nullptr;
}


void util_sleep(int32_t ms) {
#ifdef _WIN32
	Sleep(ms);
#else
	usleep(ms * 1000);
#endif
}
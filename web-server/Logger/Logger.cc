#include "Logger.hh"
#include <string>
#include <iostream>

void logger::printWarning(std::string warning) {
	std::cout << "[WARNING] : " << warning << std::endl;
}
void logger::printError(std::string error) {
	std::cout << "[ERROR] : " << error << std::endl;
}
void logger::printLogs(std::string print) {
	std::cout << "[LOGS] : " << print << std::endl;
}
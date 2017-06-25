
#include "exception.h"
#include <Windows.h>


Exception::Exception(std::string file, int line) {
	std::stringstream sstr;
	sstr << file << "; " << line << "; ";
	message = sstr.str();
}

void Exception::writeDebugString() {
	OutputDebugStringA(message.c_str());
}
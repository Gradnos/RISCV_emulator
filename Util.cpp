#include "Util.h"
#include <fstream>

namespace Util {
	bool fileExists(const std::string& s) {
		std::ifstream f(s.c_str());
		return f.good();
	}

}
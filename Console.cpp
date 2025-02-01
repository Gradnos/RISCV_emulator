#include "Console.h"

namespace Console {
	void initConsole() {
		m_text = ">";
		log("Init Console");
	}

	void deleteConsole() {}

	std::string* getStringPtr() {
		return &m_text;
	}

	void clear() {
		m_text = ">";
	}

	void log(std::string msg) {
		printf(msg.c_str());
		m_text += msg + "\n>";
	}
}
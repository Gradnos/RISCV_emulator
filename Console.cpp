#include "Console.h"

namespace Console {
	void initConsole() {
		m_text = "";
		log("Init Console");
	}

	void deleteConsole() {}

	std::string* getStringPtr() {
		return &m_text;
	}

	void clear() {
		m_text = ">";
	}
	void print(std::string msg) {
		m_text += msg;
	}
	void log(std::string msg) {
		m_text += ">" + msg + "\n";
	}
}
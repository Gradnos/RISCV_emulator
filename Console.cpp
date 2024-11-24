#include "Console.h"

namespace Console {
	void initConsole() {
		m_text = ">";
		log("Init Console");
	}

	void deleteConsole() {
	}

	const char* getStartPtr() {
		return &m_text[0];
	}
	const char* getEndPtr() {
		return &m_text[m_text.length()];
	}
	void clear() {
		m_text = ">";
	}
	void log(std::string msg) {
		m_text += msg + "\n>";
	}
}
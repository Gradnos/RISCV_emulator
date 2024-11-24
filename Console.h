#pragma once

#include <iostream>
namespace Console {
	
	//public
	void initConsole();
	void deleteConsole();
	const char* getStartPtr();
	const char* getEndPtr();
	void clear();
	void log(std::string msg);

	//private
	static	std::string m_text = ">";
}
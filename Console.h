#pragma once

#include <iostream>
namespace Console {
	
	//public
	void initConsole();
	void deleteConsole();
	std::string* getStringPtr();
	void clear();
	void log(std::string msg);

	//private
	static	std::string m_text = ">";
}
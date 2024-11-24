#include "MyTokenizer.h"
#include <iostream>
#include <regex>
#include "Console.h"

MyTokenizer::MyTokenizer(const char* text) :
	m_text(text)
{}


void MyTokenizer::setEmpty(const char* empty, const char* emptyStart) {
	m_empty = empty;
	if (emptyStart)
		m_emptyStart = emptyStart;
}

bool MyTokenizer::charInStr(std::string a, const char b) {
	int id = 0;
	char c = a[id];
	while (c != 0)
	{
		if (c == b) return true;
		id++;
		c = a[id];
	}
	return false;



}




Token MyTokenizer::nextToken() {

	
	char c = m_text[m_currId];
	Token t;

	if (c == 0) return { MY_TOKEN_END, "0"};

	// skip first empty part
	if (charInStr(m_emptyStart, c)) m_currId++;
	c = m_text[m_currId];
	while (charInStr(m_empty, c))
	{
		m_currId++;
		c = m_text[m_currId];
	}
	//

	// if empty after fisrt empty return empty token for a,    ,    b will return empty after a
	if (charInStr(m_emptyStart, c)) {
		return { MY_TOKEN_EMPTY, "0" };
	}

	// if text ends after the empty space return
	if (c == 0) return { MY_TOKEN_END, "0" };


	
	int startId = m_currId;
	//print the current token
	while (!charInStr(m_emptyStart, c) && !charInStr(m_empty, c) && c != 0)
	{
		//printf("%c", c);
		m_currId++;
		c = m_text[m_currId];
	}
	int strSize = m_currId - startId + 1;





	std::string tok;
	tok.resize(strSize);
	tok.assign(m_text + startId, m_text + m_currId);
	tok[strSize - 1] = 0;


	return { tokenTypeFromStr(tok), tok};
}


int MyTokenizer::tokenTypeFromStr(std::string& s) {
	std::string registerRegex = "((^(?:x)(?:[12]?[0-9]|3[01])$)|^ra$|^sp$|^gp$|^tp$|^t[0-6]$|^s[0-9]$|^fp$|^a[0-7]$|^s1[01]$|^zero$)";

	if (std::regex_match(s, std::regex(registerRegex)))
		return MY_TOKEN_REGISTER;


	std::string numberRegex = "(^([+-]?[1-9][0-9]*|0)$)";
	if (std::regex_match(s, std::regex(numberRegex)))
		return MY_TOKEN_NUM;

	std::string actionRegex = "(^addi?$|^subi?$|^xori?$|^ori?$|^andi?$|^slli?$|^srli?$|^lbu?$|^lhu?$|^lw$|^sb$|^sh$|^sw$|^beq$|^bne$|^blt$|^bge$|^bltu$|^bgeu$|^jalr?$|^ecall$|^mul$|^div$|^rem$|)";

	if (std::regex_match(s, std::regex(actionRegex)))
		return MY_TOKEN_ACTION;

	std::string defineRegex = "^[a-zA-Z_][a-zA-Z_0-9]+:$";
	if (std::regex_match(s, std::regex(defineRegex)))
		return MY_TOKEN_DEFINE;

	std::string addressRegex = "^([1-9][0-9]*|0)\\((((?:x)(?:[12]?[0-9]|3[01]))|ra|sp|gp|tp|t[0-6]|s[0-9]|fp|a[0-7]|s1[01]|zero)\\)";
	if (std::regex_match(s, std::regex(addressRegex)))
		return MY_TOKEN_ADDRESS;


	return MY_TOKEN_UNKNOWN;
}

std::string MyTokenizer::typeName(int type) {
	if (type == MY_TOKEN_EMPTY)
		return "Empty";
	if (type == MY_TOKEN_REGISTER)
		return "Register";
	if (type == MY_TOKEN_NUM)
		return "Number";
	if (type == MY_TOKEN_ACTION)
		return "Action";
	if (type == MY_TOKEN_ADDRESS)
		return "Address";
	if (type == MY_TOKEN_DEFINE)
		return "Define";
	if (type == MY_TOKEN_UNKNOWN)
		return "Unknown";
	if (type == MY_TOKEN_END)
		return "End";
}
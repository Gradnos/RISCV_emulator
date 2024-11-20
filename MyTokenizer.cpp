#include "MyTokenizer.h"



MyTokenizer::MyTokenizer(const char* text) :
	m_text(text)
{
	printf("%s \n\n\n", text);


}


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


	
	int startId = m_currId;
	//print the current token
	while (!charInStr(m_emptyStart, c) && !charInStr(m_empty, c) && c != 0)
	{
		printf("%c", c);
		m_currId++;
		c = m_text[m_currId];
	}
	int strSize = m_currId - startId + 1;
	std::string tok;
	tok.resize(strSize);
	tok.assign(m_text + startId, m_text + m_currId);
	tok[strSize - 1] = 0;

	return { 1, tok};
}



#pragma once

#include <string>

struct Token {
	int tokenType;
	std::string token;
};


#define MY_TOKEN_EMPTY 0
#define MY_TOKEN_REGISTER 1
#define MY_TOKEN_NUM 2
#define MY_TOKEN_ACTION 3
#define MY_TOKEN_ADDRESS 4
#define MY_TOKEN_DEFINE 5
#define MY_TOKEN_UNKNOWN 6
#define MY_TOKEN_END -1


class MyTokenizer
{
public:
	MyTokenizer(const char* text = NULL);
	Token nextToken();
	void setEmpty(const char* empty, const char* emptyStart = NULL);


private:
	const char* m_text = NULL;
	int m_currId = 0;
	std::string m_empty = " \n";
	std::string m_emptyStart = ",";  // chars that can only be included once at the start of the empty token



	bool charInStr(std::string a, const char b);
	int tokenTypeFromStr(std::string& str);
};

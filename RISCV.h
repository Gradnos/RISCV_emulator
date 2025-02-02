#pragma once
#include <iostream>
#include <string>
#include <stdio.h>
#include <string.h>
#include "MyTokenizer.h"
#include "Console.h"
#include "list"
#include "vector"
#include "map"

class RISCV {
public:
	void run();
	void step();
	void reset();
	std::string* getTextPtr();
	int getMemorySize();

	bool checkActionValidity(Token& t);
	bool handleTokenForCompile(Token& t);

	const void* getMemoryPtr();
	const int* getRegistersPtr();

	int readRegister(int id);

	RISCV();
	~RISCV();
private:
	//size_t m_initTextSize = 1024 * 16;

	//return true if token handled correctly else false
	bool handleToken(Token& t); 
	bool handleAction(Token& t);
	void handleDefine(Token& t);

	// get the tokens you expect. returns true if got what expected, false if not and logs error message
	bool getNextTokens(Token& t, Token* nextTokens, std::list<int> expected);
	int registerIdFromStr(std::string s);
	int addressToInt(std::string s);
	void* ptrFromAddress(std::string s);

	void initRegisters();
	bool compile();

	void call(std::string name, bool storeRa);

	std::map<std::string, int> defines;
	

	static const size_t m_initMemorySize = 1024 * 128;
	size_t m_memoryAllocSize = m_initMemorySize;

	MyTokenizer m_tokenizer;
	int m_neededSizeAboveAddr = 0;


	bool m_running = false;
	bool m_returned = false;

	std::string m_text;
	void* m_memory = NULL;
	int m_registers[32];

};
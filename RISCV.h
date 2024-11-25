#pragma once
#include <iostream>
#include <string>
#include <stdio.h>
#include <string.h>
#include "MyTokenizer.h"
#include "Console.h"
#include "list"

class RISCV {
public:
	void run();
	void step();
	void reset();
	std::string* getTextPtr();
	int textGrowSizeIfNeeded();
	RISCV();
	~RISCV();
private:
	//size_t m_initTextSize = 1024 * 16;

	//return true if token handled correctly else false
	bool handleToken(Token& t); 
	bool handleAction(Token& t);

	bool getNextTokens(Token& t, Token* nextTokens, std::list<int> expected);

	static const size_t m_initMemorySize = 1024 * 128;
	size_t m_memoryAllocSize = m_initMemorySize;

	MyTokenizer m_tokenizer;

	bool m_running = false;

	std::string m_text;
	void* m_memory = NULL;
	int m_registers[32];

};
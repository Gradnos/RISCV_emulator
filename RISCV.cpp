#include "RISCV.h"
#include "Console.h"



RISCV::RISCV() {
	m_memoryAllocSize = m_initMemorySize;
	m_memory = std::malloc(m_memoryAllocSize);
	m_registers[0] = 0;
	Console::log("Init Riscv");
}

RISCV::~RISCV() {
	if(m_memory)
		std::free(m_memory);
}

std::string* RISCV::getTextPtr() {
	return &m_text;
}


int RISCV::textGrowSizeIfNeeded() {
	return 0;
}

void RISCV::run() {
	m_tokenizer = MyTokenizer(&m_text[0]);
	Console::log("Run.");
	while (true) {
		Token t = m_tokenizer.nextToken();
		std::string a = std::to_string(t.tokenType) + " " + t.token;
		std::cout << a << '\n';
		Console::log(a);
		if (t.tokenType == -1) break;
	}
	Console::log("Finish.");
}

void RISCV::step() {
}



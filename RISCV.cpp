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
	m_running = true;
	m_tokenizer = MyTokenizer(&m_text[0]);
	Console::log("Run.");
	while (true) {
		Token t = m_tokenizer.nextToken();
		std::string a = m_tokenizer.typeName(t.tokenType) + "-" + t.token + "-";
		//std::cout << a << '\n';
		Console::log(a);
		if (t.tokenType == MY_TOKEN_END) break;
	}
	Console::log("Finish.");
	m_running = false;
}

void RISCV::step() {
	if (!m_running) {
		m_running = true;
		m_tokenizer = MyTokenizer(&m_text[0]);
		Console::log("Run.");
	}

	Token t = m_tokenizer.nextToken();
	std::string a = m_tokenizer.typeName(t.tokenType) + "-" + t.token + "-";
	Console::log(a);

	if (t.tokenType == MY_TOKEN_END) {
		Console::log("Finish.");
		m_running = false;
	}
}

void RISCV::reset() {
	Console::log("Reset.");
	m_running = false;
}



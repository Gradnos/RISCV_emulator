#include "RISCV.h"

RISCV::RISCV() {
	m_memory = std::malloc(m_memoryAllocSize);
	m_registers[0] = 0;

}

RISCV::~RISCV() {
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

	while (true) {
		Token t = m_tokenizer.nextToken();
		std::cout << t.tokenType << " " << t.token << std::endl;
		if (t.tokenType == -1) break;
	}
	printf("done \n");
}

void RISCV::step() {
}



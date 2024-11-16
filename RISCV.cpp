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

}

void RISCV::step() {

}



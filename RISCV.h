#pragma once
#include <iostream>
#include <string>
class RISCV {
public:
	void run();
	void step();
	void reset();
	std::string* getTextPtr();
	size_t getTextAllocSize();
	int textGrowSizeIfNeeded();
	RISCV();
	~RISCV();
private:
	//size_t m_initTextSize = 1024 * 16;
	static const size_t m_initMemorySize = 1024 * 128;
	size_t m_memoryAllocSize = m_initMemorySize;
	std::string m_text;
	void* m_memory;
	int m_registers[32];
};
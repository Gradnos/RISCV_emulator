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
		bool ok = handleToken(t);
		if (!ok) {
			Console::log("Error.");
			m_running = false;
			return;
		}
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
		return;
	}

	bool ok = handleToken(t);
	if (!ok) {
		Console::log("Error.");
		m_running = false;
		return;
	}
}

void RISCV::reset() {
	Console::log("Reset.");
	m_running = false;
}





bool RISCV::handleToken(Token& t) {
	if (t.tokenType == MY_TOKEN_ACTION) {
		bool ok = handleAction(t);
		if (!ok) 
			return false;
	}


	return true;


}

bool RISCV::handleAction(Token& t) {
	Token nextTokens[5];
	if (t.token == "addi") {
		bool ok = getNextTokens(t ,nextTokens, { MY_TOKEN_REGISTER, MY_TOKEN_REGISTER, MY_TOKEN_NUM });
		if (!ok)
			return false;
		Console::log("good");

	}
	if (t.token == "lw") {
		bool ok = getNextTokens(t, nextTokens, { MY_TOKEN_REGISTER, MY_TOKEN_ADDRESS });
		if (!ok)
			return false;
		Console::log("good");

	}






	return true;
}


// puts expected tokens in nextTokens Array and returns true if everything went well. 
// make sure that nextTokens has enough space
bool RISCV::getNextTokens(Token& t, Token* nextTokens, std::list<int> expected) {
	auto it = expected.begin();
	for (int i = 0; it != expected.end(); i++) {
		Token currT = m_tokenizer.nextToken();
		if (currT.tokenType != *it) {
			Console::log(t.token + " Expected " + m_tokenizer.typeName(*it) + " As Argument #" + std::to_string(i+1) + 
				" But Got " + m_tokenizer.typeName(currT.tokenType) + " " + currT.token + ".");
			return false;
		}
		nextTokens[i] = currT;
		it++;
	}
	return true;

}
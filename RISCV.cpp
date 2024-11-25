#include "RISCV.h"
#include "Console.h"

RISCV::RISCV() {
	m_memoryAllocSize = m_initMemorySize;
	m_memory = std::malloc(m_memoryAllocSize);
	m_registers[0] = 0;
	m_registers[2] = m_initMemorySize; // set sp to point to the end of the memory block
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
		m_registers[registerIdFromStr(nextTokens[0].token)] = m_registers[registerIdFromStr(nextTokens[1].token)] + std::stoi(nextTokens[2].token);
	}

	if (t.token == "add") {
		bool ok = getNextTokens(t, nextTokens, { MY_TOKEN_REGISTER, MY_TOKEN_REGISTER, MY_TOKEN_REGISTER });
		if (!ok)
			return false;
		m_registers[registerIdFromStr(nextTokens[0].token)] = m_registers[registerIdFromStr(nextTokens[1].token)] + m_registers[registerIdFromStr(nextTokens[2].token)];
	}

	if (t.token == "li") {
		bool ok = getNextTokens(t, nextTokens, { MY_TOKEN_REGISTER, MY_TOKEN_NUM });
		if (!ok)
			return false;
		m_registers[registerIdFromStr(nextTokens[0].token)] = std::stoi(nextTokens[1].token);
	}


	if (t.token == "lw") {
		bool ok = getNextTokens(t, nextTokens, { MY_TOKEN_REGISTER, MY_TOKEN_ADDRESS });
		if (!ok)
			return false;

		m_registers[registerIdFromStr(nextTokens[0].token)] = *(int*)ptrFromAddress(nextTokens[1].token);
	}

	if (t.token == "sw") {
		bool ok = getNextTokens(t, nextTokens, { MY_TOKEN_REGISTER, MY_TOKEN_ADDRESS });
		if (!ok)
			return false;
		*(int*)ptrFromAddress(nextTokens[1].token) = m_registers[registerIdFromStr(nextTokens[0].token)];
		
	}






	return true;
}

void* RISCV::ptrFromAddress(std::string s) {
	return (void*)((char*)m_memory + addressToInt(s));
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

// you should be gauranteed that the string is a valid register
int RISCV::registerIdFromStr(std::string s) {
	if (s[0] == 'x') {
		return std::stoi(s.substr(1));
	}
	if (s == "zero")
		return 0;
	if (s == "ra")
		return 1;
	if (s == "sp")
		return 2;
	if (s == "gp")
		return 3;
	if (s == "tp")
		return 4;
	if (s == "t0")
		return 5;
	if (s == "t1")
		return 6;
	if (s == "t2")
		return 7;
	if (s == "s0" || s == "fp")
		return 8;
	if (s == "s1")
		return 9;
	
	if (s[0] == 'a') {
		return std::stoi(s.substr(1)) + 10;
	}

	if (s[0] == 's') {
		return std::stoi(s.substr(1)) + 16;
	}

	if (s[0] == 't') {
		return std::stoi(s.substr(1)) + 25;
	}

	return 0;
}
//0 1 2 3 4 5	
//1 2 ( s p )
// you should be guaranteed that the string is an address like 4(sp) or 12(x10)...
int RISCV::addressToInt(std::string s) {
	int splitId = s.find('(');
	int offset = std::stoi(s.substr(0, splitId)); // int 
	int baseId = registerIdFromStr(s.substr(splitId + 1, s.length() - splitId - 2));   // register id
	int base = m_registers[baseId];
	return base + offset;
}
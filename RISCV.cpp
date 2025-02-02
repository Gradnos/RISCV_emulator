#include "RISCV.h"
#include "Console.h"

RISCV::RISCV() {
	m_memoryAllocSize = m_initMemorySize;
	m_memory = std::malloc(m_memoryAllocSize);
	initRegisters();
	Console::log("Init Riscv");
}

RISCV::~RISCV() {
	if(m_memory)
		std::free(m_memory);
}

void RISCV::initRegisters() {
	m_registers[0] = 0;
	m_registers[2] = m_initMemorySize;
}



std::string* RISCV::getTextPtr() {
	return &m_text;
}


int RISCV::readRegister(int id) {
	return m_registers[id];
}

void RISCV::run() {
	m_tokenizer.setText(&m_text[0]);
	bool compiled = compile();
	if (!compiled) {
		Console::log("Compilation Failed.");
		return;
	}
	m_running = true;
	initRegisters();
	m_tokenizer.setText(&m_text[0]);
	Console::log("Run.");
	while (true) {
		Token t = m_tokenizer.nextToken();
		std::string a = m_tokenizer.typeName(t.tokenType) + "-" + t.token + "-";
		//std::cout << a << '\n';
		//Console::log(a);
		bool ok = handleToken(t);
		if (!ok) {
			Console::log("Error.");
			m_running = false;
			return;
		}

		if (t.tokenType == MY_TOKEN_END || m_returned) {
			Console::log("Finish.");
			m_returned = false;
			m_running = false;
			return;
		}
	}
}

void RISCV::step() {
	if (!m_running) {
		m_running = true;
		initRegisters();
		m_tokenizer = MyTokenizer(&m_text[0]);
		compile();
		m_tokenizer = MyTokenizer(&m_text[0]);
		Console::log("Run.");
	}
	Token t = m_tokenizer.nextToken();
	std::string a = m_tokenizer.typeName(t.tokenType) + "-" + t.token;
	Console::log(a);
	

	bool ok = handleToken(t);
	if (!ok) {
		Console::log("Error.");
		m_running = false;
		return;
	}
	if (t.tokenType == MY_TOKEN_END || m_returned) {
		Console::log("Finish.");
		m_returned = false;
		m_running = false;
		return;
	}
}

void RISCV::reset() {
	Console::log("Reset.");
	m_running = false;
	initRegisters();
}



bool RISCV::handleTokenForCompile(Token& t) {
	if (t.tokenType == MY_TOKEN_ACTION) {
		bool ok = checkActionValidity(t);
		if (!ok)
			return false;
	}
	else if (t.tokenType == MY_TOKEN_DEFINE) {
		handleDefine(t);
	}
	// unexpected token
	if (t.tokenType == MY_TOKEN_UNKNOWN || t.tokenType == MY_TOKEN_NAME || t.tokenType == MY_TOKEN_ADDRESS || t.tokenType == MY_TOKEN_NUM || t.tokenType == MY_TOKEN_REGISTER) {
		std::string type = m_tokenizer.typeName(t.tokenType);
		Console::log(m_tokenizer.typeName(t.tokenType) + " " + t.token + " Found at unexpected place.");
		return false;
	}
	return true;
}



bool RISCV::handleToken(Token& t) {
	if (t.tokenType == MY_TOKEN_ACTION) {
		bool ok = handleAction(t);
		if (!ok) 
			return false;
	}
	// unexpected token
	if (t.tokenType == MY_TOKEN_UNKNOWN || t.tokenType == MY_TOKEN_NAME || t.tokenType == MY_TOKEN_ADDRESS || t.tokenType == MY_TOKEN_NUM || t.tokenType == MY_TOKEN_REGISTER) {
		std::string type = m_tokenizer.typeName(t.tokenType);
		Console::log(m_tokenizer.typeName(t.tokenType)+ " " + t.token + " Found at unexpected place.");
		return false;
	}

	return true;
}

void RISCV::handleDefine(Token& t) {
	std::string cp = t.token;
	cp.resize(cp.length() - 1);// remove the ':' from define
	int id = m_tokenizer.getIndex();
	defines[cp] = id;
}

void RISCV::call(std::string name, bool storeRa) {
	if(storeRa) m_registers[1] = m_tokenizer.getIndex();
	m_tokenizer.setIndex(defines[name]);
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

	if (t.token == "subi") {
		bool ok = getNextTokens(t, nextTokens, { MY_TOKEN_REGISTER, MY_TOKEN_REGISTER, MY_TOKEN_NUM });
		if (!ok)
			return false;
		m_registers[registerIdFromStr(nextTokens[0].token)] = m_registers[registerIdFromStr(nextTokens[1].token)] - std::stoi(nextTokens[2].token);
	}

	if (t.token == "sub") {
		bool ok = getNextTokens(t, nextTokens, { MY_TOKEN_REGISTER, MY_TOKEN_REGISTER, MY_TOKEN_REGISTER });
		if (!ok)
			return false;
		m_registers[registerIdFromStr(nextTokens[0].token)] = m_registers[registerIdFromStr(nextTokens[1].token)] - m_registers[registerIdFromStr(nextTokens[2].token)];
	}

	if (t.token == "xori") {
		bool ok = getNextTokens(t, nextTokens, { MY_TOKEN_REGISTER, MY_TOKEN_REGISTER, MY_TOKEN_NUM });
		if (!ok)
			return false;
		m_registers[registerIdFromStr(nextTokens[0].token)] = m_registers[registerIdFromStr(nextTokens[1].token)] ^ std::stoi(nextTokens[2].token);
	}

	if (t.token == "xor") {
		bool ok = getNextTokens(t, nextTokens, { MY_TOKEN_REGISTER, MY_TOKEN_REGISTER, MY_TOKEN_REGISTER });
		if (!ok)
			return false;
		m_registers[registerIdFromStr(nextTokens[0].token)] = m_registers[registerIdFromStr(nextTokens[1].token)] ^ m_registers[registerIdFromStr(nextTokens[2].token)];
	}

	if (t.token == "ori") {
		bool ok = getNextTokens(t, nextTokens, { MY_TOKEN_REGISTER, MY_TOKEN_REGISTER, MY_TOKEN_NUM });
		if (!ok)
			return false;
		m_registers[registerIdFromStr(nextTokens[0].token)] = m_registers[registerIdFromStr(nextTokens[1].token)] | std::stoi(nextTokens[2].token);
	}

	if (t.token == "or") {
		bool ok = getNextTokens(t, nextTokens, { MY_TOKEN_REGISTER, MY_TOKEN_REGISTER, MY_TOKEN_REGISTER });
		if (!ok)
			return false;
		m_registers[registerIdFromStr(nextTokens[0].token)] = m_registers[registerIdFromStr(nextTokens[1].token)] | m_registers[registerIdFromStr(nextTokens[2].token)];
	}

	if (t.token == "andi") {
		bool ok = getNextTokens(t, nextTokens, { MY_TOKEN_REGISTER, MY_TOKEN_REGISTER, MY_TOKEN_NUM });
		if (!ok)
			return false;
		m_registers[registerIdFromStr(nextTokens[0].token)] = m_registers[registerIdFromStr(nextTokens[1].token)] & std::stoi(nextTokens[2].token);
	}

	if (t.token == "and") {
		bool ok = getNextTokens(t, nextTokens, { MY_TOKEN_REGISTER, MY_TOKEN_REGISTER, MY_TOKEN_REGISTER });
		if (!ok)
			return false;
		m_registers[registerIdFromStr(nextTokens[0].token)] = m_registers[registerIdFromStr(nextTokens[1].token)] & m_registers[registerIdFromStr(nextTokens[2].token)];
	}

	if (t.token == "slli") {
		bool ok = getNextTokens(t, nextTokens, { MY_TOKEN_REGISTER, MY_TOKEN_REGISTER, MY_TOKEN_NUM });
		if (!ok)
			return false;
		m_registers[registerIdFromStr(nextTokens[0].token)] = m_registers[registerIdFromStr(nextTokens[1].token)] << std::stoi(nextTokens[2].token);
	}

	if (t.token == "sll") {
		bool ok = getNextTokens(t, nextTokens, { MY_TOKEN_REGISTER, MY_TOKEN_REGISTER, MY_TOKEN_REGISTER });
		if (!ok)
			return false;
		m_registers[registerIdFromStr(nextTokens[0].token)] = m_registers[registerIdFromStr(nextTokens[1].token)] << m_registers[registerIdFromStr(nextTokens[2].token)];
	}

	if (t.token == "srli") {
		bool ok = getNextTokens(t, nextTokens, { MY_TOKEN_REGISTER, MY_TOKEN_REGISTER, MY_TOKEN_NUM });
		if (!ok)
			return false;
		m_registers[registerIdFromStr(nextTokens[0].token)] = m_registers[registerIdFromStr(nextTokens[1].token)] >> std::stoi(nextTokens[2].token);
	}

	if (t.token == "srl") {
		bool ok = getNextTokens(t, nextTokens, { MY_TOKEN_REGISTER, MY_TOKEN_REGISTER, MY_TOKEN_REGISTER });
		if (!ok)
			return false;
		m_registers[registerIdFromStr(nextTokens[0].token)] = m_registers[registerIdFromStr(nextTokens[1].token)] >> m_registers[registerIdFromStr(nextTokens[2].token)];
	}


	if (t.token == "muli") {
		bool ok = getNextTokens(t, nextTokens, { MY_TOKEN_REGISTER, MY_TOKEN_REGISTER, MY_TOKEN_NUM });
		if (!ok)
			return false;
		m_registers[registerIdFromStr(nextTokens[0].token)] = m_registers[registerIdFromStr(nextTokens[1].token)] * std::stoi(nextTokens[2].token);
	}

	if (t.token == "mul") {
		bool ok = getNextTokens(t, nextTokens, { MY_TOKEN_REGISTER, MY_TOKEN_REGISTER, MY_TOKEN_REGISTER });
		if (!ok)
			return false;
		m_registers[registerIdFromStr(nextTokens[0].token)] = m_registers[registerIdFromStr(nextTokens[1].token)] * m_registers[registerIdFromStr(nextTokens[2].token)];
	}

	if (t.token == "divi") {
		bool ok = getNextTokens(t, nextTokens, { MY_TOKEN_REGISTER, MY_TOKEN_REGISTER, MY_TOKEN_NUM });
		if (!ok)
			return false;
		m_registers[registerIdFromStr(nextTokens[0].token)] = m_registers[registerIdFromStr(nextTokens[1].token)] / std::stoi(nextTokens[2].token);
	}

	if (t.token == "div") {
		bool ok = getNextTokens(t, nextTokens, { MY_TOKEN_REGISTER, MY_TOKEN_REGISTER, MY_TOKEN_REGISTER });
		if (!ok)
			return false;
		m_registers[registerIdFromStr(nextTokens[0].token)] = m_registers[registerIdFromStr(nextTokens[1].token)] / m_registers[registerIdFromStr(nextTokens[2].token)];
	}

	if (t.token == "remi") {
		bool ok = getNextTokens(t, nextTokens, { MY_TOKEN_REGISTER, MY_TOKEN_REGISTER, MY_TOKEN_NUM });
		if (!ok)
			return false;
		m_registers[registerIdFromStr(nextTokens[0].token)] = m_registers[registerIdFromStr(nextTokens[1].token)] % std::stoi(nextTokens[2].token);
	}

	if (t.token == "rem") {
		bool ok = getNextTokens(t, nextTokens, { MY_TOKEN_REGISTER, MY_TOKEN_REGISTER, MY_TOKEN_REGISTER });
		if (!ok)
			return false;
		m_registers[registerIdFromStr(nextTokens[0].token)] = m_registers[registerIdFromStr(nextTokens[1].token)] % m_registers[registerIdFromStr(nextTokens[2].token)];
	}

	if (t.token == "li") {
		bool ok = getNextTokens(t, nextTokens, { MY_TOKEN_REGISTER, MY_TOKEN_NUM });
		if (!ok)
			return false;
		m_registers[registerIdFromStr(nextTokens[0].token)] = std::stoi(nextTokens[1].token);
	}


	if (t.token == "lw") {
		m_neededSizeAboveAddr = 4;
		bool ok = getNextTokens(t, nextTokens, { MY_TOKEN_REGISTER, MY_TOKEN_ADDRESS });
		if (!ok)
			return false;

		m_registers[registerIdFromStr(nextTokens[0].token)] = *(int*)ptrFromAddress(nextTokens[1].token);
	}

	if (t.token == "sw") {
		m_neededSizeAboveAddr = 4;
		bool ok = getNextTokens(t, nextTokens, { MY_TOKEN_REGISTER, MY_TOKEN_ADDRESS });
		if (!ok)
			return false;
		*(int*)ptrFromAddress(nextTokens[1].token) = m_registers[registerIdFromStr(nextTokens[0].token)];
		
	}

	if (t.token == "lh") {
		m_neededSizeAboveAddr = 2;
		bool ok = getNextTokens(t, nextTokens, { MY_TOKEN_REGISTER, MY_TOKEN_ADDRESS });
		if (!ok)
			return false;

		m_registers[registerIdFromStr(nextTokens[0].token)] = *(short*)ptrFromAddress(nextTokens[1].token);
	}

	if (t.token == "sh") {
		m_neededSizeAboveAddr = 2;
		bool ok = getNextTokens(t, nextTokens, { MY_TOKEN_REGISTER, MY_TOKEN_ADDRESS });
		if (!ok)
			return false;
		*(short*)ptrFromAddress(nextTokens[1].token) = m_registers[registerIdFromStr(nextTokens[0].token)];
	}

	if (t.token == "lb") {
		m_neededSizeAboveAddr = 1;
		bool ok = getNextTokens(t, nextTokens, { MY_TOKEN_REGISTER, MY_TOKEN_ADDRESS });
		if (!ok)
			return false;

		m_registers[registerIdFromStr(nextTokens[0].token)] = *(char*)ptrFromAddress(nextTokens[1].token);
	}

	if (t.token == "sb") {
		m_neededSizeAboveAddr = 1;
		bool ok = getNextTokens(t, nextTokens, { MY_TOKEN_REGISTER, MY_TOKEN_ADDRESS });
		if (!ok)
			return false;
		*(char*)ptrFromAddress(nextTokens[1].token) = m_registers[registerIdFromStr(nextTokens[0].token)];
	}


	if (t.token == "jump" || t.token == "j") {
		bool ok = getNextTokens(t, nextTokens, { MY_TOKEN_NAME });
		if (!ok)
			return false;
		std::string name = nextTokens[0].token;
		call(name, 0);
	}

	if (t.token == "call") {
		bool ok = getNextTokens(t, nextTokens, { MY_TOKEN_NAME });
		if (!ok)
			return false;
		std::string name = nextTokens[0].token;
		call(name,1);
	}
	
	if (t.token == "ret") {
		m_tokenizer.setIndex(m_registers[1]);
	}

	//"beq" || t.token == "bne" || t.token == "blt" || t.token == "bge"
	if (t.token == "beq" || t.token == "bne" || t.token == "blt" || t.token == "bge" || t.token == "ble" || t.token == "bgt") {
		bool ok = getNextTokens(t, nextTokens, { MY_TOKEN_REGISTER, MY_TOKEN_REGISTER, MY_TOKEN_NAME });
		if (!ok)
			return false;
		int a = m_registers[registerIdFromStr(nextTokens[0].token)];
		int b = m_registers[registerIdFromStr(nextTokens[1].token)];
		std::string name = nextTokens[2].token;
		if (t.token == "beq" && a == b) call(name,0);
		if (t.token == "bne" && a != b) call(name,0);
		if (t.token == "blt" && a < b) call(name,0);
		if (t.token == "ble" && a <= b) call(name,0);
		if (t.token == "bgt" && a > b) call(name,0);
		if (t.token == "bge" && a >= b) call(name,0);
	}

	if (t.token == "ecall") {
		int op = m_registers[10];
		if (op == 10) m_returned = true;
		if (op == 1) Console::print(std::to_string(m_registers[11]));
		char c = m_registers[11];
		std::string st = "";
		st += char(m_registers[11]);
		if (op == 11) Console::print(st);
	}

	// make sure x0 stays as 0 at the end of every action
	m_registers[0] = 0;
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
		if (currT.tokenType != *it) {  // check the type is what we expect
			Console::log(t.token + " Expected " + m_tokenizer.typeName(*it) + " As Argument #" + std::to_string(i+1) + 
				", But Got " + m_tokenizer.typeName(currT.tokenType) + " " + currT.token + ".");
			return false;
		}
		if (currT.tokenType == MY_TOKEN_ADDRESS && m_running) { // if is address and is running check validity
			int offset = addressToInt(currT.token);
			if (offset < 0 || offset >= m_memoryAllocSize) {
				Console::log(t.token + " Expected " + "Correct Address As Argument #" + std::to_string(i + 1) +
					", But Got Out Of Bounds Address " + currT.token + ".");
				return false;
			}
			int spaceAbove = m_memoryAllocSize - offset;
			if (spaceAbove < m_neededSizeAboveAddr) {
				Console::log(t.token + " Expected " + "Correct Address With Space Of " + std::to_string(m_neededSizeAboveAddr) 
					+" As Argument #" + std::to_string(i + 1) + ", But Got Out Smaller Free Size At Address " + currT.token + ".");
				return false;
			}
		}

		nextTokens[i] = currT;
		it++;
	}
	return true;

}

// you should be gauranteed that the string is a valid register
int RISCV::registerIdFromStr(std::string s) {
	if (s[0] == 'x')
		return std::stoi(s.substr(1));
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
	if (s[0] == 'a')
		return std::stoi(s.substr(1)) + 10;
	if (s[0] == 's')
		return std::stoi(s.substr(1)) + 16;
	if (s[0] == 't')
		return std::stoi(s.substr(1)) + 25;
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

int RISCV::getMemorySize() {
	return m_memoryAllocSize;
}

const void* RISCV::getMemoryPtr() {
	return m_memory;
}
const int* RISCV::getRegistersPtr() {
	return m_registers;
}

bool RISCV::checkActionValidity(Token& t) {
	Token nextTokens[5];
	if (t.token == "addi" || t.token == "subi" || t.token == "xori" || t.token == "ori" || t.token == "andi" || t.token == "slli" || t.token == "srli" || t.token == "muli" || t.token == "divi" || t.token == "remi") {
		bool ok = getNextTokens(t, nextTokens, { MY_TOKEN_REGISTER, MY_TOKEN_REGISTER, MY_TOKEN_NUM });
		printf("ok = %d\n", ok);
		if (!ok)
			return false;
	}

	if (t.token == "add" || t.token == "sub" || t.token == "xor" || t.token == "or" || t.token == "and" || t.token == "sll" || t.token == "srl" || t.token == "mul" || t.token == "div" || t.token == "rem") {
		bool ok = getNextTokens(t, nextTokens, { MY_TOKEN_REGISTER, MY_TOKEN_REGISTER, MY_TOKEN_REGISTER });
		if (!ok)
			return false;
	}

	if (t.token == "li") {
		bool ok = getNextTokens(t, nextTokens, { MY_TOKEN_REGISTER, MY_TOKEN_NUM });
		if (!ok)
			return false;
	}

	if (t.token == "lw"  || t.token == "sw" || t.token == "lh" || t.token == "sh" || t.token == "lb" || t.token == "sb") {
		m_neededSizeAboveAddr = 4;
		bool ok = getNextTokens(t, nextTokens, { MY_TOKEN_REGISTER, MY_TOKEN_ADDRESS });
		if (!ok)
			return false;
	}

	if (t.token == "beq" || t.token == "bne" || t.token == "blt" || t.token == "bge" || t.token == "ble" || t.token == "bgt") {
		bool ok = getNextTokens(t, nextTokens, { MY_TOKEN_REGISTER, MY_TOKEN_REGISTER, MY_TOKEN_NAME });
		if (!ok)
			return false;
	}

	if (t.token == "jump" || t.token == "call" || t.token == "j") {
		m_neededSizeAboveAddr = 4;
		bool ok = getNextTokens(t, nextTokens, { MY_TOKEN_NAME });
		if (!ok)
			return false;
	}

	return true;
}

bool RISCV::compile() {
	Console::log("Start Compiling.");
	m_tokenizer.setText(&m_text[0]);
	while (true) {
		Token t = m_tokenizer.nextToken();

		std::string a = m_tokenizer.typeName(t.tokenType) + "-" + t.token + "-";
		//Console::log(a);

		if (t.tokenType == MY_TOKEN_END) break;
		bool ok = handleTokenForCompile(t);
		if (!ok) {
			Console::log("Error in compilation. Aborting.");
			return false;
		}
	}

	Console::log("Compilation Successful.");
	return true;
}
// TO DO: Add command to clear the output window, and a help command
// add branching instruction

#include "cMain.h"
#include <string>
#include <sstream>

#define sp (registers[SP])
#define ip (registers[IP])
// instruction set
typedef enum {
	PSH,		// pushes an integer onto the stack
	ADD,		// adds the top two values currently on the stack andsaves the result
	POP,		// pops the top value currently on the stack and prints it
	SET,		// sets the register following to the int following the register
	SETR,		// sets the value of a register to the value of another register
	ADDI,		// ex: ADDI REG VAL; adds an integer value to a register
	MULT,		// Multiply the top two values on the stack and pushes the result to the stack
	SUB,		// Subtract the second to last value from the top value on the stack and pushes the result to the stack
	DIV,		// Divide the top value on the stack by the second the to the top value on the stack and pushes the result to the stack
	LOG,		// Prints value in the given Register 
	LTS,		// load top of stack. loads top value of stack into a register
	BEQ,		// branch on equal. BEQ A B 6. if A and B are equal set instruction pointer to 6
	BNQ,		// branch not equal. BNQ A B 5. if A and B are not equal, set the instruction pointer to 5
	HLT			// when this instruction is read the program is terminated 
} InstructionSet;

typedef enum {
	A, B, C, D, E, F, L, IP, SP,
	NUM_OF_REGISTERS
} Registers;

// instruction pointer
//int ip = 0;
//int sp = -1;
int stack[245];
bool running = true;
int registers[NUM_OF_REGISTERS];

// cMain: the class producing events for, wxFrame: base class
wxBEGIN_EVENT_TABLE(cMain, wxFrame)
EVT_BUTTON(10001, OnButtonClicked)
wxEND_EVENT_TABLE()

cMain::cMain() : wxFrame(nullptr, wxID_ANY, "Virtual Machine", wxPoint(30, 30), wxSize(800, 415))
{
	m_btn1 = new wxButton(this, 10001, "Run Program", wxPoint(10, 10), wxSize(150, 50));
	m_txt1 = new wxTextCtrl(this, wxID_ANY, "", wxPoint(10, 70), wxSize(300, 300));
	m_txt2 = new wxTextCtrl(this, wxID_ANY, "", wxPoint(320, 70), wxSize(300, 300), wxTE_MULTILINE);
	regLabel = new wxStaticText(this, wxID_ANY, "Register Values:", wxPoint(630, 70), wxSize(85, 25));
	regTextA = new wxStaticText(this, wxID_ANY, "Register A:", wxPoint(630, 100), wxSize(75, 25));
	regValA = new wxStaticText(this, wxID_ANY, "Null", wxPoint(710, 100), wxSize(75, 25));
	regTextB = new wxStaticText(this, wxID_ANY, "Register B:", wxPoint(630, 130), wxSize(75, 25));
	regValB = new wxStaticText(this, wxID_ANY, "Null", wxPoint(710, 130), wxSize(75, 25));
	regTextC = new wxStaticText(this, wxID_ANY, "Register C:", wxPoint(630, 160), wxSize(75, 25));
	regValC = new wxStaticText(this, wxID_ANY, "Null", wxPoint(710, 160), wxSize(75, 25));
	regTextD = new wxStaticText(this, wxID_ANY, "Register D:", wxPoint(630, 190), wxSize(75, 25));
	regValD = new wxStaticText(this, wxID_ANY, "Null", wxPoint(710, 190), wxSize(75, 25));
	regTextE = new wxStaticText(this, wxID_ANY, "Register E:", wxPoint(630, 220), wxSize(75, 25));
	regValE = new wxStaticText(this, wxID_ANY, "Null", wxPoint(710, 220), wxSize(75, 25));
	regTextF = new wxStaticText(this, wxID_ANY, "Register F:", wxPoint(630, 250), wxSize(75, 25));
	regValF = new wxStaticText(this, wxID_ANY, "Null", wxPoint(710, 250), wxSize(75, 25));
	regTextL = new wxStaticText(this, wxID_ANY, "Register L:", wxPoint(630, 280), wxSize(75, 25));
	regValL = new wxStaticText(this, wxID_ANY, "Null", wxPoint(710, 280), wxSize(75, 25));
	regTextIP = new wxStaticText(this, wxID_ANY, "Register IP:", wxPoint(630, 310), wxSize(75, 25));
	regValIP = new wxStaticText(this, wxID_ANY, "Null", wxPoint(710, 310), wxSize(75, 25));
	regTextSP = new wxStaticText(this, wxID_ANY, "Register SP:", wxPoint(630, 340), wxSize(75, 25));
	regValSP = new wxStaticText(this, wxID_ANY, "Null", wxPoint(710, 340), wxSize(75, 25));
}


cMain::~cMain()
{
}

// read input and store words in vector
std::vector<std::string> read_program(std::string& Text) {
	std::vector<std::string> Words;
	std::stringstream ss(Text);
	std::string Buf;

	while (ss >> Buf)
		Words.push_back(Buf);

	return Words;
}

bool isnum(std::string str) {
	int size = str.size();

	for (int i = 0; i < size; i++) {

		if (!isdigit(str[i])) {
			return false;
		}
	}
	return true;
}


// convert the vector that was made from the program file 
// into and integer array to be run
int* get_instr(std::vector<std::string> input) {
	const int size = input.size();
	int* arr = new int[size];

	for (int i = 0; i < input.size(); i++) {

		if (input[i] == "PSH") {
			arr[i] = PSH;
		}
		else if (input[i] == "ADD") {
			arr[i] = ADD;
		}
		else if (input[i] == "POP") {
			arr[i] = POP;
		}
		else if (input[i] == "SET") {
			arr[i] = SET;
		}
		else if (input[i] == "SETR") {
			arr[i] = SETR;
		}
		else if (input[i] == "HLT") {
			arr[i] = HLT;
		}
		else if (input[i] == "A") {
			arr[i] = A;
		}
		else if (input[i] == "B") {
			arr[i] = B;
		}
		else if (input[i] == "C") {
			arr[i] = C;
		}
		else if (input[i] == "D") {
			arr[i] = D;
		}
		else if (input[i] == "E") {
			arr[i] = F;
		}
		else if (input[i] == "IP") {
			arr[i] = IP;
		}
		else if (input[i] == "SP") {
			arr[i] = SP;
		}
		else if (input[i] == "ADDI") {
			arr[i] = ADDI;
		}
		else if (input[i] == "MULT") {
			arr[i] = MULT;
		}
		else if (input[i] == "SUB") {
			arr[i] = SUB;
		}
		else if (input[i] == "DIV") {
			arr[i] = DIV;
		}
		else if (input[i] == "LOG") {
			arr[i] = LOG;
		}
		else if (input[i] == "LTS") {
			arr[i] = LTS;
		}
		else if (input[i] == "BEQ") {
			arr[i] = BEQ;
		}
		else if (input[i] == "BNQ") {
			arr[i] = BNQ;
		}
		else if (isnum(input[i])) {
			int x = std::stoi(input[i]);
			arr[i] = x;
		}
		else {
			// if this is reached then an invalid instruction was read
			std::cerr << "invalid instruction: " << input[i] << std::endl;
			exit(0);
		}
	}
	return arr;
}

// fetch the instruction currently pointed to by the instruction pointer
int fetch(int instr[]) {
	return instr[ip];
}

void ip_inc() {
	ip++;
}

void eval(int instr, int program[], wxTextCtrl *m_txt2) {

	switch (instr) {
	case HLT: {
		running = false;
		break;
	}
	case PSH: {
		sp++;
		stack[sp] = program[++ip];
		break;
	}
	case POP: {
		int val_popped = stack[sp--];
		wxString valstring;
		valstring << val_popped;
		(*m_txt2) << "popped " << valstring << "\n";
		break;
	}
	case ADD: {
		int a = stack[sp--];
		int b = stack[sp--];
		int result = b + a;
		sp++;
		stack[sp] = result;
		break;
	}
	case SET: {
		// reg is the register to be set. registers[reg] is the value of that register
		// set to the next value of the program
		// Example: SET B 8. sets register B to value 8
		int reg = program[ip + 1];
		registers[reg] = program[ip + 2];
		ip = ip + 2;
		break;
	}
	case SETR: {
		// reg1 is the register to be set to the value of reg2
		// ex. SETR A B. set the value of register A to the Value of register B
		int reg1 = program[ip + 1];
		int reg2 = program[ip + 2];
		registers[reg1] = registers[reg2];
		ip = ip + 2;
		break;
	}
	case ADDI: {
		// Add tp the register following ADDI the value following the rigister
		// Example: ADDI C 5. add 5 to register c
		int reg = program[ip + 1];
		int val = program[ip + 2];
		registers[reg] += val;
		ip = ip + 2;
		break;
	}
	case MULT: {
		int a = stack[sp--];
		int b = stack[sp--];
		int result = b * a;
		sp++;
		stack[sp] = result;
		break;
	}
	case SUB: {
		int a = stack[sp--];
		int b = stack[sp--];
		int result = a - b;
		sp++;
		stack[sp] = result;
		break;
	}
	case DIV: {
		int a = stack[sp--];
		int b = stack[sp--];
		int result = a / b;
		sp++;
		stack[sp] = result;
		break;
	}
	case LOG: {
		int reg = program[ip + 1];
		if (registers[reg] == NULL) {
			std::cerr << "Register A has NULL value" << std::endl;
		}
		else {
			(*m_txt2) << "Register " << reg << ": "<< registers[reg] << "\n";
		}
		ip = ip + 1;
		break;
	}
	case LTS: {
		int reg1 = program[++ip];
		registers[reg1] = stack[sp];
		sp = sp - 1;
		break;
	}
	case BEQ: {
		int reg1 = program[ip + 1];
		int reg2 = program[ip + 2];
		int val = program[ip + 3];

		if (registers[reg1] == registers[reg2])
		{
			ip = val;
		}
		else
		{
			ip = ip + 3;
		}
		break;
	}
	case BNQ: {
		int reg1 = program[ip + 1];
		int reg2 = program[ip + 2];
		int val = program[ip + 3];

		if (registers[reg1] != registers[reg2])
		{
			ip = val;
		}
		else
		{
			ip = ip + 3;
		}
		break;
	}
	default: {}
	}

}

void updateRegText(wxStaticText* regValA, wxStaticText* regValB, wxStaticText* regValC, wxStaticText* regValD, wxStaticText* regValE, wxStaticText* regValF, wxStaticText* regValL, wxStaticText* regValIP, wxStaticText* regValSP)
{
	wxString valA, valB, valC, valD, valE, valF, valL, valIP, valSP;
	valA << registers[0];
	valB << registers[1];
	valC << registers[2];
	valD << registers[3];
	valE << registers[4];
	valF << registers[5];
	valL << registers[6];
	valIP << registers[7];
	valSP << registers[8];
	regValA->SetLabel(valA);
	regValB->SetLabel(valB);
	regValC->SetLabel(valC);
	regValD->SetLabel(valD);
	regValE->SetLabel(valE);
	regValF->SetLabel(valF);
	regValL->SetLabel(valL);
	regValIP->SetLabel(valIP);
	regValSP->SetLabel(valSP);
}

void resetReg() 
{
	for (int i = 0; i < NUM_OF_REGISTERS; i++)
	{
		registers[i] = 0;
	}
}

void cMain::OnButtonClicked(wxCommandEvent& evt)
{
	// Reset running value to true
	running = true;
	// reset register values
	resetReg();
	std::string input = std::string(m_txt1->GetValue().mb_str());
	std::vector<std::string> program;
	program = read_program(input);
	const int size = program.size();
	int* instr = new int[size];
	instr = get_instr(program);

	// running the program
	while (running) {
		eval(fetch(instr), instr, m_txt2);
		//wxString instruction;
		//instruction << fetch(instr) << "\n";
		//(*m_txt2) << instruction;
		ip_inc();
		updateRegText(regValA, regValB, regValC, regValD, regValE, regValF, regValL, regValIP, regValSP);
	}
	(*m_txt2) << "Program Finished\n";
	//std::cout << input << std::endl;
	evt.Skip();
}


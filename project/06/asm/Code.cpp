#include "stdafx.h"
#include "Code.h"

using namespace hack::assembler;
using hack::Diag;

typedef std::map<std::string, int> SymbolTable;

const int MAX_CONST = (1 << 15);

const int FREE_MEM = 16;

void InitSymTab(SymbolTable& st)
{
	for (int i=0; i < 16; ++i) {
		st["R" + boost::lexical_cast<std::string>(i)] = i;
	}

	st["SP"]   = 0;
	st["LCL"]  = 1;
	st["ARG"]  = 2;
	st["THIS"] = 3;
	st["THAT"] = 4;

	st["SCREEN"] = 0x4000;
	st["KBD"]    = 0x6000;
}

void InitCompMap(std::map<std::string, std::string>& m)
{
	m["0"]   = "0101010";
	m["1"]   = "0111111";
	m["-1"]  = "0111010";
	m["D"]   = "0001100";
	m["A"]   = "0110000";
	m["!D"]  = "0001101";
	m["!A"]  = "0110001";
	m["-D"]  = "0001111";
	m["-A"]  = "0110011";
	m["D+1"] = "0011111";
	m["A+1"] = "0110111";
	m["D-1"] = "0001110";
	m["A-1"] = "0110010";
	m["D+A"] = "0000010";
	m["A+D"] = "0000010";
	m["D-A"] = "0010011";
	m["A-D"] = "0000111";
	m["D&A"] = "0000000";
	m["D|A"] = "0010101";

	m["M"]   = "1110000";
	m["!M"]  = "1110001";
	m["-M"]  = "1110011";
	m["M+1"] = "1110111";
	m["M-1"] = "1110010";
	m["D+M"] = "1000010";
	m["M+D"] = "1000010";
	m["D-M"] = "1010011";
	m["M-D"] = "1000111";
	m["D&M"] = "1000000";
	m["D|M"] = "1010101";
}

void InitJumpMap(std::map<std::string, std::string>& m)
{
	m[""]    = "000";
	m["JGT"] = "001";
	m["JEQ"] = "010";
	m["JGE"] = "011";
	m["JLT"] = "100";
	m["JNE"] = "101";
	m["JLE"] = "110";
	m["JMP"] = "111";
}

SymbolTable FirstPass(const std::vector<Command>& cmds, Diag& diag)
{
	SymbolTable symtab;
	InitSymTab(symtab);

	int pos = 0; // real code position, labels do not count
	bool didWarn = false;

	for (auto c = cmds.begin(); c != cmds.end(); ++c) {
		switch (c->GetType()) {
		case Command::LABEL:
			if (symtab.find(c->GetSymbol()) != symtab.end())
				diag.Error(c->GetPosition(), diag::err_lbl_redef) << c->GetSymbol();
			symtab[c->GetSymbol()] = pos;

			if (pos >= MAX_CONST && !didWarn) {
				diag.Warning(c->GetPosition(), diag::wrn_far_sym) << c->GetSymbol();
				didWarn = true;
			}
			break;

		case Command::A_CMD:
		case Command::C_CMD:
			pos++;
			break;
		}


	}

	return symtab;
}

std::string BinaryString(int n)
{
	std::string res;

	for (int i=14; i >= 0; --i) {
		res += (n & (1 << i)) ? "1" : "0";
	}

	return res;
}

int GetTruncConst(int val)
{
	return val & 0x7FFF;
}

std::string AInstruction(const Command& cmd, SymbolTable& symtab, int& freemem, Diag& diag)
{
	auto sym = cmd.GetSymbol();

	int val = 0;
	if (std::isdigit(sym[0])) {
		val = boost::lexical_cast<int>(sym);
	}
	else {
		if (symtab.find(sym) == symtab.end())
			symtab[sym] = freemem++;

		val = symtab[sym];
	}

	if (val >= MAX_CONST) 
		diag.Warning(cmd.GetPosition(), diag::wrn_const) <<  val << GetTruncConst(val);

	return "0" + BinaryString(val);
}

std::string Comp(const std::string& code, const hack::CodePosition& pos, Diag& diag)
{
	static std::map<std::string, std::string> map;
	static bool initialized = false;

	if (!initialized) {
		InitCompMap(map);
		initialized = true;
	}

	if (map.find(code) == map.end()) {
		diag.Error(pos, diag::err_unk_comp_code) << code;
		return "";
	}

	return map[code];
}

std::string Dest(const std::string& code)
{
	std::string res;
	
	res += (code.find("A") == code.npos) ? "0" : "1";
	res += (code.find("D") == code.npos) ? "0" : "1";
	res += (code.find("M") == code.npos) ? "0" : "1";

	return res;
}

std::string Jump(const std::string& code, const hack::CodePosition& pos, Diag& diag)
{
	static std::map<std::string, std::string> map;
	static bool initialized = false;

	if (!initialized) {
		InitJumpMap(map);
		initialized = true;
	}

	if (map.find(code) == map.end()) {
		diag.Error(pos, diag::err_unk_jump_code) << code;
		return "";
	}

	return map[code];
}

std::string CInstruction(const Command& cmd, Diag& diag)
{
	return "111" + Comp(cmd.GetComp(), cmd.GetPosition(), diag) + Dest(cmd.GetDest()) + Jump(cmd.GetJump(), cmd.GetPosition(), diag);
}

std::vector<std::string> SecondPass(const std::vector<Command>& cmds, SymbolTable& symtab, Diag& diag)
{
	std::vector<std::string> res;

	int freemem = FREE_MEM;

	for (auto c = cmds.begin(); c != cmds.end(); ++c) {
		switch (c->GetType()) {
		case Command::LABEL:
			// ignore
			break;

		case Command::A_CMD:
			res.push_back(AInstruction(*c, symtab, freemem, diag));
			break;

		case Command::C_CMD:
			res.push_back(CInstruction(*c, diag));
			break;
		}
	}

	return res;
}

std::vector<std::string> hack::assembler::Translate(const std::vector<Command>& cmds, Diag& diag)
{
	diag.SetErrorFatal(false, "Code generation");
	auto syms = FirstPass(cmds, diag);
	return SecondPass(cmds, syms, diag);
}


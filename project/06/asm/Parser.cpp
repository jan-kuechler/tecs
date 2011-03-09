#include "stdafx.h"
#include "Parser.h"

using namespace hack::assembler;

namespace
{
	bool ValidForSymbol(char c, bool first)
	{
		bool isFirst = std::isalpha(c) || c == '_' || c == '.' || c == '$' || c == ':';
		return first ? isFirst : isFirst || std::isdigit(c);
	}
};

Parser::Parser(std::istream& in)
	: inp(0)
{ 
	std::string line;
	while (std::getline(in, line))
		input += line + '\n';
}

Parser::Parser(const std::string& in)
	: inp(0), input(in)
{ }

void Parser::Parse()
{
	NextChar();
	
	Command c;
	while ( (c = ParseCmd()) != Command::NONE)
		cmds.push_back(c);
}

Command Parser::ParseCmd()
{
	SkipSpaces();

	switch (cur) {
	case '@':
		return ParseAInstr();

	case '(':
		return ParseLabel();

	case '/':
		NextChar();
		if (cur != '/')
			throw std::runtime_error("stray '/'");
			//diag.Error(Position(), diag::err_stray_cmt);
		SkipComment();
		return ParseCmd();

	case '\0':
		return Command();

	default:
		return ParseCInstr();
	}
}

std::string Parser::ReadSymbol()
{
	std::string sym;

	bool first = true;
	while (ValidForSymbol(cur, first)) {
		sym += cur;
		NextChar();
		first = false;
	}

	return sym;
}

std::string Parser::ReadConst()
{
	std::string cnst;

	while (std::isdigit(cur)) {
		cnst += cur;
		NextChar();
	}

	return cnst;
}

Command Parser::ParseLabel()
{
	assert(cur == '(');
	NextChar();

	std::string lbl = ReadSymbol();

	if (cur != ')')
		throw std::runtime_error("invalid symbol");
		// diag.Error(Position(), diag::err_inv_sym_in_label, cur);

	NextChar(); // Skip the ')'

	return Command(Command::LABEL, lbl);
}

Command Parser::ParseAInstr()
{
	assert(cur == '@');
	NextChar();

	if (std::isdigit(cur))
		return Command(Command::A_CMD, ReadConst());
	else if (ValidForSymbol(cur, true))
		return Command(Command::A_CMD, ReadSymbol());
	else
		throw std::runtime_error("invalid @");
		// diag.Error(Position(), diag::err_inv_at);
}

Command Parser::ParseCInstr()
{
	std::string instr;

	while (cur && cur != '/' && !std::isspace(cur)) {
		instr += cur;
		NextChar();
	}

	return SplitCInstr(instr);
}

Command Parser::SplitCInstr(const std::string& instr)
{
	boost::regex regex("^([ADM][ADM]?[ADM]?=)?([^;]+)(;...)?$");
	boost::smatch match;

	if (!boost::regex_search(instr, match, regex) || !match[0].matched)
		throw std::runtime_error("Invalid C instruction");
		// diag.Error(Position(), diag::err_inv_c_instr, instr);

	std::string dest, jump;

	if (match[1].matched) {
		dest = match[1];
		assert(boost::ends_with(dest, "="));
		dest = dest.substr(0, dest.length() - 1);
	}

	if (match[3].matched) {
		jump = match[3];
		assert(jump[0] == ';');
		jump = jump.substr(1);
	}

	return Command(Command::C_CMD, dest, match[2], jump);
}

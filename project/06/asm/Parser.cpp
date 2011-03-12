#include "stdafx.h"
#include "Parser.h"
#include "DiagCodes.h"

using namespace hack::assembler;

namespace
{
	bool ValidForSymbol(char c, bool first)
	{
		bool isFirst = std::isalpha(c) || c == '_' || c == '.' || c == '$' || c == ':';
		return first ? isFirst : isFirst || std::isdigit(c);
	}
};

Parser::Parser(std::istream& in, const std::string& fileName, hack::Diag& diag)
: inp(0), diag(diag), line(1), fileName(fileName)
{ 
	std::string line;
	while (std::getline(in, line))
		input += line + '\n';
}

Parser::Parser(const std::string& in, const std::string& fileName, hack::Diag& diag)
: inp(0), input(in), diag(diag), line(1), fileName(fileName)
{ }

hack::CodePosition Parser::Position()
{
	hack::CodePosition pos;
	pos.file = fileName;
	pos.line = line;
	return pos;
}

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
			diag.Error(Position(), diag::err_stray_cmt);
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
		diag.Error(Position(), diag::err_inv_sym_in_label) << cur;

	NextChar(); // Skip the ')'

	return Command(Command::LABEL, lbl, Position());
}

Command Parser::ParseAInstr()
{
	assert(cur == '@');
	NextChar();

	auto pos = Position();
	if (std::isdigit(cur))
		return Command(Command::A_CMD, ReadConst(), pos);
	else if (ValidForSymbol(cur, true))
		return Command(Command::A_CMD, ReadSymbol(), pos);
	else {
		diag.Error(pos, diag::err_inv_at);
		return Command();
	}
}

Command Parser::ParseCInstr()
{
	std::string instr;

	auto pos = Position();
	while (cur && cur != '/' && !std::isspace(cur)) {
		instr += cur;
		NextChar();
	}

	return SplitCInstr(instr, pos);
}

Command Parser::SplitCInstr(const std::string& instr, const hack::CodePosition& pos)
{
	boost::regex regex("^([ADM][ADM]?[ADM]?=)?([01+\\-&|!ADM]+)(;...)?$");
	boost::smatch match;

	if (!boost::regex_search(instr, match, regex) || !match[0].matched)
		diag.Error(pos, diag::err_inv_c_instr) << instr;

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

	return Command(Command::C_CMD, dest, match[2], jump, pos);
}

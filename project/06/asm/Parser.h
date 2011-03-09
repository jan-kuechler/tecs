#ifndef PARSER_H
#define PARSER_H

#include "Command.h"

namespace hack { namespace assembler {

class Parser
{
	std::string input;
	char cur;
	size_t inp;

	std::vector<Command> cmds;
public:
	Parser(std::istream& in);
	Parser(const std::string& in);

	void Parse();

	size_t GetNumCommands() const
	{
		return cmds.size();
	}

	Command GetCommand(size_t i) const
	{
		return cmds[i];
	}

	std::vector<Command> GetCommands() const
	{
		return cmds;
	}

private:
	Command ParseCmd();

	Command ParseLabel();
	Command ParseAInstr();
	Command ParseCInstr();

	std::string ReadSymbol();
	std::string ReadConst();

	Command SplitCInstr(const std::string& instr);

	void SkipSpaces()
	{
		while (std::isspace(cur))
			NextChar();
	}

	void SkipComment()
	{
		while (cur && cur != '\n')
			NextChar();
	}

	void NextChar()
	{
		if (inp >= input.size())
			cur = 0;
		else
			cur = input[inp++];
	}

};

}; };

#endif //PARSER_H

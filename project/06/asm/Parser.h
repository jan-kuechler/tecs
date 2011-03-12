#ifndef PARSER_H
#define PARSER_H

#include "Command.h"
#include "DiagCodes.h"

namespace hack { namespace assembler {

class Parser
{
	Diag& diag;

	std::string input;
	char cur;
	size_t inp;
	std::string fileName;
	size_t line;

	std::vector<Command> cmds;
public:
	Parser(std::istream& in, const std::string& fileName, Diag& diag);
	Parser(const std::string& in, const std::string& fileName, Diag& diag);

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

	Command SplitCInstr(const std::string& instr, const CodePosition& pos);

	void SkipSpaces()
	{
		while (std::isspace(cur)) {
			if (cur == '\n')
				line++;
			NextChar();
		}
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

	CodePosition Position();

};

}; };

#endif //PARSER_H

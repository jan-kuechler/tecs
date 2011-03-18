#ifndef PARSER_H
#define PARSER_H

#include "Command.h"
#include "DiagCodes.h"

namespace hack { namespace vm {

class Parser
{
	hack::Diag& diag;

	std::string fileName;
	std::string input;

	std::vector<Command> cmds;

public:
	Parser(const boost::filesystem::path& file, hack::Diag& diag);
	Parser(const std::string& input, const std::string& fileName, hack::Diag& diag);

	void Parse();

	std::vector<Command> GetCommands() const
	{
		return cmds;
	}

private:
	static std::map<std::string, Command::Type> typeMap;
	void InitTypeMap();

	Command ParseLine(std::string line, size_t ln) const;
	std::vector<std::string> SplitAndRemoveComments(const std::string& line) const;
};

}; };

#endif //PARSER_H

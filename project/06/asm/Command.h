#ifndef COMMAND_H
#define COMMAND_H

#include "Diag.h"

namespace hack { namespace assembler {

class Command
{
public:
	enum Type {
		A_CMD, C_CMD, LABEL,
		NONE,
	};

private:
	Type type;
	CodePosition pos;
	
	struct CData 
	{
		std::string dest;
		std::string comp;
		std::string jump;

		CData(const std::string& d, const std::string& c, const std::string& j)
		: dest(d), comp(c), jump(j)
		{ }
	};

	boost::variant<std::string, CData> data;

public:
	Command()
	: type(NONE)
	{ }

	Command(Type tp, const std::string& sym, const CodePosition& pos = CodePosition())
		: type(tp), data(sym), pos(pos)
	{
		assert(tp == A_CMD || tp == LABEL);
	}

	Command(Type tp, const std::string& d, const std::string& c, const std::string& j, const CodePosition& pos = CodePosition())
		: type(tp), data(CData(d, c, j)), pos(pos)
	{
		assert(tp == C_CMD);
	}

	Type GetType() const
	{
		return type;
	}

	std::string GetSymbol() const
	{
		assert(type == A_CMD || type == LABEL);
		return boost::get<std::string>(data);
	}

	std::string GetDest() const
	{
		assert(type == C_CMD);
		return boost::get<CData>(data).dest;
	}

	std::string GetComp() const
	{
		assert(type == C_CMD);
		return boost::get<CData>(data).comp;
	}

	std::string GetJump() const
	{
		assert(type == C_CMD);
		return boost::get<CData>(data).jump;
	}

	CodePosition GetPosition() const
	{
		return pos;
	}

	bool operator == (const Command::Type& rhs) const
	{
		return type == rhs;
	}

	bool operator != (const Command::Type& rhs) const
	{
		return !(*this == rhs);
	}
};

}; };

#endif
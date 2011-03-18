#ifndef COMMAND_H
#define COMMAND_H

#include "Diag.h"

namespace hack { namespace vm {

class Command
{
public:
	enum Type {
		Push, Pop,
		Label, Goto, If,
		Function, Return, Call,
		Add, Sub, Neg,
		And, Or, Not,
		Gt, Lt, Eq,

		NONE,
	};

private:
	Type type;

	std::string cmd;
	std::string strArg;
	int         intArg;

	hack::CodePosition pos;

public:
	Command(Type tp)
	: type(tp)
	{ }

	Command(Type tp, const hack::CodePosition& pos, const std::string& cmd)
	: type(tp), cmd(cmd), pos(pos)
	{ }

	Command(Type tp, const hack::CodePosition& pos, const std::string& cmd, const std::string& sa)
	: type(tp), cmd(cmd), strArg(sa), pos(pos)
	{ }

	Command(Type tp, const hack::CodePosition& pos, const std::string& cmd, const std::string& sa, int ia)
	: type(tp), cmd(cmd), strArg(sa), intArg(ia), pos(pos)
	{ }

	Type GetType() const
	{
		return type;
	}

	std::string GetCmd() const
	{
		return cmd;
	}

	std::string GetStringArg() const
	{
		return strArg;
	}

	int GetIntArg() const
	{
		return intArg;
	}

	hack::CodePosition GetPos() const
	{
		return pos;
	}

	static size_t GetNumArgs(Type tp)
	{
		switch (tp) {
		case Add:
		case Sub:
		case Neg:
		case And:
		case Or:
		case Not:
		case Gt:
		case Lt:
		case Eq:
		case Return:
			return 0;
		case Label:
		case Goto:
		case If:
			return 1;
		case Push:
		case Pop:
		case Function:
		case Call:
			return 2;

		default:
			throw std::runtime_error("Invalid type");
		}
	}
};

}; };

#endif //COMMAND_H

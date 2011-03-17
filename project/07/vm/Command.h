#ifndef COMMAND_H
#define COMMAND_H

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

	std::string strArg;
	int         intArg;

public:
	Command(Type tp)
	: type(tp)
	{ }

	Command(Type tp, const std::string& sa)
	: type(tp), strArg(sa)
	{ }

	Command(Type tp, const std::string& sa, int ia)
	: type(tp), strArg(sa), intArg(ia)
	{ }

	Type GetType() const
	{
		return type;
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

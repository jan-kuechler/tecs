#ifndef DIAG_H
#define DIAG_H

namespace hack {

struct CodePosition
{
	std::string file;
	size_t line;

	CodePosition()
	{ }

	CodePosition(const std::string& f, size_t l)
	: file(f), line(l)
	{ }
};

class DiagClient
{
public:
	struct Message
	{
		size_t id;
		const char *fmt;
		size_t nargs;
	};

	virtual void Print(size_t id, const CodePosition& pos, const std::vector<std::string>& args);

protected:
	void PrintPosition(std::ostream& out, const CodePosition& pos);

	virtual Message* GetMessages(size_t &num) const
	{
		num = 0;
		return 0;
	}
};

struct FatalError : public std::runtime_error
{
	std::string where;
	FatalError(std::string where)
	: std::runtime_error((where.empty() ? std::string("Fatal error") : (where + ": fatal error.")).c_str()), where(where)
	{ }
};

class Diag
{
	DiagClient& client;

	size_t nErrors, nWarnings;
	bool errFatal;
	std::string state;

public:
	struct Proxy
	{
		mutable bool alive;
		std::vector<std::string> args;

		size_t id;
		const CodePosition& pos;
		DiagClient& client;

		bool kill;
		std::string state;

		friend class Diag;

		Proxy(size_t id, DiagClient& client, const CodePosition& pos, bool kill, const std::string& st)
		: alive(true), id(id), pos(pos), client(client), kill(kill), state(st)
		{	}

	public:
		Proxy(const Proxy& p)
		: alive(true), args(p.args), id(p.id), pos(p.pos), client(p.client), kill(p.kill), state(p.state)
		{
			p.alive = false;
		}

		~Proxy();

		void Emit();

		template <typename T>
		Proxy operator << (const T& t)
		{
			args.push_back(boost::lexical_cast<std::string>(t));
			return *this;
		}
	};

	Diag(DiagClient& client)
	: client(client), nErrors(0), nWarnings(0), errFatal(true)
	{ }

	void SetErrorFatal(bool fatal, const std::string& where = "");

	Proxy Error(const CodePosition& pos, size_t code);
	Proxy Error(size_t code);

	Proxy Warning(const CodePosition& pos, size_t code);
	Proxy Warning(size_t code);

	size_t GetNumErrors() const
	{
		return nErrors;
	}

	size_t GetNumWarnings() const
	{
		return nWarnings;
	}
};

};

#endif //DIAG_H

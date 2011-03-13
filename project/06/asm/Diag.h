#ifndef DIAG_H
#define DIAG_H

namespace hack {

struct CodePosition
{
	std::string file;
	size_t line;
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

	virtual Message* GetMessages(size_t &num) const = 0;
};

class Diag
{
	DiagClient& client;

	size_t nErrors, nWarnings;
	bool errFatal;

public:
	struct Proxy
	{
		mutable bool alive;
		std::vector<std::string> args;

		size_t id;
		const CodePosition& pos;
		DiagClient& client;

		bool kill;

	public:
		Proxy(size_t id, DiagClient& client, const CodePosition& pos, bool kill)
		: alive(true), id(id), pos(pos), client(client), kill(kill)
		{
		}

		Proxy(const Proxy& p)
		: alive(true), args(p.args), id(p.id), pos(p.pos), client(p.client), kill(p.kill)
		{ 
			p.alive = false;
		}

		~Proxy()
		{
			Emit();
		}

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

	void SetErrorFatal(bool fatal);

	Proxy Error(const CodePosition& pos, size_t code);
	Proxy Error(size_t code);

	Proxy Warning(const CodePosition& pos, size_t code);
	Proxy Warning(size_t code);

};

};

#endif //DIAG_H

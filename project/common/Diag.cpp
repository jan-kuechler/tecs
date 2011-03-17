#include "stdafx.h"
#include "Diag.h"

using namespace hack;

Diag::Proxy::~Proxy()
{
	// Do not call the potentially throwing Emit()
	// when the destructor is called during stack
	// unwinding.
	if (!std::uncaught_exception())
		Emit();
}

void Diag::Proxy::Emit()
{
	if (alive) {
		client.Print(id, pos, args);
		alive = false;

		// This is save, even though the exception gets thrown
		// in the destructor of Diag::Proxy, as Emit() is not 
		// called, when the object gets destructed during stack
		// unwinding.
		if (kill)
			throw FatalError(state);
	}
}

void Diag::SetErrorFatal(bool fatal, const std::string& where)
{
	errFatal = fatal;
	state = where;
}

Diag::Proxy Diag::Error(const CodePosition& pos, size_t id)
{
	nErrors++;
	return Proxy(id, client, pos, errFatal, state);
}

Diag::Proxy Diag::Error(size_t id)
{
	CodePosition noPos;
	return Error(noPos, id);
}

Diag::Proxy Diag::Warning(const CodePosition& pos, size_t id)
{
	nWarnings++;
	return Proxy(id, client, pos, false, state);
}

Diag::Proxy Diag::Warning(size_t id)
{
	CodePosition noPos;
	return Warning(noPos, id);
}

void DiagClient::PrintPosition(std::ostream& out, const CodePosition& pos)
{
	if (pos.file.empty())
		return;
	out << pos.file << ":" << pos.line << " - ";
}

void DiagClient::Print(size_t id, const CodePosition& pos, const std::vector<std::string>& args)
{
	size_t num;
	auto msg = GetMessages(num);
	if (!msg)
		num = 0;

	for (size_t i=0; i < num; ++i) {
		if (msg[i].id != id)
			continue;

		assert(args.size() >= msg[i].nargs);

		boost::format fmt(msg[i].fmt);
		for (size_t a=0; a < std::min(msg[i].nargs, args.size()); ++a) {
			fmt % args[a];
		}

		PrintPosition(std::cerr, pos);
		std::cerr << fmt.str() << std::endl;

		return;
	}
	throw std::runtime_error("Unknown diag code");
}

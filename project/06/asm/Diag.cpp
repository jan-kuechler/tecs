#include "stdafx.h"
#include "Diag.h"

using namespace hack;

void Diag::Proxy::Emit()
{
	if (alive) {
		client.Print(id, pos, args);
		alive = false;

		if (kill)
			throw std::runtime_error("Fatal error");
	}
}

void Diag::SetErrorFatal(bool fatal)
{
	errFatal = fatal;
}

Diag::Proxy Diag::Error(const CodePosition& pos, size_t id)
{
	nErrors++;
	return Proxy(id, client, pos, errFatal);
}

Diag::Proxy Diag::Error(size_t id)
{
	CodePosition noPos;
	return Error(noPos, id);
}

Diag::Proxy Diag::Warning(const CodePosition& pos, size_t id)
{
	nWarnings++;
	return Proxy(id, client, pos, false);
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

	for (size_t i=0; i < num; ++i) {
		if (msg[i].id != id)
			continue;

		assert(args.size() >= msg[i].nargs);

		boost::format fmt(msg[i].fmt);
		for (size_t a=0; a < msg[i].nargs; ++a) {
			fmt % args[a];
		}

		PrintPosition(std::cerr, pos);
		std::cerr << fmt.str() << std::endl;

		return;
	}
	throw std::runtime_error("Unknown diag code");
}

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

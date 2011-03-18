// vm.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Parser.h"
#include "CodeGen.h"
#include "DiagCodes.h"


int main(int argc, char* argv[])
{
	using namespace hack::vm;
	std::string file = argv[1];

	std::string outFile = boost::replace_last_copy(file, ".vm", ".asm");

	VmDiagClient diagClient;
	hack::Diag diag(diagClient);

	Parser parser(file, diag);
	parser.Parse();

	std::ofstream out(outFile);
	CodeGen cg(out, diag);
	cg.Generate(file, parser.GetCommands());

	return 0;
}


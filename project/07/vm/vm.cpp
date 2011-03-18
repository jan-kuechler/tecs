// vm.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Parser.h"
#include "CodeGen.h"
#include "DiagCodes.h"

namespace po = boost::program_options;
namespace fs = boost::filesystem;

#define PROG_NAME "hack-vm"
#define PROG_VERSION "0.1"

bool parse_file(const fs::path& path, hack::vm::CodeGen& codeGen, hack::Diag& diag)
{
	hack::vm::Parser p(path, diag);
	p.Parse();
	if (diag.GetNumErrors() > 0)
		return false;

	codeGen.Generate(path.string(), p.GetCommands());
	return diag.GetNumErrors() == 0;
}

void parse_directory(const fs::path& path, hack::vm::CodeGen& codeGen, hack::Diag& diag)
{
	for (fs::directory_iterator f(path), end; f != end; ++f) {
		if (fs::is_regular_file(*f) && f->path().extension() == ".vm")
			if (!parse_file(f->path(), codeGen, diag))
				return;
	}
}

int main(int argc, char **argv)
{
	std::string outf;
	std::vector<std::string> inf;

	po::options_description optDesc;
	optDesc.add_options()
		("version,v", "Print version and exit")
		("help,h", "Print help and exit")
		(",o", po::value<std::string>(&outf)->default_value("*"), "Output file");

	po::options_description optHidden;
	optHidden.add_options()
		("infile", po::value<std::vector<std::string>>(&inf), "Input file(s)");

	po::positional_options_description optPos;
	optPos.add("infile", -1);

	po::options_description optAll;
	optAll.add(optDesc).add(optHidden);

	po::variables_map opt;
	po::store(po::command_line_parser(argc, argv).options(optAll).positional(optPos).run(), opt);
	po::notify(opt);

	if (opt.count("help")) {
		std::cout << argv[0] << " [options] <infile>\n";
		std::cout << optDesc << "\n";
		return 0;
	}
	if (opt.count("version")) {
		std::cout << PROG_NAME " - " PROG_VERSION "\n";
		return 0;
	}

	if (opt.count("infile") < 1) {
		std::cerr << "No input files\n";
		return 1;
	}

	if (outf == "*") {
		std::string first = inf[0];
		if (boost::ends_with(first, ".vm"))
			outf = boost::replace_last_copy(first, ".vm", ".asm");
		else
			outf = first + ".asm";
	}

	try {
		hack::vm::VmDiagClient diagClient;
		hack::Diag diag(diagClient);

		std::ofstream out(outf);
		hack::vm::CodeGen codeGen(out, diag);

		for (auto it = inf.begin(); it != inf.end(); ++it) {
			if (fs::is_directory(*it))
				parse_directory(*it, codeGen, diag);
			else
				parse_file(*it, codeGen, diag);

			if (diag.GetNumErrors() > 0) {
				std::cerr << "Too many errors, terminating\n";
				return 1;
			}
		}
	}
	catch (std::exception& ex) {
		std::cout << ex.what() << std::endl;
		return 1;
	}

	return 0;
}

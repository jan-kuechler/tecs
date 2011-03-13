// asm.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Parser.h"
#include "Code.h"

namespace po = boost::program_options;

#define PROG_NAME "hack-asm"
#define PROG_VERSION "0.2"

int main(int argc, char* argv[])
{
	std::string inf, outf;

	po::options_description optDesc;
	optDesc.add_options()
		("version,v", "Print version and exit")
		("help,h", "Print help message and exit")
		(",o", po::value<std::string>(&outf)->default_value("*"), "Target file");

	po::options_description optHidden;
	optHidden.add_options()
		("infile", po::value<std::string>(&inf), "Input file");
	
	po::positional_options_description optInf;
	optInf.add("infile", 1);

	po::options_description optAll;
	optAll.add(optDesc).add(optHidden);

	po::variables_map opt;
	po::store(po::command_line_parser(argc, argv).options(optAll).positional(optInf).run(), opt);
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
		if (boost::ends_with(inf, ".asm"))
			outf = boost::replace_last_copy(inf, ".asm", ".hack");
		else
			outf = inf + ".hack";
	}

	try {
		using namespace hack::assembler;

		AsmDiagClient diagClient;
		hack::Diag diag(diagClient);

		std::ifstream in(inf);
		Parser p(in, inf, diag);
		p.Parse();

		auto txt = Translate(p.GetCommands(), diag);

		auto nerr = diag.GetNumErrors();
		if (nerr) {
			std::cerr << nerr << (nerr == 1 ? " error" : " errors") << ", terminating.\n";
			return nerr;
		}
		else {
			std::ofstream out(outf);
			std::for_each(txt.begin(), txt.end(), [&out](const std::string& line) {
				out << line << "\n";
			});
		}
	}
	catch (std::exception& ex) {
		std::cout << ex.what() << std::endl;
		return 1;
	}

	return 0;
}


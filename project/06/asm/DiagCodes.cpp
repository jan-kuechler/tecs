#include "stdafx.h"
#include "DiagCodes.h"

using namespace hack::assembler;

void AsmDiagClient::Print(size_t id, const hack::CodePosition& pos, const std::vector<std::string>& args)
{
	using namespace diag;

	PrintPosition(std::cerr, pos);

	if (id > WARNINGS) {
		std::cerr << "Warning: ";
	}
	else {
		std::cerr << "Error: ";
	}

	switch (id) {
	case err_stray_cmt:
		std::cerr << "Stray '/'\n";
		break;

	case err_inv_sym_in_label:
		assert(args.size() >= 1);
		std::cerr << "Invalid symbol in label: '" << args.at(0) << "'\n";
		break;

	case err_inv_at:
		std::cerr << "Invalid '@'\n";
		break;

	case err_inv_c_instr:
		assert(args.size() >= 1);
		std::cerr << "Invalid C-instruction: '" << args.at(0) << "'\n";
		break;

	case err_lbl_redef:
		assert(args.size() >= 1);
		std::cerr << "Label redefined: '" << args.at(0) << "'\n";
		break;

	case err_unk_comp_code:
		assert(args.size() >= 1);
		std::cerr << "Unknown compare code: '" << args.at(0) << "'\n";
		break;

	case err_unk_jump_code:
		assert(args.size() >= 1);
		std::cerr << "Unknown jump code: '" << args.at(0) << "'\n";
		break;

	case wrn_const:
		assert(args.size() >= 2);
		std::cerr << "Constant '" << args.at(0) << "' is truncated to '" << args.at(1) << "'\n";
		break;

	default:
		throw std::runtime_error("Unknown diag code");
	}
}

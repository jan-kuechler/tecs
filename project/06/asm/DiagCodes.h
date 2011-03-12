#ifndef DIAG_CODES_H
#define DIAG_CODES_H

#include "Diag.h"

namespace hack { namespace assembler {

namespace diag {
	enum {
		// Parser
		ERRORS,

		err_stray_cmt,
		err_inv_sym_in_label,
		err_inv_at,
		err_inv_c_instr, 

		// Code
		err_lbl_redef,
		err_unk_comp_code,
		err_unk_jump_code,

		WARNINGS, 

		wrn_const,
	};
};

class AsmDiagClient : public hack::DiagClient
{
public:
	void Print(size_t id, const CodePosition& pos, const std::vector<std::string>& args);
};

}; };

#endif //DIAG_CODES_H

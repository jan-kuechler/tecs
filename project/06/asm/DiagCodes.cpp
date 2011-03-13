#include "stdafx.h"
#include "DiagCodes.h"

using namespace hack::assembler;

#define ERR(id, fmt, n) { diag::id, "Error: " fmt, n }
#define WRN(id, fmt, n) { diag::id, "Warning: " fmt, n }

hack::DiagClient::Message AsmDiagClient::messages[] = {
	ERR(err_stray_cmt, "Stray '/'.", 0),
	ERR(err_inv_sym_in_label, "Invalid symbol '%1%' in label.", 1),
	ERR(err_inv_at, "Invalid @-instruction.", 0),
	ERR(err_inv_c_instr, "Invalid C-instruction: '%1%'.", 1),
	ERR(err_lbl_redef, "Label '%1%' redefined.", 1),
	ERR(err_unk_comp_code, "Unknown compare code '%1%'.", 1),
	ERR(err_unk_jump_code, "Unknown jump code '%1%'.", 1),
	WRN(wrn_const, "Constant '%1%' is truncated to '%2%'.", 2),
};

hack::DiagClient::Message* AsmDiagClient::GetMessages(size_t &num) const
{
	num = sizeof(messages)/sizeof(*messages);
	return messages;
}

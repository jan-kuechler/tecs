#include "stdafx.h"
#include "DiagCodes.h"

#define ERR(id, fmt, n) {hack::vm::diag::id, "Error: " fmt, n }
#define WRN(id, fmt, n) {hack::vm::diag::id, "Warning: " fmt, n }

hack::DiagClient::Message hack::vm::VmDiagClient::msgs[hack::vm::diag::NUM_DIAG_CODES] = {
	ERR(err_unk_cmd, "Unknown command '%1%'.", 1),
	ERR(err_not_enough_args, "Not enough arguments for %1%: expected %2%, got %3%.", 3),
	ERR(err_too_many_args, "Too many arguments for %1%: expected %2%, got %3%.", 3),
	ERR(err_no_number, "Expected number, got '%1%'.", 1),

	ERR(err_unk_segment, "Unknown segment '%1%'.", 1),
	ERR(err_pop_const, "Cannot pop to the constant segment.", 0),

	WRN(wrn_const, "Constant gets truncated to %1%.", 1),

	ERR(err_not_impl, "'%1%' is not implemented.", 1),
};

#ifndef DIAG_CODES_H
#define DIAG_CODES_H

#include "Diag.h"

namespace hack { namespace vm {

namespace diag {
	enum {
		err_unk_cmd,
		err_not_enough_args,
		err_too_many_args,
		err_no_number,

		err_unk_segment,
		err_pop_const,

		wrn_const,

		err_not_impl,


		NUM_DIAG_CODES,
	};
};

class VmDiagClient : public hack::DiagClient
{
protected:
	static Message msgs[diag::NUM_DIAG_CODES];

	Message *GetMessages(size_t& num) const
	{
		num = sizeof(msgs) / sizeof(*msgs);
		return msgs;
	}
};

}; };

#endif //DIAG_CODES_H

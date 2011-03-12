#ifndef CODE_H
#define CODE_H

#include "Command.h"
#include "DiagCodes.h"

namespace hack { namespace assembler {

std::vector<std::string> Translate(const std::vector<Command>& cmds, Diag& diag);

}; };

#endif //CODE_H

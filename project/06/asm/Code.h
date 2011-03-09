#ifndef CODE_H
#define CODE_H

#include "Command.h"

namespace hack { namespace assembler {

std::vector<std::string> Translate(const std::vector<Command>& cmds);

}; };

#endif //CODE_H

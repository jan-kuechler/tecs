#ifndef CODE_GEN_H
#define CODE_GEN_H

#include "Command.h"
#include "DiagCodes.h"

namespace hack { namespace vm {

class CodeGen
{
	hack::Diag& diag;
	std::ofstream& out;

	std::string curFile;

	int lclSym;

	enum Segment
	{
		SEG_LOCAL,
		SEG_ARG,
		SEG_THIS,
		SEG_THAT,
		SEG_PTR,
		SEG_TMP,
		SEG_CONST,
		SEG_STATIC,
		NO_SEGMENT,
	};
	static std::map<std::string, Segment> segmentMap;
	void InitSegmentMap();

public:
	CodeGen(std::ofstream& out, hack::Diag& diag);

	void Generate(const std::string& fileName, const std::vector<Command>& cmds);

private:
	void WriteCmd(const Command& cmd);

	void WriteUnaryArith(const Command& cmd);
	void WriteBinaryArith(const Command& cmd);
	void WritePush(const Command& cmd);
	void WritePop(const Command& cmd);

	Segment GetSegment(const std::string& str) const;
	std::string GetAddrForSegment(Segment seg) const;

	std::string LocalSymbol(const std::string& hint = "");

	void LoadSegmentToD(Segment seg);

	void LoadArg2To(char reg);
	void LoadArg1To(char reg);
	void StoreDAtArg2();

	void StoreD();

	void LoadSegIdxAddr(Segment seg, int idx);

	void PushD();
	void TopToD();

	void TrueToA();
	void FalseToA();

	void IncSP();
	void DecSP();
};

}; };

#endif //CODE_GEN_H

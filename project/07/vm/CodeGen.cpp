#include "stdafx.h"
#include "CodeGen.h"

using namespace hack::vm;
namespace fs = boost::filesystem;

static const char *TRUE = "-1";
static const char *FALSE = "0";

static const char *TMP[3] = {
	"R13", "R14", "R15",
};

/*static*/ std::map<std::string, CodeGen::Segment> CodeGen::segmentMap;

CodeGen::CodeGen(std::ofstream& out, hack::Diag& diag)
: out(out), diag(diag), lclSym(0)
{ 
	InitSegmentMap();
}

void CodeGen::WriteStartup(int stack, const std::string& entry)
{
	// SP=256
	out << "@" << boost::lexical_cast<std::string>(stack) << "\n"
	       "D=A\n"
	       "@SP\n"
	       "M=D\n";
	WriteCall(Command(Command::Call, CodePosition("$startup$", 0), "call", entry, 0));
}

void CodeGen::Generate(const std::string& fileName, const std::vector<Command>& cmds)
{
	diag.SetErrorFatal(false, "Code generation");

	curFile = fileName;
	fileId = fs::path(fileName).stem();

	curFunc = "";

	for (auto c = cmds.begin(); c != cmds.end(); ++c)
		WriteCmd(*c);
}

static bool IsUnaryArithCmd(const Command& cmd)
{
	switch (cmd.GetType()) {
	case Command::Neg:
	case Command::Not:
		return true;
	default:
		return false;
	}
}

static bool IsBinaryArithCmd(const Command& cmd)
{
	switch (cmd.GetType()) {
	case Command::Add:
	case Command::Sub:
	case Command::And:
	case Command::Or:
	case Command::Eq:
	case Command::Lt:
	case Command::Gt:
		return true;
	default:
		return false;
	}
}

void CodeGen::WriteCmd(const Command& cmd)
{
	switch (cmd.GetType()) {
	case Command::Push:
		WritePush(cmd);
		break;
	case Command::Pop:
		WritePop(cmd);
		break;
	case Command::Label:
		WriteLabel(cmd);
		break;
	case Command::Goto:
		WriteGoto(cmd);
		break;
	case Command::If:
		WriteIf(cmd);
		break;
	case Command::Function:
		WriteFunction(cmd);
		break;
	case Command::Call:
		WriteCall(cmd);
		break;
	case Command::Return:
		WriteReturn(cmd);
		break;
			
	default:
		if (IsUnaryArithCmd(cmd))
			WriteUnaryArith(cmd);
		else if (IsBinaryArithCmd(cmd))
			WriteBinaryArith(cmd);
		else
			diag.Error(cmd.GetPos(), diag::err_not_impl) << cmd.GetCmd();
	}

}

void CodeGen::WriteUnaryArith(const Command& cmd)
{
	out << "// " << cmd.GetCmd() << "\n";
	LoadArg1To('D');
	switch (cmd.GetType()) {
	case Command::Neg:
		out << "D=-D \n";
		break;
	case Command::Not:
		out << "D=!D \n";
		break;
	default:
		assert(false);
	}
	StoreD();
}

void CodeGen::WriteBinaryArith(const Command& cmd)
{
	out << "// " << cmd.GetCmd() << "\n";
	LoadArg2To('D');
	LoadArg1To('A');

	switch (cmd.GetType()) {
	case Command::Add:
		out << "D=D+A \n";
		break;
	case Command::Sub:
		out << "D=D-A \n";
		break;
	case Command::And:
		out << "D=D&A \n";
		break;
	case Command::Or:
		out << "D=D|A \n";
		break;
		
	
	case Command::Eq: 
	case Command::Lt:
	case Command::Gt:
		{

			auto symIs = LocalSymbol("is_" + cmd.GetCmd());
			auto symCont = LocalSymbol("cont_" + cmd.GetCmd());
			 
			out << "D=D-A\n"
				   "@" << symIs << "\n";

			if (cmd.GetType() == Command::Eq)
				out << "D;JEQ\n";
			else if (cmd.GetType() == Command::Lt)
				out << "D;JLT\n";
			else if (cmd.GetType() == Command::Gt)
				out << "D;JGT\n";

			out << "D=" << FALSE << "\n"
			       "@" << symCont << "\n"
			       "0;JMP\n"
			       "(" << symIs << ")\n"
			       "D=" << TRUE << "\n"
			       "(" << symCont << ")\n";
		}
		break;

	default:
		assert(false);
	}

	StoreDAtArg2();
	DecSP();
}

void CodeGen::WritePush(const Command& cmd)
{
	out << "// " << cmd.GetCmd() << " " << cmd.GetStringArg() << " " << cmd.GetIntArg() << "\n";
	Segment seg = GetSegment(cmd.GetPos(), cmd.GetStringArg());
	if (seg == NO_SEGMENT)
		return;

	if (seg == SEG_CONST) {
		if (cmd.GetIntArg() > ((~0) & 0x7FFF)) {
			diag.Warning(cmd.GetPos(), diag::wrn_const) << (cmd.GetIntArg() & 0x7FFF);
		}

		out << "@" << boost::lexical_cast<std::string>(cmd.GetIntArg()) << "\n"
			<< "D=A \n";
		PushD();
	}
	else if (seg == SEG_STATIC) {
		out << "@" << fileId << "." << cmd.GetIntArg() << "\n"
		       "D=M\n";
		PushD();
	}
	else {
		LoadSegIdxAddr(seg, cmd.GetIntArg());
		out << "D=M\n";
		PushD();
	}
}

void CodeGen::WritePop(const Command& cmd)
{
	out << "// " << cmd.GetCmd() << " " << cmd.GetStringArg() << " " << cmd.GetIntArg() << "\n";

	Segment seg = GetSegment(cmd.GetPos(), cmd.GetStringArg());
	if (seg == NO_SEGMENT)
		return;

	if (seg == SEG_CONST) {
		diag.Error(cmd.GetPos(), diag::err_pop_const);
	}
	else if (seg == SEG_STATIC) {
		TopToD();
		out << "@" << fileId << "." << cmd.GetIntArg() << "\n"
		       "M=D\n";
		DecSP();
	}
	else {
		LoadSegIdxAddr(seg, cmd.GetIntArg());
		// store address for later use
		out << "D=A\n"
		       "@" << TMP[0] << "\n"
		       "M=D\n";
		TopToD();
		out << "@" << TMP[0] << "\n"
		       "A=M\n"
		       "M=D\n";
		DecSP();
	}
}

void CodeGen::WriteLabel(const Command& cmd)
{
	out << "// " << cmd.GetCmd() << " " << cmd.GetStringArg() << "\n";
	if (curFunc.empty())
		out << "(" << cmd.GetStringArg() << ")\n";
	else
		out << "(" << curFunc << "$" << cmd.GetStringArg() << ")\n";
}

void CodeGen::WriteGoto(const Command& cmd)
{
	out << "// " << cmd.GetCmd() << " " << cmd.GetStringArg() << "\n";
	if (curFunc.empty())
		out << "@" << cmd.GetStringArg() << "\n";
	else
		out << "@" << curFunc << "$" << cmd.GetStringArg() << "\n";
	out << "0;JMP\n";
}

void CodeGen::WriteIf(const Command& cmd)
{
	out << "// " << cmd.GetCmd() << " " << cmd.GetStringArg() << "\n";
	PopD();
	if (curFunc.empty())
		out << "@" << cmd.GetStringArg() << "\n";
	else
		out << "@" << curFunc << "$" << cmd.GetStringArg() << "\n";
	out << "D;JNE\n";
}

void CodeGen::WriteFunction(const Command& cmd)
{
	out << "// " << cmd.GetCmd() << " " << cmd.GetStringArg() << " " << cmd.GetIntArg() << "\n";
	out << "(" << cmd.GetStringArg() << ")\n";
	Push0(cmd.GetIntArg());

	curFunc = cmd.GetStringArg();
}

void CodeGen::WriteCall(const Command& cmd)
{
	out << "// " << cmd.GetCmd() << " " << cmd.GetStringArg() << "\n";

	std::string retAddr = LocalSymbol("ret_from_" + cmd.GetStringArg());
	PushSym(retAddr);
	PushVal("LCL");
	PushVal("ARG");
	PushVal("THIS");
	PushVal("THAT");

	// ARG = SP - n - 5
	out << "@" << cmd.GetIntArg() + 5 << "\n"
	       "D=A\n"
	       "@SP\n"
	       "D=M-D\n"
	       "@ARG\n"
	       "M=D\n";
	// LCL = SP
	out << "@SP\n"
	       "D=M\n"
	       "@LCL\n"
	       "M=D\n";
	out << "@" << cmd.GetStringArg() << "\n"
	       "0;JMP\n";
	out << "(" << retAddr << ")\n";
}

void CodeGen::WriteReturn(const Command& cmd)
{
	out << "// " << cmd.GetCmd() << "\n";

	// Temporary variables:
	//  0: FRAME
	//  1: RET

	// FRAME = LCL
	out << "@LCL\n"
	       "D=M\n"
	       "@" << TMP[0] << "\n"
	       "M=D\n";

	// RET = *(FRAME - 5)
	Assign(TMP[1], TMP[0], 5);

	// Move return value
	PopD();
	out << "@ARG\n"
	       "A=M\n"
	       "M=D\n";

	// Reposition SP
	out << "@ARG\n"
	       "D=M+1\n"
	       "@SP\n"
	       "M=D\n";

	Assign("THAT", TMP[0], 1);
	Assign("THIS", TMP[0], 2);
	Assign("ARG", TMP[0], 3);
	Assign("LCL", TMP[0], 4);

	out << "@" << TMP[1] << "\n"
	       "A=M;JMP\n";

}

void CodeGen::LoadSegIdxAddr(Segment seg, int idx)
{
	bool direct = (seg == SEG_PTR || seg == SEG_TMP);

	out << "@" << boost::lexical_cast<std::string>(idx) << "\n"
	       "D=A\n"
	       "@" << GetAddrForSegment(seg) << "\n";
	if (direct)
		out << "A=A+D\n";
	else
	    out << "A=M+D\n";
}

void CodeGen::Assign(const std::string& dst, const std::string src, int noffs)
{
	out << "@" << src << "\n"
	       "D=M\n"
	       "@" << noffs << "\n"
	       "A=D-A\n"
	       "D=M\n"
	       "@" << dst << "\n"
	       "M=D\n";
}

std::string CodeGen::LocalSymbol(const std::string& hint /*= ""*/)
{
	return "$lcl:"  + boost::lexical_cast<std::string>(lclSym++) + (hint.empty() ? std::string() : ":" + hint);
}

static bool IsReg(char reg)
{
	return reg == 'A' || reg == 'D' || reg == 'M';
}

/* Modifies: A, reg */
void CodeGen::LoadArg2To(char reg)
{
	assert(IsReg(reg));
	out << "@2\n"
	       "D=A\n"
	       "@SP\n"
	       "A=M-D\n"
	    << reg
	    << "=M\n";
}

/* Modifies: A, reg */
void CodeGen::LoadArg1To(char reg)
{
	assert(IsReg(reg));
	out << "@SP\n"
	       "A=M-1\n"
	    << reg
	    << "=M\n";
}

/* Modifies: A */
void CodeGen::StoreDAtArg2()
{
	out << "@SP\n"
	       "A=M-1\n"
	       "A=A-1\n"
	       "M=D\n";
}

void CodeGen::StoreD()
{
	out << "M=D\n";
}

void CodeGen::PushD()
{
	out << "@SP\n"
	       "A=M\n"
	       "M=D\n";
	IncSP();
}

void CodeGen::PushSym(const std::string& sym)
{
	out << "@" << sym << "\n"
	       "D=A\n";
	PushD();
}

void CodeGen::PushVal(const std::string& sym)
{
	out << "@" << sym << "\n"
	       "D=M\n";
	PushD();
}

void CodeGen::TopToD()
{
	out << "@SP\n"
	       "A=M-1\n"
	       "D=M\n";
}

void CodeGen::PopD()
{
	TopToD();
	DecSP();
}

void CodeGen::Push0(int n)
{
	if (n <= 0)
		return;

	out << "@SP\n"
	       "A=M\n"
	       "M=0\n";
	for (int i=1; i < n; ++i) {
		out << "A=A+1\n"
		       "M=0\n";
	}
	out << "@" << boost::lexical_cast<std::string>(n) << "\n"
	       "D=A\n"
	       "@SP\n"
	       "M=M+D\n";
}

/* Modifies: A */
void CodeGen::IncSP()
{
	out << "@SP\n"
	       "M=M+1\n";
}

/* Modifies: A */
void CodeGen::DecSP()
{
	out << "@SP\n"
	       "M=M-1\n";
}

std::string CodeGen::GetAddrForSegment(Segment seg) const
{
	switch (seg) {
	case SEG_LOCAL:
		return "LCL";
	case SEG_ARG:
		return "ARG";
	case SEG_THIS:
		return "THIS";
	case SEG_THAT:
		return "THAT";
	case SEG_PTR:
		return "R3";
	case SEG_TMP:
		return "R5";
	default:
		assert(false);
		return "";
	}
}

CodeGen::Segment CodeGen::GetSegment(const hack::CodePosition& pos, const std::string& str) const
{
	if (segmentMap.find(str) == segmentMap.end()) {
		diag.Error(pos, diag::err_unk_segment) << str;
		return NO_SEGMENT;
	}
	return segmentMap[str];
}

void CodeGen::InitSegmentMap()
{
	if (segmentMap.size() > 0)
		return;

	segmentMap["local"] = SEG_LOCAL;
	segmentMap["argument"] = SEG_ARG;
	segmentMap["this"] = SEG_THIS;
	segmentMap["that"] = SEG_THAT;
	segmentMap["pointer"] = SEG_PTR;
	segmentMap["temp"] = SEG_TMP;
	segmentMap["constant"] = SEG_CONST;
	segmentMap["static"] = SEG_STATIC;
}

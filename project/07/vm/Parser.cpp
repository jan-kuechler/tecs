#include "stdafx.h"
#include "Parser.h"

using namespace hack::vm;
namespace fs = boost::filesystem;

/*static*/ std::map<std::string, Command::Type> Parser::typeMap;

Parser::Parser(const fs::path& file, hack::Diag& diag)
	: diag(diag)
{
	fileName = file.filename().string();

	std::ifstream in(file.string());
	std::string line;

	input.reserve(static_cast<std::string::size_type>(fs::file_size(file)));

	while (std::getline(in, line)) {
		input += line + "\n";
	}

	InitTypeMap();
}

Parser::Parser(const std::string& input, const std::string& fileName, hack::Diag& diag)
	: input(input), fileName(fileName), diag(diag)
{ 
	InitTypeMap();
}

void Parser::Parse()
{
	diag.SetErrorFatal(false, "Parsing");

	std::stringstream str(input);
	std::string line;

	size_t ln = 0;
	while (std::getline(str, line)) {
		auto cmd = ParseLine(line, ++ln);
		if (cmd.GetType() != Command::NONE)
			cmds.push_back(cmd);
	}
}

std::vector<std::string> Parser::SplitAndRemoveComments(const std::string& line) const
{
	std::vector<std::string> parts;
	boost::split(parts, line, [](char c) { return std::isspace(c); }, boost::algorithm::token_compress_on);

	// Filter out comments
	size_t end = parts.size();
	for (size_t i=0; i < parts.size(); ++i) {
		if (boost::starts_with(parts[i], "//")) {
			end = i;
			break;
		}
		else if (boost::contains(parts[i], "//")) {
			auto p = parts[i].find("//");
			parts[i] = parts[i].substr(0, p);
			end = i+1;
			break;
		}
	}

	std::vector<std::string> res(parts.begin(), parts.begin() + end);

	if (res.size() > 0 && (res.end()-1)->empty())
		res.erase(res.end() - 1);

	return res;
}

Command Parser::ParseLine(std::string line, size_t ln) const
{
	boost::trim_left(line);

	if (line.empty())
		return Command::NONE;

	auto parts = SplitAndRemoveComments(line);

	if (parts.size() == 0)
		return Command::NONE;

	if (typeMap.find(parts[0]) == typeMap.end()) {
		diag.Error(Position(ln), diag::err_unk_cmd) << parts[0];
		return Command::NONE;
	}

	Command::Type ctp = typeMap[parts[0]];
	size_t nargs = Command::GetNumArgs(ctp);

	if (nargs > (parts.size()-1)) {
		diag.Error(Position(ln), diag::err_not_enough_args) << parts[0] << nargs << parts.size()-1;
		return Command::NONE;
	}
	else if (nargs < (parts.size()-1)) {
		diag.Error(Position(ln), diag::err_too_many_args) << parts[0] << nargs << parts.size()-1;
		return Command::NONE;
	}

	if (nargs == 0) {
		return Command(ctp, Position(ln), parts[0]);
	}
	else if (nargs == 1) {
		return Command(ctp, Position(ln), parts[0], parts[1]);
	}
	else if (nargs == 2) {
		try {
			return Command(ctp, Position(ln), parts[0], parts[1], boost::lexical_cast<int>(parts[2]));
		}
		catch (boost::bad_lexical_cast) {
			diag.Error(Position(ln), diag::err_no_number) << parts[2];
		}
	}
	return Command::NONE;
}

void Parser::InitTypeMap()
{
	if (typeMap.size() > 0)
		return;

	typeMap["push"] = Command::Push;
	typeMap["pop"]  = Command::Pop;
	typeMap["label"] = Command::Label;
	typeMap["goto"] = Command::Goto;
	typeMap["if-goto"] = Command::If;
	typeMap["function"] = Command::Function;
	typeMap["return"] = Command::Return;
	typeMap["call"] = Command::Call;
	typeMap["add"] = Command::Add;
	typeMap["sub"] = Command::Sub;
	typeMap["neg"] = Command::Neg;
	typeMap["and"] = Command::And;
	typeMap["or"] = Command::Or;
	typeMap["not"] = Command::Not;
	typeMap["gt"] = Command::Gt;
	typeMap["lt"] = Command::Lt;
	typeMap["eq"] = Command::Eq;
}

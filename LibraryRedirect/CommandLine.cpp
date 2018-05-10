#include <array>

#include "CommandLine.h"

namespace CommandLine
{
	Parser::Parser(std::string binaryFilename)
		: m_app("Windows Library Redirect", binaryFilename)
	{
		const size_t optionsCount = 3;
		const std::string typeName = "PATH";

		std::array<CLI::Option*, optionsCount> options
		{
			m_app.add_option("--documents", m_pathMap[Library::Documents], "Set Documents path to " + typeName),
			m_app.add_option("--downloads", m_pathMap[Library::Downloads], "Set Downloads path to " + typeName),
			m_app.add_option("--desktop", m_pathMap[Library::Desktop], "Set Desktop path to " + typeName),
		};

		for (CLI::Option* option : options)
			option->set_type_name(typeName);
	}
}
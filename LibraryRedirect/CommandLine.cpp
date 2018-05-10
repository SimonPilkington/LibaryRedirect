#include <array>

#include "CommandLine.h"

CommandLine::CommandLine(std::string binaryFilename)
	: m_app("Windows Library Redirect", binaryFilename)
{
	const std::string typeName = "PATH";

	std::array<CLI::Option*, 3> options
	{
		m_app.add_option("--documents", m_documentsPath, "Set Documents path to " + typeName),
		m_app.add_option("--downloads", m_downloadsPath, "Set Downloads path to " + typeName),
		m_app.add_option("--desktop", m_desktopPath, "Set Desktop path to " + typeName)
	};

	for (CLI::Option* option : options)
		option->set_type_name(typeName);
}

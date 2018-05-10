#pragma once

#include <string>
#include <unordered_map>
#include <CLI/CLI.hpp>

namespace CommandLine
{
	enum class Library { Desktop, Documents, Downloads };

	class Parser
	{
		CLI::App m_app;
		std::unordered_map<Library, std::string> m_pathMap;

	public:
		Parser(std::string binaryFilename);
		Parser(Parser&) = delete;

		inline void PrintHelp(std::ostream &stream) const
		{
			stream << m_app.help();
		}

		inline void Parse(int argc, char* argv[])
		{
			m_app.parse(argc, argv);
		}

		template <Library key> std::string GetPath() const
		{
			return m_pathMap.at(key);
		}

		template <Library key> bool HasPath() const
		{
			return !m_pathMap.at(key).empty();
		}

		bool Anything() const
		{
			for (std::pair<Library, std::string> kvp : m_pathMap)
			{
				if (!kvp.second.empty())
					return true;
			}

			return false;
		}
	};
}

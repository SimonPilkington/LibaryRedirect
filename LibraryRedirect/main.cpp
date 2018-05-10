#include "CommandLine.h"

#include <iostream>
#include <string>
#include <type_traits>
#include <filesystem>

#define NOMINMAX

#include <comdef.h>
#include <comutil.h>
#include <Shobjidl.h>
#include <KnownFolders.h>

#include "InitCom.h"
#include "ComObject.h"
#include "ComException.h"

#undef NOMINMAX

template <class T> bstr_t ToBStr(T pStr)
{
	static_assert(std::is_assignable_v<const char*&, T> || std::is_assignable_v<const wchar_t*&, T>);

	try
	{
		return bstr_t(pStr);
	}
	catch (const _com_error &x)
	{
		throw ComException(x.Error(), "String conversion failed.");
	}
}

std::string GetBinaryName(std::string fullPath)
{
	size_t lastSlash = fullPath.find_last_of('/');
	size_t lastBackslash = fullPath.find_last_of('\\');

	size_t index = std::max
	(
		lastSlash == std::string::npos ? 0 : lastSlash,
		lastBackslash == std::string::npos ? 0 : lastBackslash
	);

	if (index == 0)
		return fullPath;

	return fullPath.substr(index + 1);
}

void RedirectLibrary(const KNOWNFOLDERID &rfid, std::filesystem::path redirectDest)
{
	ComObject<IKnownFolderManager> knownFolderManager(CLSID_KnownFolderManager);

	LPWSTR error;
	HRESULT hr = knownFolderManager->Redirect(rfid, nullptr, KF_REDIRECT_OWNER_USER | KF_REDIRECT_SET_OWNER_EXPLICIT, redirectDest.c_str(), 0, nullptr, &error);

	if (!SUCCEEDED(hr))
		throw ComException(hr, ToBStr(error));
}

int main(int argc, char* argv[])
{
	if (!SetConsoleOutputCP(1252))
		std::cerr << "Failed to set console output codepage. Characters may display incorrectly." << std::endl;

	CommandLine::Parser cmdLine(GetBinaryName(argv[0]));

	try
	{
		cmdLine.Parse(argc, argv);

		if (!cmdLine.Anything())
			throw CLI::ParseError("No options specified.", -1);

		InitCom initCom;

		{
			using CommandLine::Library;

			if (cmdLine.HasPath<Library::Desktop>())
				RedirectLibrary(FOLDERID_Desktop, cmdLine.GetPath<Library::Desktop>());

			if (cmdLine.HasPath<Library::Documents>())
				RedirectLibrary(FOLDERID_Documents, cmdLine.GetPath<Library::Documents>());

			if (cmdLine.HasPath<Library::Downloads>())
				RedirectLibrary(FOLDERID_Downloads, cmdLine.GetPath<Library::Downloads>());
		}

		return 0;
	}
	catch (const CLI::ParseError &x)
	{
		cmdLine.PrintHelp(std::cout);
		return x.get_exit_code();
	}
	catch (const ComException &x)
	{
		std::cerr << "Fatal error: " << x.what() << 
			" (HRESULT 0x" << std::hex << x.GetHResult() << std::dec << ')' 
			<< std::endl;
	}

	return -1;
}
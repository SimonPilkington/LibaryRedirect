#include "CommandLine.h"

#include <iostream>
#include <type_traits>

#define NOMINMAX

#include <comdef.h>
#include <comutil.h>
#include <Shobjidl.h>
#include <KnownFolders.h>

#include "InitCom.h"
#include "ComObject.h"
#include "ComException.h"

#undef NOMINMAX

template <class T> bstr_t ToBStr(T str)
{
	static_assert(std::is_trivially_assignable<const char*&, T>::value || std::is_trivially_assignable<const wchar_t*&, T>::value, "T must be a pointer to string");

	try
	{
		return bstr_t(str);
	}
	catch (const _com_error &x)
	{
		throw ComException(x.Error(), "String conversion failed.");
	}
}

void RedirectLibrary(const KNOWNFOLDERID &rfid, LPCWSTR pRedirectDest)
{
	ComObject<IKnownFolderManager> knownFolderManager(CLSID_KnownFolderManager);

	LPWSTR error;
	HRESULT hr = knownFolderManager->Redirect(rfid, nullptr, KF_REDIRECT_OWNER_USER | KF_REDIRECT_SET_OWNER_EXPLICIT, pRedirectDest, 0, nullptr, &error);

	if (!SUCCEEDED(hr))
		throw ComException(hr, ToBStr(error));
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

int main(int argc, char* argv[])
{
	if (!SetConsoleOutputCP(1252))
		std::cerr << "Failed to set console output codepage. Characters may display incorrectly." << std::endl;

	CommandLine cmdLine(GetBinaryName(argv[0]));

	try
	{
		cmdLine.Parse(argc, argv);

		if (!cmdLine.Anything())
			throw CLI::ParseError("No options specified.", -1);

		InitCom initCom;

		if (cmdLine.HasDesktopPath())
			RedirectLibrary(FOLDERID_Desktop, ToBStr(cmdLine.GetDesktopPath()));

		if (cmdLine.HasDocumentsPath())
			RedirectLibrary(FOLDERID_Documents, ToBStr(cmdLine.GetDocumentsPath()));

		if (cmdLine.HasDownloadsPath())
			RedirectLibrary(FOLDERID_Downloads, ToBStr(cmdLine.GetDownloadsPath()));

		return 0;
	}
	catch (const CLI::ParseError &x)
	{
		cmdLine.PrintHelp(std::cout);
		return x.get_exit_code();
	}
	catch (const ComException &x)
	{
		std::cerr << "Fatal error: " << x.what() << " (HRESULT 0x" << std::hex << x.GetHResult() << std::dec << ')' << std::endl;
	}

	return -1;
}
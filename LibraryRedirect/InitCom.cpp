#include <Objbase.h>

#include "InitCom.h"
#include "ComException.h"

InitCom::InitCom()
{
	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	if (!SUCCEEDED(hr))
		throw ComException(hr, "COM initialization failed.");
}

InitCom::~InitCom() noexcept
{
	CoUninitialize();
}
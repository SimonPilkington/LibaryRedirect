#pragma once

#include <stdexcept>
#include <winerror.h>

class ComException : public std::runtime_error
{
	HRESULT m_hResult;

public:

	ComException(HRESULT hr, const char* message) noexcept
		: std::runtime_error(message)
	{
		m_hResult = hr;
	}

	inline HRESULT GetHResult() const noexcept
	{
		return m_hResult;
	}
};
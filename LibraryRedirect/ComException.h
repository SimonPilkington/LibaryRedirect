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

	ComException(const ComException &other) noexcept
		: std::runtime_error(other)
	{
		m_hResult = other.m_hResult;
	}

	ComException &operator=(const ComException &other) noexcept
	{
		if (this != &other)
		{
			std::runtime_error::operator=(other);
			m_hResult = other.m_hResult;
		}

		return *this;
	}

	inline HRESULT GetHResult() const noexcept
	{
		return m_hResult;
	}
};
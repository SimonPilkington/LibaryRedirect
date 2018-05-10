#pragma once

#include <algorithm>
#include <type_traits>
#include <objbase.h>

#include "ComException.h"

template <class T> class IUnknownNoAddRefRelease : public T
{
	virtual ULONG STDMETHODCALLTYPE AddRef() override = 0;
	virtual ULONG STDMETHODCALLTYPE Release() override = 0;
};

template <typename T> class ComObject
{
	static_assert(std::is_base_of<IUnknown, T>::value, "T must inherit from IUnknown");

	T *m_pInstance;

public:
	ComObject() noexcept
	{
		m_pInstance = nullptr;
	}

	ComObject(const IID &rclsid)
	{
		HRESULT hr = CoCreateInstance(rclsid, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_pInstance));

		if (!SUCCEEDED(hr))
			throw ComException(hr, "CoCreateInstance failed.");
	}

	ComObject(const ComObject &other) noexcept
	{
		m_pInstance = other.m_pInstance;

		if (m_pInstance)
			m_pInstance->AddRef();
	}

	ComObject(ComObject &&other) noexcept
		: ComObject()
	{
		swap(*this, other);
	}

	~ComObject() noexcept
	{
		if (m_pInstance)
			m_pInstance->Release();
	}

	ComObject &operator=(ComObject other) noexcept
	{
		swap(*this, other);
		return *this;
	}

	friend void swap(ComObject &first, ComObject &second) noexcept
	{
		using std::swap;
		swap(first.m_pInstance, second.m_pInstance);
	}

	inline IUnknownNoAddRefRelease<T> *operator->() const
	{
		if (!m_pInstance)
			throw ComException(E_NOT_VALID_STATE, "Instance has not been created.");

		return static_cast<IUnknownNoAddRefRelease<T> *>(m_pInstance);
	}

	inline T **operator&() const noexcept
	{
		return &m_pInstance;
	}
};

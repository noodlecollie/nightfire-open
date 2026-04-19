#pragma once

#include <RmlUi/Core/Types.h>
#include <RmlUi/Core/Log.h>
#include "udll_int.h"
#include "UIDebug.h"

template<typename T>
struct CvarAccessor
{
};

template<>
struct CvarAccessor<float>
{
	static float GetValue(const char* cvarName)
	{
		return gEngfuncs.pfnGetCvarFloat(cvarName);
	}

	static void SetValue(const char* cvarName, float value)
	{
		gEngfuncs.pfnCvarSetValue(cvarName, value);
	}

	static void DbgLog(const char* cvarName, float value, bool set)
	{
		RML_DBGLOG(Rml::Log::Type::LT_DEBUG, "[%s] cvar %s = %f", set ? "SET" : "GET", cvarName, value);
	}
};

template<>
struct CvarAccessor<int>
{
	static int GetValue(const char* cvarName)
	{
		return static_cast<int>(gEngfuncs.pfnGetCvarFloat(cvarName));
	}

	static void SetValue(const char* cvarName, int value)
	{
		gEngfuncs.pfnCvarSetValue(cvarName, static_cast<float>(value));
	}

	static void DbgLog(const char* cvarName, int value, bool set)
	{
		RML_DBGLOG(Rml::Log::Type::LT_DEBUG, "[%s] cvar %s = %d", set ? "SET" : "GET", cvarName, value);
	}
};

template<>
struct CvarAccessor<bool>
{
	static bool GetValue(const char* cvarName)
	{
		return CvarAccessor<float>::GetValue(cvarName) != 0.0f;
	}

	static void SetValue(const char* cvarName, bool value)
	{
		CvarAccessor<float>::SetValue(cvarName, value ? 1.0f : 0.0f);
	}

	static void DbgLog(const char* cvarName, bool value, bool set)
	{
		RML_DBGLOG(Rml::Log::Type::LT_DEBUG, "[%s] cvar %s = %s", set ? "SET" : "GET", cvarName, value ? "<1>" : "<0>");
	}
};

template<>
struct CvarAccessor<Rml::String>
{
	static Rml::String GetValue(const char* cvarName)
	{
		return gEngfuncs.pfnGetCvarString(cvarName);
	}

	static void SetValue(const char* cvarName, const Rml::String& value)
	{
		gEngfuncs.pfnCvarSetString(cvarName, value.c_str());
	}

	static void DbgLog(const char* cvarName, const Rml::String& value, bool set)
	{
		RML_DBGLOG(Rml::Log::Type::LT_DEBUG, "[%s] cvar %s = %s", set ? "SET" : "GET", cvarName, value.c_str());
	}
};

template<typename T>
class CvarAccessorObj
{
public:
	explicit CvarAccessorObj(Rml::String cvarName) :
		m_CvarName(std::move(cvarName))
	{
	}

	T GetValue() const
	{
		const T value = CvarAccessor<T>::GetValue(m_CvarName.c_str());
		CvarAccessor<T>::DbgLog(m_CvarName.c_str(), value, false);
		return value;
	}

	void SetValue(const T& value)
	{
		CvarAccessor<T>::DbgLog(m_CvarName.c_str(), value, true);
		CvarAccessor<T>::SetValue(m_CvarName.c_str(), value);
	}

private:
	Rml::String m_CvarName;
};

#pragma once

#include <RmlUi/Core/Types.h>
#include <RmlUi/Core/DataModelHandle.h>
#include <RmlUi/Core/Log.h>
#include "framework/DataVar.h"
#include "udll_int.h"
#include "UIDebug.h"

template<typename T>
struct CvarAccessor
{
};

template<typename T>
class CvarDataVar
{
public:
	CvarDataVar(const char* name, const char* cvarName, T value = T()) :
		m_CvarName(cvarName),
		m_Var {name, value}
	{
		ASSERT(m_CvarName && *m_CvarName);
	}

	const char* Name() const
	{
		return m_Var.name;
	}

	const char* CvarName() const
	{
		return m_CvarName;
	}

	const T& GetValue(bool refresh)
	{
		if ( refresh )
		{
			m_Var.value = CvarAccessor<T>::GetValue(m_CvarName);
		}

		return m_Var.value;
	}

	const T& CachedValue() const
	{
		return m_Var.value;
	}

	void SetValue(const T& val)
	{
		m_Var.value = val;
		CvarAccessor<T>::LogSetValue(m_CvarName, val);
		CvarAccessor<T>::SetValue(m_CvarName, val);
	}

	bool Bind(Rml::DataModelConstructor& constructor)
	{
		return constructor.BindFunc(
			Name(),
			[this](Rml::Variant& outVal)
			{
				outVal = Rml::Variant(CachedValue());
			},
			[this](const Rml::Variant& inVal)
			{
				SetValue(inVal.Get<T>());
			}
		);
	}

private:
	const char* m_CvarName = nullptr;
	DataVar<T> m_Var;
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

	static void LogSetValue(const char* cvarName, float value)
	{
		RML_DBGLOG(Rml::Log::Type::LT_DEBUG, "Setting cvar %s = %f", cvarName, value);
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

	static void LogSetValue(const char* cvarName, int value)
	{
		RML_DBGLOG(Rml::Log::Type::LT_DEBUG, "Setting cvar %s = %d", cvarName, value);
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

	static void LogSetValue(const char* cvarName, bool value)
	{
		RML_DBGLOG(Rml::Log::Type::LT_DEBUG, "Setting cvar %s = %s", cvarName, value ? "1" : "0");
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

	static void LogSetValue(const char* cvarName, const Rml::String& value)
	{
		RML_DBGLOG(Rml::Log::Type::LT_DEBUG, "Setting cvar %s = %s", cvarName, value.c_str());
	}
};

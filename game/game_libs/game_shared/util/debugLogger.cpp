#include "debugLogger.h"
#include "projectInterface/IProjectInterface.h"
#include "standard_includes.h"

#ifdef CLIENT_DLL
#include "cl_dll.h"
#endif

CDebugLogger::CDebugLogger(const char* cvarName)
{
	ASSERT(cvarName && *cvarName);

	m_EnableCvar.name = const_cast<char*>(cvarName);
}

void CDebugLogger::RegisterCvar()
{
#ifndef CLIENT_DLL
	m_pEnableCvar = &m_EnableCvar;
	CVAR_REGISTER(m_pEnableCvar);
	CVAR_SET_FLOAT(const_cast<char*>(m_pEnableCvar->name), 0);
#else
	ASSERT(!m_pEnableCvar);
	m_pEnableCvar = gEngfuncs.pfnRegisterVariable(m_EnableCvar.name, m_EnableCvar.string, 0);
	ASSERT(m_pEnableCvar);
#endif
}

bool CDebugLogger::LoggingEnabled() const
{
	return m_pEnableCvar->value != 0.0f;
}

void CDebugLogger::Log(Level level, const CUtlString& message)
{
	if ( !LoggingEnabled() )
	{
		return;
	}

	IProjectInterface::ProjectInterfaceImpl()->LogInterface().Log(level, message);
}

void CDebugLogger::LogF(Level level, const char* format, ...)
{
	if ( !LoggingEnabled() )
	{
		return;
	}

	va_list args;
	va_start(args, format);
	IProjectInterface::ProjectInterfaceImpl()->LogInterface().LogV(level, format, args);
	va_end(args);
}

void CDebugLogger::LogV(Level level, const char* format, va_list args)
{
	if ( !LoggingEnabled() )
	{
		return;
	}

	IProjectInterface::ProjectInterfaceImpl()->LogInterface().LogV(level, format, args);
}

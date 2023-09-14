#pragma once

#include "EnginePublicAPI/cvardef.h"
#include "projectInterface/ILogInterface.h"

class CDebugLogger : public ILogInterface
{
public:
	CDebugLogger(const char* cvarName);

	void RegisterCvar();
	bool LoggingEnabled() const;

	virtual void Log(Level level, const CUtlString& message) override;
	virtual void LogF(Level level, const char* format, ...) override;
	virtual void LogV(Level level, const char* format, va_list args) override;

private:
	// Doubling up to patch up cvar registration on client and server - see RegisterCvar().
	cvar_t m_EnableCvar;
	cvar_t* m_pEnableCvar = nullptr;
};

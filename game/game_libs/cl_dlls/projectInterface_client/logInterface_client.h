#pragma once

#include "projectInterface/ILogInterface.h"
#include "projectInterface/fixedSizeMessageBuffer.h"

class LogInterface_Client : public ILogInterface
{
public:
	virtual ~LogInterface_Client()
	{
	}

	virtual void Log(ILogInterface::Level level, const CUtlString& message) override;
	virtual void LogF(ILogInterface::Level level, const char* format, ...) override;
	virtual void LogV(Level level, const char* format, va_list args) override;

private:
	void LogMessage(Level level, const char* message);

	static CFixedSizeMessageBuffer<ILogInterface::MAX_MESSAGE_LENGTH> m_MsgBuffer;
};

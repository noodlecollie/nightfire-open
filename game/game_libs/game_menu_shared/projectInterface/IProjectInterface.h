#pragma once

#include "IFileLoader.h"
#include "ILogInterface.h"
#include "IRNG.h"

class IProjectInterface
{
public:
	virtual ~IProjectInterface()
	{
	}

	// Must be set by the user on initialisation, before any other functions are used.
	static void SetProjectInterfaceImpl(IProjectInterface* iface);
	static IProjectInterface* ProjectInterfaceImpl();

	virtual IFileLoader& FileLoader() = 0;
	virtual ILogInterface& LogInterface() = 0;
	virtual IRNG& RNG() = 0;

private:
	static IProjectInterface* m_StaticProjectInterface;
};

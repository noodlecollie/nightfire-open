#pragma once

#include "projectInterface/IProjectInterface.h"
#include "logInterface_server.h"
#include "fileLoaders_server.h"
#include "rng_server.h"

class ProjectInterface_Server : public IProjectInterface
{
public:
	virtual ~ProjectInterface_Server()
	{
	}

	static IProjectInterface* StaticInstance();

	virtual IFileLoader& FileLoader() override;
	virtual ILogInterface& LogInterface() override;
	virtual IRNG& RNG() override;

private:
	FileLoader_Server m_FileLoader;
	LogInterface_Server m_LogInterface;
	RNG_Server m_RNG;
};

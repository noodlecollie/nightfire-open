#pragma once

#include "projectInterface/IProjectInterface.h"
#include "logInterface_client.h"
#include "fileLoaders_client.h"
#include "rng_client.h"

class ProjectInterface_Client : public IProjectInterface
{
public:
	virtual ~ProjectInterface_Client()
	{
	}

	static IProjectInterface* StaticInstance();

	virtual IFileLoader& FileLoader() override;
	virtual ILogInterface& LogInterface() override;
	virtual IRNG& RNG() override;

private:
	FileLoader_Client m_FileLoader;
	LogInterface_Client m_LogInterface;
	RNG_Client m_RNG;
};

#pragma once

#include "projectInterface/IProjectInterface.h"
#include "logInterface_mainui.h"
#include "fileLoaders_mainui.h"
#include "rng_mainui.h"

class ProjectInterface_MainUI : public IProjectInterface
{
public:
	virtual ~ProjectInterface_MainUI()
	{
	}

	static IProjectInterface* StaticInstance();

	virtual IFileLoader& FileLoader() override;
	virtual ILogInterface& LogInterface() override;
	virtual IRNG& RNG() override;

private:
	FileLoader_MainUI m_FileLoader;
	LogInterface_MainUI m_LogInterface;
	RNG_MainUI m_RNG;
};

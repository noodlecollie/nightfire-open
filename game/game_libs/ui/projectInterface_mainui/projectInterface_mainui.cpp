#include "projectInterface_mainui.h"

IProjectInterface* ProjectInterface_MainUI::StaticInstance()
{
	static ProjectInterface_MainUI instance;
	return &instance;
}

IFileLoader& ProjectInterface_MainUI::FileLoader()
{
	return m_FileLoader;
}

ILogInterface& ProjectInterface_MainUI::LogInterface()
{
	return m_LogInterface;
}

IRNG& ProjectInterface_MainUI::RNG()
{
	return m_RNG;
}

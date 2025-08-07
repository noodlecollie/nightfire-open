#include "projectInterface_client.h"

IProjectInterface* ProjectInterface_Client::StaticInstance()
{
	static ProjectInterface_Client instance;
	return &instance;
}

IFileLoader& ProjectInterface_Client::FileLoader()
{
	return m_FileLoader;
}

ILogInterface& ProjectInterface_Client::LogInterface()
{
	return m_LogInterface;
}

IRNG& ProjectInterface_Client::RNG()
{
	return m_RNG;
}

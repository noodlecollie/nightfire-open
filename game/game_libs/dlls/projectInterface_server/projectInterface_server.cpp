#include "projectInterface_server.h"

IProjectInterface* ProjectInterface_Server::StaticInstance()
{
	static ProjectInterface_Server instance;
	return &instance;
}

IFileLoader& ProjectInterface_Server::FileLoader()
{
	return m_FileLoader;
}

ILogInterface& ProjectInterface_Server::LogInterface()
{
	return m_LogInterface;
}

IRNG& ProjectInterface_Server::RNG()
{
	return m_RNG;
}

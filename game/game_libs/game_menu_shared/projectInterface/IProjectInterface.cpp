#include "projectInterface/IProjectInterface.h"
#include <assert.h>

IProjectInterface* IProjectInterface::m_StaticProjectInterface = nullptr;

void IProjectInterface::SetProjectInterfaceImpl(IProjectInterface* iface)
{
	m_StaticProjectInterface = iface;
}

IProjectInterface* IProjectInterface::ProjectInterfaceImpl()
{
	// If this assertion fails, it means something has tried to use the project implementation
	// before it was initialised.
	assert(m_StaticProjectInterface);

	return m_StaticProjectInterface;
}

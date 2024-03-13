#include "DynamicStringArrayModel.h"
#include "utlvector.h"
#include "utlstring.h"
#include "enginecallback_menu.h"

struct CDynamicStringArrayModel::Impl
{
	CUtlVector<CUtlString> m_Strings;
};

CDynamicStringArrayModel::CDynamicStringArrayModel() :
	m_Impl(new Impl {})
{
}

CDynamicStringArrayModel::~CDynamicStringArrayModel()
{
	delete m_Impl;
	m_Impl = nullptr;
}

void CDynamicStringArrayModel::Update()
{
	// Nothing for now.
}

const char* CDynamicStringArrayModel::GetText(int line)
{
	if ( line < 0 || line >= GetRows() )
	{
		Con_Printf("StringArrayModel: Index %d was out of range for model of size %d\n", line, GetRows());
		return "";
	}

	return GetImpl().m_Strings[line].Get();
}

int CDynamicStringArrayModel::GetRows() const
{
	return GetImpl().m_Strings.Count();
}

void CDynamicStringArrayModel::Clear()
{
	GetImpl().m_Strings.Purge();
}

void CDynamicStringArrayModel::InitEmpty(size_t size)
{
	Clear();
	GetImpl().m_Strings.SetCount(static_cast<int>(size));
}

void CDynamicStringArrayModel::SetText(int line, const char* text)
{
	if ( line < 0 || line >= GetRows() )
	{
		Con_Printf("StringArrayModel: Index %d was out of range for model of size %d\n", line, GetRows());
		return;
	}

	GetImpl().m_Strings[line].Set(text);
}

void CDynamicStringArrayModel::AddRow()
{
	GetImpl().m_Strings.AddToTail(CUtlString());
}

void CDynamicStringArrayModel::AddRows(const char* const* newLines, size_t numLines)
{
	int oldSize = GetRows();
	GetImpl().m_Strings.AddMultipleToTail(static_cast<int>(numLines));

	for ( size_t index = 0; index < numLines; ++index )
	{
		SetText(oldSize + static_cast<int>(index), newLines[index]);
	}
}

CDynamicStringArrayModel::Impl& CDynamicStringArrayModel::GetImpl()
{
	return *m_Impl;
}

const CDynamicStringArrayModel::Impl& CDynamicStringArrayModel::GetImpl() const
{
	return *m_Impl;
}

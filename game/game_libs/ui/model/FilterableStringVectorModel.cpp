#include "FilterableStringVectorModel.h"

CUtlString CFilterableStringVectorModel::GetFilter() const
{
	return m_FilterString;
}

void CFilterableStringVectorModel::SetFilter(const CUtlString& filterString)
{
	if ( m_FilterString == filterString )
	{
		return;
	}

	m_FilterString = filterString;
	Update();
}

void CFilterableStringVectorModel::Update()
{
	if ( m_FilterString.IsEmpty() )
	{
		ClearAllFiltering();
		return;
	}
}

int CFilterableStringVectorModel::GetRows() const
{
	return m_AllowedIndices.Count();
}

const char* CFilterableStringVectorModel::GetText(int line)
{
	if ( line < 0 || line >= m_AllowedIndices.Count() )
	{
		return NULL;
	}

	int innerIndex = m_AllowedIndices[line];

	if ( innerIndex < 0 || innerIndex >= m_Model.Count() )
	{
		return NULL;
	}

	return m_Model[innerIndex].Get();
}

void CFilterableStringVectorModel::ClearAllFiltering()
{
	m_AllowedIndices.Purge();
	m_AllowedIndices.SetCount(m_Model.Count());

	for ( int index = 0; index < m_Model.Count(); ++index )
	{
		m_AllowedIndices[index] = index;
	}
}

void CFilterableStringVectorModel::ApplyFiltering()
{
	m_AllowedIndices.Purge();

	for ( int index = 0; index < m_Model.Count(); ++index )
	{
		if ( strstr(m_Model[index].Get(), m_FilterString.Get()) )
		{
			m_AllowedIndices.AddToTail(index);
		}
	}
}

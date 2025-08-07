#include <assert.h>
#include <cstring>
#include "BotProfileListModel.h"
#include "botprofiles/botprofileparser.h"
#include "iterators/utlhashmapconstiterator.h"

static constexpr const char* BOT_PROFILE_FILE_PATH = "scripts/bot_profiles.json";

CBotProfileListModel::CBotProfileListModel() :
	CMenuBaseModel()
{
}

void CBotProfileListModel::Update()
{
	InitialiseProfileTable();
}

int CBotProfileListModel::GetColumns() const
{
	return 1;
}

int CBotProfileListModel::GetRows() const
{
	return m_IndexToProfileName.Count();
}

const char* CBotProfileListModel::GetCellText(int row, int)
{
	const CBotProfileTable::ProfileData* data = GetProfileData(row);
	return data ? data->playerName.String() : "UNKNOWN";
}

void CBotProfileListModel::OnActivateEntry(int row)
{
	if ( !m_ItemActivated )
	{
		return;
	}

	const CBotProfileTable::ProfileData* data = GetProfileData(row);

	if ( !data )
	{
		return;
	}

	m_ItemActivated(row, *data);
}

void CBotProfileListModel::SetItemActivatedCallback(const ItemActivateCb callback)
{
	m_ItemActivated = callback;
}

void CBotProfileListModel::InitialiseProfileTable()
{
	m_ProfileTable.Clear();
	m_IndexToProfileName.Purge();

	CBotProfileParser parser(m_ProfileTable);
	parser.Parse(BOT_PROFILE_FILE_PATH);

	if ( m_ProfileTable.Count() < 1 )
	{
		return;
	}

	m_IndexToProfileName.EnsureCapacity(static_cast<int>(m_ProfileTable.Count()));

	for ( CBotProfileTable::ConstIterator it = m_ProfileTable.CBegin(); it != m_ProfileTable.CEnd(); ++it )
	{
		assert(static_cast<size_t>(m_IndexToProfileName.Count()) < m_ProfileTable.Count());

		m_IndexToProfileName.CopyAndAddToTail(it.Key().String());
	}

	// For now, we sort by profile name, instead of the
	// human-readable bot name. This could be improved...
	qsort(
		&m_IndexToProfileName.Head(),
		m_IndexToProfileName.Count(),
		sizeof(CUtlString),
		[](const void* a, const void* b)
		{
			const CUtlString* strA = static_cast<const CUtlString*>(a);
			const CUtlString* strB = static_cast<const CUtlString*>(b);
			return strcmp(strA ? strA->String() : "", strB ? strB->String() : "");
		});
}

const CBotProfileTable::ProfileData* CBotProfileListModel::GetProfileData(int index) const
{
	if ( index < 0 || index >= m_IndexToProfileName.Count() )
	{
		return nullptr;
	}

	const char* name = m_IndexToProfileName[index];
	return m_ProfileTable.GetProfile(CUtlString(name));
}

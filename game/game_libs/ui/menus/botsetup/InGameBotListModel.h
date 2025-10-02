#pragma once

#include <cstdint>
#include <functional>
#include "BaseModel.h"
#include "utlstring.h"
#include "utlvector.h"

class CInGameBotListModel : public CMenuBaseModel
{
public:
	static constexpr size_t MAX_BOTS = 31;

	typedef std::function<void(void)> ItemDeleteCb;

	struct ListEntry
	{
		CUtlString profileName;
		CUtlString playerName;
	};

	CInGameBotListModel();

	virtual void Update() override;
	virtual int GetColumns() const override;
	virtual int GetRows() const override;
	virtual const char* GetCellText(int row, int column) override;
	virtual void OnDeleteEntry(int row) override;

	bool AddEntry(const CUtlString& profileName, const CUtlString& playerName);
	bool RemoveEntryAt(int index);
	const ListEntry* Entry(int index) const;
	bool IsFull() const;
	void Clear();

	size_t GetMax() const;
	void SetMax(size_t max);

	void SetItemDeleteCallback(const ItemDeleteCb& callback);

private:
	CUtlVector<ListEntry> m_BotList;
	ItemDeleteCb m_ItemDelete;
	size_t m_iMaxBots = MAX_BOTS;
};

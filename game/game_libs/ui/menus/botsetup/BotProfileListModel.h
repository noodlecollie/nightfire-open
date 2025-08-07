#pragma once

#include <functional>
#include "BaseModel.h"
#include "botprofiles/botprofiletable.h"
#include "utlvector.h"

class CBotProfileListModel : public CMenuBaseModel
{
public:
	typedef std::function<void(int, const CBotProfileTable::ProfileData&)> ItemActivateCb;

	CBotProfileListModel();

	virtual void Update() override;
	virtual int GetColumns() const override;
	virtual int GetRows() const override;
	virtual const char* GetCellText(int row, int column) override;
	virtual void OnActivateEntry(int row) override;

	void SetItemActivatedCallback(const ItemActivateCb callback);

private:
	void InitialiseProfileTable();
	const CBotProfileTable::ProfileData* GetProfileData(int index) const;

	CBotProfileTable m_ProfileTable;
	CUtlStringList m_IndexToProfileName;
	ItemActivateCb m_ItemActivated;
};

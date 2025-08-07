#pragma once

#include "BaseArrayModel.h"
#include "utlvector.h"
#include "utlstring.h"

class CFilterableStringVectorModel : public CMenuBaseArrayModel
{
public:
	CUtlString GetFilter() const;
	void SetFilter(const CUtlString& filterString);

	// Make sure to call ApplyFilter() after having added all items,
	// so that the filter is correctly applied.
	void Clear();
	void AddItem(const CUtlString& item);
	void ApplyFilter();

	int GetRows() const override;
	const char* GetText(int line) override;

protected:
	CUtlVector<CUtlString> m_Model;

private:
	void ClearAllFiltering();
	void BuildAllowedIndices();

	CUtlString m_FilterString;
	CUtlVector<int> m_AllowedIndices;
};

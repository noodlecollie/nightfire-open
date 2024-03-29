#pragma once

#include "BaseArrayModel.h"
#include "utlvector.h"
#include "utlstring.h"

class CFilterableStringVectorModel : public CMenuBaseArrayModel
{
public:
	CUtlString GetFilter() const;
	void SetFilter(const CUtlString& filterString);

	void Update() override;
	int GetRows() const override;
	const char* GetText(int line) override;

private:
	void ClearAllFiltering();
	void ApplyFiltering();

	CUtlVector<CUtlString> m_Model;
	CUtlString m_FilterString;
	CUtlVector<int> m_AllowedIndices;
};

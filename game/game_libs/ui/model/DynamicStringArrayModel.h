#pragma once

#include "BaseArrayModel.h"

class CDynamicStringArrayModel : public CMenuBaseArrayModel
{
public:
	CDynamicStringArrayModel();
	~CDynamicStringArrayModel();

	void Update() override;
	const char* GetText(int line) final override;
	int GetRows() const final override;

	void Clear();
	void InitEmpty(size_t size);
	void SetText(int line, const char* text);
	void AddRow();
	void AddRows(const char* const* newLines, size_t numLines);

private:
	struct Impl;

	Impl& GetImpl();
	const Impl& GetImpl() const;

	Impl* m_Impl = nullptr;
};

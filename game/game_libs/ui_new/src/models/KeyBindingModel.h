#pragma once

#include "framework/BaseTableModel.h"
#include "framework/DataVar.h"
#include <RmlUi/Core/Types.h>
#include <vector>
#include <unordered_map>

class FileCharsPtr;

class KeyBindingModel : public BaseTableModel
{
public:
	enum ColumnIndex
	{
		Description = 0,
		ConsoleCommand,
		PrimaryBinding,
		SecondaryBinding,

		TotalColumns
	};

	struct Entry
	{
		Rml::String description;
		Rml::String consoleCommand;
		Rml::String primaryBinding;
		Rml::String secondaryBinding;
		bool rebindingPrimary = false;
		bool rebindingSecondary = false;
	};

	bool SetUpDataBindings(Rml::DataModelConstructor& constructor) override;
	size_t Rows() const override;
	size_t Columns() const override;
	Rml::String DisplayString(size_t row, size_t column) const override;
	void Reset() override;
	bool RowForConsoleCommand(const Rml::String& command, size_t& row) const;

	bool IsRebinding(size_t row, bool primary) const;
	void SetIsRebinding(size_t row, bool primary, bool rebinding);

private:
	enum class ParseResult
	{
		Ok,
		Skip,
		Eof,
		Error
	};

	void ParseSchemaAndResetToDefaults();
	ParseResult ParseSchemaLine(FileCharsPtr& file, Entry& entry);
	ParseResult ParseToken(
		FileCharsPtr& file,
		char* buffer,
		size_t bufferSize,
		bool allowNewline,
		const int* overrideFlags = nullptr
	);

	std::vector<Entry> m_Entries;
	std::unordered_map<Rml::String, size_t> m_ConsoleCommandToEntry;
	Rml::DataModelHandle m_ModelHandle;
};

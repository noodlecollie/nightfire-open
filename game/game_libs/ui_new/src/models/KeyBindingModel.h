#pragma once

#include "framework/BaseTableModel.h"
#include <RmlUi/Core/Types.h>
#include <RmlUi/Core/DataModelHandle.h>
#include <vector>
#include <unordered_map>

class InFileCharsPtr;

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
	void RefreshBindigsFromFile();

	bool IsRebinding(size_t row, bool primary) const;
	void SetIsRebinding(size_t row, bool primary, bool rebinding);
	void SetBinding(size_t row, bool primary, Rml::String value);

private:
	enum class ParseResult
	{
		Ok,
		Skip,
		Eof,
		Error
	};

	void ParseSchemaAndResetToDefaults();
	ParseResult ParseSchemaLine(InFileCharsPtr& file, Entry& entry);
	ParseResult ParseToken(
		InFileCharsPtr& file,
		char* buffer,
		size_t bufferSize,
		bool allowNewline,
		const int* overrideFlags = nullptr
	);
	ParseResult ReadBindings();
	void ReadBinding(const Rml::String& command, const Rml::String& key);
	void WriteBindings() const;
	Rml::String GetBindingStatement(const Entry& entry, bool primary) const;

	std::vector<Entry> m_Entries;
	std::unordered_map<Rml::String, size_t> m_ConsoleCommandToEntry;
	Rml::DataModelHandle m_ModelHandle;
};

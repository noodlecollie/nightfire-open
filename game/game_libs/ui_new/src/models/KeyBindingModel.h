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
		DESCRIPTION = 0,
		CONSOLE_COMMAND,
		PRIMARY_BINDING,
		SECONDARY_BINDING,

		TOTAL_COLUMNS
	};

	struct Entry
	{
		struct Binding
		{
			Rml::String key;
			Rml::String defaultKey;
		};

		int row = 0;
		Rml::String description;
		Rml::String consoleCommand;
		Binding primaryBinding;
		Binding secondaryBinding;
	};

	bool SetUpDataBindings(Rml::DataModelConstructor& constructor) override;
	size_t Rows() const override;
	size_t Columns() const override;
	Rml::String DisplayString(size_t row, size_t column) const override;
	bool RowForConsoleCommand(const Rml::String& command, size_t& row) const;

	// Resets all bindings in the model to their default values by loading
	// the schema file. Does not apply engine bindings.
	void Reset() override;

	// Unbinds all keys in the engine, reloads saved binding config, and then
	// applies all key bindings to the engine.
	//
	// If the schema has not already been loaded, or if reloadDefaults is true,
	// loads this first. Otherwise, existing bindings in the model are left
	// at their current values.
	//
	// If resetToDefaultsOnError is set, and the saved binding config cannot be
	// loaded, the default values are reloaded.
	void ReloadAndApplyBindings(bool reloadDefaults, bool resetToDefaultsOnError);

	void SetBinding(size_t row, bool primary, Rml::String value, bool removeDuplicates = true);
	void ClearBinding(size_t row, bool primary);
	void ResetBindingToDefault(size_t row);
	void ResetAllBindingsToDefaults();
	void WriteBindings() const;

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
	void RefreshBindigsFromFile(bool resetOnError);
	ParseResult ReadBindings();
	void ReadBinding(const Rml::String& command, const Rml::String& key);
	Rml::String GetBindingStatement(const Entry& entry, bool primary) const;
	void RemoveBindingDuplicates(const Entry& entry);
	void ApplyAllBindingsToEngine(bool unbindFirst = true) const;
	void ApplyBindingToEngine(const Entry& entry, bool unbindFirst = true) const;
	void UnbindEngineKeysForCommand(const Rml::String& command) const;
	void ResetBindingsToDefaults();

	std::vector<Entry> m_Entries;
	std::unordered_map<Rml::String, size_t> m_ConsoleCommandToEntry;
	Rml::DataModelHandle m_ModelHandle;
};

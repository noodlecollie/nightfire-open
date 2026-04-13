#include "models/KeyBindingModel.h"
#include <RmlUi/Core/DataStructHandle.h>
#include <RmlUi/Core/DataModelHandle.h>
#include "CRTLib/crtlib.h"
#include "EnginePublicAPI/keydefs.h"
#include "utils/InFilePtr.h"
#include "udll_int.h"
#include "UIDebug.h"

static constexpr const char* const NAME_KEYBINDINGS = "keybindings";
static constexpr const char* const SCHEMA_PATH = "resource/state/controls_schema.lst";
static constexpr const char* const BINDINGS_PATH = "resource/state/keybindings.lst";

static constexpr const char* const PROP_ROW = "row";
static constexpr const char* const PROP_DESCRIPTION = "description";
static constexpr const char* const PROP_CONSOLE_COMMAND = "consoleCommand";
static constexpr const char* const PROP_PRIMARY_BINDING = "primaryBinding";
static constexpr const char* const PROP_SECONDARY_BINDING = "secondaryBinding";
static constexpr const char* const PROP_KEY = "key";
static constexpr const char* const PROP_DEFAULT_KEY = "defaultKey";

bool KeyBindingModel::SetUpDataBindings(Rml::DataModelConstructor& constructor)
{
	Rml::StructHandle<Entry> entryType = constructor.RegisterStruct<Entry>();
	Rml::StructHandle<Entry::Binding> bindingType = constructor.RegisterStruct<Entry::Binding>();

	if ( !entryType || !bindingType )
	{
		return false;
	}

	if ( !bindingType.RegisterMember(PROP_KEY, &Entry::Binding::key) ||
		 !bindingType.RegisterMember(PROP_DEFAULT_KEY, &Entry::Binding::defaultKey) )
	{
		return false;
	}

	if ( !entryType.RegisterMember(PROP_ROW, &Entry::row) ||
		 !entryType.RegisterMember(PROP_DESCRIPTION, &Entry::description) ||
		 !entryType.RegisterMember(PROP_CONSOLE_COMMAND, &Entry::consoleCommand) ||
		 !entryType.RegisterMember(PROP_PRIMARY_BINDING, &Entry::primaryBinding) ||
		 !entryType.RegisterMember(PROP_SECONDARY_BINDING, &Entry::secondaryBinding) )
	{
		return false;
	}

	if ( !constructor.RegisterArray<std::vector<Entry>>() || !constructor.Bind(NAME_KEYBINDINGS, &m_Entries) )
	{
		return false;
	}

	m_ModelHandle = constructor.GetModelHandle();
	return true;
}

size_t KeyBindingModel::Rows() const
{
	return m_Entries.size();
}

size_t KeyBindingModel::Columns() const
{
	return TOTAL_COLUMNS;
}

Rml::String KeyBindingModel::DisplayString(size_t row, size_t column) const
{
	if ( row >= Rows() )
	{
		return {};
	}

	const Entry& entry = m_Entries[row];

	switch ( column )
	{
		case DESCRIPTION:
		{
			return entry.description;
		}

		case CONSOLE_COMMAND:
		{
			return entry.consoleCommand;
		}

		case PRIMARY_BINDING:
		{
			return entry.primaryBinding.key;
		}

		case SECONDARY_BINDING:
		{
			return entry.secondaryBinding.key;
		}

		default:
		{
			break;
		}
	}

	return {};
}

void KeyBindingModel::Reset()
{
	ParseSchemaAndResetToDefaults();
}

void KeyBindingModel::ReloadAndApplyBindings(bool reloadDefaults, bool resetToDefaultsOnError)
{
	gEngfuncs.pfnClientCmd(1, "unbindall");

	if ( m_Entries.empty() || reloadDefaults )
	{
		Reset();
	}

	RefreshBindigsFromFile(resetToDefaultsOnError);
	ApplyAllBindingsToEngine(false);
}

bool KeyBindingModel::RowForConsoleCommand(const Rml::String& command, size_t& row) const
{
	const auto it = m_ConsoleCommandToEntry.find(command);

	if ( it == m_ConsoleCommandToEntry.end() )
	{
		return false;
	}

	row = it->second;
	return true;
}

void KeyBindingModel::SetBinding(size_t row, bool primary, Rml::String value, bool removeDuplicates)
{
	if ( row >= m_Entries.size() )
	{
		return;
	}

	Entry& entry = m_Entries[row];

	if ( primary && !entry.primaryBinding.key.empty() && entry.secondaryBinding.key.empty() &&
		 entry.primaryBinding.key != value )
	{
		// Bump primary to secondary if we're setting a new one
		// and there is no existing secondary.
		entry.secondaryBinding.key = entry.primaryBinding.key;
	}

	Rml::String& binding = primary ? entry.primaryBinding.key : entry.secondaryBinding.key;

	if ( binding == value )
	{
		return;
	}

	m_ModelHandle.DirtyVariable(NAME_KEYBINDINGS);
	binding = std::move(value);

	if ( entry.primaryBinding.key == entry.secondaryBinding.key )
	{
		// Coalesce to primary.
		entry.secondaryBinding.key.clear();
	}

	ApplyBindingToEngine(entry);

	if ( removeDuplicates )
	{
		RemoveBindingDuplicates(entry);
	}
}

void KeyBindingModel::ClearBinding(size_t row, bool primary)
{
	if ( row >= m_Entries.size() )
	{
		return;
	}

	Entry& entry = m_Entries[row];
	Entry::Binding& binding = primary ? entry.primaryBinding : entry.secondaryBinding;

	if ( !binding.key.empty() )
	{
		binding.key.clear();
		m_ModelHandle.DirtyVariable(NAME_KEYBINDINGS);

		if ( primary && !entry.secondaryBinding.key.empty() )
		{
			entry.primaryBinding.key = entry.secondaryBinding.key;
			entry.secondaryBinding.key.clear();
		}

		ApplyBindingToEngine(entry);
	}
}

void KeyBindingModel::ResetBindingToDefault(size_t row)
{
	if ( row >= m_Entries.size() )
	{
		return;
	}

	Entry& entry = m_Entries[row];
	bool changed = false;

	if ( entry.primaryBinding.key != entry.primaryBinding.defaultKey )
	{
		entry.primaryBinding.key = entry.primaryBinding.defaultKey;
		changed = true;
	}

	if ( entry.secondaryBinding.key != entry.secondaryBinding.defaultKey )
	{
		entry.secondaryBinding.key = entry.secondaryBinding.defaultKey;
		changed = true;
	}

	if ( changed )
	{
		ApplyBindingToEngine(entry);
		m_ModelHandle.DirtyVariable(NAME_KEYBINDINGS);

		RemoveBindingDuplicates(entry);
	}
}

void KeyBindingModel::ResetAllBindingsToDefaults()
{
	if ( m_Entries.empty() )
	{
		Reset();
	}

	gEngfuncs.pfnClientCmd(1, "unbindall");

	for ( Entry& entry : m_Entries )
	{
		if ( !entry.consoleCommand.empty() )
		{
			entry.primaryBinding.key = entry.primaryBinding.defaultKey;
			entry.secondaryBinding.key = entry.secondaryBinding.defaultKey;
			ApplyBindingToEngine(entry, false);
		}
	}

	m_ModelHandle.DirtyVariable(NAME_KEYBINDINGS);
}

void KeyBindingModel::ParseSchemaAndResetToDefaults()
{
	m_ConsoleCommandToEntry.clear();
	m_Entries.clear();

	if ( m_ModelHandle )
	{
		m_ModelHandle.DirtyAllVariables();
	}

	InFileCharsPtr file(SCHEMA_PATH, PFILE_HANDLENEWLINE);

	if ( !file )
	{
		Rml::Log::Message(Rml::Log::Type::LT_ERROR, "Failed to open %s", file.Path().c_str());
		return;
	}

	Rml::Log::Message(
		Rml::Log::Type::LT_DEBUG,
		"KeyBindingModel::ParseSchemaAndResetToDefaults: Opened key binding schema %s",
		file.Path().c_str()
	);

	while ( true )
	{
		Entry entry {};

		entry.row = static_cast<int>(m_Entries.size());
		ASSERT(entry.row >= 0);

		ParseResult result = ParseSchemaLine(file, entry);

		if ( result == ParseResult::Ok || result == ParseResult::Eof )
		{
			m_ConsoleCommandToEntry.insert({entry.consoleCommand, m_Entries.size()});
			m_Entries.push_back(std::move(entry));

			if ( result == ParseResult::Eof )
			{
				Rml::Log::Message(Rml::Log::Type::LT_INFO, "Loaded key binding schema %s", file.Path().c_str());
				break;
			}
		}
		else if ( result == ParseResult::Error )
		{
			Rml::Log::Message(Rml::Log::Type::LT_ERROR, "Failed to parse key binding schema %s", file.Path().c_str());
			m_ConsoleCommandToEntry.clear();
			m_Entries.clear();
			break;
		}
	}
}

KeyBindingModel::ParseResult KeyBindingModel::ParseSchemaLine(InFileCharsPtr& file, Entry& entry)
{
	char token[256];
	ParseResult result = ParseResult::Eof;

	// Description
	result = ParseToken(file, token, sizeof(token), true);

	if ( result == ParseResult::Eof || result == ParseResult::Error )
	{
		return result;
	}

	// Empty lines are allowed, we just loop again.
	if ( Q_strcmp(token, "\n") == 0 )
	{
		return ParseResult::Skip;
	}

	entry.description = token;

	// Console command or newline
	result = ParseToken(file, token, sizeof(token), true);

	if ( result != ParseResult::Ok )
	{
		return ParseResult::Error;
	}

	if ( Q_strcmp(token, "\n") == 0 )
	{
		// This is a heading line with just a description.
		entry.consoleCommand.clear();
		entry.primaryBinding.key.clear();
		entry.primaryBinding.defaultKey.clear();
		entry.secondaryBinding.key.clear();
		entry.secondaryBinding.defaultKey.clear();

		return ParseResult::Ok;
	}

	if ( Q_strcmp(token, "blank") != 0 )
	{
		entry.consoleCommand = token;
	}

	// Primary binding
	result = ParseToken(file, token, sizeof(token), false);

	if ( result != ParseResult::Ok )
	{
		return ParseResult::Error;
	}

	if ( Q_strcmp(token, "blank") != 0 )
	{
		entry.primaryBinding.key = token;
		entry.primaryBinding.defaultKey = token;
	}

	// Secondary binding
	result = ParseToken(file, token, sizeof(token), false);

	if ( result != ParseResult::Ok )
	{
		return ParseResult::Error;
	}

	if ( Q_strcmp(token, "blank") != 0 )
	{
		entry.secondaryBinding.key = token;
		entry.secondaryBinding.defaultKey = token;
	}

	// End of line
	result = ParseToken(file, token, sizeof(token), true);

	if ( result == ParseResult::Eof )
	{
		// End of file is OK here.
		return result;
	}

	if ( Q_strcmp(token, "\n") != 0 )
	{
		Rml::Log::Message(
			Rml::Log::Type::LT_ERROR,
			"KeyBindingModel::ParseSchemaLine: Expected end of line in %s but got token \"%s\"",
			file.Path().c_str(),
			result == ParseResult::Ok ? token : "<error>"
		);

		return ParseResult::Error;
	}

	return ParseResult::Ok;
}

KeyBindingModel::ParseResult KeyBindingModel::ParseToken(
	InFileCharsPtr& file,
	char* buffer,
	size_t bufferSize,
	bool allowNewline,
	const int* overrideFlags
)
{
	int tokenLength = 0;

	if ( !file.ParseToken(buffer, bufferSize, &tokenLength, overrideFlags) )
	{
		return ParseResult::Eof;
	}

	if ( tokenLength <= 0 )
	{
		Rml::Log::Message(
			Rml::Log::Type::LT_ERROR,
			"KeyBindingModel::ParseToken: Encountered token overflow in %s",
			file.Path().c_str()
		);

		return ParseResult::Error;
	}

	if ( !allowNewline && Q_strcmp(buffer, "\n") == 0 )
	{
		Rml::Log::Message(
			Rml::Log::Type::LT_ERROR,
			"KeyBindingModel::ParseToken: Unexpected end of line in %s",
			file.Path().c_str()
		);

		return ParseResult::Error;
	}

	return ParseResult::Ok;
}

void KeyBindingModel::RefreshBindigsFromFile(bool resetOnError)
{
	if ( ReadBindings() == ParseResult::Ok )
	{
		Rml::Log::Message(Rml::Log::Type::LT_INFO, "Loaded key bindings from %s", BINDINGS_PATH);
	}
	else if ( resetOnError )
	{
		Rml::Log::Message(
			Rml::Log::Type::LT_INFO,
			"No %s file could be loaded, using default key bindings",
			BINDINGS_PATH
		);

		ResetBindingsToDefaults();
	}
}

KeyBindingModel::ParseResult KeyBindingModel::ReadBindings()
{
	InFileCharsPtr file(BINDINGS_PATH, PFILE_HANDLENEWLINE);

	if ( !file )
	{
		// Maybe we haven't saved any bindings.
		return ParseResult::Skip;
	}

	Rml::Log::Message(
		Rml::Log::Type::LT_DEBUG,
		"KeyBindingModel::ReadBindings: Opened key bindings file %s",
		file.Path().c_str()
	);

	for ( Entry& entry : m_Entries )
	{
		entry.primaryBinding.key.clear();
		entry.secondaryBinding.key.clear();
	}

	while ( true )
	{
		char command[512];
		char key[128];

		ParseResult result = ParseToken(file, command, sizeof(command), true);

		if ( result == ParseResult::Eof )
		{
			// We're done
			break;
		}

		if ( result != ParseResult::Ok )
		{
			return ParseResult::Error;
		}

		if ( Q_strcmp(command, "\n") == 0 )
		{
			// Empty line
			continue;
		}

		result = ParseToken(file, key, sizeof(key), false);

		if ( result != ParseResult::Ok )
		{
			return ParseResult::Error;
		}

		// Expect a newline or EOF to terminate this line.
		char final[8];
		result = ParseToken(file, final, sizeof(final), true);

		const bool validEndOfLine =
			result == ParseResult::Eof || (result == ParseResult::Ok && Q_strcmp(final, "\n") == 0);

		if ( !validEndOfLine )
		{
			Rml::Log::Message(
				Rml::Log::Type::LT_ERROR,
				"KeyBindingModel::ReadBindings: Expected end of line in %s but got token \"%s\"",
				file.Path().c_str(),
				result == ParseResult::Ok ? final : "<error>"
			);

			return ParseResult::Error;
		}

		RML_DBGLOG(
			Rml::Log::Type::LT_DEBUG,
			"KeyBindingModel::ReadBindings: Parsed binding: \"%s\" -> \"%s\"",
			command,
			key
		);

		ReadBinding(command, key);
	}

	return ParseResult::Ok;
}

void KeyBindingModel::ReadBinding(const Rml::String& command, const Rml::String& key)
{
	size_t row = 0;

	if ( !RowForConsoleCommand(command, row) )
	{
		ASSERT(false);
		return;
	}

	Entry& entry = m_Entries[row];

	if ( entry.primaryBinding.key.empty() )
	{
		entry.primaryBinding.key = key;
	}
	else if ( entry.secondaryBinding.key.empty() )
	{
		entry.secondaryBinding.key = key;
	}
	else
	{
		Rml::Log::Message(
			Rml::Log::Type::LT_WARNING,
			"KeyBindingModel::ReadBinding: Ignoring unexpected extra key binding %s for command %s",
			key.c_str(),
			command.c_str()
		);
	}
}

void KeyBindingModel::WriteBindings() const
{
	Rml::String output;
	output.reserve(4096);

	output += "// Key bindings as saved by the options menu\n";
	output += "// WARNING: This file is auto-generated! Modifications may be overwritten!\n\n";

	for ( const Entry& entry : m_Entries )
	{
		for ( int binding = 0; binding < 2; ++binding )
		{
			Rml::String statement = GetBindingStatement(entry, binding == 0);

			if ( statement.empty() )
			{
				continue;
			}

			output += statement;
			output += "\n";
		}
	}

	if ( gEngfuncs.COM_SaveFile(BINDINGS_PATH, output.c_str(), output.size()) )
	{
		Rml::Log::Message(Rml::Log::Type::LT_INFO, "Saved key bindings to %s", BINDINGS_PATH);
	}
	else
	{
		Rml::Log::Message(
			Rml::Log::Type::LT_ERROR,
			"KeyBindingModel::WriteBindings: Failed to write %s",
			BINDINGS_PATH
		);
	}
}

Rml::String KeyBindingModel::GetBindingStatement(const Entry& entry, bool primary) const
{
	const Rml::String& command = entry.consoleCommand;
	const Rml::String& key = primary ? entry.primaryBinding.key : entry.secondaryBinding.key;

	if ( key.empty() )
	{
		return Rml::String();
	}

	// Important! If "\" is stored, this will not be re-parsed correctly.
	// Make sure this key is stored as "\\".
	Rml::String escapedKey = Rml::StringUtilities::Replace(key, "\\", "\\\\");

	Rml::String out;
	Rml::FormatString(out, "\"%s\" \"%s\"", command.c_str(), escapedKey.c_str());

	return out;
}

void KeyBindingModel::RemoveBindingDuplicates(const Entry& entry)
{
	bool modifiedAny = false;

	const auto clearBinding = [](Rml::String& binding, const Rml::String& key, bool& modified)
	{
		if ( !key.empty() && binding == key )
		{
			binding.clear();
			modified = true;
		}
	};

	for ( Entry& other : m_Entries )
	{
		if ( other.consoleCommand == entry.consoleCommand )
		{
			continue;
		}

		bool modified = false;
		clearBinding(other.primaryBinding.key, entry.primaryBinding.key, modified);
		clearBinding(other.primaryBinding.key, entry.secondaryBinding.key, modified);
		clearBinding(other.secondaryBinding.key, entry.primaryBinding.key, modified);
		clearBinding(other.secondaryBinding.key, entry.secondaryBinding.key, modified);

		if ( other.primaryBinding.key.empty() && !other.secondaryBinding.key.empty() )
		{
			other.primaryBinding.key = other.secondaryBinding.key;
			other.secondaryBinding.key.clear();
			modified = true;
		}

		if ( modified )
		{
			modifiedAny = true;
			ApplyBindingToEngine(entry);
		}
	}

	if ( modifiedAny && m_ModelHandle )
	{
		m_ModelHandle.DirtyVariable(NAME_KEYBINDINGS);
	}
}

void KeyBindingModel::ApplyAllBindingsToEngine(bool unbindFirst) const
{
	for ( const Entry& entry : m_Entries )
	{
		// Headings in the menu won't have console commands, so skip these.
		if ( !entry.consoleCommand.empty() )
		{
			ApplyBindingToEngine(entry, unbindFirst);
		}
	}
}

void KeyBindingModel::ApplyBindingToEngine(const Entry& entry, bool unbindFirst) const
{
	ASSERT(!entry.consoleCommand.empty());

	if ( entry.consoleCommand.empty() )
	{
		return;
	}

	if ( unbindFirst )
	{
		UnbindEngineKeysForCommand(entry.consoleCommand);
	}

	if ( !entry.primaryBinding.key.empty() )
	{
		Rml::String escapedKey = Rml::StringUtilities::Replace(entry.primaryBinding.key, "\\", "\\\\");

		Rml::String bindCmd;
		Rml::FormatString(bindCmd, "bind \"%s\" \"%s\"", escapedKey.c_str(), entry.consoleCommand.c_str());

		Rml::Log::Message(Rml::Log::LT_DEBUG, "KeyBindingModel::ApplyBindingToEngine: %s", bindCmd.c_str());
		gEngfuncs.pfnClientCmd(1, bindCmd.c_str());
	}

	if ( !entry.secondaryBinding.key.empty() )
	{
		Rml::String escapedKey = Rml::StringUtilities::Replace(entry.secondaryBinding.key, "\\", "\\\\");

		Rml::String bindCmd;
		Rml::FormatString(bindCmd, "bind \"%s\" \"%s\"", escapedKey.c_str(), entry.consoleCommand.c_str());

		Rml::Log::Message(Rml::Log::LT_DEBUG, "KeyBindingModel::ApplyBindingToEngine: %s", bindCmd.c_str());
		gEngfuncs.pfnClientCmd(1, bindCmd.c_str());
	}
}

void KeyBindingModel::UnbindEngineKeysForCommand(const Rml::String& command) const
{
	for ( int keyNum = 0; keyNum < MAX_KEY_BINDINGS; ++keyNum )
	{
		const char* boundCmd = gEngfuncs.pfnKeyGetBinding(keyNum);

		if ( !boundCmd || !(*boundCmd) || Q_strcmp(boundCmd, command.c_str()) != 0 )
		{
			continue;
		}

		Rml::Log::Message(Rml::Log::LT_DEBUG, "KeyBindingModel::UnbindEngineKeysForCommand: Unbinding key %d", keyNum);
		gEngfuncs.pfnKeySetBinding(keyNum, "");
	}
}

void KeyBindingModel::ResetBindingsToDefaults()
{
	for ( Entry& entry : m_Entries )
	{
		if ( entry.consoleCommand.empty() )
		{
			// A heading, not a binding.
			continue;
		}

		entry.primaryBinding.key = entry.primaryBinding.defaultKey;
		entry.secondaryBinding.key = entry.secondaryBinding.defaultKey;
	}
}

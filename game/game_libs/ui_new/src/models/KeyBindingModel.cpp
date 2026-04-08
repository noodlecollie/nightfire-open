#include "models/KeyBindingModel.h"
#include <RmlUi/Core/DataStructHandle.h>
#include <RmlUi/Core/DataModelHandle.h>
#include "CRTLib/crtlib.h"
#include "utils/InFilePtr.h"
#include "udll_int.h"
#include "UIDebug.h"

static constexpr const char* const NAME_KEYBINDINGS = "keybindings";
static constexpr const char* const SCHEMA_PATH = "controls_schema.lst";
static constexpr const char* const BINDINGS_PATH = "keybindings.lst";

static constexpr const char* const PROP_DESCRIPTION = "description";
static constexpr const char* const PROP_CONSOLE_COMMAND = "consoleCommand";
static constexpr const char* const PROP_PRIMARY_BINDING = "primaryBinding";
static constexpr const char* const PROP_SECONDARY_BINDING = "secondaryBinding";
static constexpr const char* const PROP_REBINDING_PRIMARY = "rebindingPrimary";
static constexpr const char* const PROP_REBINDING_SECONDARY = "rebindingSecondary";

bool KeyBindingModel::SetUpDataBindings(Rml::DataModelConstructor& constructor)
{
	Rml::StructHandle<Entry> kbType = constructor.RegisterStruct<Entry>();

	if ( !kbType || !kbType.RegisterMember(PROP_DESCRIPTION, &Entry::description) ||
		 !kbType.RegisterMember(PROP_CONSOLE_COMMAND, &Entry::consoleCommand) ||
		 !kbType.RegisterMember(PROP_PRIMARY_BINDING, &Entry::primaryBinding) ||
		 !kbType.RegisterMember(PROP_SECONDARY_BINDING, &Entry::secondaryBinding) ||
		 !kbType.RegisterMember(PROP_REBINDING_PRIMARY, &Entry::rebindingPrimary) ||
		 !kbType.RegisterMember(PROP_REBINDING_SECONDARY, &Entry::rebindingSecondary) )
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
	return TotalColumns;
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
		case Description:
		{
			return entry.description;
		}

		case ConsoleCommand:
		{
			return entry.consoleCommand;
		}

		case PrimaryBinding:
		{
			return entry.primaryBinding;
		}

		case SecondaryBinding:
		{
			return entry.secondaryBinding;
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

	if ( m_ModelHandle )
	{
		m_ModelHandle.DirtyVariable(NAME_KEYBINDINGS);
	}
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

bool KeyBindingModel::IsRebinding(size_t row, bool primary) const
{
	if ( row >= m_Entries.size() )
	{
		return false;
	}

	return primary ? m_Entries[row].rebindingPrimary : m_Entries[row].rebindingSecondary;
}

void KeyBindingModel::SetIsRebinding(size_t row, bool primary, bool rebinding)
{
	if ( row >= m_Entries.size() )
	{
		return;
	}

	bool& var = primary ? m_Entries[row].rebindingPrimary : m_Entries[row].rebindingSecondary;

	if ( var != rebinding )
	{
		var = rebinding;
		m_ModelHandle.DirtyVariable(NAME_KEYBINDINGS);
	}
}

void KeyBindingModel::SetBinding(size_t row, bool primary, Rml::String value)
{
	if ( row >= m_Entries.size() )
	{
		return;
	}

	Rml::String& binding = primary ? m_Entries[row].primaryBinding : m_Entries[row].secondaryBinding;

	if ( binding != value )
	{
		binding = std::move(value);
		m_ModelHandle.DirtyVariable(NAME_KEYBINDINGS);
	}
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

	while ( true )
	{
		Entry entry {};
		ParseResult result = ParseSchemaLine(file, entry);

		if ( result == ParseResult::Ok || result == ParseResult::Eof )
		{
			m_ConsoleCommandToEntry.insert({entry.consoleCommand, m_Entries.size()});
			m_Entries.push_back(std::move(entry));

			if ( result == ParseResult::Eof )
			{
				break;
			}
		}
		else if ( result == ParseResult::Error )
		{
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
		entry.primaryBinding.clear();
		entry.secondaryBinding.clear();

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
		entry.primaryBinding = token;
	}

	// Secondary binding
	result = ParseToken(file, token, sizeof(token), false);

	if ( result != ParseResult::Ok )
	{
		return ParseResult::Error;
	}

	if ( Q_strcmp(token, "blank") != 0 )
	{
		entry.secondaryBinding = token;
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
			"Expected end of line in %s but got token \"%s\"",
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
		Rml::Log::Message(Rml::Log::Type::LT_ERROR, "Encountered token overflow in %s", file.Path().c_str());
		return ParseResult::Error;
	}

	if ( !allowNewline && Q_strcmp(buffer, "\n") == 0 )
	{
		Rml::Log::Message(Rml::Log::Type::LT_ERROR, "Unexpected end of line in %s", file.Path().c_str());
		return ParseResult::Error;
	}

	return ParseResult::Ok;
}

void KeyBindingModel::RefreshBindigsFromFile()
{
	if ( ReadBindings() != ParseResult::Ok )
	{
		// No file found, or an error occurred, so use defaults.
		Reset();
	}
}

KeyBindingModel::ParseResult KeyBindingModel::ReadBindings()
{
	InFileCharsPtr file(BINDINGS_PATH, PFILE_HANDLENEWLINE);

	if ( !file )
	{
		// Maybe we haven't saved any bindings.
		Rml::Log::Message(
			Rml::Log::Type::LT_INFO,
			"No %s file could be loaded, using default key bindings",
			BINDINGS_PATH
		);

		return ParseResult::Skip;
	}

	for ( Entry& entry : m_Entries )
	{
		entry.primaryBinding.clear();
		entry.secondaryBinding.clear();
	}

	while ( true )
	{
		char command[512];
		char key[128];

		ParseResult result = ParseToken(file, command, sizeof(command), false);

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
				"Expected end of line in %s but got token \"%s\"",
				file.Path().c_str(),
				result == ParseResult::Ok ? final : "<error>"
			);

			return ParseResult::Error;
		}

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

	if ( entry.primaryBinding.empty() )
	{
		entry.primaryBinding = key;
	}
	else if ( entry.secondaryBinding.empty() )
	{
		entry.secondaryBinding = key;
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

	const int writeResult = gEngfuncs.COM_SaveFile(BINDINGS_PATH, output.c_str(), output.size());

	if ( writeResult < 0 || static_cast<size_t>(writeResult) != output.size() )
	{
		Rml::Log::Message(
			Rml::Log::Type::LT_WARNING,
			"KeyBindingModel::WriteBindings: Tried to write %zu bytes to %s, but call returned %d",
			output.size(),
			BINDINGS_PATH,
			writeResult
		);
	}
}

Rml::String KeyBindingModel::GetBindingStatement(const Entry& entry, bool primary) const
{
	const Rml::String& command = entry.consoleCommand;
	const Rml::String& key = primary ? entry.primaryBinding : entry.secondaryBinding;

	if ( key.empty() )
	{
		return Rml::String();
	}

	Rml::String out;
	Rml::FormatString(out, "\"%s\" \"%s\"", command.c_str(), key.c_str());

	return out;
}

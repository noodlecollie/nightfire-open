#include "models/KeyBindingModel.h"
#include <RmlUi/Core/DataStructHandle.h>
#include <RmlUi/Core/DataModelHandle.h>
#include "CRTLib/crtlib.h"
#include "utils/FilePtr.h"
#include "udll_int.h"

static constexpr const char* const NAME_KEYBINDINGS = "keybindings";
static constexpr const char* const SCHEMA_PATH = "controls_schema.lst";

bool KeyBindingModel::SetUpDataBindings(Rml::DataModelConstructor& constructor)
{
	Rml::StructHandle<Entry> kbType = constructor.RegisterStruct<Entry>();

	if ( !kbType || !kbType.RegisterMember("description", &Entry::description) ||
		 !kbType.RegisterMember("consoleCommand", &Entry::consoleCommand) ||
		 !kbType.RegisterMember("primaryBinding", &Entry::primaryBinding) ||
		 !kbType.RegisterMember("secondaryBinding", &Entry::secondaryBinding) )
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

void KeyBindingModel::ParseSchemaAndResetToDefaults()
{
	m_Entries.clear();

	FileCharsPtr file(SCHEMA_PATH, PFILE_HANDLENEWLINE);

	if ( !file )
	{
		Rml::Log::Message(Rml::Log::Type::LT_ERROR, "Failed to open %s", SCHEMA_PATH);
		return;
	}

	while ( true )
	{
		Entry entry {};
		ParseResult result = ParseSchemaLine(file, entry);

		if ( result == ParseResult::Ok || result == ParseResult::Eof )
		{
			m_Entries.push_back(std::move(entry));

			if ( result == ParseResult::Eof )
			{
				break;
			}
		}
		else if ( result == ParseResult::Error )
		{
			m_Entries.clear();
			break;
		}
	}
}

KeyBindingModel::ParseResult KeyBindingModel::ParseSchemaLine(FileCharsPtr& file, Entry& entry)
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
			SCHEMA_PATH,
			token
		);

		return ParseResult::Error;
	}

	return ParseResult::Ok;
}

KeyBindingModel::ParseResult KeyBindingModel::ParseToken(
	FileCharsPtr& file,
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
		Rml::Log::Message(Rml::Log::Type::LT_ERROR, "Encountered token overflow in %s", SCHEMA_PATH);
		return ParseResult::Error;
	}

	if ( !allowNewline && Q_strcmp(buffer, "\n") == 0 )
	{
		Rml::Log::Message(Rml::Log::Type::LT_ERROR, "Unexpected end of line in %s", SCHEMA_PATH);
		return ParseResult::Error;
	}

	return ParseResult::Ok;
}

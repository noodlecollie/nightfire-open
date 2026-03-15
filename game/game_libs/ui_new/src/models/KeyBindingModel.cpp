#include "models/KeyBindingModel.h"
#include <RmlUi/Core/DataStructHandle.h>
#include <RmlUi/Core/DataModelHandle.h>

static const char* const NAME_KEYBINDINGS = "keybindings";

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
	m_Entries.clear();

	if ( m_ModelHandle )
	{
		m_ModelHandle.DirtyVariable(NAME_KEYBINDINGS);
	}
}

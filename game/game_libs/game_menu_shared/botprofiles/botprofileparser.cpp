#include "botprofileparser.h"
#include "botprofiletable.h"
#include "rapidjson/document.h"
#include "rapidjson_helpers/rapidjson_helpers.h"
#include "utlstring.h"
#include "projectInterface/IProjectInterface.h"

#define LOG(level, ...) \
	IProjectInterface::ProjectInterfaceImpl()->LogInterface().LogF(level, "BotProfileParser: " __VA_ARGS__);

namespace
{
	bool PropertyExistsOnObject(
		rapidjson::SizeType index,
		const rapidjson::Value& value,
		const char* name,
		rapidjson::Type type,
		bool optional = false)
	{
		if ( !value.HasMember(name) )
		{
			if ( !optional )
			{
				LOG(ILogInterface::Level::Warning, "Profile %u has no property '%s', skipping.\n", index, name);
			}

			return false;
		}

		if ( value[name].GetType() != type )
		{
			if ( !optional )
			{
				LOG(ILogInterface::Level::Warning,
					"Profile %u property '%s' was not of type '%s', skipping.\n",
					index,
					name,
					rapidjson::ValueTypeAsString(type));
			}

			return false;
		}

		return true;
	}
}  // namespace

CBotProfileParser::CBotProfileParser(CBotProfileTable& table) :
	m_Table(table)
{
}

bool CBotProfileParser::Parse(const CUtlString& filePath)
{
	m_Table.Clear();

	rapidjson::Document document;

	if ( !rapidjson::LoadJsonFile(filePath, document, "BotProfileParser") )
	{
		return false;
	}

	return ReadParsedDocument(document);
}

bool CBotProfileParser::ReadParsedDocument(const rapidjson::Document& document)
{
	if ( !document.IsArray() )
	{
		LOG(ILogInterface::Level::Error, "Bot profile document did not contain a root array.\n");
		return false;
	}

	for ( rapidjson::SizeType index = 0; index < document.Size(); ++index )
	{
		ReadProfileEntry(index, document[index]);
	}

	LOG(ILogInterface::Level::Debug, "Loaded %u bot profiles.\n", m_Table.Count());
	return true;
}

void CBotProfileParser::ReadProfileEntry(rapidjson::SizeType index, const rapidjson::Value& object)
{
	if ( !object.IsObject() )
	{
		LOG(ILogInterface::Level::Warning, "Bot profile %u is not a JSON object, skipping.\n", index);
		return;
	}

	// Mandatory:

	if ( !PropertyExistsOnObject(index, object, "name", rapidjson::kStringType) )
	{
		return;
	}

	CUtlString name(object["name"].GetString());
	name.TrimWhitespace();

	if ( name.Length() < 1 )
	{
		LOG(ILogInterface::Level::Warning, "Bot profile %u name was empty.\n", index);
		return;
	}

	// Reject if profile name begins with a digit, because we want to be able to distinguish
	// profile names from bot counts when using bot_add.
	if ( name.String()[0] >= '0' && name.String()[0] <= '9' )
	{
		LOG(ILogInterface::Level::Warning,
			"Bot profile %u name '%s' began with a digit, which is not allowed.\n",
			index,
			name.String());
		return;
	}

	// Reject if profile name contains any spaces.
	for ( uint32_t nameIndex = 0; nameIndex < name.Length(); ++nameIndex )
	{
		if ( V_isspace(name.String()[nameIndex]) )
		{
			LOG(ILogInterface::Level::Warning,
				"Bot profile %u name '%s' contains whitespace, which is not allowed.\n",
				nameIndex,
				name.String());
			return;
		}
	}

	if ( !PropertyExistsOnObject(index, object, "skin", rapidjson::kStringType) )
	{
		return;
	}

	CUtlString skin(object["skin"].GetString());

	// Optional:

	CUtlString playerName("Bot");

	if ( PropertyExistsOnObject(index, object, "playerName", rapidjson::kStringType) )
	{
		playerName.Set(object["playerName"].GetString());
	}

	if ( m_Table.ProfileExists(name) )
	{
		LOG(ILogInterface::Level::Warning,
			"Bot profile %u: another profile with the name '%s' already existed.\n",
			index,
			name.String());
		return;
	}

	CBotProfileTable::ProfileData& profile = m_Table.CreateProfile(name);
	profile.skin = skin;
	profile.playerName = playerName;

	LOG(ILogInterface::Level::Debug, "Successfully created bot profile '%s'.\n", name.String());
}

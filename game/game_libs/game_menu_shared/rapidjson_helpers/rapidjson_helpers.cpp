#include "rapidjson_helpers.h"
#include "projectInterface/IProjectInterface.h"
#include "projectInterface/IFileLoader.h"
#include "projectInterface/ILogInterface.h"
#include "utlstring.h"
#include "rapidjson/error/en.h"

RAPIDJSON_NAMESPACE_BEGIN

const char* ValueTypeAsString(Type valueType)
{
	switch ( valueType )
	{
		case kNullType:
		{
			return "Null";
		}

		case kFalseType:
		{
			return "False";
		}

		case kTrueType:
		{
			return "True";
		}

		case kObjectType:
		{
			return "Object";
		}

		case kArrayType:
		{
			return "Array";
		}

		case kStringType:
		{
			return "String";
		}

		case kNumberType:
		{
			return "Number";
		}

		default:
		{
			return "Unknown";
		}
	}
}

bool LoadJsonFile(const CUtlString& path, Document& document, const char* moduleName)
{
	IProjectInterface* iface = IProjectInterface::ProjectInterfaceImpl();

	size_t length = 0;
	uint8_t* fileData = iface->FileLoader().Load(path, length);

	if ( !fileData )
	{
		CUtlString log;

		if ( moduleName )
		{
			log.AppendFormat("%s: ", moduleName);
		}

		log.AppendFormat("Could not load file %s.\n", path.String());
		iface->LogInterface().Log(ILogInterface::Level::Error, log);
		return false;
	}

	rapidjson::ParseResult parseResult = document.Parse(reinterpret_cast<char*>(fileData));
	iface->FileLoader().Free(fileData);

	if ( parseResult.IsError() )
	{
		CUtlString log;

		if ( moduleName )
		{
			log.AppendFormat("%s: ", moduleName);
		}

		log.AppendFormat(
			"JSON document %s could not be parsed. Error at offset %zu: %s\n",
			path.String(),
			parseResult.Offset(),
			rapidjson::GetParseError_En(parseResult.Code()));

		iface->LogInterface().Log(ILogInterface::Level::Error, log);
		return false;
	}

	return true;
}

RAPIDJSON_NAMESPACE_END

#pragma once

#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"

class CUtlString;
class IProjectInterface;

RAPIDJSON_NAMESPACE_BEGIN

const char* ValueTypeAsString(Type valueType);

template<typename T>
inline T GetProperty(const Value& value, const char* name, Type type, const T& defaultValue = T())
{
	return (value.HasMember(name) && value[name].GetType() == type) ? value[name].Get<T>() : defaultValue;
}

bool LoadJsonFile(const CUtlString& path, Document& document, const char* moduleName = nullptr);

RAPIDJSON_NAMESPACE_END

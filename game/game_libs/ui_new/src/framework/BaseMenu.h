#pragma once

namespace Rml
{
	class Context;
	class DataModelConstructor;
}  // namespace Rml

class BaseMenu
{
public:
	virtual ~BaseMenu() = default;

	const char* Name() const;
	const char* RmlFilePath() const;
	const char* DataModelName() const;

	virtual bool SetUpDataBindings(Rml::DataModelConstructor& constructor);
	virtual void Update(float currentTime) = 0;

protected:
	BaseMenu(const char* name, const char* rmlFilePath, const char* dataModelName = nullptr);

private:
	const char* m_Name;
	const char* m_RmlFilePath;
	const char* m_DataModelName;
};

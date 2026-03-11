#pragma once

class BaseMenu
{
public:
	virtual ~BaseMenu() = default;

	const char* Name() const;
	const char* RmlFilePath() const;

	virtual void Update(float currentTime) = 0;

protected:
	BaseMenu(const char* name, const char* rmlFilePath);

private:
	const char* m_Name;
	const char* m_RmlFilePath;
};

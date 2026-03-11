#pragma once

#include <vector>
#include <memory>
#include <type_traits>

class BaseMenu;

class MenuStack
{
public:
	template<typename T, typename... Args>
	typename std::enable_if<std::is_base_of<BaseMenu, T>::value, void>::type Push(Args&&... args)
	{
		m_Stack.push(std::unique_ptr<BaseMenu>(new T(std::move<Args...>(args))))
	}

	void Update(float currentTime);

	BaseMenu* Top() const;
	bool IsEmpty() const;

private:
	std::vector<std::unique_ptr<BaseMenu>> m_Stack;
};

#pragma once

#include <vector>
#include <type_traits>
#include <RmlUi/Core/Types.h>
#include <RmlUi/Core/Log.h>

namespace Rml
{
	class Element;
}

class ElementFinder
{
public:
	template<typename T>
	static constexpr bool IsElement()
	{
		return std::is_base_of<Rml::Element, T>::value;
	}

	template<typename T, typename R>
	static constexpr bool AreElements()
	{
		return IsElement<T>() && IsElement<R>();
	}

	template<typename T, typename R>
	typename std::enable_if<AreElements<T, R>(), bool>::type
	Add(R* const* root, Rml::String selector, T** outElement, bool optional = false)
	{
		auto typeCheckFunc = [](Rml::Element* element) -> bool
		{
			return element && rmlui_dynamic_cast<T*>(element);
		};

		// An Rml::Element** pointer is not technically the same
		// as a T** pointer where T is derived from Rml::Element.
		// This is because if you treat T** the same as Rml::Element**,
		// you could end up setting something else derived from
		// Rml::Element into the T*, and that might not necessarily
		// be an instance of T.
		// Here, we force a cast to Rml::Element**, but this is safe
		// because typeCheckFunc makes sure that the type actually
		// matches T.
		return AddInternal(
			reinterpret_cast<Rml::Element* const*>(root),
			std::move(selector),
			reinterpret_cast<Rml::Element**>(outElement),
			std::move(typeCheckFunc),
			optional
		);
	}

	template<typename T, typename R>
	typename std::enable_if<AreElements<T, R>(), bool>::type
	AddMulti(R* const* root, Rml::String selector, std::vector<T*>* outElements)
	{
		auto castAndAppendFunc = [outElements](Rml::Element* element) -> bool
		{
			T* downcasted = element ? rmlui_dynamic_cast<T*>(element) : nullptr;

			if ( !downcasted )
			{
				return false;
			}

			outElements->push_back(downcasted);
			return true;
		};

		auto clearFunc = [outElements]()
		{
			outElements->clear();
		};

		return AddMultiInternal(
			reinterpret_cast<Rml::Element* const*>(root),
			selector,
			outElements,
			std::move(castAndAppendFunc),
			std::move(clearFunc)
		);
	}

	bool FindAll(bool resetAllIfAnyMissed = true) const;

private:
	using TypeCheckFunc = std::function<bool(Rml::Element*)>;
	using ClearFunc = std::function<void(void)>;

	struct ElementDef
	{
		Rml::Element** element = nullptr;
		TypeCheckFunc typeCheckFunc;
		Rml::String selector;
		Rml::Element* const* root = nullptr;
		bool optional = false;
	};

	struct MultiElementDef
	{
		void* container = nullptr;
		TypeCheckFunc castAndAppendFunc;
		ClearFunc clearFunc;
		Rml::String selector;
		Rml::Element* const* root = nullptr;
	};

	bool AddInternal(
		Rml::Element* const* root,
		Rml::String selector,
		Rml::Element** outElement,
		TypeCheckFunc&& typeCheckFunc,
		bool optional
	);

	bool AddMultiInternal(
		Rml::Element* const* root,
		Rml::String selector,
		void* outElements,
		TypeCheckFunc&& castAndAppend,
		ClearFunc&& clear
	);

	bool FindSingleElements() const;
	bool FindMultiElements() const;

	std::vector<ElementDef> m_Defs;
	std::vector<MultiElementDef> m_MultiElementDefs;
};

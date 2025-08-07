#pragma once

#include <utility>
#include "standard_includes.h"
#include "EnginePublicAPI/event_args.h"

namespace EventConstructor
{
	// T: Type of value that this object holds.
	// M: Type of member variable on event struct.
	// P: Pointer to member in struct.
	template<typename T, typename M, M event_fire_args_t::*PTR>
	struct ExplicitStreamItem
	{
		inline explicit ExplicitStreamItem(const T& inVal) :
			value(inVal)
		{
		}

		inline explicit ExplicitStreamItem(const T&& inVal) :
			value(std::move(inVal))
		{
		}

		inline void Apply(event_fire_args_t& args) const
		{
			args.*PTR = value;
		}

		T value;
	};

	// Specialisation of the above, where T and M are the same.
	template<typename T, T event_fire_args_t::*PTR>
	struct StreamItem : public ExplicitStreamItem<T, T, PTR>
	{
		inline explicit StreamItem(const T& inVal) :
			ExplicitStreamItem<T, T, PTR>(inVal)
		{
		}

		inline explicit StreamItem(const T&& inVal) :
			ExplicitStreamItem<T, T, PTR>(std::move(inVal))
		{
		}
	};

	// Specialisation for bools, which should default to true.
	template<int event_fire_args_t::*PTR>
	struct BoolStreamItem : public ExplicitStreamItem<bool, int, PTR>
	{
		inline explicit BoolStreamItem(const bool& inVal = true) :
			ExplicitStreamItem<bool, int, PTR>(inVal)
		{
		}
	};

	// Specialisation for Vectors, which map to const float*.
	template<const float* event_fire_args_t::*PTR>
	struct VectorStreamItem : public ExplicitStreamItem<Vector, const float*, PTR>
	{
		inline explicit VectorStreamItem(const Vector& inVal) :
			ExplicitStreamItem<Vector, const float*, PTR>(inVal)
		{
		}

		inline explicit VectorStreamItem(const Vector&& inVal) :
			ExplicitStreamItem<Vector, const float*, PTR>(std::move(inVal))
		{
		}
	};

	using Flags = StreamItem<int, &event_fire_args_t::flags>;
	using Invoker = StreamItem<const edict_t*, &event_fire_args_t::invoker>;
	using EventIndex = StreamItem<unsigned short, &event_fire_args_t::eventIndex>;
	using Delay = StreamItem<float, &event_fire_args_t::delay>;
	using Origin = VectorStreamItem<&event_fire_args_t::vec3Origin>;
	using Angles = VectorStreamItem<&event_fire_args_t::vec3Angles>;
	using FloatParam1 = StreamItem<float, &event_fire_args_t::fparam1>;
	using FloatParam2 = StreamItem<float, &event_fire_args_t::fparam2>;
	using IntParam1 = StreamItem<int, &event_fire_args_t::iparam1>;
	using IntParam2 = StreamItem<int, &event_fire_args_t::iparam2>;
	using BoolParam1 = BoolStreamItem<&event_fire_args_t::bparam1>;
	using BoolParam2 = BoolStreamItem<&event_fire_args_t::bparam2>;
	using VecParam1 = VectorStreamItem<&event_fire_args_t::vec3param1>;

	class CEventConstructor
	{
	public:
		CEventConstructor();

		void Clear();
		void Send();

		// Convenience - corresponds to the old PLAYBACK_EVENT
		// and PLAYBACK_EVENT_DELAY macros.
		void InitSimple(int flags, const edict_t* edict, int eventIndex, float delay = 0.0f);

		event_fire_args_t& Args();
		const event_fire_args_t& Args() const;

		template<typename T, typename M, M event_fire_args_t::*PTR>
		inline CEventConstructor& operator<<(const ExplicitStreamItem<T, M, PTR>& item)
		{
			item.Apply(m_Args);
			return *this;
		}

	private:
		event_fire_args_t m_Args;
	};
}  // namespace EventConstructor

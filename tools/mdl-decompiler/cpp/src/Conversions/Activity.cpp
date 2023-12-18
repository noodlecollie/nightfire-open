#include <cstring>
#include "Conversions/ConversionHelpers.h"
#include "Conversions/Activity.h"

namespace Conversion
{
	static const Pair<MDLv14::Activity> MDLV14_ACTIVITY_STRING_TO_VALUE[] = {
#define LIST_ITEM(constant, value) {#constant, MDLv14::constant},
		MDLV14_ACTIVITY_LIST
#undef LIST_ITEM
	};

	static const Pair<QCv10::Activity> QCV10_ACTIVITY_STRING_TO_VALUE[] = {
#define LIST_ITEM(constant, value) {#constant, QCv10::constant},
		QCV10_ACTIVITY_LIST
#undef LIST_ITEM
	};

	template<typename T, size_t SIZE>
	static const char* GetActivityName(const Pair<T> (&array)[SIZE], int32_t value)
	{
		const Pair<T>* pair = FindByValue(array, value);

		if ( !pair )
		{
			throw std::invalid_argument("Unknown activity value " + std::to_string(value));
		}

		return pair->first;
	}

	const char* ActivityName(MDLv14::Activity activity)
	{
		return GetActivityName(MDLV14_ACTIVITY_STRING_TO_VALUE, activity);
	}

	const char* ActivityName(QCv10::Activity activity)
	{
		return GetActivityName(QCV10_ACTIVITY_STRING_TO_VALUE, activity);
	}

	MDLv14::Activity ConvertActivity_IntToV14(int32_t activity)
	{
		const Pair<MDLv14::Activity>* found = FindByValue(MDLV14_ACTIVITY_STRING_TO_VALUE, activity);

		if ( !found )
		{
			throw std::invalid_argument("Unknown activity value " + std::to_string(activity));
		}

		return found->second;
	}

	// The original C# code did an output enum parse based on the string of the input enum constant.
	// We replicate this method here.
	QCv10::Activity ConvertActivity_V14ToV10(MDLv14::Activity activity)
	{
		const Pair<MDLv14::Activity>* inActivityPair = FindByValue(MDLV14_ACTIVITY_STRING_TO_VALUE, activity);

		if ( !inActivityPair )
		{
			throw std::invalid_argument("Unknown activity value " + std::to_string(activity));
		}

		const Pair<QCv10::Activity>* outActivityPair =
			FindByString(QCV10_ACTIVITY_STRING_TO_VALUE, inActivityPair->first);

		if ( !outActivityPair )
		{
			throw std::invalid_argument(
				"MDL activity " + std::string(inActivityPair->first) + " does not have a corresponding QC activity");
		}

		return outActivityPair->second;
	}
}  // namespace Conversion

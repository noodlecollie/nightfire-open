#include <utility>
#include <cstring>
#include "Conversions/Activity.h"

template<typename T>
using Pair = std::pair<const char*, T>;

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
static const Pair<T>* FindByString(const Pair<T> (&array)[SIZE], const char* str)
{
	for ( size_t index = 0; index < SIZE; ++index )
	{
		if ( strcmp(str, array[index].first) == 0 )
		{
			return &array[index];
		}
	}

	return nullptr;
}

template<typename T, size_t SIZE>
static const Pair<T>* FindByValue(const Pair<T> (&array)[SIZE], int32_t value)
{
	for ( size_t index = 0; index < SIZE; ++index )
	{
		if ( static_cast<int32_t>(array[index].second) == value )
		{
			return &array[index];
		}
	}

	return nullptr;
}

template<typename T, size_t SIZE>
static const char* GetActivityName(const Pair<T> (&array)[SIZE], int32_t value)
{
	const Pair<T>* pair = FindByValue(array, value);
	return pair ? pair->first : "ACT_UNKNOWN";
}

const char* ActivityName(MDLv14::Activity activity)
{
	return GetActivityName(MDLV14_ACTIVITY_STRING_TO_VALUE, activity);
}

const char* ActivityName(QCv10::Activity activity)
{
	return GetActivityName(QCV10_ACTIVITY_STRING_TO_VALUE, activity);
}

bool ConvertActivity_IntToV14(int32_t inActivity, MDLv14::Activity& outActivity)
{
	const Pair<MDLv14::Activity>* found = FindByValue(MDLV14_ACTIVITY_STRING_TO_VALUE, inActivity);

	if ( !found )
	{
		return false;
	}

	outActivity = found->second;
	return true;
}

// The original C# code did an output enum parse based on the string of the input enum constant.
// We replicate this method here.
bool ConvertActivity_V14ToV10(MDLv14::Activity inActivity, QCv10::Activity& outActivity)
{
	const Pair<MDLv14::Activity>* inActivityPair = FindByValue(MDLV14_ACTIVITY_STRING_TO_VALUE, inActivity);

	if ( !inActivityPair )
	{
		return false;
	}

	const Pair<QCv10::Activity>* outActivityPair = FindByString(QCV10_ACTIVITY_STRING_TO_VALUE, inActivityPair->first);

	if ( !outActivityPair )
	{
		return false;
	}

	outActivity = outActivityPair->second;
	return true;
}

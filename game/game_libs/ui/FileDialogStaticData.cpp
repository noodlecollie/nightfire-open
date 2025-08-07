#include "BaseMenu.h"
#include "utlstring.h"
#include "utlvector.h"

struct FileDialogGlobals::Impl
{
	CUtlVector<CUtlString> m_Patterns;
	CUtlString m_Result;
	std::function<void(const char*)> m_ResultCallback;
	bool m_CallingCallback = false;
	bool m_ResultHasPreview = false;
};

FileDialogGlobals& FileDialogGlobals::GlobalData()
{
	static FileDialogGlobals data;
	return data;
}

FileDialogGlobals::FileDialogGlobals() :
	m_Impl(new Impl {})
{
}

FileDialogGlobals::~FileDialogGlobals()
{
	delete m_Impl;
	m_Impl = nullptr;
}

size_t FileDialogGlobals::PatternCount() const
{
	return GetImpl().m_Patterns.Count();
}

void FileDialogGlobals::ClearPatterns()
{
	GetImpl().m_Patterns.Purge();
}

void FileDialogGlobals::AddPattern(const char* pattern)
{
	if ( !pattern || !(*pattern) )
	{
		return;
	}

	GetImpl().m_Patterns.AddToTail(CUtlString(pattern));
}

bool FileDialogGlobals::HasResult() const
{
	return !GetImpl().m_Result.IsEmpty();
}

const char* FileDialogGlobals::GetPattern(size_t index) const
{
	return index < static_cast<size_t>(GetImpl().m_Patterns.Count())
		? GetImpl().m_Patterns[static_cast<int>(index)].Get()
		: nullptr;
}

void FileDialogGlobals::SetResultCallback(std::function<void(const char*)> resultCallback)
{
	GetImpl().m_ResultCallback = std::move(resultCallback);
}

void FileDialogGlobals::ClearResultCallback()
{
	GetImpl().m_ResultCallback = nullptr;
}

bool FileDialogGlobals::ResultHasPreview() const
{
	return GetImpl().m_ResultHasPreview;
}

void FileDialogGlobals::SetResultHasPreview(bool preview)
{
	GetImpl().m_ResultHasPreview = preview;
}

const char* FileDialogGlobals::GetResult() const
{
	return HasResult() ? GetImpl().m_Result.Get() : nullptr;
}

void FileDialogGlobals::SetResultAndCallCallback(const char* result)
{
	Impl& impl = GetImpl();

	if ( impl.m_CallingCallback )
	{
		return;
	}

	impl.m_Result.Set(result ? result : "");

	if ( impl.m_ResultCallback )
	{
		impl.m_CallingCallback = true;
		impl.m_ResultCallback((!impl.m_Result.IsEmpty()) ? impl.m_Result.Get() : nullptr);
		impl.m_CallingCallback = false;
	}
}

FileDialogGlobals::Impl& FileDialogGlobals::GetImpl()
{
	return *m_Impl;
}

const FileDialogGlobals::Impl& FileDialogGlobals::GetImpl() const
{
	return *m_Impl;
}

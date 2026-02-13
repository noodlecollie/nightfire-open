#include "rmlui/FileInterfaceImpl.h"

Rml::FileHandle FileInterfaceImpl::Open(const Rml::String& /* path */)
{
	// TODO
	return (Rml::FileHandle)-1;
}

void FileInterfaceImpl::Close(Rml::FileHandle /* file */)
{
	// TODO
}

size_t FileInterfaceImpl::Read(void* /* buffer */, size_t /* size */, Rml::FileHandle /* file */)
{
	// TODO
	return 0;
}

bool FileInterfaceImpl::Seek(Rml::FileHandle /* file */, long /* offset */, int /* origin */)
{
	// TODO
	return false;
}

size_t FileInterfaceImpl::Tell(Rml::FileHandle /* file */)
{
	// TODO
	return 0;
}

size_t FileInterfaceImpl::Length(Rml::FileHandle /* file */)
{
	// TODO
	return 0;
}

bool FileInterfaceImpl::LoadFile(const Rml::String& /* path */, Rml::String& /* out_data */)
{
	// TODO
	return false;
}

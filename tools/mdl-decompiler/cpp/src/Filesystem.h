#pragma once

#include <string>

bool DirectoryExists(const std::string& path);
bool FileExists(const std::string& path);
bool MakeDirectoryRecursive(const std::string& path);
std::string GetCurrentDirectory();

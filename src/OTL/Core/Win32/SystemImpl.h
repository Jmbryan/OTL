#pragma once
#include <OTL/Core/Time.h>
#include <string>

namespace otl
{

class SystemImpl
{
public:
   static void CreateDirectory(const std::string& directory);
   static std::string GetCurrentDirectory();
   static Time GetCurrentTime();
};

} // namespace otl

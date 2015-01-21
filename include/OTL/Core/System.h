#pragma once
#include <OTL/Core/Export.h>
#include <OTL/Core/Time.h>
#include <string>

namespace otl
{

class OTL_CORE_API System
{
public:
   void CreateDirectory(const std::string& directory);
    std::string GetCurrentDirectory();
    Time GetCurrentTime();
};

extern System OTL_CORE_API gSystem;

} // namespace otl
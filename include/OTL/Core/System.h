#pragma once
#include <OTL/Core/Time.h>
#include <string>

namespace otl
{

class System
{
public:
    std::string GetCurrentDirectory();
    Time GetCurrentTime();
    
};

extern System gSystem;

} // namespace otl
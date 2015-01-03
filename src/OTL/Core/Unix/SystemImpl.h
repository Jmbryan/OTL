#pragma once
#include <OTL/Core/Time.h>
#include <string>

namespace otl
{

class SystemImpl
{
public:
    static std::string GetCurrentDirectory();
    static Time GetCurrentTime();
};

} // namespace otl
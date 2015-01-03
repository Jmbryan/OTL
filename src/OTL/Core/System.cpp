#include <OTL/Core/System.h>
#ifdef _WIN32
    #include <OTL/Core/Win32/SystemImpl.h>
#else
    #include <OTL/Core/Unix/SystemImpl.h>
#endif

namespace otl
{

System gSystem;

std::string System::GetCurrentDirectory()
{
    return SystemImpl::GetCurrentDirectory();
}

Time System::GetCurrentTime()
{
    return SystemImpl::GetCurrentTime();
}

} // namespace otl
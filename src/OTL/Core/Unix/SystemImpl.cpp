#include <OTL/Core/Unix/SystemImpl.h>
#include <OTL/Core/Logger.h>
#include <unistd.h>
#include <time.h>

namespace otl
{

std::string SystemImpl::GetCurrentDirectory()
{
    char buffer[256];
    char* dir = getcwd(buffer, sizeof(buffer));
    if (dir)
    {
        return std::string(dir);
    }
    OTL_ERROR() << "Failed to get current directory";
    return std::string();
}

void SystemImpl::GetCurrentTime()
{
    timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    return Time::Seconds(static_cast<Uint64>(time.tv_sec) + time.tv_nsec / 1000000000);

}

} // namespace otl
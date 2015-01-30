#include <OTL/Core/Win32/SystemImpl.h>
#include <OTL/Core/Logger.h>
#include <windows.h>
#include <direct.h>

#ifdef CreateDirectory
#undef CreateDirectory
#endif

#ifdef GetCurrentDirectory
#undef GetCurrentDirectory
#endif

#ifdef GetCurrentTime
#undef GetCurrentTime
#endif

namespace otl
{

////////////////////////////////////////////////////////////
void SystemImpl::CreateDirectory(const std::string& directory)
{
   OTL_ERROR() << "Not implemented yet";
}

////////////////////////////////////////////////////////////
std::string SystemImpl::GetCurrentDirectory()
{
   char buffer[256];
   char* dir = _getcwd(buffer, sizeof(buffer));
   if (dir)
   {
      return std::string(dir);
   }
   OTL_ERROR() << "Failed to get current directory";
   return std::string();
}

////////////////////////////////////////////////////////////
Time SystemImpl::GetCurrentTime()
{
   // Force the following code to run on first core
   // (see http://msdn.microsoft.com/en-us/library/windows/desktop/ms644904(v=vs.85).aspx)
   HANDLE currentThread = GetCurrentThread();
   DWORD_PTR previousMask = SetThreadAffinityMask(currentThread, 1);

   // Get the frequency of the performance counter
   // (it is constant across the program lifetime)
   static LARGE_INTEGER frequency;
   QueryPerformanceFrequency(&frequency);

   // Get the current time
   LARGE_INTEGER time;
   QueryPerformanceCounter(&time);

   // Restore the thread affinity
   SetThreadAffinityMask(currentThread, previousMask);

   // Return the current time as seconds
   return Time::Seconds(static_cast<double>(time.QuadPart / frequency.QuadPart));
}

} // namespace otl

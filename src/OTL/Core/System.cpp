#include <OTL/Core/System.h>
#if defined (OTL_SYSTEM_WINDOWS)
    #include <OTL/Core/Win32/SystemImpl.h>
#else
    #include <OTL/Core/Unix/SystemImpl.h>
#endif
#include <OTL/Core/Exceptions.h>

#if defined (OTL_COMPILER_MSVC)
   #include <filesystem>
   #define OTL_USE_TR2_FILESYSTEM
#endif

namespace otl
{

System gSystem;

////////////////////////////////////////////////////////////
void System::CreateDirectory(const std::string& directory)
{
#if defined (OTL_USE_TR2_FILESYSTEM)
   std::tr2::sys::path path(directory);
   try
   {
      if (!std::tr2::sys::exists(path))
      {
         std::tr2::sys::create_directory(path);
      }
   }
   catch (std::tr2::sys::filesystem_error er)
   {
      std::cout << "Filesystem error caught when trying to create directory " << Bracket(directory) << ": " << er.what() << std::endl;
      throw Exception("Failed to create directory");
   }
#else
   SystemImpl::CreateDirectory(directory);
#endif
}

////////////////////////////////////////////////////////////
std::string System::GetCurrentDirectory()
{
#if defined (OTL_USE_TR2_FILESYSTEM)
   try
   {
#if (_MSC_VER >= 1900) // Visual Studio 14 2015
      return std::tr2::sys::current_path().string();
#else
	  return std::tr2::sys::current_path<std::string>();
#endif
   }
   catch (std::tr2::sys::filesystem_error er)
   {
      std::cout << "Filesystem error caught when trying to get current directory: " << er.what() << std::endl;
      throw Exception("Failed to get current directory");
   }
#else
   return SystemImpl::GetCurrentDirectory();
#endif
}

////////////////////////////////////////////////////////////
Time System::GetCurrentTime()
{
   return SystemImpl::GetCurrentTime();
}

} // namespace otl
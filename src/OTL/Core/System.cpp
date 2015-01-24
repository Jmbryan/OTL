#include <OTL/Core/System.h>
#ifdef _WIN32
    #include <OTL/Core/Win32/SystemImpl.h>
#else
    #include <OTL/Core/Unix/SystemImpl.h>
#endif
#include <OTL/Core/Exceptions.h>
#include <filesystem>

namespace otl
{

System gSystem;

void System::CreateDirectory(const std::string& directory)
{
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
      std::cout << "Filesystem error caught when trying to create directory [" << directory << "]: " << er.what() << std::endl;
      throw Exception("Failed to create directory");
   }
}

std::string System::GetCurrentDirectory()
{
   try
   {
      return std::tr2::sys::current_path<std::string>();
   }
   catch (std::tr2::sys::filesystem_error er)
   {
      std::cout << "Filesystem error caught when trying to get current directory: " << er.what() << std::endl;
      throw Exception("Failed to get current directory");
   }
}

Time System::GetCurrentTime()
{
   return SystemImpl::GetCurrentTime();
}

} // namespace otl
#include <OTL/Core/Spdlog/LoggerImpl.h>
#include <OTL/Core/Exceptions.h>
#include "spdlog/spdlog.h"

namespace otl
{

SpdLogPointer LoggerImpl::m_log;

static std::map<LogLevel, spdlog::level::level_enum> gLogLevelMap;

void LoggerImpl::Init(const std::string& logDirectory, const std::string& logFilename, int maxFileSize, int numRotatingFiles)
{
   std::string loggerName = "OTL";
   bool auto_flush = true;
   std::string logFile = logDirectory + "\\" + logFilename;

   gLogLevelMap[LogLevel::Debug] = spdlog::level::debug;
   gLogLevelMap[LogLevel::Info] = spdlog::level::info;
   gLogLevelMap[LogLevel::Warning] = spdlog::level::warn;
   gLogLevelMap[LogLevel::Error] = spdlog::level::err;
   gLogLevelMap[LogLevel::Fatal] = spdlog::level::critical;

   try
   {
      // Console
      auto consoleSink = std::shared_ptr<spdlog::sinks::stdout_sink_mt>(
         new spdlog::sinks::stdout_sink_mt());

      // Log file
      auto logfileSink = std::shared_ptr<spdlog::sinks::rotating_file_sink_mt>(
         new spdlog::sinks::rotating_file_sink_mt(
         logFile,
         "txt",
         maxFileSize,
         numRotatingFiles,
         auto_flush));

      m_log = spdlog::create(loggerName, { consoleSink, logfileSink });
      m_log->set_level(spdlog::level::trace);
   }
   catch (spdlog::spdlog_ex ex)
   {
      std::cout << "Exception caught while trying to initialize log file: " << ex.what() << std::endl;
      throw Exception("Failed to initialize log file");
   }
}

void LoggerImpl::Log(const std::string& message, const LogLevel& logLevel, bool _enabled, bool _abort, bool _throw)
{
    auto it = gLogLevelMap.find(logLevel);
    if (it == gLogLevelMap.end())
    {
        std::cout << "Invalid log level [" << static_cast<int>(logLevel) << "]" << std::endl;
        throw Exception("Invalid log level");
    }
    auto msg_level = it->second;
    if (_enabled)
    {
        spdlog::details::line_logger line_logger(m_log.get(), msg_level, true);
        line_logger << message;
    }
    if (_abort)
    {
       abort();
    }
    if (_throw)
    {
        throw Exception(message);
    }
}

} // namespace otl

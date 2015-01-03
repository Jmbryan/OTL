#include <OTL/Core/Spdlog/LoggerImpl.h>
#include <OTL/Core/Exceptions.h>
#include "spdlog/spdlog.h"

namespace otl
{

SpdLoggerPointer LoggerImpl::m_log;

static std::map<LogLevel, spdlog::level::level_enum> gLogLevelMap;

void LoggerImpl::Init()
{
   std::string loggerName = "otl_log";
   int file_size = 30 * 1024 * 1024;
   int rotating_files = 5;
   bool auto_flush = true;

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
         "D:/Dev/OTL/build/logs/otl_log",
         "txt",
         file_size,
         rotating_files,
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

void LoggerImpl::Log(const std::string& message, const LogLevel& logLevel, bool enabled, bool throwException)
{
   auto msg_level = gLogLevelMap[logLevel];
   if (enabled)
   {
      spdlog::details::line_logger line_logger(m_log.get(), msg_level, true);
      line_logger << message;
   }
   if (throwException)
   {
      throw Exception(message);
   }
}

} // namespace otl

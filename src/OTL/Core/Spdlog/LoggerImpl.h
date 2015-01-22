#pragma once
#include <OTL/Core/Base.h>

// Forward declarations
namespace spdlog { class logger; }
typedef std::shared_ptr<spdlog::logger> SpdLogPointer;

namespace otl
{

class LoggerImpl
{
public:
   static void Init(const std::string& logDirectory, const std::string& logFilename, int maxFileSize, int numRotatingFiles);
   static void Log(const std::string& message, const LogLevel& logLevel, bool _enabled, bool _abort, bool _throw);

private:
   static SpdLogPointer m_log;
};

} // namespace otl

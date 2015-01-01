#pragma once
#include <OTL/Core/Base.h>
#include <sstream>

// Forward declarations
namespace spdlog { class logger; }
typedef std::shared_ptr<spdlog::logger> SpdLoggerPointer;

namespace otl
{

class LoggerImpl
{
public:
   static void Init();
   static void Log(const std::stringstream& stream, const LogLevel& logLevel, bool enabled, bool throwException);

private:
   static SpdLoggerPointer m_log;
};

} // namespace otl
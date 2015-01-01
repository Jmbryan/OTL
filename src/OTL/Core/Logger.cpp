#include <OTL/Core/Logger.h>
#ifdef USE_GLOG
   #include <OTL/Core/Glog/LoggerImpl.h>
#else
   #include <OTL/Core/Spdlog/LoggerImpl.h>
#endif

namespace otl
{

Logger gLogger;

////////////////////////////////////////////////////////////
LineLogger::LineLogger(const LoggerPointer& logger, const LogLevel& logLevel) :
m_logger(logger),
m_logLevel(logLevel)
{

}

////////////////////////////////////////////////////////////
LineLogger::~LineLogger()
{
   if (m_logger)
   {
      m_logger->VLog(m_stream, m_logLevel);
   }
}

////////////////////////////////////////////////////////////
Logger::Logger() :
    m_initialized(false),
    m_logLevel(LogLevel::Fatal),
    m_throwLevel(LogLevel::Error)
{

}

////////////////////////////////////////////////////////////
Logger::~Logger()
{

}

////////////////////////////////////////////////////////////
void Logger::Initialize()
{
    VInitialize();
    m_initialized = true;
}

////////////////////////////////////////////////////////////
LineLogger Logger::Info()
{
    if (!m_initialized)
        Initialize();
    return LineLogger(std::make_shared<Logger>(*this), LogLevel::Info);
}

////////////////////////////////////////////////////////////
LineLogger Logger::Warn()
{
    if (!m_initialized)
        Initialize();
    return LineLogger(std::make_shared<Logger>(*this), LogLevel::Warning);
}

////////////////////////////////////////////////////////////
LineLogger Logger::Error()
{
    if (!m_initialized)
        Initialize();
    return LineLogger(std::make_shared<Logger>(*this), LogLevel::Error);
}

////////////////////////////////////////////////////////////
LineLogger Logger::Fatal()
{
    if (!m_initialized)
        Initialize();
    return LineLogger(std::make_shared<Logger>(*this), LogLevel::Fatal);
}

////////////////////////////////////////////////////////////
void Logger::SetLogLevel(LogLevel logLevel)
{
    m_logLevel = logLevel;
}

////////////////////////////////////////////////////////////
void Logger::SetThrowLevel(LogLevel throwLevel)
{
    m_throwLevel = throwLevel;
}

////////////////////////////////////////////////////////////
bool Logger::ShouldLog(LogLevel logLevel)
{
    return logLevel <= m_logLevel;
}

////////////////////////////////////////////////////////////
bool Logger::ShouldThrow(LogLevel throwLevel)
{
    return throwLevel >= m_throwLevel;
}

////////////////////////////////////////////////////////////
void Logger::VLog(const std::stringstream& stream, const LogLevel& logLevel)
{
   LoggerImpl::Log(stream, logLevel, ShouldLog(logLevel), ShouldThrow(logLevel));
}

////////////////////////////////////////////////////////////
void Logger::VInitialize()
{
   LoggerImpl::Init();
}

} // namespace otl

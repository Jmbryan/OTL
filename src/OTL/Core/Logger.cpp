#include <OTL/Core/Logger.h>
#ifdef USE_GLOG
   #error "GLog support not implemented yet"
   //#include <OTL/Core/Glog/LoggerImpl.h>
#else
   #include <OTL/Core/Spdlog/LoggerImpl.h>
#endif
#include <OTL/Core/System.h>

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
      m_logger->Log(m_stream.str(), m_logLevel);
   }
}

////////////////////////////////////////////////////////////
Logger::Logger() :
m_initialized(false),
m_logLevel(LogLevel::Info)
{
    std::string currentDirectory = gSystem.GetCurrentDirectory();
    m_logDirectory = currentDirectory + "\\logs";

    m_logFilename = "otl_log";
    m_maxFileSize = 10 * 1024 * 1024;
    m_numRotatingFiles = 5; 
}

////////////////////////////////////////////////////////////
Logger::~Logger()
{

}

////////////////////////////////////////////////////////////
void Logger::Initialize()
{
   gSystem.CreateDirectory(m_logDirectory);
   VInitialize();
   m_initialized = true;
}

////////////////////////////////////////////////////////////
void Logger::Log(const std::string& message, const LogLevel& logLevel)
{
    if (!m_initialized)
        Initialize();
    VLog(message, logLevel);
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
void Logger::SetLogDirectory(const std::string& logDirectory)
{
    m_logDirectory = logDirectory;
}

////////////////////////////////////////////////////////////
void Logger::SetLogFilename(const std::string& logFilename)
{
    m_logFilename = logFilename;
}

////////////////////////////////////////////////////////////
void Logger::SetMaxFileSize(int maxFileSize)
{
    m_maxFileSize = maxFileSize;
}

////////////////////////////////////////////////////////////
void Logger::SetNumRotatingFiles(int numRotatingFiles)
{
    m_numRotatingFiles = numRotatingFiles;
}

////////////////////////////////////////////////////////////
void Logger::VInitialize()
{
   LoggerImpl::Init(m_logDirectory, m_logFilename, m_maxFileSize, m_numRotatingFiles);
}

////////////////////////////////////////////////////////////
bool Logger::VShouldLog(LogLevel logLevel)
{
   return (logLevel >= m_logLevel);
}

////////////////////////////////////////////////////////////
bool Logger::VShouldThrow(LogLevel logLevel)
{
   return (logLevel == LogLevel::Error);
}

////////////////////////////////////////////////////////////
bool Logger::VShouldAbort(LogLevel logLevel)
{
   return (logLevel == LogLevel::Fatal);
}

////////////////////////////////////////////////////////////
void Logger::VLog(const std::string& message, const LogLevel& logLevel)
{
   LoggerImpl::Log(message, logLevel, VShouldLog(logLevel), VShouldAbort(logLevel), VShouldThrow(logLevel));
}

} // namespace otl

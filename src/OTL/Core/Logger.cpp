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
    m_logLevel(LogLevel::Fatal),
    m_throwLevel(LogLevel::Error)
{
    std::string currentDirectory = gSystem.GetCurrentDirectory();
    m_logDirectory = currentDirectory + "\\logs";
    gSystem.CreateDirectory(m_logDirectory);

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
void Logger::SetThrowLevel(LogLevel throwLevel)
{
    m_throwLevel = throwLevel;
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
void Logger::VLog(const std::string& message, const LogLevel& logLevel)
{
   LoggerImpl::Log(message, logLevel, ShouldLog(logLevel), ShouldThrow(logLevel));
}

////////////////////////////////////////////////////////////
void Logger::VInitialize()
{
   LoggerImpl::Init(m_logDirectory, m_logFilename, m_maxFileSize, m_numRotatingFiles);
}

} // namespace otl

#pragma once
#include <OTL/Core/Export.h>
#include <sstream>
#include <memory>

namespace otl
{

// Forward declarations
class Logger;
typedef std::shared_ptr<Logger> LoggerPointer;
 
enum class LogLevel
{
   Invalid = -1,  ///< Invalid log level
   Info,          ///< Logs if logLevel is <= Info
   Warning,       ///< Logs if logLevel is <= Warning
   Error,         ///< Logs if logLevel is <= Error, throws Exception()
   Fatal,         ///< Logs if loglevel is <= Fatal, calls abort()
   None,          ///< Disables all logging
   Count          ///< Number of log levels
};

////////////////////////////////////////////////////////////
class OTL_CORE_API LineLogger
{
public:
   LineLogger(const LoggerPointer& logger, const LogLevel& logLevel);
   ~LineLogger();

   template<typename T>
   LineLogger& operator<<(const T& what)
   {
      m_stream << what;
      return *this;
   }

private:
   LoggerPointer m_logger;
   LogLevel m_logLevel;
   std::stringstream m_stream;
};

////////////////////////////////////////////////////////////
class OTL_CORE_API Logger
{
public:
    Logger();
    virtual ~Logger();
    void Initialize();

    void Log(const std::string& message, const LogLevel& logLevel);

    LineLogger Info();
    LineLogger Warn();
    LineLogger Error();
    LineLogger Fatal();

    // Log messages of this severity and greater
    void SetLogLevel(LogLevel logLevel);

    void SetLogDirectory(const std::string& logDirectory);
    void SetLogFilename(const std::string& logFilename);
    void SetMaxFileSize(int maxFileSize);
    void SetNumRotatingFiles(int numRotatingFiles);

protected:
    virtual void VInitialize();
    virtual void VLog(const std::string& message, const LogLevel& logLevel);
    bool ShouldLog(LogLevel logLevel);
    bool ShouldThrow(LogLevel logLevel);
    bool ShouldAbort(LogLevel logLevel);
    
private:
    bool m_initialized;
    LogLevel m_logLevel;
    std::string m_logDirectory;
    std::string m_logFilename;
    int m_maxFileSize;
    int m_numRotatingFiles;
};

template<typename T>
std::string Bracket(const T& object)
{
   std::stringstream ss;
   ss << "[" << object << "]";
   return ss.str();
}

extern Logger OTL_CORE_API gLogger;

#define OTL_LOG(message, level) do \
   { std::stringstream ss; ss << message; gLogger.Log(ss.str(), level); } while(0)
#define OTL_INFO() gLogger.Info()
#define OTL_WARN() gLogger.Warn()
#define OTL_ERROR() gLogger.Error()
#define OTL_FATAL() gLogger.Fatal()

//#define OTL_INFO(message) OTL_LOG(message, LogLevel::Info)
//#define OTL_WARN(message) OTL_LOG(message, LogLevel::Warning)
//#define OTL_ERROR(message) OTL_LOG(message, LogLevel::Error)
//#define OTL_FATAL(message) OTL_LOG(message, LogLevel::Fatal)

#define OTL_INFO_IF(condition, message) do \
   { if ((condition)) { OTL_LOG(message, LogLevel::Info); } } while(0)
#define OTL_WARN_IF(condition, message) do \
   { if ((condition)) { OTL_LOG(message, LogLevel::Warning); } } while(0)
#define OTL_ERROR_IF(condition, message) do \
   { if ((condition)) { OTL_LOG(message, LogLevel::Error); } } while(0)
#define OTL_FATAL_IF(condition, message) do \
   { if ((condition)) { OTL_LOG(message, LogLevel::Fatal); } } while(0)

#define OTL_ASSERT(condition, message) OTL_FATAL_IF(!(condition), message)

} // namespace otl

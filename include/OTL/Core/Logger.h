#pragma once
#include <OTL/Core/Base.h>
#include <sstream>

namespace otl
{

// Forward declarations
class Logger;
typedef std::shared_ptr<Logger> LoggerPointer;

////////////////////////////////////////////////////////////
class LineLogger
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
class Logger
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

    // Log messages less than or below this level
    void SetLogLevel(LogLevel logLevel);

    // Throw an exception if greater than or equal to this level
    void SetThrowLevel(LogLevel throwLevel);

    void SetLogDirectory(const std::string& logDirectory);
    void SetLogFilename(const std::string& logFilename);
    void SetMaxFileSize(int maxFileSize);
    void SetNumRotatingFiles(int numRotatingFiles);

protected:
    virtual void VInitialize();
    virtual void VLog(const std::string& message, const LogLevel& logLevel);
    bool ShouldLog(LogLevel logLevel);
    bool ShouldThrow(LogLevel throwLevel);
    
private:
    bool m_initialized;
    LogLevel m_logLevel;
    LogLevel m_throwLevel;
    std::string m_logDirectory;
    std::string m_logFilename;
    int m_maxFileSize;
    int m_numRotatingFiles;
};

extern Logger gLogger;
#define OTL_LOG(message, level) gLogger.Log(message, level)
#define OTL_INFO() gLogger.Info()
#define OTL_WARN() gLogger.Warn()
#define OTL_ERROR() gLogger.Error()
#define OTL_FATAL() gLogger.Fatal()

} // namespace otl

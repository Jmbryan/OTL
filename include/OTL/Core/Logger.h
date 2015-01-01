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

    LineLogger Info();
    LineLogger Warn();
    LineLogger Error();
    LineLogger Fatal();

    // Log messages less than or below this level
    void SetLogLevel(LogLevel logLevel);

    // Throw an exception if greater than or equal to this level
    void SetThrowLevel(LogLevel throwLevel);

protected:
    virtual void VInitialize();
    virtual void VLog(const std::stringstream& stream, const LogLevel& logLevel);

private:
   bool ShouldLog(LogLevel logLevel);
   bool ShouldThrow(LogLevel throwLevel);
    
private:
    bool m_initialized;
    LogLevel m_logLevel;
    LogLevel m_throwLevel;
    friend LineLogger;
};

extern Logger gLogger;
#define OTL_INFO() gLogger.Info()
#define OTL_WARN() gLogger.Warn()
#define OTL_ERROR() gLogger.Error()
#define OTL_FATAL() gLogger.Fatal()

} // namespace otl

#pragma once
#include <OTL/Core/Export.h>
#include <sstream>
#include <memory>
#include <tuple>

namespace otl
{

// Forward declarations
class Logger;
typedef std::shared_ptr<Logger> LoggerPointer;
 
enum class LogLevel
{
   Invalid = -1,  ///< Invalid log level
   Debug,         ///< Logs if logLEvel is <= Debug
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
   LineLogger(const LineLogger& other);
   LineLogger(const LoggerPointer& logger, const LogLevel& logLevel);
   ~LineLogger();

   template<typename T>
   LineLogger& operator<<(const T& what)
   {
      m_stream << what;
      return *this;
   }

   const LoggerPointer& GetLogger() const { return m_logger; }
   const LogLevel& GetLogLevel() const { return m_logLevel; }

private:
   LoggerPointer m_logger;
   LogLevel m_logLevel;
   std::ostringstream m_stream;
};

////////////////////////////////////////////////////////////
class OTL_CORE_API Logger
{
public:
    Logger();
    virtual ~Logger();
    void Initialize();

    void Log(const std::string& message, const LogLevel& logLevel);

    LineLogger Debug();
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
    virtual bool VShouldLog(LogLevel logLevel);
    virtual bool VShouldThrow(LogLevel logLevel);
    virtual bool VShouldAbort(LogLevel logLevel);
    virtual void VLog(const std::string& message, const LogLevel& logLevel);
    
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
   std::ostringstream ss;
   ss << "[" << object << "]";
   return ss.str();
}

extern Logger OTL_CORE_API gLogger;

// Logging macros similar to std::cout
#define OTL_DEBUG() gLogger.Debug()
#define OTL_INFO() gLogger.Info()
#define OTL_WARN() gLogger.Warn()
#define OTL_ERROR() gLogger.Error()
#define OTL_FATAL() gLogger.Fatal()

// Standard logging macro
#define OTL_LOG(message, level) do \
   { std::ostringstream os; os << message; gLogger.Log(os.str(), level); } while(0)

// Conditional logging macros
// No processing occurs if the condition is false
#define OTL_DEBUG_IF(condition, message) do \
   { if ((condition)) { OTL_LOG(message, LogLevel::Debug); } } while(0)
#define OTL_INFO_IF(condition, message) do \
   { if ((condition)) { OTL_LOG(message, LogLevel::Info); } } while(0)
#define OTL_WARN_IF(condition, message) do \
   { if ((condition)) { OTL_LOG(message, LogLevel::Warning); } } while(0)
#define OTL_ERROR_IF(condition, message) do \
   { if ((condition)) { OTL_LOG(message, LogLevel::Error); } } while(0)
#define OTL_FATAL_IF(condition, message) do \
   { if ((condition)) { OTL_LOG(message, LogLevel::Fatal); } } while(0)

// Some varadiac macro magic to support overloaded macros
// OTL_OVERLOAD allows passing variadic data to non-variadic macros
// Reference: http://www.boost.org/doc/libs/master/libs/preprocessor/doc/topics/variadic_macros.html
#define OTL_EMPTY()
#define OTL_CAT_I(a, b) a ## b
#define OTL_CAT(a, b) OTL_CAT_I(a, b)
#define OTL_VAR_SIZE_I(e0, e1, size, ...) size
#define OTL_VAR_SIZE(...) OTL_CAT(OTL_VAR_SIZE_I(__VA_ARGS__, 2, 1,),)
#define OTL_OVERLOAD(prefix, ...) OTL_CAT(prefix, OTL_VAR_SIZE(__VA_ARGS__))

// Overloaded assertion macro
// Reference: http://www.boost.org/doc/libs/master/libs/preprocessor/doc/topics/variadic_macros.html
#define OTL_ASSERT_1(condition) OTL_FATAL_IF(!(condition), "")
#define OTL_ASSERT_2(condition, message) OTL_FATAL_IF(!(condition), message)
#define OTL_ASSERT(...) OTL_CAT(OTL_OVERLOAD(OTL_ASSERT_,__VA_ARGS__)(__VA_ARGS__),OTL_EMPTY())

} // namespace otl

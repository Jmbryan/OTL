#pragma once
#include <OTL/Core/Details/SpdlogWrapper.h>
#include <string>
#include <memory>

namespace otl
{

enum class LogLevel
{
    Invalid = -1,
    Info,
    Warning,
    Error,
    Fatal
};

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
    bool ShouldLog(LogLevel logLevel);
    bool ShouldThrow(LogLevel throwLevel);
    virtual void VInitialize();

protected:
    LogPointer m_log;

private:
    bool m_initialized;
    LogLevel m_logLevel;
    LogLevel m_throwLevel;
};

extern Logger gLogger;
#define OTL_INFO() gLogger.Info()
#define OTL_WARN() gLogger.Warn()
#define OTL_ERROR() gLogger.Error()
#define OTL_FATAL() gLogger.Fatal()

} // namespace otl

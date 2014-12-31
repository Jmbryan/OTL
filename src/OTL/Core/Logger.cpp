#include <OTL/Core/Logger.h>

namespace otl
{

Logger::Logger() :
    m_initialized(false),
    m_logLevel(LogLevel::Fatal),
    m_throwLevel(LogLevel::Error)
{

}

Logger::~Logger()
{

}

void Logger::Initialize()
{
    VInitialize();
    m_initialized = true;
}

LineLogger Logger::Info()
{
    if (!m_initialized)
        Initialize();
    return LineLogger(m_log, spdlog::level::info, ShouldLog(LogLevel::Info), ShouldThrow(LogLevel::Info));
}

LineLogger Logger::Warn()
{
    if (!m_initialized)
        Initialize();
    return LineLogger(m_log, spdlog::level::warn, ShouldLog(LogLevel::Warning), ShouldThrow(LogLevel::Warning));
}

LineLogger Logger::Error()
{
    if (!m_initialized)
        Initialize();
    return LineLogger(m_log, spdlog::level::err, ShouldLog(LogLevel::Error), ShouldThrow(LogLevel::Error));
}

LineLogger Logger::Fatal()
{
    if (!m_initialized)
        Initialize();
    return LineLogger(m_log, spdlog::level::critical, ShouldLog(LogLevel::Fatal), ShouldThrow(LogLevel::Fatal));
}

void Logger::SetLogLevel(LogLevel logLevel)
{
    m_logLevel = logLevel;
}

void Logger::SetThrowLevel(LogLevel throwLevel)
{
    m_throwLevel = throwLevel;
}

bool Logger::ShouldLog(LogLevel logLevel)
{
    return logLevel <= m_logLevel;
}

bool Logger::ShouldThrow(LogLevel throwLevel)
{
    return throwLevel >= m_throwLevel;
}

void Logger::VInitialize()
{
    std::string loggerName = "otl_log";
    int file_size = 30 * 1024 * 1024;
    int rotating_files = 5;
    bool auto_flush = true;

    // Console
    auto consoleSink = std::shared_ptr<spdlog::sinks::stdout_sink_mt>(
        new spdlog::sinks::stdout_sink_mt());

    // Log file
    auto logfileSink = std::shared_ptr<spdlog::sinks::rotating_file_sink_mt>(
        new spdlog::sinks::rotating_file_sink_mt(
        "D:/Dev/OTL/build/logs/otl_log",
        "txt",
        file_size,
        rotating_files,
        auto_flush));

    m_log = spdlog::create(loggerName, { consoleSink, logfileSink });
    m_log->set_level(spdlog::level::trace);
}

Logger gLogger;

} // namespace otl
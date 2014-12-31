#pragma once
#include "spdlog/spdlog.h"

namespace otl
{

// Forward declarations
typedef std::shared_ptr<spdlog::logger> LogPointer;

class LineLogger
{
public:
    LineLogger(const LogPointer& logger, const spdlog::level::level_enum& msg_level, bool enabled, bool throwException);
    ~LineLogger();

    template<typename T>
    LineLogger& operator<<(const T& what)
    {
        m_log_msg.raw << what;
        return *this;
    }

private:
    LogPointer m_logger;
    spdlog::level::level_enum m_msg_level;
    bool m_enabled;
    bool m_throwException;
    spdlog::details::log_msg m_log_msg;
};

//class Logger;
//class SpdlogLogger : public Logger
//{
//
//};

} // namespace otl
#include <OTL/Core/Details/SpdlogWrapper.h>
#include <OTL/Core/Exceptions.h>

namespace otl
{

////////////////////////////////////////////////////////////
LineLogger::LineLogger(const LogPointer& logger, const spdlog::level::level_enum& msg_level, bool enabled, bool throwException) :
m_logger(logger),
m_msg_level(msg_level),
m_enabled(enabled),
m_throwException(throwException)
{

}

////////////////////////////////////////////////////////////
LineLogger::~LineLogger()
{
    auto msg = m_log_msg.raw.c_str();
    if (m_enabled)
    {
        spdlog::details::line_logger line_logger(m_logger.get(), m_msg_level, true);
        line_logger << msg;
    }
    if (m_throwException)
    {
        throw Exception(msg);
    }
}

} // namespace otl
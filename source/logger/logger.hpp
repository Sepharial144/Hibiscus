#ifndef _HIBISCUS_LOGGER_HPP_
#define _HIBISCUS_LOGGER_HPP_

#include <string>
#include <thread>

#include <fmt/color.h>
#include <fmt/compile.h>
#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/printf.h>
#include <fmt/ranges.h>
//#include <fmt/std.h>

namespace logger {
/*
template <typename... Args>
std::string makeString(fmt::format_string<Args...> s, Args&&... args)
{
    return fmt::format(s, std::forward<Args>(args)...);
}
*/

template <typename... Args>
const std::string makeBraces(size_t num_of_lines)
{
    // Generate formatting string "{} "...
    //std::array<char, num_of_lines * 3 + 1> braces{};
    std::string line {};
    line.reserve(num_of_lines * 3 + 1);
    constexpr const char c[4] = "{} ";
    for (auto i = 0ul; i != line.size() - 1; ++i)
        line[i] = c[i % 3];
    line.back() = '\0';

    return line;
}
/*
template <class... Args>
void fatal(Args&&... args)
{
#if defined(LOGGING) && (LOGGING_LEVEL <= LOGGING_FATAL_VALUE)
    fmt::print(fmt::fg(fmt::color::red), 
    FMT_COMPILE(sizeof...(Args) + 1), 
    std::this_thread::get_id(),
    fmt::format(makeBraces(sizeof...(Args)), std::forward<Args>(args)...));
#endif // LOGGING
}

template <class... Args>
void error(std::string&& format_string, Args&&... args)
{
#if defined(LOGGING) && (LOGGING_LEVEL <= LOGGING_ERROR_VALUE)
    fmt::print(fmt::fg(fmt::color::orange_red), 
    FMT_COMPILE("{}: {}\n"), 
    std::this_thread::get_id(),
    fmt::format(makeBraces(sizeof...(Args)), std::forward<Args>(args)...));
#endif // LOGGING
}

template <class... Args>
void warning(std::string&& format_string, Args&&... args)
{
#if defined(LOGGING) && (LOGGING_LEVEL <= LOGGING_WARNING_VALUE)
    fmt::print(fmt::fg(fmt::color::orange), 
    FMT_COMPILE("{}: {}\n"), 
    std::this_thread::get_id(),
    fmt::format(makeBraces(sizeof...(Args)), std::forward<Args>(args)...));
#endif // LOGGING
}
*/

template <class... Args>
void info(const Args&... args)
{
#if defined(LOGGING) && (LOGGING_LEVEL <= LOGGING_INFO_VALUE)
    fmt::print(fmt::format(FMT_COMPILE("{}"), fmt::format(makeBraces(sizeof...(Args) + 1), std::this_thread::get_id(), args...)));
#endif // LOGGING
}

template <class... Args>
void debug(const Args&... args)
{
#if defined(LOGGING) && (LOGGING_LEVEL <= LOGGING_DEBUG_VALUE)
    fmt::print(fmt::fg(fmt::color::yellow), fmt::format(FMT_COMPILE("{}"), fmt::format(makeBraces(sizeof...(Args) + 1), std::this_thread::get_id(), args...)));
#endif // LOGGING
}
/*
template <class... Args>
void debug_low(std::string&& format_string, Args&&... args)
{
#if defined(LOGGING) && (LOGGING_LEVEL <= LOGGING_DEBUG_LOW_VALUE)
    fmt::print(fmt::fg(fmt::color::violet), 
    FMT_COMPILE("{}: {}\n"), 
    std::this_thread::get_id(),
    fmt::format(makeBraces(sizeof...(Args)), std::forward<Args>(args)...));
#endif // LOGGING
}

template <class... Args>
void debug_medium(std::string&& format_string, Args&&... args)
{
#if defined(LOGGING) && (LOGGING_LEVEL <= LOGGING_DEBUG_MEDIUM_VALUE)
    fmt::print(fmt::fg(fmt::color::dark_orchid), 
    FMT_COMPILE("{}: {}\n"), 
    std::this_thread::get_id(),
    fmt::format(makeBraces(sizeof...(Args)), std::forward<Args>(args)...));
#endif // LOGGING
}

template <class... Args>
void debug_high(std::string&& format_string, Args&&... args)
{
#if defined(LOGGING) && (LOGGING_LEVEL <= LOGGING_DEBUG_HIGH_VALUE)
    fmt::print(fmt::fg(fmt::color::dark_violet), 
    FMT_COMPILE("{}: {}\n"), 
    std::this_thread::get_id(),
    fmt::format(makeBraces(sizeof...(Args)), std::forward<Args>(args)...));
#endif // LOGGING
}
*/
} // namespace logger

#endif // !_HIBISCUS_LOGGER_HPP_
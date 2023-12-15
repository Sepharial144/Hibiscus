#ifndef _HIBISCUS_LOGGER_HPP_
#define _HIBISCUS_LOGGER_HPP_

#include <thread>
#include <string>

#include <fmt/core.h>
#include <fmt/printf.h>
#include <fmt/compile.h>
#include <fmt/format.h>
#include <fmt/std.h>
#include <fmt/color.h>

namespace logger
{
	template <typename... Args>
	std::string makeString(fmt::format_string<Args...> s, Args&&... args)
	{
		return fmt::format(s, std::forward<Args>(args)...);
	}


	template <class... Args>
	void fatal(std::string&& format_string, Args&&... args)
	{
#if defined(LOGGING) && (LOGGING_LEVEL <= LOGGING_FATAL_VALUE)
		fmt::print(FMT_COMPILE("{}: {}\n"),
			std::this_thread::get_id(),
			makeString(format_string, std::forward<Args>(args)...),
			fmt::fg(fmt::color::red));
#endif // LOGGING
	}


	template <class... Args>
	void error(std::string&& format_string, Args&&... args)
	{
		#if defined(LOGGING) && (LOGGING_LEVEL <= LOGGING_ERROR_VALUE)
			fmt::print(//fmt::fg(fmt::color::orange_red),
				FMT_COMPILE("{}: {}\n"),
				std::this_thread::get_id(),
				makeString(format_string, std::forward<Args>(args)...),
				fmt::fg(fmt::color::orange_red));
		#endif // LOGGING
	}


	template <class... Args>
	void warning(std::string&& format_string, Args&&... args)
	{
		#if defined(LOGGING) && (LOGGING_LEVEL <= LOGGING_WARNING_VALUE)
			fmt::print(//fmt::fg(fmt::color::orange),
				FMT_COMPILE("{}: {}\n"),
				std::this_thread::get_id(),
				makeString(format_string, std::forward<Args>(args)...),
				fmt::fg(fmt::color::orange));
		#endif // LOGGING
	}


	template <class... Args>
	void info(std::string&& format_string, Args&&... args)
	{
		#if defined(LOGGING) && (LOGGING_LEVEL <= LOGGING_INFO_VALUE)
			fmt::print(FMT_COMPILE("{}: {}\n"),
				std::this_thread::get_id(),
				makeString(format_string, std::forward<Args>(args)...));
		#endif // LOGGING
	}


	template <class... Args>
	void debug(std::string&& format_string, Args&&... args)
	{
		#if defined(LOGGING) && (LOGGING_LEVEL <= LOGGING_DEBUG_VALUE)
			fmt::print(FMT_COMPILE("{}: {}\n"),
				std::this_thread::get_id(),
				makeString(format_string, std::forward<Args>(args)...));
		#endif // LOGGING
	}


	template <class... Args>
	void debug_low(std::string&& format_string, Args&&... args)
	{
		#if defined(LOGGING) && (LOGGING_LEVEL <= LOGGING_DEBUG_LOW_VALUE)
			fmt::print(FMT_COMPILE("{}: {}\n"),
				std::this_thread::get_id(),
				makeString(format_string, std::forward<Args>(args)...),
				fmt::fg(fmt::color::violet));
		#endif // LOGGING
	}


	template <class... Args>
	void debug_medium(std::string&& format_string, Args&&... args)
	{
		#if defined(LOGGING) && (LOGGING_LEVEL <= LOGGING_DEBUG_MEDIUM_VALUE)
			fmt::print(FMT_COMPILE("{}: {}\n"),
				std::this_thread::get_id(),
				makeString(format_string, std::forward<Args>(args)...),
				fmt::fg(fmt::color::dark_orchid));
		#endif // LOGGING
	}


	template <class... Args>
	void debug_high(std::string&& format_string, Args&&... args)
	{
		#if defined(LOGGING) && (LOGGING_LEVEL <= LOGGING_DEBUG_HIGH_VALUE)
			fmt::print(FMT_COMPILE("{}: {}\n"),
				std::this_thread::get_id(),
				makeString(format_string, std::forward<Args>(args)...),
				fmt::fg(fmt::color::dark_violet));
		#endif // LOGGING
	}
} // namespace logger

#endif // !_HIBISCUS_LOGGER_HPP_
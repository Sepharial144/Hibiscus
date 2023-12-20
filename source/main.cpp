#include <iostream>

#include "FileObserver.hpp"
#include "logger/logger.hpp"

int main(int argc, char* argv[])
{
    logger::fatal("Hello {} value: {}", "fatal error", LOGGING_FATAL_VALUE);
    logger::error("Hello {} value: {}", "error", LOGGING_ERROR_VALUE);
    logger::warning("Hello {} value: {}", "warning", LOGGING_WARNING_VALUE);
    logger::info("Hello {} value: {}", "info", LOGGING_INFO_VALUE);
    logger::debug("Hello {} value: {}", "debug", LOGGING_DEBUG_VALUE);
    logger::debug_low("Hello {} value: {}", "debug low", LOGGING_DEBUG_LOW_VALUE);
    logger::debug_medium("Hello {} value: {}", "debug medium", LOGGING_DEBUG_MEDIUM_VALUE);
    logger::debug_high("Hello {} value: {}", "debug high", LOGGING_DEBUG_HIGH_VALUE);
    // FileObserver FileObserver { "/workspaces/Hibiscus/data", 5000 };
    // FileObserver.start();
    return 0;
}
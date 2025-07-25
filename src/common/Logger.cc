#include "unpackers/common/Logger.hh"

using namespace unpackers;

//LOGGER

Logger::Logger(std::string logName, int threshold)
    : logName_(logName),
    threshold_(threshold),
    verbosity_(0) {}

void Logger::SetVerbosity(int verbosity) { verbosity_ = verbosity; }

//LOGGERHOLDER

void LoggerHolder::SetVerbosity(int verbosity) {
    verbosity_ = verbosity;
    this->InfoLogger.SetVerbosity(verbosity);
    this->DebugLogger.SetVerbosity(verbosity);
    this->WarningLogger.SetVerbosity(verbosity);
}


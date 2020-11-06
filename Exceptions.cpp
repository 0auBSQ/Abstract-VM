#include "Exceptions.hpp"

ParsingException::ParsingException(std::string msg = "") : std::runtime_error("[Parsing exception] : " + msg) {}

NoExitException::NoExitException(std::string msg = "") : std::runtime_error("[No exit exception] : " + msg) {}

RuntimeException::RuntimeException(std::string msg = "") : std::runtime_error("[Runtime exception] : " + msg) {}

MathException::MathException(std::string msg = "") : std::logic_error("[Math exception] : " + msg) {}

OverflowException::OverflowException(std::string msg = "") : std::overflow_error("[Overflow exception] : " + msg) {}

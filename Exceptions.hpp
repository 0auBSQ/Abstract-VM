#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <string>
#include <iostream>

class ParsingException : public std::runtime_error {
  public :
    ParsingException(std::string msg);
};

class NoExitException : public std::runtime_error {
  public :
    NoExitException(std::string msg);
};

class RuntimeException : public std::runtime_error {
  public :
    RuntimeException(std::string msg);
};

class MathException : public std::logic_error {
  public :
    MathException(std::string msg);
};

class OverflowException : public std::overflow_error {
  public :
    OverflowException(std::string msg);
};

#endif

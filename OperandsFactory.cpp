#include "Operands.hpp"

IOperand const * OperandsFactory::createOperand( eOperandType type, std::string const & value ) const {
  static IOperand const *(OperandsFactory::*dispatcher[])( std::string const & ) const = {
    &OperandsFactory::createInt8,
    &OperandsFactory::createInt16,
    &OperandsFactory::createInt32,
    &OperandsFactory::createFloat,
    &OperandsFactory::createDouble
  };
  std::istringstream  iss(value);
  long double         lhs = 0.0L;

  iss >> lhs;
  try {
    if (type == INT8 && (lhs < -128 || lhs > 127))
      throw OverflowException("Overflow on Int8");
    else if (type == INT16 && (lhs < -32768 || lhs > 32767))
      throw OverflowException("Overflow on Int16");
    else if (type == INT32 && (lhs < -2147483648 || lhs > 2147483647))
      throw OverflowException("Overflow on Int32");
    else if (type == FLOAT && (lhs < -std::numeric_limits<float>::max() || lhs > std::numeric_limits<float>::max()))
      throw OverflowException("Overflow on Float");
    else if (type == DOUBLE && (lhs < -std::numeric_limits<double>::max() || lhs > std::numeric_limits<double>::max()))
      throw OverflowException("Overflow on Double");
    else if (type < INT8 || type > DOUBLE)
      throw OverflowException("Unrecognized type");
  }
  catch (OverflowException &oe) {
    std::cout << oe.what() << std::endl;
  }

  if (type >= INT8 && type <= DOUBLE)
    return ((this->*dispatcher[static_cast<int>(type)])(value));
  return (NULL);
}

IOperand const * OperandsFactory::createInt8( std::string const & value ) const {
  int8_t  nb;

  try {
    if (sscanf(value.c_str(), "%hhd", &nb) != 1)
      throw ParsingException("Incorrect number");
  }
  catch (ParsingException &pe) {
    std::cout << pe.what() << std::endl;
    return (NULL);
  }
  return (new Operand<int8_t>(INT8, nb));
}

IOperand const * OperandsFactory::createInt16( std::string const & value ) const {
  int16_t  nb;

  try {
    if (sscanf(value.c_str(), "%hd", &nb) != 1)
      throw ParsingException("Incorrect number");
  }
  catch (ParsingException &pe) {
    std::cout << pe.what() << std::endl;
    return (NULL);
  }
  return (new Operand<int16_t>(INT16, nb));
}

IOperand const * OperandsFactory::createInt32( std::string const & value ) const {
  int32_t  nb;

  try {
    if (sscanf(value.c_str(), "%d", &nb) != 1)
      throw ParsingException("Incorrect number");
  }
  catch (ParsingException &pe) {
    std::cout << pe.what() << std::endl;
    return (NULL);
  }
  return (new Operand<int32_t>(INT32, nb));
}

IOperand const * OperandsFactory::createFloat( std::string const & value ) const {
  float  nb;

  try {
    if (sscanf(value.c_str(), "%f", &nb) != 1)
      throw ParsingException("Incorrect number");
  }
  catch (ParsingException &pe) {
    std::cout << pe.what() << std::endl;
    return (NULL);
  }
  return (new Operand<float>(FLOAT, nb));
}

IOperand const * OperandsFactory::createDouble( std::string const & value ) const {
  double  nb;

  try {
    if (sscanf(value.c_str(), "%lf", &nb) != 1)
      throw ParsingException("Incorrect number");
  }
  catch (ParsingException &pe) {
    std::cout << pe.what() << std::endl;
    return (NULL);
  }
  return (new Operand<double>(DOUBLE, nb));
}

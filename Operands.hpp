#ifndef OPERANDS_HPP
#define OPERANDS_HPP

#include <sstream>
#include <string>
#include <iostream>
#include <limits>
#include "IOperand.hpp"

#include "Exceptions.hpp"

enum eOperation {
  ADD,
  SUB,
  MUL,
  DIV,
  MOD
};

class OperandsFactory {
  public:
    IOperand const * createOperand( eOperandType type, std::string const & value ) const;
  private:
    IOperand const * createInt8( std::string const & value ) const;
    IOperand const * createInt16( std::string const & value ) const;
    IOperand const * createInt32( std::string const & value ) const;
    IOperand const * createFloat( std::string const & value ) const;
    IOperand const * createDouble( std::string const & value ) const;
};

static const OperandsFactory Factory;

template<typename T> class Operand : public IOperand {
  public:
    Operand<T>( void );
    Operand<T>( eOperandType const &type, T const &nb );
    Operand<T>( Operand<T> const &rhs );
    Operand<T> & operator=( Operand<T> const &rhs );
    virtual ~Operand<T>( void );

    int getPrecision( void ) const;
    eOperandType getType( void ) const;
    T getNb( void ) const;
    IOperand const * wrapper( IOperand const & rhs, eOperation const ope ) const;
    IOperand const * operator+( IOperand const & rhs ) const;
    IOperand const * operator-( IOperand const & rhs ) const;
    IOperand const * operator*( IOperand const & rhs ) const;
    IOperand const * operator/( IOperand const & rhs ) const;
    IOperand const * operator%( IOperand const & rhs ) const;
    std::string const & toString( void ) const;

  private:
    T             _nb;
    eOperandType  _type;
};

template<typename T> Operand<T>::Operand() : _nb(0), _type(UNDEFINED) {}

template<typename T> Operand<T>::Operand(eOperandType const &type, T const &nb) : _nb(nb), _type(type) {}

template<typename T> Operand<T>::Operand(Operand<T> const &rhs) {
  *this = rhs;
}

template<typename T> Operand<T>::~Operand() {}

template<typename T> Operand<T> &Operand<T>::operator=(Operand<T> const &rhs) {
  this->_nb = rhs.getNb();
  this->_type = rhs.getType();
  return (*this);
}

template<typename T> eOperandType Operand<T>::getType() const {
  return (this->_type);
}

template<typename T> T Operand<T>::getNb() const {
  return (this->_nb);
}

template<typename T> int Operand<T>::getPrecision() const {
  return (static_cast<int>(this->_type));
}

template<typename T> IOperand const *Operand<T>::wrapper(IOperand const & rhs, eOperation const ope) const {
  eOperandType        type = this->_type;
  long double         lhs;
  long double         rhs_val = 0.0L;
  std::istringstream  iss(rhs.toString());
  std::stringstream   ss;
  std::string         s;

  if (this->getPrecision() < rhs.getPrecision())
    type = rhs.getType();
  iss >> rhs_val;

  try {
    if ((ope == DIV || ope == MOD) && rhs_val == 0)
      throw MathException("Division or Modulo by 0");
    else if (type > INT32 && ope == MOD)
      throw MathException("Modulo can only be applied to integers");
    else {
      if (ope == ADD)
        lhs = this->getNb() + rhs_val;
      else if (ope == SUB)
        lhs = this->getNb() - rhs_val;
      else if (ope == MUL)
        lhs = this->getNb() * rhs_val;
      else if (ope == DIV)
        lhs = this->getNb() / rhs_val;
      else if (ope == MOD)
        lhs = static_cast<int32_t>(this->getNb()) % static_cast<int32_t>(rhs_val);
      else
        throw MathException("Unrecognized operation");
    }
  }
  catch (MathException &me) {
    std::cout << me.what() << std::endl;
    lhs = 0.0L;
  }

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
    lhs = 0.0L;
  }

  s = std::to_string(lhs);

  return (Factory.createOperand(type, s));

}

template<typename T> IOperand const *Operand<T>::operator+(IOperand const & rhs) const {
  return (wrapper(rhs, ADD));
}

template<typename T> IOperand const *Operand<T>::operator-(IOperand const & rhs) const {
  return (wrapper(rhs, SUB));
}

template<typename T> IOperand const *Operand<T>::operator*(IOperand const & rhs) const {
  return (wrapper(rhs, MUL));
}

template<typename T> IOperand const *Operand<T>::operator/(IOperand const & rhs) const {
  return (wrapper(rhs, DIV));
}

template<typename T> IOperand const *Operand<T>::operator%(IOperand const & rhs) const {
  return (wrapper(rhs, MOD));
}

template<typename T> std::string const &Operand<T>::toString() const {
  std::stringstream ss;
  static std::string s;

  if (this->_type != INT8)
    ss << this->_nb;
  else {
    ss << static_cast<int16_t>(this->_nb);
  }
  s.clear();
  ss >> s;
  return (s);
}

#endif

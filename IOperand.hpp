#ifndef IOPERAND_HPP
#define IOPERAND_HPP

#include <iostream>

enum eOperandType {
  UNDEFINED = -1,
  INT8 = 0,
  INT16 = 1,
  INT32 = 2,
  FLOAT = 3,
  DOUBLE = 4
};

class IOperand {
  public:
    virtual int getPrecision( void ) const = 0; // Precision of the type of the instance
    virtual eOperandType getType( void ) const = 0; // Type of the instance
    virtual IOperand const * operator+( IOperand const & rhs ) const = 0; // Sum
    virtual IOperand const * operator-( IOperand const & rhs ) const = 0; // Difference
    virtual IOperand const * operator*( IOperand const & rhs ) const = 0; // Product
    virtual IOperand const * operator/( IOperand const & rhs ) const = 0; // Quotient
    virtual IOperand const * operator%( IOperand const & rhs ) const = 0; // Modulo
    virtual std::string const & toString( void ) const = 0; // String representation of the instance
    virtual ~IOperand( void ) {}
};

#endif

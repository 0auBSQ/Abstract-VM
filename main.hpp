#ifndef MAIN_HPP
#define MAIN_HPP

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <stack>
#include <regex>
#include "Exceptions.hpp"
#include "IOperand.hpp"
#include "Operands.hpp"


extern const std::map<std::string, size_t>  instr;
extern std::stack<const IOperand *>         stack;
extern std::ifstream                        ifs;

#endif

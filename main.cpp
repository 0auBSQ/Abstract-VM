#include "main.hpp"

const std::map<std::string, size_t> instr = {
  {"push", 2},
  {"pop", 1},
  {"dump", 1},
  {"assert", 2},
  {"add", 1},
  {"sub", 1},
  {"mul", 1},
  {"div", 1},
  {"mod", 1},
  {"print", 1},
  {"exit", 1}
};

std::stack<const IOperand *>  stack;
std::ifstream                 ifs;

const IOperand *parse_operands(std::string &s) {
  static const std::regex  i8("^int8\\([-]?[0-9]+\\)$");
  static const std::regex  i16("^int16\\([-]?[0-9]+\\)$");
  static const std::regex  i32("^int32\\([-]?[0-9]+\\)$");
  static const std::regex  f("^float\\([-]?[0-9]+\\.[0-9]+\\)$");
  static const std::regex  d("^double\\([-]?[0-9]+\\.[0-9]+\\)$");
  int     op = s.find("(") + 1;
  int     ed = s.find(")");

  try {
    if (op < 0 || ed < 0)
      throw ParsingException("Invalid variable : " + s);
    if (std::regex_match(s, i8))
      return (Factory.createOperand(INT8, s.substr(op, ed - op)));
    else if (std::regex_match(s, i16))
      return (Factory.createOperand(INT16, s.substr(op, ed - op)));
    else if (std::regex_match(s, i32))
      return (Factory.createOperand(INT32, s.substr(op, ed - op)));
    else if (std::regex_match(s, f))
      return (Factory.createOperand(FLOAT, s.substr(op, ed - op)));
    else if (std::regex_match(s, d))
      return (Factory.createOperand(DOUBLE, s.substr(op, ed - op)));
    else
      throw ParsingException("Invalid variable : " + s);
  }
  catch (ParsingException &pe) {
    std::cout << pe.what() << std::endl;
  }
  return (NULL);
}

void exec_ope(eOperation ope) {
  try {
    if (stack.size() < 2)
      throw RuntimeException("Operation execution needs 2 values on stack");
  }
  catch (RuntimeException &re) {
    std::cout << re.what() << std::endl;
    return ;
  }

  const IOperand *rhs = stack.top();
  stack.pop();
  const IOperand *lhs = stack.top();
  stack.pop();
  const IOperand *result;

  try {
    if (ope == ADD)
      result = *lhs + *rhs;
    else if (ope == SUB)
      result = *lhs - *rhs;
    else if (ope == MUL)
      result = *lhs * *rhs;
    else if (ope == DIV)
      result = *lhs / *rhs;
    else if (ope == MOD)
      result = *lhs % *rhs;
    if (!result)
      throw RuntimeException("Operation execution failed");
    stack.push(result);
  }
  catch (RuntimeException &re) {
    std::cout << re.what() << std::endl;
  }
  delete rhs;
  delete lhs;
}

void assert(std::string &arg1) {
  const IOperand *op = parse_operands(arg1);

  if (!op)
    return ;
  try {
    if (stack.size() < 1)
      throw RuntimeException("Can't assert an empty stack");
    std::istringstream  iss1(stack.top()->toString());
    std::istringstream  iss2(op->toString());
    long double         ld1 = 0.0L;
    long double         ld2 = 0.0L;
    iss1 >> ld1;
    iss2 >> ld2;
    if (stack.top()->getType() != op->getType())
      throw RuntimeException("assert call returned false : Different type");
    if (ld1 != ld2)
      throw RuntimeException("assert call returned false : Different value");
  }
  catch (RuntimeException &re) {
    std::cout << re.what() << std::endl;
  }
  delete op;
}

void push(std::string &arg1) {
  const IOperand *op = parse_operands(arg1);

  if (op) {
    stack.push(op);
  }
}

void pop() {
  try {
    if (stack.size() < 1)
      throw RuntimeException("Can't pop an empty stack");
    delete stack.top();
    stack.pop();
  }
  catch (RuntimeException &re) {
    std::cout << re.what() << std::endl;
  }
}

void print() {
  try {
    if (stack.size() < 1)
      throw RuntimeException("Can't print an empty stack");
    else if (stack.top()->getType() != INT8)
      throw RuntimeException("Stack top isn't an INT8 value");
    std::cout << static_cast<char>(atoi(stack.top()->toString().c_str()));
  }
  catch (RuntimeException &re) {
    std::cout << re.what() << std::endl;
  }
}

void dump() {
  for (std::stack<const IOperand *> dump = stack; !dump.empty(); dump.pop()) {
    std::cout << dump.top()->toString() << std::endl;
  }
}

void quit() {
  for (; !stack.empty(); stack.pop()) {
    delete stack.top();
  }
  ifs.close();
  exit(0);
}

size_t string_split(std::string &s, char delim, std::vector<std::string> &arr) {
  size_t  start;
  size_t  end = 0;
  size_t  nb_words = 0;

  arr.clear();
  while ((start = s.find_first_not_of(delim, end)) != std::string::npos)
  {
    end = s.find(delim, start);
    arr.push_back(s.substr(start, end - start));
    nb_words++;
  }
  return (nb_words);
}

void parse_tokens(std::string &s) {
  std::vector<std::string>     tokens;
  size_t                       nb_words;

  nb_words = string_split(s, ' ', tokens);
  if (nb_words > 0) {
    if (!instr.count(tokens[0]) || instr.find(tokens[0])->second != nb_words) {
      try {
        if (!instr.count(tokens[0])) {
          throw ParsingException("Invalid token : " + tokens[0]);
        }
        else {
          throw ParsingException("Invalid arg count : " + tokens[0]);
        }
      }
      catch (ParsingException &pe) {
        std::cout << pe.what() << std::endl;
      }
    }
    else {
      if (tokens[0] == "exit")
        quit();
      else if (tokens[0] == "pop")
        pop();
      else if (tokens[0] == "print")
        print();
      else if (tokens[0] == "dump")
        dump();
      else if (tokens[0] == "push")
        push(tokens[1]);
      else if (tokens[0] == "assert")
        assert(tokens[1]);
      else if (tokens[0] == "add")
        exec_ope(ADD);
      else if (tokens[0] == "sub")
        exec_ope(SUB);
      else if (tokens[0] == "mul")
        exec_ope(MUL);
      else if (tokens[0] == "div")
        exec_ope(DIV);
      else if (tokens[0] == "mod")
        exec_ope(MOD);
    }
  }
}

int main (int ac, char **av) {
  std::string             line;
  std::string::size_type  comment;

  if (ac < 2) {
    while (std::getline(std::cin, line)) {
      if (line == ";;")
        break ;
      comment = line.find(';');
      if (comment != std::string::npos)
        line = line.substr(0, comment);
      parse_tokens(line);
    }
  }
  else {
    ifs.open(av[1], std::ifstream::in);
    if (ifs.fail()) {
      std::cerr << "Notice: " << std::strerror(errno) << std::endl;
      exit(0);
    }

    while (std::getline(ifs, line)) {
      comment = line.find(';');
      if (comment != std::string::npos)
        line = line.substr(0, comment);
      parse_tokens(line);
    }
  }
  try {
    throw NoExitException("No exit instruction found");
  }
  catch (NoExitException &nee) {
    std::cout << nee.what() << std::endl;
  }
  quit();
}

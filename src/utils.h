#include <iostream>
#include <cmath>
#include <stack>
#include <string>
#include <sstream>
#include <cctype>
#include <map>

class ExpressionParser {
private:
  static std::map<std::string, int> precedence;

  float evaluatePostfix(const std::string& postfix, float x, float y) const {
    std::stack<float> stack;
    std::istringstream iss(postfix);
    std::string token;

    while (iss >> token) {
      if (token == "x") stack.push(x);
      else if (token == "y") stack.push(y);
      else if (token == "e") stack.push(2.71828f);
      else if (isdigit(token[0]) || (token[0] == '-' && token.length() > 1)) {
        stack.push(std::stof(token));
      }
      else {
        float b = stack.top(); stack.pop();
        if (token == "sin") stack.push(sin(b));
        else if (token == "cos") stack.push(cos(b));
        else if (token == "tan") stack.push(tan(b));
        else if (token == "cot") stack.push(1.0f/tan(b));
        else if (token == "ln") stack.push(log(b));
        else if (token == "sqrt") stack.push(sqrt(b));
        else if (token == "abs") stack.push(fabs(b));
        else if (token == "e^") stack.push(exp(b));
        else {
          float a = stack.top(); stack.pop();
          if (token == "+") stack.push(a + b);
          else if (token == "-") stack.push(a - b);
          else if (token == "*") stack.push(a * b);
          else if (token == "/") stack.push(a / b);
          else if (token == "^") stack.push(pow(a, b));
        }
      }
    }
    return stack.top();
  }

  std::string tokenize(const std::string& input) const {
    std::string result;
    for (size_t i = 0; i < input.length(); i++) {
      if (input[i] == ' ') continue;
      
      if (isdigit(input[i]) || (input[i] == '-' && i+1 < input.length() && isdigit(input[i+1]))) {
        while (i < input.length() && (isdigit(input[i]) || input[i] == '.' || (input[i] == '-' && result.empty()))) {
          result += input[i++];
        }
        result += ' ';
        i--;
        continue;
      }
      
      if (input[i] == '+' || input[i] == '-' || input[i] == '*' || input[i] == '/' || 
          input[i] == '^' || input[i] == '(' || input[i] == ')') {
        result += input[i];
        result += ' ';
        continue;
      }
      
      if (isalpha(input[i])) {
        std::string temp;
        while (i < input.length() && isalpha(input[i])) {
          temp += input[i++];
        }
        i--;
        result += temp + ' ';
      }
    }
    return result;
  }

  std::string infixToPostfix(const std::string& infix) const {
    std::string postfix;
    std::stack<std::string> ops;
    std::string token;
    std::istringstream iss(tokenize(infix));

    while (iss >> token) {
      if (token == "(" || token == "sin" || token == "cos" || token == "tan" || 
          token == "cot" || token == "ln" || token == "e^") {
        ops.push(token);
      }
      else if (token == ")") {
        while (!ops.empty() && ops.top() != "(") {
          postfix += ops.top() + " ";
          ops.pop();
        }
        if (!ops.empty()) ops.pop();
      }
      else if (precedence.find(token) != precedence.end()) {
        while (!ops.empty() && ops.top() != "(" && precedence[ops.top()] >= precedence[token]) {
          postfix += ops.top() + " ";
          ops.pop();
        }
        ops.push(token);
      }
      else {
        postfix += token + " ";
      }
    }
    
    while (!ops.empty()) {
      postfix += ops.top() + " ";
      ops.pop();
    }
    return postfix;
  }

public:
  float Evaluate(const std::string& expr, float x, float y) const {
    std::string postfix = infixToPostfix(expr);
    return evaluatePostfix(postfix, x, y);
  }
};

std::map<std::string, int> ExpressionParser::precedence = {
    {"^", 3},
    {"*", 2},
    {"/", 2},
    {"+", 1},
    {"-", 1}
};

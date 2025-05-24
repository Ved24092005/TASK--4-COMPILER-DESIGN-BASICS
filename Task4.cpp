#include <iostream>
#include <string>
#include <cctype>
#include <stdexcept>

using namespace std;

class Parser {
    string expr;
    size_t pos;

public:
    Parser(const string& s) : expr(s), pos(0) {}

    // Helper: skip whitespace
    void skipWhitespace() {
        while (pos < expr.size() && isspace(expr[pos])) pos++;
    }

    // Parse factor: number or parenthesis expression
    int parseFactor() {
        skipWhitespace();
        int result = 0;

        if (pos < expr.size() && expr[pos] == '(') {
            pos++; // consume '('
            result = parseExpression();
            skipWhitespace();
            if (pos >= expr.size() || expr[pos] != ')') {
                throw runtime_error("Expected ')'");
            }
            pos++; // consume ')'
        } else {
            // parse integer number
            if (pos >= expr.size() || !isdigit(expr[pos])) {
                throw runtime_error("Expected number or '('");
            }
            while (pos < expr.size() && isdigit(expr[pos])) {
                result = result * 10 + (expr[pos] - '0');
                pos++;
            }
        }
        return result;
    }

    // Parse term: factor [* or / factor]*
    int parseTerm() {
        int result = parseFactor();
        while (true) {
            skipWhitespace();
            if (pos < expr.size() && (expr[pos] == '*' || expr[pos] == '/')) {
                char op = expr[pos++];
                int rhs = parseFactor();
                if (op == '*')
                    result *= rhs;
                else {
                    if (rhs == 0) throw runtime_error("Division by zero");
                    result /= rhs;
                }
            } else {
                break;
            }
        }
        return result;
    }

    // Parse expression: term [+ or - term]*
    int parseExpression() {
        int result = parseTerm();
        while (true) {
            skipWhitespace();
            if (pos < expr.size() && (expr[pos] == '+' || expr[pos] == '-')) {
                char op = expr[pos++];
                int rhs = parseTerm();
                if (op == '+')
                    result += rhs;
                else
                    result -= rhs;
            } else {
                break;
            }
        }
        return result;
    }

    // Parse and evaluate the whole expression
    int parse() {
        int val = parseExpression();
        skipWhitespace();
        if (pos != expr.size()) {
            throw runtime_error("Unexpected characters at end");
        }
        return val;
    }
};

int main() {
    cout << "Enter an arithmetic expression to evaluate:\n";
    string input;
    getline(cin, input);

    try {
        Parser parser(input);
        int result = parser.parse();
        cout << "Result = " << result << "\n";
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}

#include <algorithm>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <queue>
#include <regex>
#include <sstream>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

// Precedence of operators
#define PRECEDENCE(x) \
    ((x) == "+" || (x) == "-" ? 0 : ((x) == "*" || (x) == "/" || (x) == "%" ? 1 : ((x) == "^" ? 2 : -1)))

int line_num;

enum op_code
{
    PRT,    // Prints the tokens
    SCN,    // Takes inputs upto the next space
    SCNL,   // Takes inputs upto the next newline
    MOV,    // Assigns a value to a variable
    INT,    // Converts a variable to an integer
    CHAR,   // Converts an integer to its ASCII equivalent
    ADD,    // Adds two variables
    SUB,    // Subtracts two variables
    MUL,    // Multiplies two variables
    DIV,    // Divides two variables
    MOD,    // Modulo of two variables
    POW,    // Raises one variable to the power of another
    EXPR,   // Evaluates an expression
    AND,    // Logical AND of two variables
    OR,     // Logical OR of two variables
    NOT,    // Logical NOT of a variable
    BAND,   // Bitwise AND of two variables
    BOR,    // Bitwise OR of two variables
    BNOT,   // Bitwise NOT of a variable
    BXOR,   // Bitwise XOR of two variables
    LSHIFT, // Bitwise left shift of a variable
    RSHIFT, // Bitwise right shift of a variable
    IF,     // If condition is true, executes the block
    JMP,    // Jumps to the specified line
    ARR,    // Stores a value in an array
    ARRI,   // Initializes an array
    ARRV,   // Retrieves a value from an array
    BYE,    // Exits the program
    STR,    // Stores strings as char arrays
    CAT,    // Concatenates strings
    CALL,   // Calls a module
    RECV,   // Receives values from a module
    RET,    // Returns from a module
    FCALL,  // Calls a function
    FRET,   // Returns from a function
    FREAD,  // Reads a file
    FWRITE, // Writes to a file
    FDEL    // Deletes a file
};

enum log_code
{
    EQL,  // == (equal to)
    NEQ,  // != (not equal to)
    LSS,  // < (less than)
    LEQ,  // <= (less than or equal to)
    GTR,  // > (greater than)
    GEQ,  // >= (greater than or equal to)
    SEQL, // == for strings
    SNEQ  // != for strings
};

template <typename T1, typename T2>
void update_map(unordered_map<T1, T2> &map, T1 key, T2 value)
{
    auto itr = map.find(key);

    if (itr != map.end())
        itr->second = value;
    else
        map[key] = value;
}

template <typename T>
void reverse_stack(stack<T> &s)
{
    stack<T> temp;

    while (!s.empty())
    {
        T x = s.top();
        s.pop();
        temp.push(x);
    }

    s = temp;
}

double modulo(double a, double b)
{
    return a - b * floor(a / b);
}

bool is_operator(char c)
{
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^';
}

double expr_eval(string infix)
{
    string token;

    infix += " ";

    bool is_num = false;
    bool dot_found = false;

    string s = "";
    string exp = "";

    smatch match;
    regex re("\\([ ]*([\\+|-]?[\\d.]+)[ ]*?\\)");
    regex_search(infix, match, re);
    infix = regex_replace(infix, re, match[1].str());

    int j = 0;
    int len = infix.length();

    for (int i = 0; i < len; i++)
    {
        char c = infix.at(i);

        if (c != '(' && c != ')' && c != ' ')
        {
            if (c == '+' || c == '-')
            {
                if (j % 2 == 0)
                {
                    is_num = true;
                    s += c;

                    if (is_operator(infix.at(i + 1)) || infix.at(i + 1) == ' ')
                    {
                        cerr << "Error: Line " << line_num + 1 << ": EXPR: Invalid expression!\n";
                        exit(1);
                    }
                }
                else
                {
                    if (s != "")
                    {
                        exp += s + " ";
                        s = "";

                        is_num = false;
                        dot_found = false;
                    }

                    exp += c;
                    exp += " ";
                    j++;
                }
            }
            else if (c == '*' || c == '/' || c == '%' || c == '^')
            {
                if (j % 2 == 0)
                {
                    cerr << "Error: Line " << line_num + 1 << ": EXPR: Invalid expression!\n";
                    exit(1);
                }

                if (s != "")
                {
                    exp += s + " ";
                    s = "";

                    is_num = false;
                    dot_found = false;
                }

                exp += c;
                exp += " ";
                j++;
            }
            else if (c == '.')
            {
                if (j % 2 == 1 || dot_found || is_operator(infix.at(i + 1)) || infix.at(i + 1) == ' ')
                {
                    cerr << "Error: Line " << line_num + 1 << ": EXPR: Invalid expression!\n";
                    exit(1);
                }

                s += c;

                is_num = true;
                dot_found = true;
            }
            else
            {
                if (j % 2 == 1)
                {
                    cerr << "Error: Line " << line_num + 1 << ": EXPR: Invalid expression!\n";
                    exit(1);
                }

                is_num = true;

                s += c;

                if (is_operator(infix.at(i + 1)))
                {
                    j++;

                    is_num = false;
                    dot_found = false;

                    continue;
                }
            }
        }
        else
        {
            if (s != "")
            {
                exp += s + " ";
                s = "";
                j++;

                is_num = false;
                dot_found = false;
            }

            if (c != ' ')
            {
                exp += c;
                exp += " ";
            }
        }
    }

    if (s != "")
        exp += s;

    infix = exp;

    stringstream ss;
    istringstream iss(infix);

    copy(istream_iterator<string>(iss), istream_iterator<string>(), ostream_iterator<string>(ss, " "));

    int parentheses_count = 0;
    int operator_count = 0;
    int operand_count = 0;
    int i = 0;

    while (getline(ss, token, ' '))
    {
        if (token == "(")
        {
            parentheses_count++;
        }
        else if (token == ")")
        {
            parentheses_count--;
        }
        else if (token == "+" || token == "-" || token == "*" || token == "/" || token == "%" || token == "^")
        {
            if (i % 2 == 1)
            {
                operator_count++;
            }
            else
            {
                cerr << "Error: Line " << line_num + 1 << ": EXPR: Invalid expression!\n";
                exit(1);
            }

            i++;
        }
        else
        {
            if (i % 2 == 0)
            {
                stringstream ss_num(token);
                double num;
                ss_num >> num;

                if (ss_num.fail())
                {
                    cerr << "Error: Line " << line_num + 1 << ": EXPR: Invalid expression!\n";
                    exit(1);
                }

                operand_count++;
            }
            else
            {
                cerr << "Error: Line " << line_num + 1 << ": EXPR: Invalid expression!\n";
                exit(1);
            }

            i++;
        }

        if (parentheses_count < 0)
        {
            cerr << "Error: Line " << line_num + 1 << ": EXPR: Invalid expression!\n";
            exit(1);
        }
    }

    if (parentheses_count != 0)
    {
        cerr << "Error: Line " << line_num + 1 << ": EXPR: Invalid expression!\n";
        exit(1);
    }

    if (operator_count != operand_count - 1)
    {
        cerr << "Error: Line " << line_num + 1 << ": EXPR: Invalid expression!\n";
        exit(1);
    }

    stack<string> stk;
    stack<string> postfix;

    ss.clear();
    ss.seekg(0);

    while (getline(ss, token, ' '))
    {
        if (token == "+" || token == "-" || token == "*" || token == "/" || token == "%" || token == "^")
        {
            while (!stk.empty() && PRECEDENCE(stk.top()) >= PRECEDENCE(token))
            {
                postfix.push(stk.top());
                stk.pop();
            }

            stk.push(token);
        }
        else if (token == "(")
        {
            stk.push(token);
        }
        else if (token == ")")
        {
            bool found_element = false;

            while (stk.top() != "(")
            {
                postfix.push(stk.top());
                stk.pop();
                found_element = true;
            }

            if (found_element)
                stk.pop();
            else
            {
                cerr << "Error: Line " << line_num + 1 << ": EXPR: Invalid expression!\n";
                exit(1);
            }
        }
        else
        {
            postfix.push(token);
        }
    }

    while (!stk.empty())
    {
        postfix.push(stk.top());
        stk.pop();
    }

    reverse_stack(postfix);

    stack<double> eval;

    while (!postfix.empty())
    {
        token = postfix.top();
        postfix.pop();

        if (token == "+" || token == "-" || token == "*" || token == "/" || token == "%" || token == "^")
        {
            double y = eval.top();
            eval.pop();
            double x = eval.top();
            eval.pop();

            if (token == "+")
                eval.push(x + y);
            else if (token == "-")
                eval.push(x - y);
            else if (token == "*")
                eval.push(x * y);
            else if (token == "/")
                eval.push(x / y);
            else if (token == "%")
                eval.push(modulo(x, y));
            else if (token == "^")
                eval.push(pow(x, y));
        }
        else
        {
            eval.push(stod(token));
        }
    }

    return eval.top();
}

void create_pairs(vector<string> lines, unordered_map<int, int> &pairs)
{
    stack<int> stk;

    for (int i = 0; i < lines.size(); i++)
    {
        string line = lines[i];

        if (line.find("[") != string::npos)
        {
            stk.push(i + 1);
        }
        else if (line.find("]") != string::npos)
        {
            if (stk.empty())
            {
                cerr << "Error: Unmatched ]\n";
                exit(1);
            }

            pairs[stk.top()] = i + 1;
            pairs[i + 1] = stk.top();

            stk.pop();
        }
    }

    if (!stk.empty())
    {
        cerr << "Error: Unmatched [\n";
        exit(1);
    }
}

void create_labels(vector<string> lines, unordered_map<string, string> &memory)
{
    for (int i = 0; i < lines.size(); i++)
    {
        string line = lines[i];

        stringstream ss;
        istringstream iss(line);

        copy(istream_iterator<string>(iss), istream_iterator<string>(), ostream_iterator<string>(ss, " "));

        int j = 0;

        op_code op;
        string token;

        while (getline(ss, token, ' '))
        {
            if (j == 0)
            {
                if (token != ">")
                    break;
            }
            else
            {
                update_map(memory, token, to_string(i + 1));
                break;
            }

            j++;
        }
    }
}

void define_variables(unordered_map<string, string> &memory)
{
    memory["$ENDL"] = "\n";
    memory["$TAB"] = "\t";
    memory["$SPC"] = " ";
    memory["$EXL"] = "!";
    memory["$DOL"] = "$";
    memory["$AMP"] = "&";
    memory["$LBR"] = "[";
    memory["$RBR"] = "]";
}

string eval(string str, unordered_map<string, string> memory)
{
    if (str.at(0) == '$')
        str = memory[str];
    else if (str.at(0) == '&')
        str = memory["$" + memory["$" + str.substr(1)]];

    return str;
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        cerr << "Usage: " << argv[0] << " <file> [optional: args]\n";
        exit(1);
    }

    ifstream file;
    file.open(argv[1], ios::binary);

    if (!file.is_open())
    {
        cerr << argv[0] << ": error: " << argv[1] << ": No such file\n";
        exit(1);
    }

    // Read the file into a vector of strings
    vector<string> lines;

    for (string line; getline(file, line);)
    {
        line = regex_replace(line, regex("!.*"), ""); // Remove comments
        lines.push_back(line);
    }

    file.close();

    string dir(argv[1]);

    if (dir.find("/") != string::npos || dir.find("\\ ") != string::npos)
    {
        dir = dir.substr(0, dir.find_last_of("/") + 1); // GNU/Linux
    }
    else if (dir.find("\\") != string::npos)
    {
        dir = dir.substr(0, dir.find_last_of("\\") + 1); // Windows
    }
    else
    {
        dir = ""; // No directory
    }

    unordered_map<string, vector<string>> lines_map;
    unordered_map<string, unordered_map<int, int>> pairs_map;
    stack<string> mod_name_stk;
    stack<int> line_num_stk;
    stack<unordered_map<string, string>> memory_stk;
    stack<int> func_line_num_stk;
    stack<stack<int>> func_line_num_stk_stk;

    unordered_map<string, string> memory;

    memory["$ARGC"] = to_string(argc - 1);

    for (int i = 0; i < argc - 1; i++)
        memory["$" + to_string(i)] = argv[i + 1];

    define_variables(memory);

    line_num = 0;
    string mod_name(argv[1]);
    queue<string> ret_vals;
    unordered_map<int, int> pairs;

    create_pairs(lines, pairs);
    create_labels(lines, memory);

    while (true)
    {
    outer:
        while (line_num < lines.size())
        {
            string line = lines[line_num];

            stringstream ss;
            istringstream iss(line);

            copy(istream_iterator<string>(iss), istream_iterator<string>(), ostream_iterator<string>(ss, " "));

            int i = 0;
            string token;
            op_code op;
            log_code log;
            string arr_name;
            string index;
            vector<string> mod_lines;
            queue<string> mod_args;

            string key, value1, value2;
            string string_sum;
            string exp;

            int file_error = 0;

            while (getline(ss, token, ' '))
            {
                if (i == 0)
                {
                    if (token == "PRT")
                        op = PRT;
                    else if (token == "SCN")
                        op = SCN;
                    else if (token == "SCNL")
                        op = SCNL;
                    else if (token == "MOV")
                        op = MOV;
                    else if (token == "INT")
                        op = INT;
                    else if (token == "CHAR")
                        op = CHAR;
                    else if (token == "ADD")
                        op = ADD;
                    else if (token == "SUB")
                        op = SUB;
                    else if (token == "MUL")
                        op = MUL;
                    else if (token == "DIV")
                        op = DIV;
                    else if (token == "MOD")
                        op = MOD;
                    else if (token == "POW")
                        op = POW;
                    else if (token == "EXPR")
                        op = EXPR;
                    else if (token == "AND")
                        op = AND;
                    else if (token == "OR")
                        op = OR;
                    else if (token == "NOT")
                        op = NOT;
                    else if (token == "BAND")
                        op = BAND;
                    else if (token == "BOR")
                        op = BOR;
                    else if (token == "BNOT")
                        op = BNOT;
                    else if (token == "BXOR")
                        op = BXOR;
                    else if (token == "LSHIFT")
                        op = LSHIFT;
                    else if (token == "RSHIFT")
                        op = RSHIFT;
                    else if (token == "IF")
                        op = IF;
                    else if (token == "JMP")
                        op = JMP;
                    else if (token == "ARR")
                        op = ARR;
                    else if (token == "ARRI")
                        op = ARRI;
                    else if (token == "ARRV")
                        op = ARRV;
                    else if (token == "STR")
                        op = STR;
                    else if (token == "CAT")
                        op = CAT;
                    else if (token == "BYE")
                        op = BYE;
                    else if (token == "CALL")
                        op = CALL;
                    else if (token == "RECV")
                        op = RECV;
                    else if (token == "RET")
                        op = RET;
                    else if (token == "FCALL")
                        op = FCALL;
                    else if (token == "FRET")
                        op = FRET;
                    else if (token == "FREAD")
                        op = FREAD;
                    else if (token == "FWRITE")
                        op = FWRITE;
                    else if (token == "FDEL")
                        op = FDEL;
                    else if (token == ">")
                        break;
                    else if (token == "[" || token == "]")
                        break;
                    else
                    {
                        cerr << "Error: Line " << line_num + 1 << ": " << token << " is not a valid op code\n";
                        exit(1);
                    }
                }
                else
                {
                    if (op == PRT)
                    {
                        if (i != 1)
                            cout << " ";

                        cout << eval(token, memory);
                    }
                    else if (op == SCN)
                    {
                        string input;
                        cin >> input;

                        update_map(memory, token, input);
                    }
                    else if (op == SCNL)
                    {
                        string input;
                        getline(cin, input);

                        update_map(memory, token, input);
                    }
                    else if (op == MOV)
                    {
                        if (i == 1)
                        {
                            key = token;
                        }
                        else
                        {
                            update_map(memory, key, eval(token, memory));
                            break;
                        }
                    }
                    else if (op == INT)
                    {
                        if (i == 1)
                        {
                            key = token;
                        }
                        else
                        {
                            try
                            {
                                update_map(memory, key, to_string(stoi(eval(token, memory))));
                            }
                            catch (const invalid_argument &ia)
                            {
                                cerr << "Error: Line " << line_num + 1 << ": "
                                     << "Cast to int failed\n";
                                exit(1);
                            }

                            break;
                        }
                    }
                    else if (op == CHAR)
                    {
                        if (i == 1)
                        {
                            key = token;
                        }
                        else
                        {
                            try
                            {
                                update_map(memory, key, string(1, (char)stoi(eval(token, memory))));
                            }
                            catch (const invalid_argument &ia)
                            {
                                cerr << "Error: Line " << line_num + 1 << ": "
                                     << "Cast to char failed\n";
                                exit(1);
                            }

                            break;
                        }
                    }
                    else if (op == ADD || op == SUB || op == MUL || op == DIV || op == MOD || op == POW || op == AND ||
                             op == OR || op == BAND || op == BOR || op == BXOR || op == LSHIFT || op == RSHIFT)
                    {
                        if (i == 1)
                        {
                            key = token;
                        }
                        else if (i == 2)
                        {
                            value1 = eval(token, memory);
                        }
                        else if (i == 3)
                        {
                            value2 = eval(token, memory);

                            double result;

                            try
                            {
                                if (op == ADD)
                                    result = stod(value1) + stod(value2);
                                else if (op == SUB)
                                    result = stod(value1) - stod(value2);
                                else if (op == MUL)
                                    result = stod(value1) * stod(value2);
                                else if (op == DIV)
                                    result = stod(value1) / stod(value2);
                                else if (op == MOD)
                                    result = modulo(stod(value1), stod(value2));
                                else if (op == POW)
                                    result = pow(stod(value1), stod(value2));
                                else if (op == AND)
                                    result = stod(value1) && stod(value2);
                                else if (op == OR)
                                    result = stod(value1) || stod(value2);
                                else if (op == BAND)
                                    result = stoi(value1) & stoi(value2);
                                else if (op == BOR)
                                    result = stoi(value1) | stoi(value2);
                                else if (op == BXOR)
                                    result = stoi(value1) ^ stoi(value2);
                                else if (op == LSHIFT)
                                    result = stoi(value1) << stoi(value2);
                                else if (op == RSHIFT)
                                    result = stoi(value1) >> stoi(value2);
                            }
                            catch (const invalid_argument &ia)
                            {
                                cerr << "Type Error: Line " << line_num + 1
                                     << ": We only deal with integers and floats\n";
                                exit(1);
                            }

                            update_map(memory, key, to_string(result));
                            break;
                        }
                    }
                    else if (op == NOT || op == BNOT)
                    {
                        if (i == 1)
                        {
                            key = token;
                        }
                        else
                        {
                            double result;

                            try
                            {
                                if (op == NOT)
                                    result = !stod(eval(token, memory));
                                else if (op == BNOT)
                                    result = ~stoi(eval(token, memory));
                            }
                            catch (const invalid_argument &ia)
                            {
                                cerr << "Type Error: Line " << line_num + 1
                                     << ": We only deal with integers and floats\n";
                                exit(1);
                            }

                            update_map(memory, key, to_string(result));
                            break;
                        }
                    }
                    else if (op == EXPR)
                    {
                        if (i == 1)
                        {
                            key = token;
                            exp = "";
                        }
                        else
                        {
                            exp += eval(token, memory) + " ";
                        }
                    }
                    else if (op == IF)
                    {
                        if (i == 1)
                        {
                            if (token == "EQL")
                                log = EQL;
                            else if (token == "NEQ")
                                log = NEQ;
                            else if (token == "LSS")
                                log = LSS;
                            else if (token == "LEQ")
                                log = LEQ;
                            else if (token == "GTR")
                                log = GTR;
                            else if (token == "GEQ")
                                log = GEQ;
                            else if (token == "SEQL")
                                log = SEQL;
                            else if (token == "SNEQ")
                                log = SNEQ;
                            else
                            {
                                cerr << "Error: Line " << line_num + 1 << ": " << token
                                     << " is not a valid logical operator\n";
                                exit(1);
                            }
                        }
                        else if (i == 2)
                        {
                            value1 = eval(token, memory);
                        }
                        else if (i == 3)
                        {
                            value2 = eval(token, memory);

                            bool condition;

                            try
                            {
                                condition = (log == EQL && !(stod(value1) == stod(value2))) ||
                                            (log == NEQ && !(stod(value1) != stod(value2))) ||
                                            (log == LSS && !(stod(value1) < stod(value2))) ||
                                            (log == LEQ && !(stod(value1) <= stod(value2))) ||
                                            (log == GTR && !(stod(value1) > stod(value2))) ||
                                            (log == GEQ && !(stod(value1) >= stod(value2))) ||
                                            (log == SEQL && !(value1 == value2)) ||
                                            (log == SNEQ && !(value1 != value2));
                            }
                            catch (const invalid_argument &ia)
                            {
                                cerr << "Type Error: Line " << line_num + 1 << ": We only deal with doubles!\n";
                                exit(1);
                            }

                            if (condition)
                            {
                                line_num = pairs[(line_num + 1) + 1] - 1;
                                goto outer;
                            }

                            break;
                        }
                    }
                    else if (op == JMP)
                    {
                        try
                        {
                            line_num = stoi(eval(token, memory)) - 1;
                        }
                        catch (const invalid_argument &ia)
                        {
                            cerr << "Index Error: Line " << line_num + 1 << ": Invalid jump index!\n";
                            exit(1);
                        }

                        goto outer;
                    }
                    else if (op == ARR)
                    {
                        if (i == 1)
                        {
                            arr_name = token;
                        }
                        else if (i == 2)
                        {
                            try
                            {
                                index = to_string(stoi(eval(token, memory)));
                            }
                            catch (const invalid_argument &ia)
                            {
                                cerr << "Type Error: Line " << line_num + 1 << ": Index must be an integer!\n";
                                exit(1);
                            }
                        }
                        else if (i == 3)
                        {
                            update_map(memory, arr_name + "(" + index + ")", eval(token, memory));
                            break;
                        }
                    }
                    else if (op == ARRI)
                    {
                        if (i == 1)
                            arr_name = token;
                        else
                            update_map(memory, arr_name + "(" + to_string(i - 2) + ")", eval(token, memory));
                    }
                    else if (op == ARRV)
                    {
                        if (i == 1)
                        {
                            key = token;
                        }
                        else if (i == 2)
                        {
                            arr_name = token;
                        }
                        else if (i == 3)
                        {
                            try
                            {
                                index = to_string(stoi(eval(token, memory)));
                            }
                            catch (const invalid_argument &ia)
                            {
                                cerr << "Type Error: Line " << line_num + 1 << ": Index must be an integer!\n";
                                exit(1);
                            }

                            update_map(memory, key, memory[arr_name + "(" + index + ")"]);
                            break;
                        }
                    }
                    else if (op == STR)
                    {
                        string str = eval(token, memory);

                        for (int i = 0; i < str.length(); i++)
                            update_map(memory, token + "(" + to_string(i) + ")", string(1, str.at(i)));

                        update_map(memory, token + "(" + to_string(str.length()) + ")", string("NULL"));
                    }
                    else if (op == CAT)
                    {
                        if (i == 1)
                        {
                            key = token;
                            string_sum = "";
                        }
                        else
                        {
                            string_sum += eval(token, memory);
                        }
                    }
                    else if (op == CALL)
                    {
                        if (i == 1)
                        {
                            string str = dir + eval(token, memory) + ".fasml";

                            ifstream file;
                            file.open(str, ios_base::binary);

                            if (!file.is_open())
                            {
                                cerr << argv[0] << ": error: " << str << ": No such file\n";
                                exit(1);
                            }

                            mod_name_stk.push(mod_name);

                            if (lines_map.find(mod_name) == lines_map.end())
                                lines_map[mod_name] = lines;

                            if (pairs_map.find(mod_name) == pairs_map.end())
                                pairs_map[mod_name] = pairs;

                            line_num_stk.push(line_num);
                            memory_stk.push(memory);

                            mod_name = str;
                            mod_args.push(mod_name);

                            if (lines_map.find(mod_name) == lines_map.end())
                            {
                                for (string line; getline(file, line);)
                                {
                                    line = regex_replace(line, regex("!.*"), "");
                                    mod_lines.push_back(line);
                                }
                            }
                            else
                            {
                                mod_lines = lines_map[mod_name];
                            }

                            func_line_num_stk_stk.push(func_line_num_stk);

                            file.close();
                        }
                        else
                        {
                            mod_args.push(eval(token, memory));
                        }
                    }
                    else if (op == RECV)
                    {
                        if (ret_vals.empty())
                            break;

                        update_map(memory, token, ret_vals.front());
                        ret_vals.pop();
                    }
                    else if (op == RET)
                    {
                        ret_vals.push(eval(token, memory));
                    }
                    else if (op == FCALL)
                    {
                        try
                        {
                            func_line_num_stk.push(line_num);
                            line_num = stoi(eval(token, memory)) - 1;
                        }
                        catch (const invalid_argument &ia)
                        {
                            cerr << "Function Error: Line " << line_num + 1 << ": Invalid function index!\n";
                            exit(1);
                        }

                        goto outer;
                    }
                    else if (op == FREAD)
                    {
                        if (i == 1)
                        {
                            key = token;
                        }
                        else if (i == 2)
                        {
                            ifstream my_file;
                            my_file.open(eval(token, memory), ios_base::binary);

                            if (!my_file.is_open())
                            {
                                file_error = 1;
                                i++;
                                continue;
                            }

                            string buf;
                            copy(istreambuf_iterator<char>(my_file), istreambuf_iterator<char>(), back_inserter(buf));

                            my_file.close();

                            update_map(memory, key, buf);
                        }
                        else
                        {
                            update_map(memory, token, string(file_error != 0 ? "1" : "0"));
                            break;
                        }
                    }
                    else if (op == FWRITE)
                    {
                        if (i == 1)
                        {
                            key = token;
                        }
                        else if (i == 2)
                        {
                            ofstream my_file;
                            my_file.open(eval(key, memory), ios_base::binary);

                            if (!my_file.is_open())
                            {
                                file_error = 1;
                                i++;
                                continue;
                            }

                            my_file << eval(token, memory);

                            my_file.close();
                        }
                        else
                        {
                            update_map(memory, token, string(file_error != 0 ? "1" : "0"));
                            break;
                        }
                    }
                    else if (op == FDEL)
                    {
                        if (i == 1)
                        {
                            file_error = remove(eval(token, memory).c_str());
                        }
                        else
                        {
                            update_map(memory, token, string(file_error != 0 ? "1" : "0"));
                            break;
                        }
                    }
                    else if (op == BYE)
                    {
                        try
                        {
                            exit(stoi(eval(token, memory)));
                        }
                        catch (const invalid_argument &ia)
                        {
                            cerr << "Type Error: Line " << line_num + 1 << ": Exit code must be an integer!\n";
                            exit(1);
                        }
                    }
                }

                i++;
            }

            if (i != 0)
            {
                if (op == EXPR)
                {
                    update_map(memory, key, to_string(expr_eval(exp)));
                }
                else if (op == CAT)
                {
                    update_map(memory, key, string_sum);
                }
                else if (op == CALL)
                {
                    while (!ret_vals.empty())
                        ret_vals.pop();

                    line_num = 0;
                    lines = mod_lines;

                    while (!func_line_num_stk.empty())
                        func_line_num_stk.pop();

                    memory.clear();
                    memory["$ARGC"] = to_string(mod_args.size());

                    for (int i = 0; !mod_args.empty(); i++)
                    {
                        memory["$" + to_string(i)] = mod_args.front();
                        mod_args.pop();
                    }

                    define_variables(memory);

                    create_pairs(lines, pairs);
                    create_labels(lines, memory);

                    continue;
                }
                else if (op == RET)
                {
                    break;
                }
                else if (op == FRET)
                {
                    try
                    {
                        line_num = func_line_num_stk.top() + 1;
                        func_line_num_stk.pop();
                    }
                    catch (const out_of_range &oor)
                    {
                        cerr << "Function Error: Line " << line_num + 1 << ": No function to return from!\n";
                        exit(1);
                    }

                    continue;
                }
            }

            line_num++;
        }

        if (mod_name_stk.empty())
            break;

        lines = lines_map[mod_name_stk.top()];
        pairs = pairs_map[mod_name_stk.top()];

        mod_name = mod_name_stk.top();
        mod_name_stk.pop();

        line_num = line_num_stk.top() + 1;
        line_num_stk.pop();

        func_line_num_stk = func_line_num_stk_stk.top();
        func_line_num_stk_stk.pop();

        memory = memory_stk.top();
        memory_stk.pop();
    }

    return 0;
}

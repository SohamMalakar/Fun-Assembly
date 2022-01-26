#include <algorithm>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

// Precedence of operators
#define PRECEDENCE(x)                                                                                                  \
    ((x) == "+" || (x) == "-" ? 0 : ((x) == "*" || (x) == "/" || (x) == "%" ? 1 : ((x) == "^" ? 2 : -1)))

/* comments: ! */

typedef enum
{
    PRT,  /* Prints the tokens */
    SCN,  /* Takes inputs upto next space */
    SCNL, /* Takes inputs upto next line */
    MOV,  /* Assigns a value to a variable */
    INT,  /* Converts a variable to an integer */
    ADD,  /* Adds two variables */
    SUB,  /* Subtracts two variables */
    MUL,  /* Multiplies two variables */
    DIV,  /* Divides two variables */
    MOD,  /* Modulo of two variables */
    POW,  /* Raises a variable to the given power */
    EXPR, /* Evaluates expression */
    IF,   /* If condition is true, execute the block */
    JMP,  /* Jumps to the given label */
    ARR,  /* Store data in array */
    ARRI, /* Initialize array */
    ARRV, /* Accesses value in array */
    BYE,  /* Exits the program */
    STR,  /* Store strings as char arrays */
    CAT   /* Concatenates strings */

} op_code;

/* op_code: RPXE, >, ! */

/* keyword: NULL */
string NULL_OP("NULL");

typedef enum
{
    EQL,  /* == */
    NEQ,  /* != */
    GTR,  /* > */
    LSS,  /* < */
    GEQ,  /* >= */
    LEQ,  /* <= */
    SEQL, /* == for strings */
    SNEQ  /* != for strings */

} log_code;

template <typename T1, typename T2>
void update_map(unordered_map<T1, T2> &map, T1 key, T2 value)
{
    auto itr = map.find(key);

    if (itr != map.end())
        itr->second = value;
    else
        map[key] = value;
}

void remove_file(string name)
{
    try
    {
        filesystem::remove(name);
    }
    catch (const std::filesystem::filesystem_error &err)
    {
        std::cout << "filesystem error: " << err.what() << '\n';
    }
}

float modulo(float x, float y)
{
    return x - y * floor(x / y);
}

/* template <typename T>
void print_stack(stack<T> s, string &str)
{
    if (s.empty())
        return;

    T x = s.top();
    s.pop();

    print_stack(s, str);
    str += x + " ";

    s.push(x);
} */

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

// Assuming that the input is valid
void evaluate_expression(string key, string &exp, unordered_map<string, string> &memory)
{
    // TODO: Check if the infix expression is valid
    // Pass line number, file name and file pointer to the function

    string token;

    exp = "( " + exp + ")";

    stringstream ss(exp);

    stack<string> stk, postfix;

    while (getline(ss, token, ' '))
    {
        if (token == "+" || token == "-" || token == "*" || token == "/" || token == "%" || token == "^")
        {
            while (PRECEDENCE(stk.top()) >= PRECEDENCE(token))
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
            while (stk.top() != "(")
            {
                postfix.push(stk.top());
                stk.pop();
            }

            stk.pop();
        }
        else
        {
            try
            {
                // Evaluate the token
                if (token.at(0) == '$')
                    postfix.push(memory[token]);
                else if (token.at(0) == '&')
                    postfix.push(memory["$" + memory["$" + token.substr(1)]]);
                else
                    postfix.push(token);
            }
            catch (const out_of_range &e)
            {
                cout << "Error: " << e.what() << endl;
                exit(1);
            }
        }
    }

    reverse_stack(postfix);

    // Evaluate the postfix expression
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
            try
            {
                eval.push(stod(token));
            }
            catch (const invalid_argument &e)
            {
                cout << "Error: " << e.what() << endl;
                exit(1);
            }
        }
    }

    // Update the memory
    update_map(memory, key, to_string(eval.top()));

    // Reset the expression
    exp = "";
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        cout << "Usage: " << argv[0] << " <file> [optional: args]" << endl;
        exit(1);
    }

    ifstream file;

    file.open(argv[1], ios_base::binary);

    if (!file.is_open())
    {
        cout << argv[0] << ": error: " << argv[1] << ": No such file" << endl;
        exit(1);
    }

    // Temp file
    string name(argv[1]);

    string tmp = name + ".tmp";

    ofstream file_tmp(tmp, ios_base::binary | ios_base::out);

    // Copying
    file_tmp << file.rdbuf();

    // Original file closed
    file.close();

    // Copied temp file closed
    file_tmp.close();

    file.open(tmp);

    string line;

    vector<int> lines;

    lines.push_back(0);

    unordered_map<int, int> pairs;

    stack<int> stk;

    int line_num = 0;

    unordered_map<string, string> memory;

    while (getline(file, line))
    {
        lines.push_back(file.tellg());

        if (line.find("[") != string::npos)
        {
            stk.push(line_num + 1);
        }
        else if (line.find("]") != string::npos)
        {
            if (stk.empty())
            {
                cout << "Error: Line " << line_num + 1 << ": ] without matching [" << endl;

                file.close();
                remove_file(tmp);

                exit(1);
            }

            int top = stk.top();
            stk.pop();

            pairs[top] = line_num + 1;
            pairs[line_num + 1] = top;
        }

        stringstream ss;
        istringstream iss(line);

        // Removes extra spaces
        copy(istream_iterator<string>(iss), istream_iterator<string>(), ostream_iterator<string>(ss, " "));

        int i = 0;

        op_code op;
        string token;

        while (getline(ss, token, ' '))
        {
            if (token.at(0) == '!')
                break;

            if (i == 0)
            {
                if (token != ">")
                    break;
            }
            else
            {
                if (token.at(0) == '$')
                    update_map(memory, token, to_string(line_num + 1));

                break;
            }

            i++;
        }

        line_num++;
    }

    if (!stk.empty())
    {
        cout << "Error: Line " << line_num + 1 << ": [ without matching ]" << endl;

        file.close();
        remove_file(tmp);

        exit(1);
    }

    file.clear();
    file.seekg(0);

    /* Add those args here */
    string lang_argc = to_string(argc - 1);

    // Add predefined variables
    memory["$ARGC"] = lang_argc;
    memory["$ENDL"] = "\n";
    memory["$TAB"] = "\t";
    memory["$SPC"] = " ";
    memory["$EXL"] = "!";
    memory["$DOL"] = "$";
    memory["$AMP"] = "&";

    for (int i = 0; i < argc - 1; i++)
    {
        string key_args = "$" + to_string(i);
        string value_args(argv[i + 1]);
        memory[key_args] = value_args;
    }

    string key, value1, value2;
    string key_exp, exp;

    line_num = 0;

outer:
    while (getline(file, line))
    {
        stringstream ss;
        istringstream iss(line);

        copy(istream_iterator<string>(iss), istream_iterator<string>(), ostream_iterator<string>(ss, " "));

        int i = 0;
        op_code op;
        string token;

        string arr_name;
        string index;
        string val;

        string string_sum = "";
        int num_of_cats = 0;

        while (getline(ss, token, ' '))
        {
            log_code log;

            // Comments
            if (token.at(0) == '!')
                break;

            if (i == 0)
            {
                /* For op code only */
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
                else if (token == "RPXE")
                    evaluate_expression(key_exp, exp, memory);
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
                else if (token == ">")
                    break;
                else if (token == "[" || token == "]")
                    break;
                else
                {
                    cout << "Error: Line " << line_num + 1 << ": " << token << " is not a valid op code" << endl;

                    file.close();
                    remove_file(tmp);

                    exit(1);
                }
            }
            else
            {
                /* For operands only */
                if (op == PRT)
                {
                    if (i != 1)
                        cout << " ";

                    if (token.at(0) == '&')
                    {
                        string var;

                        try
                        {
                            var = "$" + token.substr(1);
                        }
                        catch (const std::out_of_range &oor)
                        {
                            cout << "Error: Line " << line_num + 1 << ": " << oor.what() << endl;

                            file.close();
                            remove_file(tmp);

                            exit(1);
                        }

                        string value = "$" + memory[var];

                        // Do something with value
                        cout << memory[value];
                    }
                    else if (token.at(0) == '$')
                    {
                        cout << memory[token];
                    }
                    else
                    {
                        cout << token;
                    }
                }
                else if (op == SCN)
                {
                    // Asks for input
                    string input;
                    cin >> input;

                    // Store the input in the hashtable
                    update_map(memory, token, input);
                }
                else if (op == SCNL)
                {
                    // Asks for input up to a new line
                    string input;
                    getline(cin, input);

                    // Store the input in the hashtable
                    update_map(memory, token, input);
                }
                else if (op == MOV)
                {
                    // Move the value from one variable to another
                    if (i == 1)
                    {
                        key = token;
                    }
                    else
                    {
                        if (token.at(0) == '&')
                        {
                            string var;

                            try
                            {
                                var = "$" + token.substr(1);
                            }
                            catch (const std::out_of_range &oor)
                            {
                                cout << "Error: Line " << line_num + 1 << ": " << oor.what() << endl;

                                file.close();
                                remove_file(tmp);

                                exit(1);
                            }

                            string value = "$" + memory[var];

                            // Do something with value
                            update_map(memory, key, memory[value]);
                        }
                        else if (token.at(0) == '$')
                        {
                            update_map(memory, key, memory[token]);
                        }
                        else
                        {
                            update_map(memory, key, token);
                        }

                        break; /* 3 tokens */
                    }
                }
                else if (op == INT)
                {
                    // Convert the value to integer
                    if (i == 1)
                    {
                        key = token;
                    }
                    else
                    {
                        try
                        {
                            if (token.at(0) == '&')
                            {
                                string var = "$" + token.substr(1);
                                string value = "$" + memory[var];

                                // Do something with value
                                update_map(memory, key, to_string(stoi(memory[value])));
                            }
                            else if (token.at(0) == '$')
                            {
                                update_map(memory, key, to_string(stoi(memory[token])));
                            }
                            else
                            {
                                update_map(memory, key, to_string(stoi(token)));
                            }
                        }
                        catch (const std::invalid_argument &ia)
                        {
                            cout << "Error: Line " << line_num + 1 << ": " << ia.what() << endl;

                            file.close();
                            remove_file(tmp);

                            exit(1);
                        }
                        catch (const std::out_of_range &oor)
                        {
                            cout << "Error: Line " << line_num + 1 << ": " << oor.what() << endl;

                            file.close();
                            remove_file(tmp);

                            exit(1);
                        }

                        break; /* 3 tokens */
                    }
                }
                else if (op == ADD || op == SUB || op == MUL || op == DIV || op == MOD || op == POW)
                {
                    // Add, Sub, Mul, Div, Mod, Pow
                    if (i == 1)
                    {
                        // Save the key
                        key = token;
                    }
                    else if (i == 2)
                    {
                        if (token.at(0) == '&')
                        {
                            string var;

                            try
                            {
                                var = "$" + token.substr(1);
                            }
                            catch (const std::out_of_range &oor)
                            {
                                cout << "Error: Line " << line_num + 1 << ": " << oor.what() << endl;

                                file.close();
                                remove_file(tmp);

                                exit(1);
                            }

                            string value = "$" + memory[var];

                            // Do something with value
                            value1 = memory[value];
                        }
                        else if (token.at(0) == '$')
                        {
                            value1 = memory[token];
                        }
                        else
                        {
                            value1 = token;
                        }
                    }
                    else if (i == 3)
                    {
                        if (token.at(0) == '&')
                        {
                            string var;

                            try
                            {
                                var = "$" + token.substr(1);
                            }
                            catch (const std::out_of_range &oor)
                            {
                                cout << "Error: Line " << line_num + 1 << ": " << oor.what() << endl;

                                file.close();
                                remove_file(tmp);

                                exit(1);
                            }

                            string value = "$" + memory[var];

                            // Do something with value
                            value2 = memory[value];
                        }
                        else if (token.at(0) == '$')
                        {
                            value2 = memory[token];
                        }
                        else
                        {
                            value2 = token;
                        }

                        // Perform the operation
                        float result;

                        try
                        {
                            if (op == ADD)
                                result = stof(value1) + stof(value2);
                            else if (op == SUB)
                                result = stof(value1) - stof(value2);
                            else if (op == MUL)
                                result = stof(value1) * stof(value2);
                            else if (op == DIV)
                                result = stof(value1) / stof(value2);
                            else if (op == MOD)
                                result = modulo(stof(value1), stof(value2));
                            else if (op == POW)
                                result = pow(stof(value1), stof(value2));
                        }
                        catch (const std::invalid_argument &ia)
                        {
                            cout << "Type Error: Line " << line_num + 1 << ": We only deal with floats!" << endl;

                            file.close();
                            remove_file(tmp);

                            exit(1);
                        }

                        // Store the result in the hashtable
                        update_map(memory, key, to_string(result));

                        break; /* 4 tokens */
                    }
                }
                else if (op == EXPR)
                {
                    if (i == 1)
                    {
                        key_exp = token;
                    }
                    else
                    {
                        // Create the infix expression
                        exp += token + " ";
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
                        else if (token == "GTR")
                            log = GTR;
                        else if (token == "LSS")
                            log = LSS;
                        else if (token == "GEQ")
                            log = GEQ;
                        else if (token == "LEQ")
                            log = LEQ;
                        else if (token == "SEQL")
                            log = SEQL;
                        else if (token == "SNEQ")
                            log = SNEQ;
                        else
                        {
                            cout << "Error: Line " << line_num + 1 << ": " << token
                                 << " is not a valid comparison operator" << endl;

                            file.close();
                            remove_file(tmp);

                            exit(1);
                        }
                    }
                    else if (i == 2)
                    {
                        if (token.at(0) == '&')
                        {
                            string var;

                            try
                            {
                                var = "$" + token.substr(1);
                            }
                            catch (const std::out_of_range &oor)
                            {
                                cout << "Error: Line " << line_num + 1 << ": " << oor.what() << endl;

                                file.close();
                                remove_file(tmp);

                                exit(1);
                            }

                            string value = "$" + memory[var];

                            // Do something with value
                            value1 = memory[value];
                        }
                        else if (token.at(0) == '$')
                        {
                            value1 = memory[token];
                        }
                        else
                        {
                            value1 = token;
                        }
                    }
                    else if (i == 3)
                    {
                        if (token.at(0) == '&')
                        {
                            string var;

                            try
                            {
                                var = "$" + token.substr(1);
                            }
                            catch (const std::out_of_range &oor)
                            {
                                cout << "Error: Line " << line_num + 1 << ": " << oor.what() << endl;

                                file.close();
                                remove_file(tmp);

                                exit(1);
                            }

                            string value = "$" + memory[var];

                            // Do something with value
                            value2 = memory[value];
                        }
                        else if (token.at(0) == '$')
                        {
                            value2 = memory[token];
                        }
                        else
                        {
                            value2 = token;
                        }

                        // Perform the operation
                        bool condition;

                        try
                        {
                            condition = (log == EQL && !(stof(value1) == stof(value2))) ||
                                        (log == GTR && !(stof(value1) > stof(value2))) ||
                                        (log == LSS && !(stof(value1) < stof(value2))) ||
                                        (log == GEQ && !(stof(value1) >= stof(value2))) ||
                                        (log == LEQ && !(stof(value1) <= stof(value2))) ||
                                        (log == NEQ && !(stof(value1) != stof(value2))) ||
                                        (log == SEQL && !(value1 == value2)) || (log == SNEQ && !(value1 != value2));
                        }
                        catch (const std::invalid_argument &ia)
                        {
                            cout << "Type Error: Line " << line_num + 1 << ": We only deal with floats!" << endl;

                            file.close();
                            remove_file(tmp);

                            exit(1);
                        }

                        // If the condition is true, jump to the line specified
                        if (condition)
                        {
                            line_num = pairs[(line_num + 1) + 1] - 1;

                            file.clear();
                            file.seekg(lines[line_num]);

                            goto outer;
                        }
                    }
                }
                else if (op == JMP)
                {
                    try
                    {
                        if (token.at(0) == '&')
                        {
                            string var = "$" + token.substr(1);
                            string value = "$" + memory[var];

                            // Do something with value
                            line_num = stoi(memory[value]) - 1;
                        }
                        else if (token.at(0) == '$')
                        {
                            line_num = stoi(memory[token]) - 1;
                        }
                        else
                        {
                            line_num = stoi(token) - 1;
                        }
                    }
                    catch (const std::invalid_argument &ia)
                    {
                        cout << "Index Error: Line " << line_num + 1 << ": Invalid jump index!" << endl;

                        file.close();
                        remove_file(tmp);

                        exit(1);
                    }
                    catch (const std::out_of_range &oor)
                    {
                        cout << "Error: Line " << line_num + 1 << ": " << oor.what() << endl;

                        file.close();
                        remove_file(tmp);

                        exit(1);
                    }

                    // Jump to the line specified
                    file.clear();
                    file.seekg(lines[line_num]);
                    goto outer;
                }
                else if (op == ARR)
                {
                    try
                    {
                        if (i == 1)
                        {
                            arr_name = token;
                        }
                        else if (i == 2)
                        {
                            // Find the index
                            if (token.at(0) == '$')
                                index = to_string(stoi(memory[token]));
                            else
                                index = to_string(stoi(token));
                        }
                        else if (i == 3)
                        {
                            // Assign the value
                            if (token.at(0) == '$')
                                val = memory[token];
                            else
                                val = token;

                            update_map(memory, arr_name + "(" + index + ")", val);

                            break;
                        }
                    }
                    catch (const std::invalid_argument &ia)
                    {
                        cout << "Type Error: Line " << line_num + 1 << ": We only deal with floats!" << endl;

                        file.close();
                        remove_file(tmp);

                        exit(1);
                    }
                }
                else if (op == ARRI)
                {
                    try
                    {
                        if (i == 1)
                        {
                            arr_name = token;
                        }
                        else
                        {
                            // Find the value
                            if (token.at(0) == '$')
                                val = memory[token];
                            else if (token.at(0) == '&')
                                val = memory["$" + memory["$" + token.substr(1)]];
                            else
                                val = token;

                            // Assign the value
                            update_map(memory, arr_name + "(" + to_string(i - 2) + ")", val);
                        }
                    }
                    catch (const out_of_range &oor)
                    {
                        cout << "Error: Line " << line_num + 1 << ": " << oor.what() << endl;

                        file.close();
                        remove_file(tmp);

                        exit(1);
                    }
                }
                else if (op == ARRV)
                {
                    try
                    {
                        if (i == 1)
                        {
                            key = token;
                        }
                        else if (i == 2)
                        {
                            // Find the array name
                            arr_name = token;
                        }
                        else if (i == 3)
                        {
                            // Assign the value
                            if (token.at(0) == '$')
                                index = to_string(stoi(memory[token]));
                            else
                                index = to_string(stoi(token));

                            update_map(memory, key, memory[arr_name + "(" + index + ")"]);

                            break;
                        }
                    }
                    catch (const std::invalid_argument &ia)
                    {
                        cout << "Type Error: Line " << line_num + 1 << ": Index should be integer only!" << endl;

                        file.close();
                        remove_file(tmp);

                        exit(1);
                    }
                }
                else if (op == STR)
                {
                    string str;

                    if (token.at(0) == '&')
                    {
                        string var;

                        try
                        {
                            var = "$" + token.substr(1);
                        }
                        catch (const std::out_of_range &oor)
                        {
                            cout << "Error: Line " << line_num + 1 << ": " << oor.what() << endl;

                            file.close();
                            remove_file(tmp);

                            exit(1);
                        }

                        string value = "$" + memory[var];

                        // Do something with value
                        str = memory[value];
                    }
                    else if (token.at(0) == '$')
                    {
                        str = memory[token];
                    }
                    else
                    {
                        str = token;
                    }

                    // Convert the string to c style string
                    for (int i = 0; i < str.length(); i++)
                    {
                        char c[2] = {str.at(i), '\0'};
                        string s(c);
                        update_map(memory, token + "(" + to_string(i) + ")", s);
                    }

                    update_map(memory, token + "(" + to_string(str.length()) + ")", NULL_OP);
                }
                else if (op == CAT)
                {
                    if (i == 1)
                    {
                        // Number of strings to concatenate
                        try
                        {
                            if (token.at(0) == '&')
                            {
                                string var = "$" + token.substr(1);
                                string value = "$" + memory[var];

                                // Do something with value
                                num_of_cats = stoi(memory[value]);
                            }
                            else if (token.at(0) == '$')
                            {
                                num_of_cats = stoi(memory[token]);
                            }
                            else
                            {
                                num_of_cats = stoi(token);
                            }
                        }
                        catch (const std::invalid_argument &ia)
                        {
                            cout << "Type Error: Line " << line_num + 1
                                 << ": Number of strings to concatenate should be integer only!" << endl;

                            file.close();
                            remove_file(tmp);

                            exit(1);
                        }
                        catch (const std::out_of_range &oor)
                        {
                            cout << "Error: Line " << line_num + 1 << ": " << oor.what() << endl;

                            file.close();
                            remove_file(tmp);

                            exit(1);
                        }
                    }
                    else if (i == 2)
                    {
                        key = token;
                    }
                    else
                    {
                        string str;

                        if (token.at(0) == '&')
                        {
                            string var;

                            try
                            {
                                var = "$" + token.substr(1);
                            }
                            catch (const std::out_of_range &oor)
                            {
                                cout << "Error: Line " << line_num + 1 << ": " << oor.what() << endl;

                                file.close();
                                remove_file(tmp);

                                exit(1);
                            }

                            string value = "$" + memory[var];

                            // Do something with value
                            str = memory[value];
                        }
                        else if (token.at(0) == '$')
                        {
                            str = memory[token];
                        }
                        else
                        {
                            str = token;
                        }

                        string_sum += str;

                        if (i == num_of_cats + 2)
                        {
                            update_map(memory, key, string_sum);
                            break;
                        }
                    }
                }
                else if (op == BYE)
                {
                    file.close();
                    remove_file(tmp);

                    try
                    {
                        exit(stoi(token));
                    }
                    catch (const std::invalid_argument &ia)
                    {
                        cout << "Type Error: Line " << line_num + 1 << ": Exit code should be integer only!" << endl;
                        exit(1);
                    }
                }
            }

            i++;
        }

        line_num++;
    }

    // Copied file closed
    file.close();

    // Deleting the copied tmp file
    remove_file(tmp);

    return 0;
}

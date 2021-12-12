#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iterator>
#include <algorithm>
#include <cmath>
#include <vector>
#include <stack>
#include <unordered_map>

using namespace std;

/* comments: ! */

typedef enum
{
    PRT,
    SCN,
    MOV,
    ADD,
    SUB,
    MUL,
    DIV,
    MOD,
    POW,
    IF,
    JMP,
    LINE /* > */

} op_code;

/* op_code: ENDL, TAB, SPC, >, ! */

typedef enum
{
    EQL,
    NEQ,
    GRT,
    LOW,
    GEQ,
    LEQ

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

float modulo(float x, float y)
{
    return x - y * floor(x / y);
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        cout << "Usage: " << argv[0] << " <file> " << endl;
        exit(1);
    }

    ifstream file;

    file.open(argv[1]);

    if (!file.is_open())
    {
        cout << argv[0] << ": error: " << argv[1] << ": No such file" << endl;
        exit(1);
    }

    string line;

    vector<int> lines;

    lines.push_back(0);

    unordered_map<int, int> pairs;

    stack<int> stk;

    int line_num = 0;

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
                cout << "Error: ] without matching [" << endl;
                exit(1);
            }

            int top = stk.top();
            stk.pop();

            pairs[top] = line_num + 1;
            pairs[line_num + 1] = top;
        }

        line_num++;
    }

    if (!stk.empty())
    {
        cout << "Error: [ without matching ]" << endl;
        exit(1);
    }

    file.clear();
    file.seekg(0);

    unordered_map<string, string> memory;

    string key, value1, value2;

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

        while (getline(ss, token, ' '))
        {
            log_code log;

            if (i == 0)
            {
                /* For op code only */
                if (token == "PRT")
                    op = PRT;
                else if (token == "ENDL") /* newline */
                    cout << endl;
                else if (token == "TAB") /* tab */
                    cout << "\t";
                else if (token == "SPC") /* space */
                    cout << " ";
                else if (token == "SCN")
                    op = SCN;
                else if (token == "MOV")
                    op = MOV;
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
                else if (token == "IF")
                    op = IF;
                else if (token == "JMP")
                    op = JMP;
                else if (token == ">") /* jump line */
                    op = LINE;
                else if (token == "!") /* comment */
                    break;
                else if (token == "[" || token == "]") /* ignore */
                    break;
                else
                {
                    cout << "Error: " << token << " is not a valid op code" << endl;
                    exit(1);
                }

                if (token == "ENDL" || token == "TAB" || token == "SPC")
                    break;
            }
            else
            {
                /* For operands only */
                if (op == PRT)
                {
                    if (i != 1)
                        cout << " ";

                    if (token.at(0) != '$')
                        cout << token;
                    else // Fetch the value from the hashtable and print it
                        cout << memory[token];
                }
                else if (op == SCN)
                {
                    // Asks for input
                    string input;
                    cin >> input;

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
                        if (token.at(0) != '$')
                            update_map(memory, key, token);
                        else
                            update_map(memory, key, memory[token]);

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
                        if (token.at(0) != '$')
                            value1 = token;
                        else // Fetch the value from the hashtable
                            value1 = memory[token];
                    }
                    else if (i == 3)
                    {
                        if (token.at(0) != '$')
                            value2 = token;
                        else // Fetch the value from the hashtable
                            value2 = memory[token];

                        // Perform the operation
                        float result;

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

                        // Store the result in the hashtable
                        update_map(memory, key, to_string(result));

                        break; /* 4 tokens */
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
                        else if (token == "GRT")
                            log = GRT;
                        else if (token == "LOW")
                            log = LOW;
                        else if (token == "GEQ")
                            log = GEQ;
                        else if (token == "LEQ")
                            log = LEQ;
                        else
                        {
                            cout << "Error: " << token << " is not a valid comparison operator" << endl;
                            exit(1);
                        }
                    }
                    else if (i == 2)
                    {
                        if (token.at(0) != '$')
                            value1 = token;
                        else // Fetch the value from the hashtable
                            value1 = memory[token];
                    }
                    else if (i == 3)
                    {
                        if (token.at(0) != '$')
                            value2 = token;
                        else // Fetch the value from the hashtable
                            value2 = memory[token];

                        // Perform the condition
                        bool condition = (log == EQL && !(stof(value1) == stof(value2))) || (log == GRT && !(stof(value1) > stof(value2))) || (log == LOW && !(stof(value1) < stof(value2))) || (log == GEQ && !(stof(value1) >= stof(value2))) || (log == LEQ && !(stof(value1) <= stof(value2))) || (log == NEQ && !(stof(value1) != stof(value2)));

                        // If the condition is true, jump to the line specified
                        if (condition)
                        {
                            line_num = pairs[(line_num + 1) + 1] - 1;

                            file.clear();
                            file.seekg(lines[line_num]);

                            continue;
                        }
                    }
                }
                else if (op == JMP)
                {
                    if (token.at(0) != '$')
                        line_num = stoi(token) - 1;
                    else
                        line_num = stoi(memory[token]) - 1;

                    // Jump to the line specified
                    file.clear();
                    file.seekg(lines[line_num]);
                    goto outer;
                }
                else if (op == LINE)
                {
                    update_map(memory, token, to_string(line_num + 1));
                    break; /* 2 tokens */
                }
            }

            i++;
        }

        line_num++;
    }

    return 0;
}

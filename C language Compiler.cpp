#include <bits/stdc++.h>

using namespace std;

const int MAX = 100;

struct Node
{
    int serialNo;
    string identifier;
    string scope;
    string type;
    string dataType;
    string value;
    Node *next;

    Node(int serialNo, string identifier, string scope, string type, string dataType, string value)
    {
        this->serialNo = serialNo;
        this->identifier = identifier;
        this->scope = scope;
        this->type = type;
        this->dataType = dataType;
        this->value = value;
        next = NULL;
    }

    void print()
    {
        cout << left
             << setw(10) << serialNo
             << setw(10) << identifier
             << setw(10) << type
             << setw(12) << dataType
             << setw(10) << scope
             << setw(10) << value
             << endl;
    }
};

struct SymbolTable
{
    Node *head[MAX];
    int counter;

    SymbolTable()
    {
        for (int i = 0; i < MAX; i++)
            head[i] = nullptr;
        counter = 1;
    }

    int hashf(string id)
    {
        int asciiSum = 0;
        for (char c : id)
        {
            asciiSum += c;
        }
        return (asciiSum % 100);
    }

    bool insert(string id, string scope, string type, string dataType, string value)
    {
        int index = hashf(id);
        Node *p = new Node(counter, id, scope, type, dataType, value);
        counter++;

        if (head[index] == nullptr)
        {
            head[index] = p;
            return true;
        }
        else
        {
            Node *start = head[index];
            while (start->next != nullptr)
            {
                if (start->identifier == id && start->scope == scope)
                {
                    return false;
                }
                start = start->next;
            }

            if (start->identifier == id && start->scope == scope)
            {
                return false;
            }

            start->next = p;
            return true;
        }
    }

    string find(string id)
    {
        int index = hashf(id);
        Node *start = head[index];

        if (start == nullptr)
            return "-1";

        while (start != nullptr)
        {
            if (start->identifier == id)
            {
                return start->scope;
            }
            start = start->next;
        }
        return "-1";
    }

    bool modify(string id, string scope, string type, string dataType, string value)
    {
        int index = hashf(id);
        Node *start = head[index];

        if (start == nullptr)
            return false;

        while (start != nullptr)
        {
            if (start->identifier == id)
            {
                start->scope = scope;
                start->type = type;
                if (dataType != "")
                {
                    start->dataType = dataType;
                }
                start->value = value;
                return true;
            }
            start = start->next;
        }
        return false;
    }
};

void printSymbolTable(SymbolTable &st)
{
    cout << "SYMBOL TABLE:" << endl;

    vector<Node *> entries;

    for (int i = 0; i < MAX; i++)
    {
        Node *start = st.head[i];
        while (start != nullptr)
        {
            entries.push_back(start);
            start = start->next;
        }
    }

    sort(entries.begin(), entries.end(), [](Node *a, Node *b)
         { return a->serialNo < b->serialNo; });

    cout << left
         << setw(10) << "Sl. No."
         << setw(10) << "Name"
         << setw(10) << "Id Type"
         << setw(12) << "Data Type"
         << setw(10) << "Scope"
         << setw(10) << "Value"
         << endl;

    for (Node *entry : entries)
    {
        entry->print();
    }
}

int is_identifier(const string &lex)
{
    int i = 0, l, s;

    if (isalpha(lex[i]))
    {
        s = 1;
        i++;
    }
    else if (lex[i] == '_')
    {
        s = 1;
        i++;
    }
    else
    {
        s = 0;
    }

    l = lex.length();

    if (s == 1)
    {
        for (; i < l; i++)
        {
            if (isalpha(lex[i]) || lex[i] == '_' || isdigit(lex[i]))
            {
                s = 1;
            }
            else
            {
                s = 0;
                break;
            }
        }
    }
    return s;
}

int num_rec(const string &lex)
{
    int i, l, s;
    i = 0;
    if (isdigit(lex[i]))
    {
        s = 1;
        i++;
    }
    else if (lex[i] == '.')
    {
        s = 2;
        i++;
    }
    else
        s = 0;

    l = lex.length();

    if (s == 1)
        for (; i < l; i++)
        {
            if (isdigit(lex[i]))
                s = 1;
            else if (lex[i] == '.')
            {
                s = 2;
                i++;
                break;
            }
            else
            {
                s = 0;
                break;
            }
        }

    if (s == 2)
        if (isdigit(lex[i]))
        {
            s = 3;
            i++;
        }
        else
            s = 0;

    if (s == 3)
        for (; i < l; i++)
        {
            if (isdigit(lex[i]))
                s = 3;
            else
            {
                s = 0;
                break;
            }
        }

    if (s == 3)
        s = 1;

    return s;
}

int main()
{
    ifstream input_file;
    ofstream output_file;
    char c;

    output_file.open("input.txt");

    if (!output_file)
    {
        cout << "File cant be opened" << endl;
    }
    else
    {
        cout << "Enter the input code (terminate with Ctrl+Z):" << endl;

        while (cin.get(c))
        {
            output_file.put(c);
        }
    }
    output_file.close();

    // Save immediate code for Detecting Simple Syntax Errors later

    input_file.open("input.txt");
    output_file.open("intermediate.txt");

    if (!input_file)
    {
        cout << "Error opening the file!" << endl;
        return 1;
    }

    if (!output_file)
    {
        cout << "Error creating file!" << endl;
        return 1;
    }

    char prev = '\0';
    string input;
    string line;
    bool needSpace = false;

    while (input_file.get(c))
    {
        if (c == '/')
        {
            input_file.get(c);
            if (c == '*')
            {
                while (input_file.get(c) && c != '*')
                    ;
                input_file.get(c);
                if (c == '/')
                    continue;
            }
            else if (c == '/')
            {
                while (input_file.get(c) && c != '\n')
                    ;
                input += c;
            }
        }
        else if (c == '(')
        {
        if (prev != ' ' && prev != '\n')
        {
            input += ' ';
        }
        input += c;
        needSpace = true;
        }
        else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == ')' || c == '{' || c == '}' || c == ',' || c == ';' || c == '[' || c == ']')
        {
            if (prev != ' ' && prev != '\n')
            {
                input += ' ';
            }
            input += c;
            input_file.get(c);

            if (c != ' ')
            {
                input += ' ';
                input += c;
            }
            else
            {
                input += c;
            }
        }
        else
        {
        if (needSpace)
        {
            input += ' ';
            needSpace = false;
        }
        input += c;
        }
        prev = c;
    }

    input = input.substr(0, input.size() - 1);

    istringstream iss0(input);
    vector<string> tokens;
    string token;
    string line1;

    while (getline(iss0, line1))
    {
        istringstream iss2(line1);
        string token;
        while (iss2 >> token)
        {
            tokens.push_back(token);
        }
        tokens.push_back("\n");
    }

    // cout << "\ninput : \n" <<input <<endl;
    input.clear();

    for (int i = 0; i < tokens.size(); i++)
    {
        if (tokens[i] == "int" || tokens[i] == "char" || tokens[i] == "float" || tokens[i] == "double" || tokens[i] == "if" || tokens[i] == "else" || tokens[i] == "for" || tokens[i] == "while" || tokens[i] == "return" || tokens[i] == "void")
        {
            if (tokens[i - 1] != " ")
                input += ' ';
            input += tokens[i];
        }
        else if (is_identifier(tokens[i]) == 1)
        {
            if (tokens[i - 1] != " ")
                input += ' ';
            if (tokens[i] != "void")
                input += "id ";
            input += tokens[i];
        }
        else
        {
            if (tokens[i - 1] != " ")
                input += ' ';
            input += tokens[i];
        }
    }

    istringstream iss(input);
    int line_no = 0;

    while (getline(iss, line))
    {
        ++line_no;
        output_file << line_no << line << endl;
    }
    output_file << ++line_no;

    input_file.close();
    output_file.close();

    // input_file.open("intermediate.txt");

    // cout << "\n\nintermediate:\n";
    // while ((c = input_file.get()) != EOF)
    // {
    //     cout << c;
    // }
    // input_file.close();

    istringstream iss1(input);
    vector<string> tokens0;

    while (getline(iss1, token, ' '))
    {
        if (!token.empty())
        {
            tokens0.push_back(token);
        }
    }

    // Intermediate Output End

    // ====== 1. scanning and filtering of a source program ======

    input_file.open("input.txt");
    output_file.open("output1.txt");

    if (!input_file)
    {
        cout << "File cant be opened" << endl;
    }
    else
    {
        while ((c = input_file.get()) != EOF)
        {
            if (c == ' ')
            {
                output_file.put(c);
                while ((c = input_file.get()) == ' ')
                    ;
                output_file.put(c);
                continue;
                // if ((c = input_file.get()) != ' ')
                // {
                //     output_file << ' ' << c;
                // }
            }
            else if (c == '/')
            {
                char temp = c; // to handle divide
                if ((c = input_file.get()) == '*')
                {
                    while ((c = input_file.get()) != '*')
                        ;
                    if ((c = input_file.get()) == '/')
                        continue;
                }
                else if (c == '/')
                {
                    while ((c = input_file.get()) != '\n')
                        ;
                    continue;
                }
                else
                {
                    output_file.put(temp);
                    output_file.put(c);
                }
            }
            else if (c == '\n' || c == '\t')
            {
                continue;
            }
            else
            {
                output_file.put(c);
            }
        }
    }
    input_file.close();
    output_file.close();

    input_file.open("output1.txt");

    if (!input_file)
    {
        cout << "Output file cant be opened" << endl;
    }
    else
    {
        cout << "\n==== scanning and filtering of a source program output ====\n"
             << endl;
        while (input_file.get(c))
        {
            cout << c;
        }
        input_file.close();
    }

    // ====== 2. Lexical Analysis ======

    string array;

    input_file.open("output1.txt");
    output_file.open("step1.txt");

    if (!input_file)
    {
        cout << "File cant be opened" << endl;
    }
    else
    {
        while ((c = input_file.get()) != EOF)
        {
            if (c == ',' || c == ';' || c == ')' || c == '(' || c == '=' || c == '+' || c == '-' || c == '*' || c == '/' || c == '{' || c == '}')
            {
                output_file << " " << c << " ";
            }
            else if (c == '\'')
            {
                output_file << c << " ";
                c = input_file.get();
                output_file << c;
                c = input_file.get();
                output_file << " " << c;
            }
            else if (c == '=')
            {
                output_file << " " << c;
                if ((c = input_file.get()) == '=')
                {
                    output_file << c << " ";
                }
                else
                {
                    output_file << " ";
                }
            }
            else if (c == '!')
            {
                output_file << " " << c;
                if ((c = input_file.get()) == '=')
                {
                    output_file << c << " ";
                }
                else
                {
                    output_file << " ";
                }
            }
            else if (c == '<')
            {
                output_file << " " << c;
                if ((c = input_file.get()) == '=')
                {
                    output_file << c << " ";
                }
                else
                {
                    output_file << " ";
                }
            }
            else if (c == '>')
            {
                output_file << " " << c;
                if ((c = input_file.get()) == '=')
                {
                    output_file << c << " ";
                }
                else
                {
                    output_file << " ";
                }
            }
            else
            {
                output_file << c;
            }
        }
    }

    input_file.close();
    output_file.close();

    input_file.open("step1.txt");

    cout << "\n\n==== Lexical Analysis Output ====\n\n";
    cout << "Step 1:\n";
    while ((c = input_file.get()) != EOF)
    {
        cout << c;
        array.push_back(c);
    }

    input_file.close();

    output_file.open("step2.txt");

    vector<int> errorTokens;
    int n = 0;

    size_t pos = 0;
    while (pos < array.length())
    {
        while (pos < array.length() && array[pos] == ' ')
        {
            pos++;
        }

        size_t next_pos = array.find(" ", pos);
        string token = array.substr(pos, next_pos - pos);

        n++;

        if (token == "'" || token == "," || token == ";")
        {
            output_file << "[sep " << token << "] ";
        }
        else if (token == "(" || token == ")")
        {
            output_file << "[par " << token << "] ";
        }
        else if (token == "{" || token == "}")
        {
            output_file << "[brc " << token << "] ";
        }
        else if (token == "[" || token == "]")
        {
            output_file << "[brc " << token << "] ";
        }
        else if (token == "=" || token == "+" || token == "-" || token == "*" || token == "/" || token == "<" || token == ">" || token == "==" || token == "!=" || token == "<=" || token == ">=")
        {
            output_file << "[op " << token << "] ";
        }
        else if (token == "int" || token == "char" || token == "float" || token == "double" || token == "if" || token == "else" || token == "return" || token == "void")
        {
            output_file << "[kw " << token << "] ";
        }
        else if (is_identifier(token) == 1)
        {
            output_file << "[id " << token << "] ";
        }
        else if (num_rec(token) == 1)
        {
            output_file << "[num " << token << "] ";
        }
        else
        {
            output_file << "[unkn " << token << "] ";
            errorTokens.push_back(n); // Store the error token number
        }

        if (next_pos == string::npos)
            break;

        pos = next_pos + 1;
    }

    output_file.close();

    input_file.open("step2.txt");
    cout << "\n\nStep 2:\n";
    while ((c = input_file.get()) != EOF)
    {
        cout << c;
    }
    input_file.close();

    if (!errorTokens.empty())
    {
        cout << "\n\nErrors found in tokens numbers:";
        for (int i = 0; i < errorTokens.size(); ++i)
        {
            cout << " " << errorTokens[i];
        }
        cout << "\n";
    }
    else
    {
        cout << "\n\nNo errors found.";
    }

    // ====== 3. Symbol Table generation ======

    input_file.open("step2.txt");
    if (!input_file)
    {
        cout << "Error opening the file!" << endl;
        return 1;
    }

    string step1;

    while (input_file.get(c))
    {
        if (c == '[')
        {
            output_file << c;
            step1 += c;
            string kw;
            while (input_file.get(c) && c != ' ')
            {
                kw += c;
            }
            if (kw == "id")
            {
                output_file << "id ";
                step1 += "id ";
            }
        }
        else
        {
            if (c == ' ' && prev == ' ')
            {
                continue;
            }
            output_file << c;
            step1 += c;
        }
        prev = c;
    }
    cout << "\n\n==== Symbol Table generation Output ====\n\n";
    cout << "Step 1:" << endl;
    cout << step1 << endl;

    input_file.close();

    // step-2

    istringstream iss2(step1);
    vector<string> tokens1;
    string token1;

    while (getline(iss2, token1, ' '))
    {
        token1.erase(remove(token1.begin(), token1.end(), '['), token1.end());
        token1.erase(remove(token1.begin(), token1.end(), ']'), token1.end());

        if (!token1.empty())
        {
            tokens1.push_back(token1);
        }
    }

    string scope = "global";
    SymbolTable st;
    string check;

    cout << endl;

//    for (int i = 0; i < tokens1.size(); i++)
//    {
//        cout << "tokens : " << tokens1[i] << endl;
//    }

    for (int i = 0; i < tokens1.size(); i++)
    {
        if (tokens1[i] == "id")
        {
            if (tokens1[i + 2] == "(")
            {
                if (tokens1[i - 1] == "int" || tokens1[i - 1] == "float" || tokens1[i - 1] == "double" || tokens1[i - 1] == "char")
                {
                    st.insert(tokens1[i + 1], scope, "func", tokens1[i - 1], "");
                    scope = tokens1[i + 1];
                }
            }
            else if (tokens1[i + 2] == "=")
            {
                //cout<<"\nis var: " << tokens1[i + 1] << " before: " << tokens1[i - 1] <<endl;
                if (tokens1[i - 1] == "int" || tokens1[i - 1] == "float" || tokens1[i - 1] == "double" || tokens1[i - 1] == "char")
                {
                    //cout<<"\nis var declare: " << tokens1[i + 1] <<endl;
                    if (st.find(tokens1[i + 1]) == scope)
                    {
                        cout << "Duplicate identifier declaration in the same scope: " << tokens1[i + 1] << endl;
                    }
                    else
                    {
                        //cout<<"\nnot found in same scope, INSERTED: " << tokens1[i + 1] << " scope: " << scope <<endl;
                        st.insert(tokens1[i + 1], scope, "var", tokens1[i - 1], tokens1[i + 3]);
                    }
                }
                else if(tokens1[i - 1] != "(" && tokens1[i - 1] != ")" && tokens1[i - 1] != "else" && tokens1[i - 1] != "return" && tokens1[i - 1] != "if")
                {
                    //cout<<"\nis var initialize: " << tokens1[i + 1] << " before: " << tokens1[i - 1] <<endl;
                    string sn = st.find(tokens1[i + 1]);
                    if (sn != "-1")
                    {
                        //cout<<"\nvar already declared: " << tokens1[i + 1] <<endl;
                        if (tokens1[i + 3] != "id")
                        {
                            //cout<<"\nvar is now modified: " << tokens1[i + 1] <<endl;
                            st.modify(tokens1[i + 1], scope, "var", "", tokens1[i + 3]);
                        }
                    }
                    else
                    {
                        cout << "Undeclared identifier: " << tokens1[i + 1] << endl;
                    }
                }
            }
            else if (tokens1[i + 2] == ")" || tokens1[i + 2] == ";" || tokens1[i + 2] == ",")
            {
                if(tokens1[i - 1] != "(" && tokens1[i - 1] != ")" && tokens1[i - 1] != "else" && tokens1[i - 1] != "return" && tokens1[i - 1] != "if")
                {
                    //cout<<"\nis var in bracket or at end: " << tokens1[i + 1] << " before: " << tokens1[i - 1] <<endl;
                    if (tokens1[i - 1] == "int" || tokens1[i - 1] == "float" || tokens1[i - 1] == "double" || tokens1[i - 1] == "char")
                    {
                        if (st.find(tokens1[i + 1]) == scope)
                        {
                            cout << "Duplicate identifier declaration in the same scope: " << tokens1[i + 1] << endl;
                        }
                        else
                        {
                            //cout<<"\nis var in bracket or at end, INSERTED: " << tokens1[i + 1] << " scope: " << scope <<endl;
                            st.insert(tokens1[i + 1], scope, "var", tokens1[i - 1], "");
                        }
                    }
                    else if(tokens1[i - 1] == ",")
                    {
                        if(tokens1[i - 4] == "int" || tokens1[i - 4] == "float" || tokens1[i - 4] == "double" || tokens1[i - 4] == "char")
                        {
                            if (st.find(tokens1[i + 1]) == scope)
                            {
                                cout << "Duplicate identifier declaration in the same scope: " << tokens1[i + 1] << endl;
                            }
                            else
                            {
                                //cout<<"\nis var in bracket or at end, INSERTED: " << tokens1[i + 1] << " scope: " << scope <<endl;
                                st.insert(tokens1[i + 1], scope, "var", tokens1[i - 4], "");
                            }
                        }
                    }
                }
            }
        }
        else if (tokens1[i] == "}")
        {
            scope = "global";
        }
    }

    printSymbolTable(st);

    // ====== 4. Detecting Simple Syntax Errors ======

    output_file.open("errors.txt");

    if (!output_file)
    {
        cout << "Error creating file!" << endl;
        return 1;
    }

    int lineno = 1;
    stack<string> parenthesesStack;
    stack<string> curlyBracesStack;
    stack<string> squareBracesStack;
    stack<string> ifStack;
    stack<int> parenthesesLineStack;
    stack<int> curlyBracesLineStack;
    stack<int> squareBracesLineStack;

    // for (int i = 0; i < tokens0.size(); i++)
    // {
    //     cout << "tokens : " << tokens0[i] << endl;
    // }

    for (int i = 0; i < tokens0.size(); i++)
    {
        if (tokens0[i] == "\n")
        {
            lineno++;
        }

        if (tokens0[i] == tokens0[i + 1])
        {
            if (tokens0[i] != "(" && tokens0[i] != ")" && tokens0[i] != "{" && tokens0[i] != "}" && tokens0[i] != "\n")
            {
                output_file << "Duplicate token '" << tokens0[i] << "' at line " << lineno << ", " << endl;
            }
        }

        if (tokens0[i] == "(")
        {
            parenthesesStack.push("(");
            parenthesesLineStack.push(lineno);
        }
        else if (tokens0[i] == ")")
        {
            if (parenthesesStack.empty())
            {
                output_file << "Misplaced '" << tokens0[i] << "' at line " << lineno << ", " << endl;
            }
            else
            {
                parenthesesStack.pop();
                parenthesesLineStack.pop();
            }
        }

        if (tokens0[i] == "{")
        {
            curlyBracesStack.push("{");
            curlyBracesLineStack.push(lineno);
        }
        else if (tokens0[i] == "}")
        {
            if (curlyBracesStack.empty())
            {
                output_file << "Misplaced '" << tokens0[i] << "' at line " << lineno << ", " << endl;
            }
            else
            {
                curlyBracesStack.pop();
                curlyBracesLineStack.pop();
            }
        }

         if (tokens0[i] == "[")
         {
             squareBracesStack.push("[");
             squareBracesLineStack.push(lineno);
         }
         else if (tokens0[i] == "]")
         {
             if (squareBracesStack.empty())
             {
                 output_file << "Misplaced '" << tokens0[i] << "' at line " << lineno << ", " << endl;
             }
             else
             {
                 squareBracesStack.pop();
                 squareBracesLineStack.pop();
             }
         }

        if (tokens0[i] == "if")
        {
            ifStack.push("if");
        }
        else if (tokens0[i] == "else")
        {
            if (ifStack.empty())
            {
                output_file << "Unmatched '" << tokens0[i] << "' at line " << lineno << ", " << endl;
            }
            else
            {
                ifStack.pop();
            }
        }

        if (tokens0[i] == "for")
        {
            //output_file << "token: " << tokens0[i+1] << " " << tokens0[i+2] << endl;
            int forColonCheck = 0;
            if(tokens0[i+1] == "(")
            {
                while (tokens0[i] != ")")
                {
                    if(tokens0[i] == ";")
                    {
                        forColonCheck++;
                    }
                    i++;
                }
                //output_file << "colon: " << forColonCheck << endl;
                if(forColonCheck!=2)
                {
                    output_file << "Unknown syntax of 'for' at line no " << lineno << endl;
                }
            }
            else
            {
                output_file << "Wrong syntax of 'for' at line no " << lineno << endl;
            }
        }
//        if (tokens0[i] == "while")
//        {
//            if(tokens0[i+1] == "(")
//            {
//                int operatorCheck = 0;
//                while (tokens0[i] != ")")
//                {
//                    if(tokens0[i] == "=")
//                    {
//                        if(tokens0[i+1] == "=")
//                        {
//                            operatorCheck++;
//                        }
//                    }
//                    else if(tokens0[i] == "<")
//                    {
//                        if(tokens0[i+1] == "=")
//                        {
//                            operatorCheck++;
//                            continue;
//                        }
//                        operatorCheck++;
//                    }
//                    else if(tokens0[i] == ">")
//                    {
//                        if(tokens0[i+1] == "=")
//                        {
//                            operatorCheck++;
//                            continue;
//                        }
//                        operatorCheck++;
//                    }
//                    else if(tokens0[i] == "!")
//                    {
//                        if(tokens0[i+1] == "=")
//                        {
//                            operatorCheck++;
//                            continue;
//                        }
//                        operatorCheck++;
//                    }
//                    i++;
//                }
//                if(operatorCheck > 1)
//                {
//                    output_file << "Unknown syntax of 'while' at line no " << lineno << endl;
//                }
//            }
//            else
//            {
//                output_file << "Wrong syntax of 'while' at line no " << lineno << endl;
//            }
//        }
    }

    while (!parenthesesStack.empty())
    {
        int line = parenthesesLineStack.top();
        parenthesesStack.pop();
        output_file << "Misplaced '(" << "' at line " << line << ", " << endl;
    }
    while (!curlyBracesStack.empty())
    {
        int line = curlyBracesLineStack.top();
        curlyBracesStack.pop();
        output_file << "Misplaced '{" << "' at line " << line << ", " << endl;
    }
    while (!squareBracesStack.empty())
    {
        int line = squareBracesLineStack.top();
        squareBracesStack.pop();
        output_file << "Misplaced '[" << "' at line " << line << ", " << endl;
    }

    output_file.close();

    input_file.open("errors.txt");
    cout << "\n==== Detecting Simple Syntax Errors Output ====\n\n";
    while ((c = input_file.get()) != EOF)
    {
        cout << c;
    }
    input_file.close();
    cout << "\n\n";

    return 0;
}

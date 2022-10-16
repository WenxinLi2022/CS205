/*
CS205 Project2
12011536 Li Wenxin
2022/10/16
 */

#include "include.hpp"
int getPriority(char op);
int getPriority(char *op);

/* find token; generate postfix expression */
vector<char *> &parse(char *in, vector<Variable> &vars, vector<char *> &token)
{
    stack<char *> opbuf; // a stack buffer to rearrange operators
    while (in != NULL)
    {
        char c = in[0];
        //printf("char = %c\n", c);
        if (isdigit(c))
        {
            int i = 1;
            while (isdigit(in[i]) || in[i] == '.')
            {
                ++i;
            }
            char *digitchar = new char[i + 1](); // remember to delete
            strncpy(digitchar, in, i);
            token.push_back(digitchar);
            //cout << "token.push: " << digitchar << endl;
            in = in + i;
            continue;
        }

        if (isspace(c))
        {
            ++in;
            continue;
        }

        char *opchar = new char[2]();
        switch (c)
        {
        case '-':
        case '+':
        case '*':
        case '/':
        case '^':
            if (!opbuf.empty() && getPriority(c) <= getPriority(opbuf.top()))
            {
                token.push_back(opbuf.top());
                //cout << "token.push: " << opbuf.top() << endl;
                opbuf.pop();
            }
        case '(':
            opchar[0] = c;
            opbuf.push(opchar);
            ++in;
            continue;
            break;

        case ')':
            while (strcmp(opbuf.top(), "("))
            {
                token.push_back(opbuf.top());
                //cout << "token.push: " << opbuf.top() << endl;
                opbuf.pop();
            }
            opbuf.pop();
            ++in;
            continue;
        }
        delete opchar;

        if (isalpha(c))
        {
            /* check whether is a variable or function */
            bool findvar = 0;
            for (int i = 0; i < vars.size(); i++)
            {
                int j = 0;
                while (vars[i].name[j] != '\0' && vars[i].name[j] == in[j])
                {
                    j++;
                }
                if (j > 0)
                {
                    in = in + j - 1;
                    token.push_back(vars[i].name);
                    findvar = 1;
                    //cout << "find variable " << vars[i].name << endl;
                    break;
                }
            }
            if (!findvar)
            {
                cerr << "cannot find variable name!" << endl;
                exit(-1);
            }
            ++in;
            continue;
        }
        else if (c == '\0')
        {
            //cout << "Break" << endl;
            ++in;
            break;
        }
        else
        {
            cerr << "invalid input!" << endl;
            exit(-1);
        }
    }
    /* pop out all operators from the stack */
    while (!opbuf.empty())
    {
        token.push_back(opbuf.top());
        //cout << "token.push: " << opbuf.top() << endl;
        opbuf.pop();
    }
    return token;
}

int getPriority(char op)
{
    switch (op)
    {
    case '+':
    case '-':
        return 10;
    case '*':
    case '/':
        return 20;
    case '^':
        return 30;
    case '(':
    case ')':
        return 0;
    default:
        return -1;
    }
}

int getPriority(char *op)
{
    switch (op[0])
    {
    case '+':
    case '-':
        return 10;
    case '*':
    case '/':
        return 20;
    case '^':
        return 30;
    case '(':
    case ')':
        return 0;
    /* add case func name */
    default:
        return -1;
    }
}
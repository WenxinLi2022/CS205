/*
CS205 Project2
12011536 Li Wenxin
2022/10/16
 */

#include "include.hpp"
bool isop(char c);
int isvar(char *token, vector<Variable> vars);

/* calculate the expression through the postfix expression */
f100 calculate(vector<char *> tokens, vector<Variable> &vars)
{
    stack<f100> numbuf;
    for (int i = 0; i < tokens.size(); i++)
    {
        if (isdigit(tokens[i][0]))
        {
            f100 num(tokens[i]);
            numbuf.push(num);
            //cout << "numbuf.push = " << num << endl;
        }
        else if (isop(tokens[i][0]))
        {
            //cout << "op = " << tokens[i][0] << endl;
            f100 num2 = numbuf.top();
            //cout << "numbuf.pop = " << num2 << endl;
            numbuf.pop();
            f100 num1 = numbuf.top();
            //cout << "numbuf.pop = " << num1 << endl;
            numbuf.pop();
            switch (tokens[i][0])
            {
            case '+':
                numbuf.push(num1 + num2);
                break;
            case '-':
                numbuf.push(num1 - num2);
                break;
            case '*':
                numbuf.push(num1 * num2);
                break;
            case '/':
                numbuf.push(num1 / num2);
                break;
            case '^':
                numbuf.push(pow(num1, num2));
                break;
                return -1;
            }
            //cout << "numbuf.push result = " << numbuf.top() << endl;
        }
        else if (isvar(tokens[i], vars) >= 0)
        {
            f100 num = vars[isvar(tokens[i], vars)].value;
            numbuf.push(num);
            //cout << "numbuf.push from var " << vars[isvar(tokens[i], vars)].name << " = " << num << endl;
        }
    }
    if (!numbuf.empty())
    {
        return numbuf.top();
    }
    else
    {
        return 0;
    }
}

bool isop(char c)
{
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^');
}

int isvar(char *token, vector<Variable> vars)
{
    for (int i = 0; i < vars.size(); i++)
    {
        if (!strcmp(token, vars[i].name))
        {
            return i;
        }
    }
    return -1;
}

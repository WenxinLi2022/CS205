/*
CS205 Project2
12011536 Li Wenxin
2022/10/16
 */

#include "include.hpp"
bool haveequal(const char *in);

/* check for assignment */
f100 assign(char *in, vector<Variable> &vars)
{
    if (haveequal(in))
    {
        char equal[] = "=";
        char *var = NULL;
        stack<char *> varbuf;
        var = strtok(in, equal);
        while (var != NULL)
        {
            //printf("push in \"%s\"\n", var);
            varbuf.push(var);
            var = strtok(NULL, equal);
        }
        f100 varvalue = expr(varbuf.top(), vars);
        varbuf.pop();
        while (!varbuf.empty())
        {
            char *varname = varbuf.top();
            //cout << "varname = " << varname << endl;
            if (!isalpha(varname[0]))
            {
                while (!isalpha(varname[0]))
                {
                    ++varname;
                }
                if (varname == NULL)
                {
                    cerr << "invalid variable name!" << endl;
                    exit(-1);
                }
                else
                {
                    cout << "varname can only start with an alphabet" << endl;
                }
            }
            /* save into a struct */
            Variable *var = new Variable{varname, varvalue};
            vars.push_back(*var);
            cout << varname << " = " << varvalue << endl;
            varbuf.pop();
        }
        // cout << vars.back().name <<endl;
        return varvalue;
    }
    else
    {
        return expr(in, vars);
    }
}

bool haveequal(const char *in)
{
    while (*in)
    {
        if (*in == '=')
        {
            return true;
        }
        ++in;
    }
    return false;
}

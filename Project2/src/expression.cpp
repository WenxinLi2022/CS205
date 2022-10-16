/*
CS205 Project2
12011536 Li Wenxin
2022/10/16
 */

#include "include.hpp"

f100 expr(char *in, vector<Variable> &vars)
{
    vector<char *> token;
    vector<char *> postfix = parse(in, vars, token);
    return calculate(postfix, vars);
}
#pragma once
#include <iostream>
#include <stack>
#include <boost/multiprecision/cpp_bin_float.hpp>

using namespace std;
typedef boost::multiprecision::cpp_bin_float_100 f100;

struct Variable{
    char* name;
    f100 value;
};

f100 assign(char* in,vector<Variable>& vars);
f100 expr(char* in,vector<Variable>& vars);
vector<char*>& parse(char* in, vector<Variable>& vars, vector<char*>& token);
f100 calculate(vector<char *> tokens, vector<Variable> &vars);

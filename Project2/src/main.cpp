/*
CS205 Project2
12011536 Li Wenxin
2022/10/16
 */

#include "include.hpp"

int main()
{
    vector<Variable> vars;
    while (true)
    {
        char *in = new char[BUFSIZ]();
        cin.getline(in, BUFSIZ);
        if (cin.eof())
        { // EOF = Ctrl+D in WSL
            cout << "Exit" << endl;
            break;
        }
        f100 result = assign(in, vars);
        cout << "result = " << result << endl;
    }
}
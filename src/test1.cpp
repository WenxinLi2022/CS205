#include <iostream>
#include "string.h"



using namespace std;



int main(){
/*     while (!cin.eof()){//EOF = Ctrl+D in WSL
        string in;
        getline(cin,in);
        cout <<in.length()<< " in = " << in << endl;
        char* c;
        strcpy(c,in.c_str());
        cout <<strlen(c)<< " c = " << c << endl;
    } */
    // char* in = NULL;
    string s = "bcd";
    // //cout << in[0] << endl;
    cout << s.find("b") << endl;

}
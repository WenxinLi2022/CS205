#include <iostream>
#include <string>
#include <vector>
#include<stdlib.h>


using namespace std;




struct Variable {
    string name;
    double value;
};

//for global variables
struct Calculator{
    vector<Variable> vars;
};

double parse(string in, Calculator cal){
    int findeq = in.find("=");
    if (findeq!=string::npos){
        /* manage assignment */
        if (findeq!=0){
            Variable invar;
            invar.name = in.substr(0,findeq);
            printf("name = %s\n", invar.name.c_str());
            invar.value = parse(in.substr(findeq+1),cal);
            printf("value = %f\n", invar.value);
            cal.vars.push_back(invar);
            return invar.value;
        }else{
            cerr << "Error: Invalid variable assignment!" << endl;
            exit(0);
        }
    }else{
        /* manage expression */
        return stod(in);
    }
}


int main(){
    /* for testing */
    string in = "x=y=1.2";
    Calculator cal;
    double result = parse(in,cal);
    cout << "result = " << result << endl;

/*     while (!cin.eof()){//EOF = Ctrl+D in WSL
        string in;
        getline(cin,in);
        string result = parse(in);
        if (!result.empty()){
            cout << result << endl;
        }
    } */

}


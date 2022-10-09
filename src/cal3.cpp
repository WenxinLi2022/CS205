#include <iostream>
#include <string.h>
#include <string>
#include <vector>
#include <stdlib.h>


using namespace std;


struct Token{
    char type;
    double value;
};

struct Variable {
    string name;
    double value;
};

//for global variables
struct Calculator{
    vector<Variable> vars;
    bool flag;
};


Token getToken(char *inc){
    Token currToken;
    currToken.value = 0;
    while(true) {
        char c = inc[0];
        printf("char = %c\n",c);
        if (isdigit(c)) {
            double value = c - '0';
            int i=1;
            while (isdigit(c = inc[i])){
                value = value*10 + (c - '0');
                i++;
            }
            if (c=='.'){
                i++;
                while (isdigit(c = inc[i])){
                    value = value*10 + (c - '0');
                    i++;
                }
            }
            printf("char = %c\n",c);
            currToken.type = 'n';
            currToken.value = value;
            return currToken;
        }

        if (isspace(c))
            continue;

        switch (c) {
            case '+':
            case '-':
            case '*':
            case '/':
            case '%':
            case '^':
            case '(':
            case ')':
            case EOF:
                curr.type = c;
                return curr;
        }

        if (isprint(c))
            error("Unexpected character: \"%c\"", (char)c);
        else
            error("Unexpected character: 0x%2X", (unsigned int)c);
    }
    
}

double expr(int prec, char* inc, Calculator cal){
    Token currToken = getToken(inc);

}

double parse(string in, Calculator cal){
    if(in.empty()){
        cal.flag=1;
        cerr << "Error: Missing tokens!" << endl;
        return 0;
    }
    int findeq = in.find("=");
    if (findeq!=string::npos){
        /* manage assignment */
        if ((findeq!=0)/*&&in.length()>findeq+1*/){
            Variable invar;
            invar.name = in.substr(0,findeq);
            if(!isalpha(invar.name[0])){
                cerr << "Error: Variable name should start with a letter!" << endl;
                cal.flag = 1;
                return 0;
            }
            printf("name = %s\n", invar.name.c_str());
            invar.value = parse(in.substr(findeq+1),cal);
            printf("value = %f\n", invar.value);
            cal.vars.push_back(invar);
            return invar.value;
        }else{
            cerr << "Error: Invalid variable assignment!" << endl;
            cal.flag = 1;
            return 0;
        }
    }else{
        /* manage expression */
        char* inc;
        strcpy(inc,in.c_str());
        return expr(0,inc,cal);
    }
}


int main(){
    /* for testing */
    string in = "x=y=1.2";
    Calculator cal;
    cal.flag = 0;
    double result = parse(in,cal);
    if (cal.flag!=0){
    cout << "result = " << result << endl;}

/*     while (!cin.eof()){//EOF = Ctrl+D in WSL
        string in;
        getline(cin,in);
        string result = parse(in);
        if (!result.empty()){
            cout << result << endl;
        }
    } */

}


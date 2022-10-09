#include <iostream>
#include <string.h>
#include <string>
#include <vector>
#include <stdlib.h>
#include<math.h>


using namespace std;


struct Token{
    char type;
    double value;
};

struct Variable {
    string name;
    double value;
};

struct Function {
    int index;
    string name;
};

//for global variables
const int NUM_FUNC = 2;
struct Calculator{
    bool flag;
    char* inc; //currently input string
    vector<Variable> vars;
    Function funcs[NUM_FUNC] ={{0,"sqrt("},{1,"abs("}}; 
};


Token getToken(Calculator cal){
    Token currToken;
    currToken.value = 0;
    char* inc = cal.inc;
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
                int loc=i;
                i++;
                while (isdigit(c = inc[i])){
                    value = value + (c - '0')*pow(10,loc-i);
                    i++;
                }
            }
            printf("value = %f\n",value);
            currToken.type = 'n';
            currToken.value = value;
            cal.inc=cal.inc+i;
            return currToken;
        }

        if (isspace(c))
            continue;

        switch (c) {
            case '+':
            case '-':
            case '*':
            case '/':
            // case '%':
            // case '^':
            case '(':
            case ')':
            case EOF:
                currToken.type = c;
                return currToken;
        }

        if (isalpha(c)){
            string ins(inc);
            for (int i =0;i<NUM_FUNC;i++){
                if(!ins.find(cal.funcs[i].name)){
                    
                }
            }
        }
            
        else{
            cerr << "Error: Unexpected character!" << endl;
            cal.flag = 1;
            return currToken;}
    }
    
}

double expr(int prec, Calculator cal){
    Token currToken = getToken(cal);

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
        strcpy(cal.inc,in.c_str());
        return expr(0,cal);
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


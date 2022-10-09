#include <iostream>
#include <string.h>

using namespace std;






string parse(char* in);

int main(){
    char in[1000];
    while (!cin.eof()){
        cin.getline(in,1000); //EOF = Ctrl+D in WSL
        //printf("%s %lud\n",in,strlen(in));
        string result = parse(in);
        if (!result.empty()){
            cout << result << endl;
        }
    }
}

string parse(char* in){
    char* equal = "=";
    char* assign = strstr(in,equal);
    if (assign!=NULL){
        
    }
}
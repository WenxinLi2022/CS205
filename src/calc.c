// (Public domain, created by Ryan Johnson)
// A simple calculator in C using Pratt parsing.
// compile with `cc -lm calc.c`

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <inttypes.h>
#include <ctype.h>
#include <math.h>


typedef double f64;

typedef struct {
    int type;
    f64 num;
} Token;

// The current token.
Token curr;

// Exit with an error.
void error(const char *fmt, ...) {
    va_list list;
    va_start(list, fmt);
    vfprintf(stderr, fmt, list);
    fputc('\n', stderr);
    exit(1);
}

// Get the next token from stdin.
Token next(void) {
    for (;;) {
        int c = getchar();
        printf("char = %c\n",c);
        if (isdigit(c)) {
            f64 num = 0;
            do num = num * 10 + (c - '0');
            while (isdigit(c = getchar()));
            ungetc(c, stdin);
            curr.type = 'n';
            curr.num = num;
            return curr;
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

// Return the current Token as a string.
const char *str_type(void) {
    switch (curr.type) {
        case '+':
            return "'+'";
        case '-':
            return "'-'";
        case '*':
            return "'*'";
        case '/':
            return "'/'";
        case '%':
            return "'%'";
        case '^':
            return "'^'";
        case 'n':
            return "number";
        case '(':
            return "'('";
        case ')':
            return "')'";
        case EOF:
            return "EOF";
    }
    return NULL;
}

// Production Rules:
// add: mul (("+" | "-") mul)* ;
// mul: unary (("*" | "/" | "%") unary)* ;
// unary: ("+" | "-")? exp ;
// exp: primary ("^" exp)? ;
// primary: number | "(" add ")" ;

double expr(int rbp);

// Null detonate current token.
// This handles prefix operators.
double nud(void) {
    double v;
    switch (curr.type) {
        case 'n':
            v = curr.num;
            next(); // skip number
            return v;
        case '-':
            next(); // skip '-'
            return -expr(30);
        case '(':
            next(); // skip '('
            v = expr(0);
            if (curr.type != ')')
                error("Expected ')', found %s", str_type());
            next(); // skip ')'
            return v;
        default:
            error("Expected '(', '-', number, found %s", str_type());
    }
}

// Left detonate current token.
// This handles infix (and postfix) operators.
double led(double left) {
    switch (curr.type) {
        case '+':
            next();
            return left + expr(10);
        case '-':
            next();
            return left - expr(10);
        case '*':
            next();
            return left * expr(20);
        case '/':
            next();
            return left / expr(20);
        /* case '%':
            next();
            return fmod(left, expr(20));
        case '^':
            next();
            // This binding power prevents the expr loop from exiting immediately, which is what we
            // need for right associative operators.
            return pow(left, expr(40 - 1)); */
        default:
            error("Expected operator, found %s", str_type());
    }
}

// Binding power of current infix/postfix operator.
int bp(void) {
    switch (curr.type) {
        case '+':
        case '-':
            return 10;
        case '*':
        case '/':
        case '%':
            return 20;
        case '^':
            return 40;
        case ')':
        case EOF:
            return 0;
        default:
            error("Unexpected token %s", str_type());
    }
}

// Parse expression with the given binding power.
// If we encounter an operator with a binding power >= the given binding
// power, then exit and return the accumulated value.
double expr(int rbp) {
    double left = nud();
    
    while (rbp < bp())
        left = led(left);
    printf("left = %f\n", left);
    return left;
}

int main(void) {
    next(); // Initialize first token
    printf("expr = %.2f\n", expr(0));
}
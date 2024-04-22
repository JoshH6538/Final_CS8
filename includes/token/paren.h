#ifndef PAREN_H
#define PAREN_H
#include "token.h"

class LeftParen : public Token
{
public:
    LeftParen();
    LeftParen(string op);
    LeftParen(const char* op);

     //accessors
    /*virtual*/ string get_tokenstr();

    //mutators
    /*virtual*/ void set_tokenstr(string s);
private:
    string _token;
};

class RightParen : public Token
{
public:
    RightParen();
    RightParen(string op);
    RightParen(const char* op);

     //accessors
    /*virtual*/ string get_tokenstr();

    //mutators
    /*virtual*/ void set_tokenstr(string s);
private:
    string _token;
};



#endif
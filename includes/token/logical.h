#ifndef LOGICAL_H
#define LOGICAL_H
#include "token.h"

class Logical : public Token
{
public:
    Logical();
    Logical(string op);
    Logical(const char* op);

     //accessors
    /*virtual*/ string get_tokenstr();

    //mutators
    /*virtual*/ void set_tokenstr(string s);
private:
    string _token;
};

class And : public Token
{
public:
    And();
    And(string op);
    And(const char* op);

     //accessors
    /*virtual*/ string get_tokenstr();

    //mutators
    /*virtual*/ void set_tokenstr(string s);
private:
    string _token;
};

class Or : public Token
{
public:
    Or();
    Or(string op);
    Or(const char* op);

     //accessors
    /*virtual*/ string get_tokenstr();

    //mutators
    /*virtual*/ void set_tokenstr(string s);
private:
    string _token;
};

#endif
#ifndef RELATIONAL_H
#define RELATIONAL_H
#include "token.h"

class Relational : public Token
{
public:
    Relational();
    Relational(string op);
    Relational(const char* op);
     //accessors
    /*virtual*/ string get_tokenstr();

    //mutators
    /*virtual*/ void set_tokenstr(string s);
private:
    string _token;
};

#endif
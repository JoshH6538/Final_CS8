#ifndef TOKENSTR_H
#define TOKENSTR_H
#include "token.h"

class TokenStr : public Token
{
public:
    TokenStr();
    TokenStr(string s);
    TokenStr(const char* c);

    //accessors
    /*virtual*/ string get_tokenstr();

    //mutators
    /*virtual*/ void set_tokenstr(string s);

private:
    string _token;
};



#endif
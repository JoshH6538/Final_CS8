#ifndef STOKEN_H
#define STOKEN_H
#include <iostream>
#include "constants.h"
using namespace std;

class SToken
{
public:
    SToken();
    SToken(string str, int type);
    SToken(string str);
    friend ostream& operator <<(ostream& outs, const SToken& t);
    int type() const;
    string type_string() const;
    string token_str() const;
    void set_type();
private:
    string _token;
    int _type;
};

#endif
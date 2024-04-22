#ifndef STOKENIZE_H
#define STOKENIZE_H
#include <iostream>
#include "token.h"
#include "constants.h"
#include <string.h>
using namespace std;
class STokenizer
{
public:
    STokenizer();
    STokenizer(char str[]);
    STokenizer(const char str[]);
    bool done();            //true: there are no more tokens
    bool more();            //true: there are more tokens
    //

    //---------------
    //extract one token (very similar to the way cin >> works)
    friend STokenizer& operator >> (STokenizer& s, SToken& t);

    //set a new string as the input string
    void set_string(char str[]);

private:
    bool one_more;
    bool _done;
    //create table for all the tokens we will recognize
    //                      (e.g. doubles, words, etc.)
    void make_table(int _table[][MAX_COLUMNS]);

    //extract the longest string that match
    //     one of the acceptable token types
    bool get_token(int& start_state, string& token);
    //---------------------------------
    char _buffer[MAX_BUFFER];       //input string
    int _pos;                       //current position in the string
    static int _table[MAX_ROWS][MAX_COLUMNS];
};

#endif
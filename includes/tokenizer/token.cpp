#include "token.h"

SToken::SToken()
{_token="";_type=0;}

SToken::SToken(string str, int type)
{_token=str;_type=type;}

SToken::SToken(string str):_type(0)
{_token=str;}

int SToken::type() const {return _type;}

ostream& operator <<(ostream& outs, const SToken& t)
{
    outs<<"|"<<t.token_str()<<"|";
    return outs;
}

string SToken::type_string() const
{
    switch(_type)
    {
        case 1:
            return "NUMBER";
            break;
        case 2:
            return "ALPHA";
            break;
        case 3:
            return "SPACE";
            break;
        case 4:
            return "OPERATOR";
            break;
        case 5:
            return "PUNC";
            break;
        default:
            return "UNKNOWN";
            break;
    }
}

string SToken::token_str() const
{return _token;}

void SToken::set_type()
{
    
    //loop for each element in string
    for(int i=0;i<_token.length();i++)
    {
        //check for comma
        for(int j=0;COMMA[j];j++)
            if(_token[i]==COMMA[j])
            {
                _type=TOKEN_COMMA;
                return;
            }
        //check for star
        for(int j=0;STARS[j];j++)
            if(_token[i]==STARS[j])
            {
                _type=TOKEN_STAR;
                return;
            }
        //check for REL
        for(int j=0;REL[j];j++)
            if(_token[i]==REL[j])
            {
                _type=TOKEN_REL;
                return;
            }
        //check for PAREN
        for(int j=0;PAREN[j];j++)
            if(_token[i]==PAREN[j])
            {
                _type=TOKEN_PAREN;
                return;
            }
        //check for space
        for(int j=0;SPACES[j];j++)
            if(_token[i]==SPACES[j])
            {
                _type=TOKEN_SPACE;
                return;
            }
        //check for quotes
        for(int j=0;QUOTES[j];j++)
            if(_token[i]==QUOTES[j])
            {
                _type=TOKEN_QUOTES;
                return;
            }
        //check for number
        for(int j=0;DIGITS[j];j++)
            if(_token[i]==DIGITS[j])
            {
                _type=TOKEN_NUMBER;
                return;
            }
        //check for punc
        for(int j=0;PUNC[j];j++)
            if(_token[i]==PUNC[j])
            {
                _type=TOKEN_PUNC;
                return;
            }
        //check for alpha
        for(int j=0;ALFA[j];j++)
            if(_token[i]==ALFA[j])
            {
                _type=TOKEN_ALPHA;
                return;
            }
            
    }
}

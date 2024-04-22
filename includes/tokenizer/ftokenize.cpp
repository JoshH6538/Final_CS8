#include "ftokenize.h"

FTokenizer::FTokenizer(char* fname)
{
    _f.open(fname);
    if (_f.fail()) 
    {
        cout<<"Failed to open "<<fname<<".\n";
        exit(1);
    }
    _blockPos=-1;
    _pos=-1;
    _more=true;
}

SToken FTokenizer::next_token()
{
    SToken t;
    if(_stk.done())
    {
        cout<<"Getting new block!"<<endl;
        if(!get_new_block()) {
            _more=false;
            //return;
        }
    }
    _stk>>t;
    string token=t.token_str();
    //cout<<token<<"   "<<token.length()<<endl;
    //cout<<_pos<<_blockPos<<endl;
    _pos+=token.length();
    _blockPos+=token.length();
    //cout<<"token: "<<t.token_str()<<"     "<<_pos<<"   "<<_blockPos<<endl;
    return t;
}

bool FTokenizer::more(){return _more;}

int FTokenizer::pos(){return _pos;}

int FTokenizer::block_pos(){return _blockPos;}


FTokenizer& operator >> (FTokenizer& f, SToken& t)
{
    //if(!f.get_new_block()) f._more=false;
    t=f.next_token();
    t.set_type();
}

bool FTokenizer::get_new_block()
{
    _blockPos=-1;
    char buffer[MAX_BLOCK+1];   //declare
    _f.read(buffer,MAX_BLOCK); 
    //at this point, the mumber of chars read is _f.gcount();
    buffer[_f.gcount()] = '\0';
    cout<<_f.gcount()<<endl;
    //at this point, buffer has a valid cstring.
    //if _f.gcount() is zero, get new block is false.
    if(_f.gcount()==0) {cout<<"No more blocks!"<<endl; 
    return false;}
    _stk.set_string(buffer);
    // for(int i=0;buffer[i]!='\0';i++) cout<< buffer[i];
    // cout<<endl<<endl;
    return true;
}
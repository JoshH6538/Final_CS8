#include "stokenize.h"
#include "state_machine_functions.h"

int STokenizer::_table[MAX_ROWS][MAX_COLUMNS];

STokenizer::STokenizer()
{
    _pos=0;
    make_table(_table);
    for(int i=0;i<MAX_BUFFER;i++)
    {
        _buffer[i]='\0';
    }
}

STokenizer::STokenizer(char str[])
{
    make_table(_table);
    _pos=0;
    for(int i=0;i<MAX_BUFFER;i++)
    {
        _buffer[i]='\0';
    }
    for(int i=0;str[i];i++)
    {
        _buffer[i]=str[i];
    }
}

STokenizer::STokenizer(const char str[])
{
    make_table(_table);
    _pos=0;
    for(int i=0;i<MAX_BUFFER;i++)
    {
        _buffer[i]='\0';
    }
    for(int i=0;str[i];i++)
    {
        _buffer[i]=str[i];
    }
}

bool STokenizer::done(){return _done;}

bool STokenizer::more(){return !_done;}

STokenizer& operator >> (STokenizer& s, SToken& t)
    {
        if(s._buffer[s._pos]=='\0') 
        {
            // cout<<"\t\tdone at pos: "<<s._pos<<endl;
            s._done=true;
        }

        int start_state=0;
        string tokenstr="";
        s.get_token(start_state, tokenstr);
        //if(tokenstr=="") s._pos++;
        t = SToken(tokenstr, 0);
        t.set_type();
        //cout<<"token: "<<t<<endl;

        // if(s.one_more)
        // {
        //     //cout<<"stopping"<<endl; 
        //     s._done=true;
        // }
        
        return s;
    }

bool STokenizer::get_token(int& start_state, string& token)
{
    int last_success_pos=_pos;
    int state=start_state;
    string temp = "";
    //cout<<_buffer[_pos]<<endl;
    //cout<<boolalpha<<(_buffer[_pos] != 0)<<endl; 
    //cout<<"pos: " <<_pos<<endl; 
    //cout<<"start char: "<<_buffer[_pos]<<endl;
    while(_buffer[_pos] > 0 && state>=0)
    {
        //cout<<"here\n";
        //cout<<"pos: " <<_pos<<endl; 
        state=_table[state][_buffer[_pos]];
        temp+=_buffer[_pos];
        //cout<<"state: "<<state<<endl;
        //cout<<"char: "<<_buffer[_pos]<<endl; 
        //if fails
        
        //if success
        if(is_success(_table, state))
        {
            last_success_pos=_pos;
            token+=temp;
            //cout<<"token: "<<token<<endl; 
            //cout<<"last success pos: "<<_buffer[last_success_pos]<<endl; 
            temp="";
        }
        _pos++;
    }
    
    //cout<<"here\n"; 
    //cout<<"last success pos: "<<_buffer[last_success_pos]<<endl; 
    if(!token.empty())
    {
        //out<<"token: "<<token<<endl; 

        _pos = last_success_pos+1;
        return true;
    }
    else{
        _pos = last_success_pos + 1; 
        return false;
    } 

    
    
    /*
    int last_success_state=-1, last_success_pos=0;
    int token_size=0;
    char curr;
    int start_pos=_pos;
    int state=0;
    int type=0;
    bool run=true;
    //loop for each element in string  
    for(int i=_pos;_buffer[i]!=NULL;i++)
    {
        //asign current character
        curr=_buffer[i];
        int value = curr;

        //cout<<"curr: "<<curr<<"  :  "<<last_success_state<<" "<<last_success_pos<<endl;

        //check for failure
        if(_table[state][value]==TOKEN_UNKNOWN || (_buffer[i] && _buffer[i]=='\0'))
        {
    
            if(_buffer[i]=='\0') last_success_pos++;
            //if a success state was never reached
            if(last_success_state<=0)
            {
                //leave token blank and return false
                token="";
                if(_buffer[i]=='\0') run=false;
                return false;
            }
            //if a success state was reached
            else
            {
                token_size=last_success_pos-start_pos;
                token="";
                for(int i=start_pos;i<=last_success_pos;i++)
                {
                    token+=_buffer[i];
                }
                _pos=last_success_pos+1;
                //cout<<token<<endl;
                return true;
            }
        }
        //check for not success and success
        else
        {
            //check if token is 1 char long
            if(_pos==start_pos && _buffer[i+1])
            {
                //see if next char is fail or different token type
                if(_table[state][_buffer[i+1]]==TOKEN_UNKNOWN || _table[state][_buffer[i+1]]!=_table[state][_buffer[i]])
                {
                    int curr_state=_table[state][_buffer[i]];
                    int next_state=_table[state][_buffer[i+1]];
                    if(next_state>curr_state+20)
                    {
                        if(_table[state][_buffer[i+2]] && _table[state][_buffer[i+2]]<=3 && _table[state][_buffer[i+2]]>=0) ;
                        else continue;
                    }
                    //cout<<"current: "<<curr_state<<"  next: "<<next_state<<endl;
                    //if( _table[state][_buffer[i+2]] && _table[state][_buffer[i+2]]!=)
                    state=_table[state][value];
                }
            }
            //check for not success
            if(_table[state][0]==0)
            {
                //cout<<"not success\n";
                //cout<<"current: "<<curr<<"  state: "<<state<<endl;
                //assign new state
                state=_table[state][value];
                //cout<<"current: "<<curr<<"  new state: "<<state<<" sucess: "<<_table[state][0]<<endl;
                _pos++;
                //cont. to prevent updated state from triggering twice
                continue;
            }
            //check for success
            if(_table[state][0]==1)
            {
                
                
                //assign new last success state and pos
                last_success_state=state;
                last_success_pos=_pos;
                
                _pos++;

                //assign new state
                //cout<<"current: "<<curr<<"  state: "<<state<<endl;
                state=_table[state][value];
                //cout<<"current: "<<curr<<"  new state: "<<state<<" sucess: "<<_table[state][0]<<endl;
                //increase size of token
                //cont. to prevent updated state from triggering twice
                continue;
            }
        }
    }
    //cout<<start_pos<<"   "<<last_success_pos<<endl;
    //if(_buffer[start_pos+1]==NULL)
        token+=_buffer[start_pos];
    if(_buffer[start_pos+2]!=NULL)
        for(int i=start_pos+1;_buffer[i]!=NULL;i++)
            token+=_buffer[i];
    //else
        //for(int i=start_pos;_buffer[i]!=NULL;i++)
            //token+=_buffer[i];
    //cout<<"SToken not returned   curr:"<<curr<<endl;
    //cout<<"token: "<<token<<endl;
    */

   _pos = last_success_pos + 1; 
    return true;
}


void STokenizer::set_string(char str[])
{
    _pos=0;
    for(int i=0;i<sizeof(_buffer);i++)
        _buffer[i]='\0';
    for(int i=0;str[i];i++)
        _buffer[i]=str[i];
    _done=false;
}

void STokenizer::make_table(int _table[][MAX_COLUMNS])
{
    //initialize table
    init_table(_table);

    //set success and not success
    mark_fail(_table,0);
    mark_success(_table, 1);
    mark_success(_table, 3);
    mark_success(_table, 10);
    mark_success(_table, 20);
    mark_success(_table, 30);
    mark_success(_table, 40);
    mark_success(_table, 50);
    mark_success(_table, 60);
    mark_success(_table, 70);
    mark_success(_table, 80);

    mark_fail(_table, 2);

    //set states for NUMBER
    mark_cells(0,_table,DIGITS,1);
    mark_cells(1,_table,DIGITS,1);
    mark_cell(1,_table,'.',2);
    mark_cells(2,_table,DIGITS,3);
    mark_cells(3, _table, DIGITS,3);
    
    //set states for ALPHA
    mark_cells(0,_table,ALFA,10);
    mark_cells(10,_table,ALFA,10);
    
    //set states for SPACE
    mark_cells(0,_table,SPACES,20);
    mark_cells(20,_table,SPACES,20);

    //set states for PUNC
    mark_cells(0,_table,PUNC,30);
    mark_cells(30,_table,PUNC,30);

    //set states for QUOTES
    mark_cells(0,_table,QUOTES,40);
    mark_cells(40,_table,QUOTES,40);

    //set states for COMMA
    mark_cells(0,_table,COMMA,50);
    mark_cells(50,_table,COMMA,50);

    //set states for STARS
    mark_cells(0,_table,STARS,60);
    mark_cells(60,_table,STARS,60);

    //set states for REL
    mark_cells(0,_table,REL,70);
    mark_cells(70,_table,REL,70);

    //set states for PAREN
    mark_cells(0,_table,PAREN,80);
    //mark_cells(80,_table,PAREN,80);

}
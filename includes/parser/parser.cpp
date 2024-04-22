#include "parser.h"

//----------------------------------------
//               C T O R S
//----------------------------------------
Parser::Parser()
{

}
Parser::Parser(string s)
{
    _buffer=s;
    make_table(ptable);
    build_keyword_list();
    set_string(s);
}
Parser::Parser(const char* c)
{
    _buffer=c;
    make_table(ptable);
    // cout<<"Printing"<<endl;
    // print_table(ptable);
    build_keyword_list();
    set_string(c);
}

mmap_ss Parser::parse_tree()
{
    return ptree;
}

void Parser::set_string(const string& s)
{
    string temp="";
    _token_queue.clear();
    STokenizer stk(s.c_str());
    SToken t;
    stk>>t;
    while(stk.more())
    {
        _statement+=t.token_str();
        switch(t.type())
        {
            case TOKEN_SPACE:
                stk>>t;
                break;
            case TOKEN_QUOTES:
                temp="";
                stk>>t;
                while(t.type()!=TOKEN_QUOTES)
                {
                    temp+=t.token_str();
                    stk>>t;
                }
                _token_queue.push_back(temp);
                stk>>t;
                break;
            case TOKEN_COMMA:
            case TOKEN_ALPHA:
            case TOKEN_NUMBER:
            case TOKEN_STAR:
            case TOKEN_OPERATOR:
            case TOKEN_PAREN:
            case TOKEN_REL:
                _token_queue.push_back(t.token_str());
            default:   
                stk>>t;
                break;
        }
    }
    // cout<<"statement: "<<_statement<<endl;
    _fail_flag=get_parse_tree();
}

void Parser::build_keyword_list()
{
    _keyword_list.insert("make",MAKE);
    _keyword_list.insert("create",MAKE);
    _keyword_list.insert("table",TABLE);
    _keyword_list.insert("fields",FIELDS);
    _keyword_list.insert("insert",INSERT);
    _keyword_list.insert("into",INTO);
    _keyword_list.insert("make",MAKE);
    _keyword_list.insert("select",SELECT);
    _keyword_list.insert("*",STAR);
    _keyword_list.insert("from",FROM);
    _keyword_list.insert("col",COL);
    _keyword_list.insert(",",COMMAS);
    _keyword_list.insert("values",VALUES);
    _keyword_list.insert("where",WHERE);
    _keyword_list.insert("=",RELATIONAL);
    _keyword_list.insert(">",RELATIONAL);
    _keyword_list.insert("<",RELATIONAL);
    _keyword_list.insert(">=",RELATIONAL);
    _keyword_list.insert("<=",RELATIONAL);
    _keyword_list.insert("(",LP);
    _keyword_list.insert(")",RP);
}

bool Parser::get_parse_tree()
{
    ptree.clear();
    int state=0;
    SToken temp;
    for(int i=0;i<_token_queue.size();i++)
    {
        // cout<<ptree<<endl;
        string token=_token_queue[i];
        // cout<<"token: "<<token<<" : ";
        // cout<<get_column(token)<<endl;
        state=ptable[state][get_column(token)];
        // cout<<token<<" : "<<state<<endl;
        switch(state)
        {
            //make state machine
            case 1:
                ptree["command"]+=token;
                break;
            case 3:
                ptree["table_name"]+=token;
                break;
            case 5:
                ptree["col"]+=token;
                break;

            //insert state machine
            case 10:
                ptree["command"]+=token;
                break;
            case 12:
                ptree["table_name"]+=token;
                break;
            case 14:
                ptree["values"]+=token;
                break;
            
            //select state machine no cond
            case 20:
                ptree["command"]+=token;
                break;
            case 21:
                ptree["fields"]+=token;
                break;
            case 23:
                ptree["table_name"]+=token;
                break;
            case 24:
                ptree["fields"]+=token;
                break;
            //select state machine w/ cond
            case 30:
                if(!ptree.contains("where"))
                {
                    ptree["where"]+=string("yes");
                }
                break;
            case 31:
            case 32:
            case 33:
                ptree["condition"]+=token;
                break;
            //select with where
            case 34:
            case 35:
            case 36:
            case 37:
            case 38:
            case 39:
            case 40:
            case 45:
            case 46:
                ptree["condition"]+=token;
                break;
            default:
                break;
        }
    }
    // cout<<ptree<<endl;
    // cout<<"PTREE:\n"<<ptree<<endl;
    if(is_success(ptable,state))return true;
    else
    {
        ptree.clear();
        return false;
    }
}

int Parser::get_column(string token)
{
    if(_keyword_list.contains(token))
    {
        return _keyword_list[token];
    }       
    else return SYM;
}

void Parser::make_table(int ptable[][MAX_COLUMNS])
{
    init_table(ptable);
    mark_fail(ptable,0);

    // M A K E  /  C R E A T E
    //set success / not success
    mark_fail(ptable,1);
    mark_fail(ptable,2);
    mark_fail(ptable,3);
    mark_fail(ptable,4);
    mark_success(ptable,5);
    mark_fail(ptable,6);
    
    //state machine
    mark_cell(0,ptable,MAKE,1);
    mark_cell(1,ptable,TABLE,2);
    mark_cell(2,ptable,SYM,3);
    mark_cell(3,ptable,FIELDS,4);
    mark_cell(4,ptable,SYM,5);
    mark_cell(5,ptable,COMMAS,6);
    mark_cell(6,ptable,SYM,5);

    // I N S E R T
    //set success / not success
    mark_fail(ptable,10);
    mark_fail(ptable,11);
    mark_fail(ptable,12);
    mark_fail(ptable,13);
    mark_success(ptable,14);
    mark_fail(ptable,15);
    //state machine
    mark_cell(0,ptable,INSERT,10);
    mark_cell(10,ptable,INTO,11);
    mark_cell(11,ptable,SYM,12);
    mark_cell(12,ptable,VALUES,13);
    mark_cell(13,ptable,SYM,14);
    mark_cell(14,ptable,COMMAS,15);
    mark_cell(15,ptable,SYM,14);

    // S E L E C T  W/O  C O N D
    //set success / not success
    mark_fail(ptable,20);
    mark_fail(ptable,21);
    mark_fail(ptable,22);
    mark_fail(ptable,24);
    mark_fail(ptable,25);
    mark_success(ptable,23);

    //state machine
    mark_cell(0,ptable,SELECT,20);
    mark_cell(20,ptable,STAR,21);
    mark_cell(21,ptable,FROM,22);
    mark_cell(22,ptable,SYM,23);
    mark_cell(20,ptable,SYM,24);
    mark_cell(24,ptable,COMMAS,25);
    mark_cell(25,ptable,SYM,24);
    mark_cell(24,ptable,FROM,22);
    // S E L E C T  W/  C O N D
    mark_fail(ptable,30);
    mark_success(ptable,31);
    mark_cell(23,ptable,WHERE,30);

    mark_cell(30,ptable,SYM,31);
    mark_cell(30,ptable,RELATIONAL,31);
    mark_cell(30,ptable,COMMAS,31);
    mark_cell(30,ptable,QUOTE,31);
    mark_cell(30,ptable,LP,31);
    mark_cell(30,ptable,RP,31);

    mark_cell(31,ptable,SYM,31);
    mark_cell(31,ptable,RELATIONAL,31);
    mark_cell(31,ptable,COMMAS,31);
    mark_cell(31,ptable,QUOTE,31);
    mark_cell(31,ptable,LP,31);
    mark_cell(31,ptable,RP,31);
}


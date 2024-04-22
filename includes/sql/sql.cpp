#include "sql.h"

SQL::SQL()
{

}

Table SQL::command(const string& s)
{
    // cout<<"main command"<<endl<<"============="<<endl;;
    str_to_parser(s);
    // cout<<ptree<<endl;
    return run_command();
}

void SQL::str_to_parser(const string& s)
{
    Parser parser(s.c_str());
    ptree=parser.parse_tree();
}

Table SQL::run_command()
{
    vectorstr command=ptree["command"];
    vectorstr table_name=ptree["table_name"];
    // cout<<"command: "<<command<<endl;
    // cout<<command.size()<<endl;

    //make / create
    if(command.size()<1 || table_name.size()<1)
    {
        cout<<"Didn't recognise command."<<endl;
        return Table();
    }
    if(command[0]=="make" || command[0]=="create")
    {
        vectorstr cols=ptree["col"];
        Table t(table_name[0],cols);
        cout<<"Table: "<<table_name[0]<<" created."<<endl;
        return t;
    }
    //insert
    else if(command[0]=="insert")
    {
        vectorstr values=ptree["values"];
        Table t(table_name[0]);
        t.insert_into(values);
        return t;
    }
    //select
    else if(command[0]=="select")
    {
        Table t(table_name[0]);
        vectorstr fields=ptree["fields"];
        //case 1: *, select all
        if(fields[0]=="*")
        {
            if(!ptree.contains("where"))
            {
                cout<<"There is no condition."<<endl;
                _select_recnos=t.select_recnos();
                t=t.select_all();
                // cout<<"here"<<endl;
                return t;
            }
            else
            {
                vectorstr condition=ptree["condition"];
                // cout<<"CONDITION: "<<condition<<endl;
                fields=t.get_fields();
                // cout<<condition<<endl;

                t=t.select_all(fields);
                t=t.select(fields,condition);
                _select_recnos=t.select_recnos();
                return t;
            }
        }
        //case 1: fields, select
        else
        {
            if(!ptree.contains("where"))
            {
                cout<<"There is no condition."<<endl;
                _select_recnos=t.select_recnos();
                return t.select_all(fields);
            }
            else
            {
                vectorstr condition=ptree["condition"];
                // cout<<condition<<endl;
                t=t.select_all(fields);
                t=t.select(fields,condition);
                _select_recnos=t.select_recnos();
                return t;
            }
        }
    }
    else
    {
        cout<<"Didn't recognise command."<<endl;
        return Table();
    }
}

vector<long> SQL::select_recnos()
{
    return _select_recnos;
}

void SQL::welcome()
{
    cout<<"========================================================================================\n"
        <<setw(35)      <<                       "SQL DATABASE"                             <<endl
        <<"========================================================================================\n";
    cout<<"Please enter a sql command. To exit, input \"exit\". For command structure, input \"help\"."<<endl;
}

void SQL::runtime()
{
    bool exit=false;
    string cmd_line;
    while(!exit)
    {
        cmd_line=get_input();
        cout<<"commmand: "<<cmd_line<<endl;
        if(cmd_line=="exit") exit=true;
        else if(cmd_line=="help")
        {
            cout<<"Instructions here."<<endl;
        }
        else
        {
            command(cmd_line);
        }
    }
}

string SQL::get_input()
{
    cout<<"\n$ ";
    string value;
    getline(cin, value);
    return value;
}
#ifndef SQL_H
#define SQL_H
using namespace std;
#include "../table/table.h"
#include "../parser/parser.h"
#include <filesystem>

class SQL
{
public:
    SQL();
    Table command(const string& s);
        void str_to_parser(const string& s);
        Table run_command();
        
    vector<long> select_recnos();
    
    //interface
    void welcome();
    void runtime();
    string get_input();
private:
    vector<long> _select_recnos;
    MMap<string, string> ptree;
};

#endif
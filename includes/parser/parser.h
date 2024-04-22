#ifndef PARSER_H
#define PARSER_H
#include "typedefs.h"
#include <vector>
#include "../tokenizer/stokenize.h"
#include "../tokenizer/token.h"
#include "../tokenizer/constants.h"
#include "../tokenizer/state_machine_functions.h"
#include "../bplustree/map.h"

class Parser{
public:
    Parser();
    Parser(string s);
    Parser(const char* c);
    mmap_ss parse_tree();
    int get_column(string token);
    void set_string(const string&);
    
private:
    void build_keyword_list();
    bool get_parse_tree();
    Map<string,int> _keyword_list;
    mmap_ss ptree;
    string _buffer;
    void make_table(int ptable[][MAX_COLUMNS]);
    int ptable[MAX_ROWS][MAX_COLUMNS];
    vector<string> _token_queue;
    string _statement;
    bool _fail_flag;
};

#endif
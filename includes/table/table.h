#ifndef TABLE_H
#define TABLE_H
#include <iostream>
#include <iomanip>
#include <set>
#include <vector>
#include <fstream>
#include <algorithm>
// #include <bits/stdc++.h>
#include "typedefs.h"
#include "../binary_files/file_record.h"
#include "../binary_files/utilities.h"
#include "../bplustree/bplustree.h"
#include "../bplustree/map.h"
#include "../bplustree/multimap.h"
#include "../queue/MyQueue.h"
#include "../stack/MyStack.h"
#include "../token/token.h"
#include "../token/tokenstr.h"
#include "../token/relational.h"
#include "../token/logical.h"
#include "../token/paren.h"
#include "../token/recordset.h"
#include "../stl_utils/vector_utilities.h"

using namespace std;

class Table
{
public:
    Table();
    Table(const string& name, const vectorstr& field_names);//used to create new table
    Table(const string& name);//used to open existing table

    friend ostream& operator<<(ostream& outs, const Table& t);
                            
    void write_info(); //writes info into file
    void read_info(); //takes info from file

    void reindex(Table& dest, Table& src, string src_fname);

    Table select_all(); //returns entire table
    Table select_all(vectorstr fields); //returns entire table
    Table select(const vectorstr& fields, const string field1, 
                        const string op, const string field2); //DEFAULT SELECT
    Table select(const vectorstr& fields, Queue<Token *> post_fix); //RPN
    Table select(const vectorstr& fields, const vectorstr& cond); //SHUNTING YARD
    vector<long> select_recnos();   //recnos of select, no table

    //inserts new record into table, updates index structures
    void insert_into(const vectorstr& v); 

    bool empty();
    vector<string> get_fields(); //retrieves _field_names vectorstr
    
    //member variables
    static int serial;
private:
    //member functions
    void set_fields(const vectorstr& v); //sets _field_names vectorstr

    void set_field_map(); //connects field name with an index number

    void update_indices(const vectorstr& info, long recno); //adds recnos to corresponding mmap

    //returns recnos that fulfill relational condition (<,>,=,etc.)
    vector<long> op_relational(const string field, const string op, const string query); 
    //returns recnos that fulfill logical condition (and,&&,or,||)
    vector<long> op_logical(const vector<long> recno1, const string op, const vector<long> recno2);
    //builds new dest table with records from src table
    void records_to_table(vector<long> records, Table src, Table dest, string src_filename, string dest_filename);

    //check type from string
    bool is_rel(const string& s);
    bool is_log(const string& s);
    // bool is_log(const string& s);
    bool is_and(const string& s);
    bool is_or(const string& s);
    bool is_lp(const string& s);
    bool is_rp(const string& s);
    bool is_str(const string& s);

    //member variables 
    string _name; //name of table
    vector<MMap<string,long>> _indices; //stores multimaps of each field
    vectorstr _field_names; //holds all of the field names
    Map<string, int> _field_map; //links field name to index number
    
    int _serial=0;
    int _last_record=0;
    vector<long> _select_recnos;
};


#endif
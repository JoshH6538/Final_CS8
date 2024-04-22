#include "table.h"

int Table::serial=0;
//-------------------------------------
//              C T O R S
//-------------------------------------

Table::Table()
{
    _field_names.clear();
}

Table::Table(const string& name, const vectorstr& field_names) :_name(name)
{
    serial++;
    fstream file;
    string fname=_name;
    fname+=".bin";

    //assign member variables and write to new binary file
    set_fields(field_names);

    //create initial space for number of fields in _indices vector
    for(int i=0;i<_field_names.size();i++)
        _indices.emplace_back();

    open_fileW(file, fname.c_str());
    //creates fields.txt
    write_info();
    file.close();
}

Table::Table(const string& name) : _name(name)
{
    //open exisiting files
    fstream file;
    string fname=name;
    fname+=".bin";
    open_fileRW(file,fname.c_str());
    if(file.fail())
    {
        cout<<"Failed to open "<<fname<<endl;
        exit(1);
    }

    //move info from files to members
    read_info();

    //create initial space for number of fields in _indices vector
    for(int i=0;i<_field_names.size();i++)
        _indices.emplace_back();
    
    //first iteration and declarations
    long recno=0;//keeps track of current record
    long size=0;//size of vector within record
    FileRecord r;
    size = r.read(file,recno);
    update_indices(r.info(),recno);

    while(size>0)//loops until there is no info in file record
    {
        recno++;
        size = r.read(file,recno);
       if(size>0) update_indices(r.info(),recno);
    }
    file.close();
    vector<long> all_records;
    for(int i=0;i<_field_names.size();i++)
    {
        long index=_field_map[_field_names[i]];
        MMap<string,long>::Iterator start=_indices[index].begin();
        if(start!=_indices[index].end())
        while(start!=_indices[index].end())
        {
            MPair<string, long> temp=*start;
            vector<long> mmap__records=temp.value_list;
            for(int i=0;i<mmap__records.size();i++)
            {
                all_records.push_back(mmap__records[i]);
            }
            start++;
        }
    }
    sort(all_records.begin(),all_records.end());
    vector<long>::iterator last = unique(all_records.begin(),all_records.end());
    all_records.resize(distance(all_records.begin(),last));
    _select_recnos=all_records;
}

//-------------------------------------
//          R E A D / W R I T E
//-------------------------------------

void Table::write_info()
{
    fstream file;
    string fname=_name;
    fname+="_fields.txt";
    //creates _fields.txt file
    open_fileW(file,fname.c_str());
    //one record is written to file: _field_names
    FileRecord r(_field_names);
    file.seekg(0, file.end);
    r.write(file);
    file.close();
}

//opens _fields.txt file, 
//reads the 1 record into file record, 
//record.info() is a vectorstr that _field_names is set to
void Table::read_info() 
{
    fstream file;
    string fname=_name;
    fname+="_fields.txt";
    open_fileRW(file,fname.c_str());
    if(file.fail())
    {
        cout<<"Failed to open "<<fname<<endl;
        exit(1);
    }
    FileRecord r;
    r.read(file,0);
    file.close();
    set_fields(r.info());
}

//-------------------------------------
//           O P E R A T O R S
//-------------------------------------

ostream& operator<<(ostream& outs,
                               const Table& t)
{
    outs<<"  Recno"<<setw(13);
    int char_count=15;
    string line1="";
    string line2="";
    //display field names and recno
    for(int i=0;i<t._field_names.size()-1;i++)
    {
        outs<<t._field_names[i]<<setw(15);
        char_count+=t._field_names[i].size()+15;
    }
    
    //write last element out of loop for no setw format
    outs<<t._field_names[t._field_names.size()-1];
    char_count+=t._field_names[t._field_names.size()-1].size();
    outs<<endl;
    for(int i=0;i<char_count;i++)
    {
        line1+="=";
        line2+="-";
    }
    outs<<line1<<endl;
    //open bin file to read from
    fstream file;
    open_fileRW(file,(t._name+".bin").c_str());
    if(file.fail())
    {
        cout<<"Failed to open "<<t._name<<".bin"<<endl;
        exit(1);
    }
    long recno=0;
    long size=0;
    FileRecord r;
    size = r.read(file,recno);
    while(size>0)//loops until there is no info in file record
    {
        //if no info, stop printing
        outs<<setw(5)<<recno<<setw(15);
        outs<<r<<endl;
        outs<<line2<<endl;
        recno++;
        size = r.read(file,recno);
    }
    file.close();
    outs<<endl;
    return outs;
}

vector<long> Table::op_relational(const string field, const string op, const string query)
{
    // cout<<field<<" "<<op<<" "<<query<<endl;
    //gets index of corresponding field
    long index=_field_map[field];
    vector<long> post_filter;
    MMap<string, long>::Iterator start=_indices[index].begin();
    MMap<string, long>::Iterator finish=_indices[index].end();
    
    if(op=="=")
    {
        // cout<<"E Q U A L"<<endl;
        return _indices[index][query];
    }
    else if(op==">")
    {
        // cout<<"G R E A T E R  T H A N"<<endl;
        start=_indices[index].upper_bound(query);
        finish=_indices[index].end();
    }
    else if(op==">=")
    {
        // cout<<"G R E A T E R  E Q U A L  T H A N"<<endl;
        start=_indices[index].lower_bound(query);
        finish=_indices[index].end();
        
    }
    else if(op=="<")
    {
        // cout<<"L E S S  T H A N"<<endl;
        start=_indices[index].begin();
        finish=_indices[index].lower_bound(query);
    }
    else if(op=="<=")
    {
        // cout<<"L E S S  E Q U A L  T H A N"<<endl;
        start=_indices[index].begin();
        finish=_indices[index].upper_bound(query);
    }
    else if(op=="!=")
    {
        // cout<<"N O T  E Q U A L"<<endl;
        start=_indices[index].begin();
        finish=_indices[index].end();
        while(start!=finish)
        {
            MPair<string, long> temp=*start;
            vector<long> mmap__records=temp.value_list;
            if(temp.value_list!=_indices[index][query])
            for(int i=0;i<mmap__records.size();i++)
            {
                post_filter.push_back(mmap__records[i]);
            }
            start++;
        }
        return post_filter;
    }

    if(start!=_indices[index].end())
        while(start!=finish)
        {
            MPair<string, long> temp=*start;
            vector<long> mmap__records=temp.value_list;
            for(int i=0;i<mmap__records.size();i++)
            {
                post_filter.push_back(mmap__records[i]);
            }
            start++;
        }
        return post_filter;
}

vector<long> Table::op_logical(const vector<long> recno1, const string op, const vector<long> recno2)
{
    vector<long> records;
    // cout<<"{"<<recno1<<"}"<<endl
    // <<"\t"<<op<<endl
    // <<"{"<<recno2<<"}"<<endl;
    if(op=="and" || op=="&&")
    {
        // cout<<" A N D "<<endl;
        vector<long> intersect1=recno1, intersect2=recno2;
        sort(intersect1.begin(),intersect1.end());
        sort(intersect2.begin(),intersect2.end());
        
        set_intersection(intersect1.begin(),intersect1.end(),
        intersect2.begin(),intersect2.end(),back_inserter(records));
    }
    else if(op=="or" || op =="||")
    { 
        // cout<<" O R "<<endl;
        records=recno1;
        for(int i=0;i<recno2.size();i++)
           records+=recno2[i];
        sort(records.begin(),records.end());
        vector<long>::iterator last = unique(records.begin(),records.end());
        records.resize(distance(records.begin(),last));
    }
    // cout<<"FINAL: "<<records<<endl;
    return records;
}

//-------------------------------------
//             S E L E C T
//-------------------------------------

void Table::reindex(Table& dest, Table& src, string src_fname)
{
    //creates a vector that holds reindexed vector in
    //correspondence with src field_map
    // cout<<"pulling from"<<endl;
    // cout<<src<<endl;
    vector<int> new_index;
    for(int i=0;i<dest._field_names.size();i++)
        new_index.push_back(src._field_map[dest._field_names[i]]);
    //open src file to read from
    fstream file;
    open_fileRW(file,src_fname.c_str());
    if(file.fail())
    {
        cout<<"Opening "<<src_fname<<" failed in reindex."<<endl;
        exit(1);
    }
    long recno=0, size=1;
    FileRecord r;
    //makes a reformatted vector to insert into dest table
    vectorstr new_record;
    //will continue to read from file while their is info to read
    while(size>0)
    {
        size=r.read(file,recno);
        // cout<<size<<endl;
        recno++;
        //break on cond stops from inserting same record twice
        if(size<1)
        {
            new_record.clear();
            break;
        }
        //each element of vector is pushed into new_record
        //in the order of new_index
        for(int i=0;i<new_index.size();i++)
            new_record.push_back(r.info()[new_index[i]]);
        dest.insert_into(new_record);
        new_record.clear();
    }
    file.close();
}

Table Table::select_all()
{
    return *this;
}

Table Table::select_all(vectorstr fields)
{
    
    Table reformat("Reformatted_"+_name,fields);
    // cout<<"before\n"<<_indices[0]<<endl;
    // cout<<"========================"<<endl;
    reindex(reformat,*this,(_name+".bin"));
    // cout<<"========================"<<endl;
    // cout<<"after\n"<<reformat._indices[0]<<endl;
    return reformat;
}

Table Table::select(const vectorstr& fields, const string field, const string op, const string query)
{
    serial++;
    string fname=(_name+".bin");
    string filtered_fname=("select_filter_"+_name+to_string(serial)+".bin");
    Table filtered(filtered_fname, _field_names);
    
    //open table & select files

    

    // fstream src, dest;
    // open_fileRW(src,fname.c_str());
    // open_fileRW(dest,(filtered_fname).c_str());

    // FileRecord r;
    // //records is filled with all recnos that match criteria
    vector<long> records=op_relational(field,op,query);
    _select_recnos=records;
    
    records_to_table(records,*this,filtered,fname,filtered_fname);
    // for(int i=0;i<records.size();i++)
    // {
    //     //read info from original file
    //     r.read(src,records[i]);
    //     //insert into select_table
    //     filtered.insert_into(r.info());
    // }
    // src.close();
    // dest.close();



    //table has been filtered at this point
    //check if reindex is needed
    // cout<<"pulling from"<<endl;
    // cout<<*this<<endl;
    // cout<<records<<endl;
    // cout<<"then"<<endl;
    
    if(fields!=_field_names)
    {
        //create a reindexed table from the filtered table
        Table reindexed(("select_reindex_"+_name+to_string(serial)), fields);
        reindex(reindexed,filtered,filtered_fname+".bin");
        //removes preliminary filtered table
        remove((filtered_fname+".bin").c_str());
        remove((filtered_fname+"_fields.txt").c_str());
        return reindexed;
    }
    else return filtered;
}

Table Table::select(const vectorstr& fields, Queue<Token *> post_fix)
{
    // cout<<"post: "<<post_fix<<endl;
    Stack<Token*> stack;
    // Stack<vector<string>> record_stack;
    Token* curr;
    vector<long> records,temp_records, temp_records2;
    vector<vector<long>> vecrec;
    
    while(post_fix.size()>0)
    {
        curr=post_fix.pop();
        // cout<<curr->get_tokenstr()<<": ";
        // cout<<curr->get_typestr()<<endl;
        TokenType type = curr->get_type();
        string field, query, op;
        Token* temp;
        switch(type)
        {
            case TokenType::STRING:
                stack.push(curr);
                break;

            case TokenType::RELATIONAL:
                query<<stack.pop();
                field<<stack.pop();
                op<<curr;
                temp_records=op_relational(field,op,query);
                stack.push(new RecordSet(temp_records));
                cout<<stack<<endl;
                // vecrec.push_back(temp_records);
                // for(int i=0;i<temp_records.size();i++)
                //     records+=temp_records[i];
                break;

            //case TokenType::LOGICAL:
            case TokenType::AND:
            case TokenType::OR:
                // temp_records2=vecrec[vecrec.size()-1];
                // vecrec.pop_back();
                // temp_records=vecrec[vecrec.size()-1];
                // vecrec.pop_back();
                temp_records=static_cast<RecordSet*>(stack.pop())->get_RecordSet();
                temp_records2=static_cast<RecordSet*>(stack.pop())->get_RecordSet();
                // cout<<"1: "<<temp_records<<endl
                // <<"2: "<<temp_records2<<endl<<endl;
                // vecrec.clear();
                op<<curr;
                records=op_logical(temp_records,op,temp_records2);
                stack.push(new RecordSet(records));
                // vecrec.push_back(records);
                break;

            default:
                cout<<"INVALID TOKEN"<<endl;
                exit(1);
        }
    }
    _select_recnos=static_cast<RecordSet*>(stack.pop())->get_RecordSet();
    // cout<<"RECORDS: "<<_select_recnos<<endl;

    serial++;
    string fname=(_name+".bin");
    string filtered_fname=("select_filter_"+_name+to_string(serial)+".bin");
    Table filtered(filtered_fname, _field_names);
    
    records_to_table(_select_recnos,*this,filtered,fname,filtered_fname);
    // cout<<filtered._indices[0]<<endl;
    // cout<<"AFTER:"<<endl<<filtered._indices[0]<<endl;
    if(fields!=_field_names)
    {
        //create a reindexed table from the filtered table
        Table reindexed(("select_reindex_"+_name+to_string(serial)), fields);
        reindex(reindexed,filtered,filtered_fname+".bin");
        //removes preliminary filtered table
        remove((filtered_fname+".bin").c_str());
        remove((filtered_fname+"_fields.txt").c_str());
        reindexed._select_recnos=_select_recnos;
        return reindexed;
    }
    else
    {
        filtered._select_recnos=_select_recnos;
        return filtered;
    }
}

//shunting yard
Table Table::select(const vectorstr& fields, const vectorstr& cond)
{
    Queue<Token*> post_fix;
    Queue<Token*> pre_fix;
    Stack<Token*> stack;
    //converts vectorstr into pre_fix queue
    for(int i=0;i<cond.size();i++)
    {
        if(is_rel(cond[i])) pre_fix.push(new Relational(cond[i]));
        // else if(is_log(cond[i])) pre_fix.push(new Logical(cond[i]));
        else if(is_and(cond[i])) pre_fix.push(new And(cond[i]));
        else if(is_or(cond[i])) pre_fix.push(new Or(cond[i]));
        else if(is_lp(cond[i])) pre_fix.push(new LeftParen(cond[i]));
        else if(is_rp(cond[i])) pre_fix.push(new RightParen(cond[i]));
        else if(is_str(cond[i])) pre_fix.push(new TokenStr(cond[i]));
        else
        {
            cout<<"INVALID TOKEN"<<endl;
            exit(1);
        }
    }
    while(pre_fix.size()>0)
    {
        Token* curr=pre_fix.pop();
        TokenType type=curr->get_type();
        switch(type)
        {
            //token strs are just pushed into post_fix
            case TokenType::STRING:
                post_fix.push(curr);
                break;

            //relationals can only pop other relationals from the stack
            //this popping does not occur to my knowledge
            case TokenType::RELATIONAL:
                while(!stack.empty() && stack.top()->get_type()==TokenType::RELATIONAL)
                    if(stack.top()->get_type()==TokenType::RELATIONAL)
                        post_fix.push(stack.pop());
                stack.push(curr);
                break;
                

            //logicals can pop both logicals and relationals from the stack
            /*
            case TokenType::LOGICAL:
                while(!stack.empty() && stack.top()->get_type()!=TokenType::LEFT_PAREN)
                    if(stack.top()->get_type()==TokenType::RELATIONAL
                        || stack.top()->get_type()==TokenType::LOGICAL)
                        post_fix.push(stack.pop());
                stack.push(curr);
                break;
            */

           
        ////////////////////////////////////////////////////////////////////////////////////////////////
            //and can pop and, relationals
            case TokenType::AND:
                while(!stack.empty() && stack.top()->get_type()!=TokenType::LEFT_PAREN && stack.top()->get_type()!=TokenType::OR)
                    if(stack.top()->get_type()==TokenType::RELATIONAL
                        || stack.top()->get_type()==TokenType::AND)
                        post_fix.push(stack.pop());
                stack.push(curr);
                break;

            //or can pop and, or, relationals
            case TokenType::OR:
                while(!stack.empty() && stack.top()->get_type()!=TokenType::LEFT_PAREN)
                    if(stack.top()->get_type()==TokenType::RELATIONAL
                        || stack.top()->get_type()==TokenType::OR || stack.top()->get_type()==TokenType::AND)
                        post_fix.push(stack.pop());
                stack.push(curr);
                break;
         ////////////////////////////////////////////////////////////////////////////////////////////////


            //left parentheses only get pushed into stack
            case TokenType::LEFT_PAREN:
                stack.push(curr);
                break;

            //right parentheses will pop everything and push into post_fix
            //until it reaches left parentheses
            //left parentheses gets popped, neither parentheses
            //will go into post_fix
            case TokenType::RIGHT_PAREN:
                while(!stack.empty() && stack.top()->get_type()
                                        !=TokenType::LEFT_PAREN)
                    post_fix.push(stack.pop());
                if(stack.top()->get_type()==TokenType::LEFT_PAREN)
                    stack.pop();
                break;

            default:
                cout<<"INVALID TOKEN"<<endl;
                exit(1);
        }
        
        //pops rest of stack into post_fix
        if(pre_fix.size()==0)
            while(!stack.empty())
                post_fix.push(stack.pop());
    }
    //we now have post_fix so call other select
    // cout<<"POST FIX: "<<post_fix<<endl;
    return select(fields,post_fix);

}

vector<long> Table::select_recnos()
{
    return _select_recnos;
}


//-------------------------------------
//              B U I L D
//-------------------------------------

void Table::set_fields(const vectorstr& v)
{
    //set vector member
    _field_names=v;
    //update indices
    set_field_map();
}

vectorstr Table::get_fields()
{
    return _field_names;
}

void Table::set_field_map()
{
    //ensure field map is clear
    if(_field_map.empty()) _field_map.clear();
    for(int i=0;i<_field_names.size();i++)
        _field_map.insert(_field_names[i],i);
}

void Table::update_indices(const vectorstr& info, long recno)
{
    //adds recno to _indices multimap
    for(int i=0;i<info.size();i++)
    {
        _indices[i][info[i]]+=recno;
        // cout<<"Added "<<recno<<" to "<<_indices[i]<<endl;
    }
}

//-------------------------------------
//             I N S E R T 
//-------------------------------------

void Table::insert_into(const vectorstr& v)
{
    // cout<<"inserting "<<v<<endl;
    fstream file;
    open_fileRW(file,(_name+".bin").c_str());
    if(file.fail())
    {
        cout<<"Failed to open "<<_name<<".bin"<<endl;
        exit(1);
    }
    //write vector elements into binary file
    FileRecord r(v);
    file.seekg(0, file.end);
    long recno = r.write(file);
    //update index structures
    vectorstr info=r.info();
    update_indices(info,recno);
    file.close();
}

//-------------------------------------
//         V A L I D A T I O N
//-------------------------------------

bool Table::empty()
{
    return true;
}

//-------------------------------------
//              M I S C 
//-------------------------------------

void Table::records_to_table(vector<long> records, Table src, Table dest, string src_filename, string dest_filename)
{
    fstream from, to;
    open_fileRW(from,src_filename.c_str());
    open_fileRW(to,dest_filename.c_str());
    FileRecord r;

    for(int i=0;i<records.size();i++)
    {
        //read info from original file
        r.read(from,records[i]);
        //insert into select_table
        dest.insert_into(r.info());
    }
    from.close();
    to.close();
}

bool Table::is_rel(const string& s)
{
    vectorstr rel_ops={">","<",">=","<=","=","!="};
    for(int i=0;i<rel_ops.size();i++)
        if(s==rel_ops[i]) return true;
    return false;
}

// bool Table::is_log(const string& s)
// {
//     vectorstr log_ops={"and","or","&&","||"};
//     for(int i=0;i<log_ops.size();i++)
//         if(s==log_ops[i]) return true;
//     return false;
// }

bool Table::is_and(const string& s)
{
    vectorstr and_ops={"and","&&"};
    for(int i=0;i<and_ops.size();i++)
        if(s==and_ops[i]) return true;
    return false;
}

bool Table::is_or(const string& s)
{
    vectorstr or_ops={"or","||"};
    for(int i=0;i<or_ops.size();i++)
        if(s==or_ops[i]) return true;
    return false;
}

bool Table::is_lp(const string& s)
{
    if(s=="(") return true;
    return false;
}

bool Table::is_rp(const string& s)
{
    if(s==")") return true;
    return false;
}

bool Table::is_str(const string& s)
{
    if(is_rel(s)) return false;
    // if(is_log(s)) return false;
    if(is_and(s)) return false;
    if(is_or(s)) return false;
    if(is_lp(s)) return false;
    if(is_rp(s)) return false;
    return true;
}
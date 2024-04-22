#include "recordset.h"

//-------------------------------------
//              C T O R S
//-------------------------------------

RecordSet::RecordSet():Token()
{

}

RecordSet::RecordSet(vector<long> v):Token(TokenType::RECORD_SET), _record_set(v)
{
    
}

//-------------------------------------
//          A C C E S S O R S
//-------------------------------------

vector<long> RecordSet::get_RecordSet()
{
    return _record_set;
}


//-------------------------------------
//          M U T A T O R S
//-------------------------------------

void RecordSet::set_RecordSet(vector<long> v)
{
    _record_set=v;
}
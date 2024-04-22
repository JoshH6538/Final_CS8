#ifndef RECORDSET_H
#define RECORDSET_H
#include "token.h"
class RecordSet : public Token
{
public:
    RecordSet();
    RecordSet(vector<long> v);
    vector<long> get_RecordSet();
    void set_RecordSet(vector<long> v);
private:
    vector<long> _record_set;
};

#endif
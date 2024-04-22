#ifndef PARSER_TYPEDEFS_H
#define PARSER_TYPEDEFS_H
#include <string>
#include "../bplustree/multimap.h"
using namespace std;

typedef MMap<string, string> mmap_ss;

enum Keywords:int{
    STATE,
    SYM,
    MAKE,
    TABLE,
    FIELDS,
    INSERT,
    INTO,
    VALUES,
    SELECT,
    STAR,
    FROM,
    WHERE,
    COL,
    COMMAS,
    QUOTE,
    RELATIONAL,
    LP,
    RP,
    SUCCESS=1,
    FAIL=-1
};

#endif
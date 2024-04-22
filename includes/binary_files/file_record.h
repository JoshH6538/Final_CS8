#ifndef FILE_FileRecord_H
#define FILE_FileRecord_H
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

class FileRecord{
public:
    //when you construct a FileRecord, it's either empty or it
    //  contains a word
    FileRecord(){
        _record[0][0]='\0';
        recno = -1;
        set_null();
    }

    FileRecord(char str[]){
        set_null();
        strncpy(_record[0], str, MAX);
        recno = -1;
    }
    FileRecord(string s){
        set_null();
        strncpy(_record[0], s.c_str(), MAX);
        recno = -1;
    }
    template <typename T>
    FileRecord(vector<T> v)
    {
        set_null();
        for(int i=0;i<v.size();i++)
            strcpy(_record[i], v[i].c_str());
        recno = -1;
    }

    long write(fstream& outs);              //returns the FileRecord number
    long read(fstream& ins, long recno);    //returns the number of bytes
                                            //      read = MAX, or zero if
                                            //      read passed the end of file


    friend ostream& operator<<(ostream& outs,
                               const FileRecord& r);
    friend bool simple_test(bool);
    vector<string> info();
private:
    void set_null();
    static const int MAX = 100;
    int recno;
    char _record[MAX+1][MAX+1];
};

#endif
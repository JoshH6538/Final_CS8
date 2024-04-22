#include "file_record.h"

long FileRecord::write(fstream &outs){
    //r.write(f); //take the FileRecord r and write it into file f
    //  and return the FileRecord number of this FileRecord
    
    //write to the end of the file.
    outs.seekg(0, outs.end);


    long pos = outs.tellp();    //retrieves the current position of the
                                //      file pointer
    

    //outs.write(&FileRecord[0], sizeof(FileRecord));
    // for(int i=0;i<=MAX;i++)
    // {
    //     string s(_record[i]);
        outs.write((char*)_record,sizeof(_record));
    // }
    return pos/sizeof(_record);  //FileRecord number
}

long FileRecord::read(fstream &ins, long recno){
    //returns the number of bytes read.
    //    r.read(f, 6);
    //    cout<<r<<endl;
    long pos= recno * sizeof(_record);
    // cout<<"read, pos: "<<pos<<endl;
    ins.seekg(pos, ios_base::beg);

    //ins.read(&_record[0], sizeof(_record))
    // for(int i=0;i<=MAX;i++)
    // {
    //     string s(_record[i]);
        ins.read(&_record[0][0], sizeof(_record));
        // ins.read((char *)_record, sizeof(_record));
    // }
    
    //don't you want to mark the end of  the cstring with null?
    //_record[] => [zero'\0'trash trash trash trash]
    //don't need the null character, but for those of us with OCD and PTSD:
    //_record[ins.gcount()] = '\0';
    return ins.gcount();
}
ostream& operator<<(ostream& outs,
                    const FileRecord& r)
{
    for(int i=0;i<r.MAX+1 && r._record[i][0]!='\0';i++)
    {
        outs<<r._record[i]<<setw(15);
    }
    outs<<endl;
    return outs;
}

void FileRecord::set_null()
{
    for(int i=0;i<MAX+1;i++)
    {
        _record[i][0]='\0';
    }
}

vector<string> FileRecord::info()
{
    vector<string> info;
    for(int i=0;i<MAX+1 && _record[i][0]!='\0';i++)
        info.push_back(_record[i]);
    return info;
}
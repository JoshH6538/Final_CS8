#include "utilities.h"

bool file_exists(const char filename[]){
    const bool debug = false;
    fstream ff;
    ff.open (filename,
        std::fstream::in | std::fstream::binary );
    if (ff.fail()){
        if (debug) cout<<"file_exists(): File does NOT exist: "<<filename<<endl;
        return false;
    }
    if (debug) cout<<"file_exists(): File DOES exist: "<<filename<<endl;
    ff.close();
    return true;
}

void open_fileRW(fstream& f, const char filename[]) throw(char*){
    const bool debug = false;
    //opening a nonexistent file for in|out|app causes a fail()
    //  so, if the file does not exist, create it by openning it for
    //  output:
    if (!file_exists(filename)){
        //create the file
        f.open(filename, std::fstream::out|std::fstream::binary);
        if (f.fail()){
            cout<<"file open (RW) failed: with out|"<<filename<<"]"<<endl;
            throw("file RW failed  ");
        }
        else{
            if (debug) cout<<"open_fileRW: file created successfully: "<<filename<<endl;
        }
    }
    else{
        f.open (filename,
            std::fstream::in | std::fstream::out| std::fstream::binary );
        if (f.fail()){
            cout<<"file open (RW) failed. ["<<filename<<"]"<<endl;
            throw("file failed to open.");
        }
    }

}
void open_fileW(fstream& f, const char filename[]){
    f.open (filename,
            std::fstream::out| std::fstream::binary );
    if (f.fail()){
        cout<<"file open failed: "<<filename<<endl;
        throw("file failed to open.");
    }

}

// void simple_test()
// {
//     fstream f;
//     //reset the file: use this for new files
//     open_fileW(f, "record_list.bin");
    
//     FileRecord r;
//     int recno=-1;
//     r = FileRecord("Joe Zero");      //filling the envelop
//     recno = r.write(f);
//     cout<<"wrote into record: "<<recno<<endl;

//     r = FileRecord("One Arm Jane");      //filling the envelop
//     recno = r.write(f);
//     cout<<"wrote into record: "<<recno<<endl;


//     r = FileRecord("Two face Harvey");      //filling the envelop
//     recno = r.write(f);
//     cout<<"wrote into record: "<<recno<<endl;


//     r = FileRecord("Three Eyed Raven");      //filling the envelop
//     recno = r.write(f);
//     cout<<"wrote into record: "<<recno<<endl;


//     r = FileRecord("Four Eyes, Four Eyes! ");      //filling the envelop
//     recno = r.write(f);
//     cout<<"wrote into record: "<<recno<<endl;


//     r = FileRecord("Cinco De Mayo, Bryan! ");      //filling the envelop
//     recno = r.write(f);
//     cout<<"wrote into record: "<<recno<<endl;


//     r = FileRecord("The Six Shooter Kid! ");      //filling the envelop
//     recno = r.write(f);
//     cout<<"wrote into record: "<<recno<<endl;


//     f.close();



//     FileRecord r2;
//     //open the file for reading and writing.
//     open_fileRW(f, "record_list.bin" );
//     r2.read(f, 3);       //empty envelop to be filled by the FileRecord object
//     cout<<"record 3: "<<r2<<endl;    //insertion operator of the FileRecord object
//     r2.read(f, 6);
//     cout<<"record 6: "<<r2<<endl;

//     //reading passed the end of file:
//     long bytes = r2.read(f, 19);
//     cout<<"number of bytes read: "<<bytes<<endl;

// }
// void confuse_me(){
//     string list[10] = {"zero",
//                        "one",
//                        "two",
//                        "three",
//                        "four",
//                        "five",
//                        "six",
//                        "seven",
//                        "eight",
//                        "nine"};

//     cout << "[ ]Show the list of records:" << endl;
//     cout << "------------------------------" << endl;
//     for (int i = 0; i < 10; i++)
//     {
//         cout<<setw(6)<<list[i];
//     }
//     cout << endl<<endl;

//     cout << "[ ]Write / INSERT records into the file:" << endl;
//     cout << "--------------------------------------------" << endl;


//     fstream f;
//     //reset the file:
//     open_fileW(f, "record_list.bin");
//     for (int i= 0; i<10; i++){
//         FileRecord r(list[i]);      //filling the envelop
//         long recno = r.write(f);
//         cout<<"["<<setw(5)<<list[i] <<"]"<< " was written into file as record: "<<setw(2)<<recno<<endl;
//     }
//     f.close();
//     cout << endl
//           << endl;

//     cout << "[ ]Read / RETRIEVE / SELECT records from the file: " << endl;
//     cout << "------------------------------------------------------" << endl;

//     FileRecord r;
//     int selected_records[] = {4,6,8,9};
//     // open the file for reading and writing.
//     open_fileRW(f, "record_list.bin" );
//     for (int i = 0; i < 4; i++){
//         r.read(f, i);
//         cout << "reading record " << i << ": " << r << endl;
//     }

//     cout << "\n\n[ ]Reading passed the end of file: " << endl;
//     cout << "---------------------------------------------" << endl;

//     long bytes = r.read(f, 19);
//     cout<<"  number of bytes read: "<<bytes<<endl;

// }
#ifndef BTREE_ARRAY_FUNCTIONS_H
#define BTREE_ARRAY_FUNCTIONS_H
#include <cmath>
#include <iostream>
#include <iomanip>
#include <set>
#include <vector>
using namespace std;


template <class T>
T maximal(const T& a, const T& b);                      //return the larger of the two items

template <class T>
void swap(T& a, T& b);  //swap the two items

template <class T>
int index_of_maximal(T data[ ], int n);                 //return index of the largest item in data

template <class T>
void ordered_insert(T data[ ], int& n, T entry);        //insert entry into the sorted array
                                                        //data with length n

template <class T>
int first_ge(const T data[ ], int n, const T& entry);   //return the first element in data that is
                                                        //not less than entry

template <class T>
void attach_item(T data[ ], int& n, const T& entry);    //append entry to the right of data

template <class T>
void insert_item(T data[ ], int i, int& n, T entry);    //insert entry at index i in data

template <class T>
void detach_item(T data[ ], int& n, T& entry);          //remove the last element in data and place
                                                        //it in entry

template <class T>
void delete_item(T data[ ], int i, int& n, T& entry);   //delete item at index i and place it in entry

template <class T>
void merge(T data1[ ], int& n1, T data2[ ], int& n2);   //append data2 to the right of data1

template <class T>
void split(T data1[ ], int& n1, T data2[ ], int& n2);   //move n/2 elements from the right of data1
                                                        //and move to data2

template <class T>
void copy_array(T dest[], const T src[],
                int& dest_size, int src_size);              //copy src[] into dest[]

template <class T>
void print_array(const T data[], int n, int pos = -1);  //print array data

template <class T>
bool is_gt(const T data[], int n, const T& item);       //item > all data[i]

template <class T>
bool is_le(const T data[], int n, const T& item);       //item <= all data[i]

//-------------- Vector Extra operators: ---------------------

template <typename T>
ostream& operator <<(ostream& outs, const vector<T>& list); //print vector list

template <typename T>
vector<T>& operator +=(vector<T>& list, const T& addme); //list.push_back addme

//////////////////////////////////////////////////////////////////////////////////////////////

template <class T>
int first_ge(const T data[ ], int n, const T& entry)    //return the first element in data that is
                                                        //not less than entry
{
    for(int i=0;i<n;i++)
        if(data[i]>=entry) return i;
    return n;
}

template <class T>
int first_g(const T data[ ], int n, const T& entry)    //return the first element in data that is
                                                        //not less than entry
{
    for(int i=0;i<n;i++)
        if(data[i]>entry) return i;
    return n;
}

template <class T>
void attach_item(T data[ ], int& n, const T& entry)
{
    data[n]=entry;
    n++;
}

template <class T>
void insert_item(T data[ ], int i, int& n, T entry)
{
    // assert(i>=0 );
    // assert(i<=n );
    for(int j=n;j>i;j--)
        data[j]=data[j-1];
    data[i]=entry;
    n++;
}

template <class T>
void detach_item(T data[ ], int& n, T& entry)
{
    entry=data[n-1];
    n--;
}

template <class T>
void delete_item(T data[ ], int i, int& n, T& entry)
{
    //assert(false);
    if(i>=n)
    {
        cout<<"index out of bounds"<<endl;
        //assert(false);
        return;
    }
    entry=data[i];
    for(int j=i;j<n-1;j++)
    {
        data[j]=data[j+1];
    }
    n--;
}

template <class T>
void merge(T data1[ ], int& n1, T data2[ ], int& n2)
{
    for(int i=0;i<n2;i++)
    {
        data1[n1]=data2[i];
        n1++;
    }
    n2=0;
}

template <class T>
void split(T data1[ ], int& n1, T data2[ ], int& n2)
{
    int mid=n1/2;
    int minus=0;
    if(n1%2==0)mid--;
    for(int i=mid+1, j=0;i<n1;i++,j++)
    {
        insert_item(data2,j,n2,data1[i]);
        minus++;
    }
    n1-=minus;
}

template <class T>
void copy_array(T dest[], const T src[], int& dest_size, int src_size)
{
    dest_size=0;
    for(int i=0;i<src_size;i++)
    {
        dest[i]=src[i];
        dest_size++;
    }
}

template <class T>
void print_array(const T data[], int n, int pos)
{
    for(int i=pos+1;i<n;i++)
    {
        cout<<data[i]<<": "<<i<<" |";
    }
    cout<<endl;
}

#endif
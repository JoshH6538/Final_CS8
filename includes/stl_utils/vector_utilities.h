#ifndef VECTOR_UTILITIES_H
#define VECTOR_UTILITIES_H
#include <cmath>
#include <iostream>
#include <iomanip>
#include <set>
#include <vector>
using namespace std;

template <typename T>
ostream& operator <<(ostream& outs, const vector<T>& list); //print vector list

template <typename T>
vector<T>& operator +=(vector<T>& list, const T& addme); //list.push_back addme

template <typename T>
ostream& operator <<(ostream& outs, const vector<T>& list) //print vector list
{
    if((list.size()>0))
    {
        for(int i=0;i<list.size()-1;i++)
        {
            cout<<list[i]<<" ";
        }
        cout<<list[list.size()-1];
    }
    
    return outs;
}

template <typename T>
vector<T>& operator +=(vector<T>& list, const T& addme) //list.push_back addme
{
    list.push_back(addme);
    return list;
}

#endif
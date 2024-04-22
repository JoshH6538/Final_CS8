#ifndef MULTI_MAP_H
#define MULTI_MAP_H
#include <cmath>
#include <iostream>
#include <iomanip>
#include <set>
#include <vector>
#include "bplustree.h"
using namespace std;

template <typename K, typename V>
struct MPair{
    K key;
    vector<V> value_list;

    //--------------------------------------------------------------------------------
    /*      MPair CTORs:
     *  must have these CTORs:
     *  - default CTOR / CTOR with a key and no value: this will create an empty vector
     *  - CTOR with a key AND a value: pushes the value into the value_list
     *  _ CTOR with a key and a vector of values (to replace value_list)
     */
    //--------------------------------------------------------------------------------
    MPair(const K& k=K()):key(k){}
    MPair(const K& k, const V& v):key(k)
    {
        value_list.push_back(v);
    }
    MPair(const K& k, const vector<V>& vlist):key(k),value_list(vlist)
    {
        
    }
    //--------------------------------------------------------------------------------

    //You'll need to overlod << for your vector:
    friend ostream& operator <<(ostream& outs, const MPair<K, V>& print_me)
    {
        outs<<print_me.key<<":"
        <<setw(10);
        for(int i=0;i<print_me.value_list.size();i++)
        {
            outs<<print_me.value_list[i]<<" ";
        }
        return outs;
    }
    
    friend bool operator ==(const MPair<K, V>& lhs, const MPair<K, V>& rhs){return lhs.key==rhs.key;}
    friend bool operator < (const MPair<K, V>& lhs, const MPair<K, V>& rhs){return lhs.key<rhs.key;}
    friend bool operator <= (const MPair<K, V>& lhs, const MPair<K, V>& rhs){return !(lhs.key>rhs.key);}
    friend bool operator >= (const MPair<K, V>& lhs, const MPair<K, V>& rhs){return !(lhs.key<rhs.key);}
    friend bool operator > (const MPair<K, V>& lhs, const MPair<K, V>& rhs){return lhs.key>rhs.key;}
    friend MPair<K, V> operator + (const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
        MPair<K, V> mp;
        mp.key=lhs.key;
        mp.value_list=lhs.value_list;
        for(int i=0;i<rhs.value_list.size();i++)
            mp.value_list.push_back(rhs.value_list[i]);
        return mp;
    }
};

template <typename K, typename V>
class MMap
{
public:
    typedef BPlusTree<MPair<K, V> > map_base;
    class Iterator{
    public:
        friend class MMap;
        Iterator(typename map_base::Iterator it=nullptr):_it(it){}
        Iterator operator ++(int unused)
        {
           _it++;
           return *this;
        }
        Iterator operator ++()
        {
           ++_it;
           return *this;
        }
        MPair<K, V> operator *() {return *_it;}
        friend bool operator ==(const Iterator& lhs, const Iterator& rhs){return lhs._it==rhs._it;}
        friend bool operator !=(const Iterator& lhs, const Iterator& rhs){return !(lhs._it==rhs._it);}

    private:
        typename map_base::Iterator _it;
    };

    MMap();

//  Iterators
    Iterator begin(){return mmap.begin();}
    Iterator end(){return mmap.end();}
    Iterator lower_bound(K i){return mmap.lower_bound(i);}
    Iterator upper_bound(K i){return mmap.upper_bound(i);}

//  Capacity
    int size() const;
    bool empty() const;

//  Element Access
    const vector<V>& operator[](const K& key) const
    {
        return mmap.get(MPair<K,V>(key)).value_list;
    }
    vector<V>& operator[](const K& key)
    {
        return mmap.get(MPair<K,V>(key)).value_list;
    }

//  Modifiers
    void insert(const K& k, const V& v);
    void erase(const K& key);
    void clear();

//  Operations:
    bool contains(const K& key) const;
    vector<V> &get(const K& key);
    vector<V> at(K key){return mmap.get(MPair<K,V>(key)).value_list;}

    Iterator find(const K& key){return mmap.find(key);}
    int count(const K& key){return 0;}
    // I have not written these yet, but we will need them:
    //    int count(const K& key);
    //    lower_bound
    //    upper_bound
    //    equal_range:

    void print_lookup();

    bool is_valid();

    friend ostream& operator<<(ostream& outs, const MMap<K, V>& print_me){
        outs<<print_me.mmap<<endl;
        return outs;
    }

private:
    BPlusTree<MPair<K, V> > mmap;
};

template <typename K, typename V>
MMap<K,V>::MMap() : mmap(true)
{
    
}
template <typename K, typename V>
int MMap<K,V>::size() const
{
    return mmap.size();
}

template <typename K, typename V>
bool MMap<K,V>::empty() const
{
    return mmap.empty();
}
template <typename K, typename V>
void MMap<K,V>::insert(const K& k, const V& v)
{
    mmap.insert(MPair<K,V>(k,v));
}
template <typename K, typename V>
void MMap<K,V>::erase(const K& key)
{
    mmap.erase(key);
}
template <typename K, typename V>
void MMap<K,V>::clear()
{
    mmap.clear_tree();
}

template <typename K, typename V>
bool MMap<K,V>::is_valid()
{
    return mmap.is_valid();
}

template <typename K, typename V>
bool MMap<K,V>::contains(const K& key) const
{
    return mmap.contains(key);
}

template <typename K, typename V>
vector<V>& MMap<K,V>::get(const K& key)
{
    return mmap.get(MPair<K,V>(key)).value_list;
}

template <typename K, typename V>
void MMap<K,V>::print_lookup()
{
   vector<string> columns={"command","table_name","col","fields","where","condition","values"};

   for(int i=0;i<columns.size();i++)
   {
        MMap<K,V>::Iterator it=mmap.find(columns[i]);
        if(it!=MMap<K,V>::Iterator(NULL))
        cout<<*it<<endl;
   }
}


#endif
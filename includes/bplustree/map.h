#ifndef MAP_H
#define MAP_H
#include <cmath>
#include <iostream>
#include <iomanip>
#include <set>
#include <vector>
#include "bplustree.h"
using namespace std;

template <typename K, typename V>
struct Pair{
    K key;
    V value;

    Pair(const K& k=K(), const V& v=V()):key(k),value(v){}
    friend ostream& operator <<(ostream& outs, const Pair<K, V>& print_me)
    {
        outs<<print_me.key<<":"<<print_me.value;
        return outs;
    }
    friend bool operator ==(const Pair<K, V>& lhs, const Pair<K, V>& rhs){return lhs.key==rhs.key;}
    friend bool operator < (const Pair<K, V>& lhs, const Pair<K, V>& rhs){return lhs.key<rhs.key;}
    friend bool operator > (const Pair<K, V>& lhs, const Pair<K, V>& rhs){return lhs.key>rhs.key;}
    friend bool operator >= (const Pair<K, V>& lhs, const Pair<K, V>& rhs){return lhs.key>=rhs.key;}
    friend bool operator <= (const Pair<K, V>& lhs, const Pair<K, V>& rhs){return lhs.key<=rhs.key;}
    friend Pair<K, V> operator + (const Pair<K, V>& lhs, const Pair<K, V>& rhs)
    {
        Pair<K, V> p;
        p.key=(lhs.key + rhs.key);
        p.value=(lhs.value + rhs.value);
        return p;
    }
};

template <typename K, typename V>
class Map
{
public:
    typedef BPlusTree<Pair<K, V> > map_base;
    class Iterator{
    public:
        friend class Map;
        Iterator(typename map_base::Iterator it=NULL):_it(it){}
        Pair<K, V> operator *(){return *_it;}
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
        
        friend bool operator ==(const Iterator& lhs, const Iterator& rhs){return lhs._it==rhs._it;}
        friend bool operator !=(const Iterator& lhs, const Iterator& rhs){return !(lhs._it==rhs._it);}
    private:
        typename map_base::Iterator _it;
    };

    Map();
//  Iterators
    Iterator begin();
    Iterator end();
    Iterator lower_bound(K i){return map.lower_bound(i);}
    Iterator upper_bound(K i){return map.upper_bound(i);}


//  Capacity
    int size() const;
    bool empty() const;

//  Element Access
    V& operator[](const K& key)
    {
        return map.get(Pair<K,V>(key,V())).value;
    }
    V& at(const K& key){return map.get(Pair<K,V>(key,V())).value;}
    const V& at(const K& key) const{const V v; return v;}


//  Modifiers
    void insert(const K& k, const V& v);
    void erase(const K& key);
    void clear();
    V get(const K& key);

//  Operations:
    Iterator find(const K& key);
    bool contains(const Pair<K, V>& target) const;

    // I have not writtent hese yet, but we will need them:
    //    int count(const K& key);
    //    lower_bound
    //    upper_bound
    //    equal_range:

    bool is_valid(){return map.is_valid();}

    friend ostream& operator<<(ostream& outs, const Map<K, V>& print_me){
        outs<<print_me.map;
        return outs;
    }
private:
    int key_count;
    BPlusTree<Pair<K, V> > map;
};

//---------------------------------------------------------------------
//            C T O R  /  B I G   T H R E E
//---------------------------------------------------------------------

template <typename K, typename V>
Map<K,V>::Map():key_count(0)
{

}

template <typename K, typename V>
int Map<K,V>::size() const
{
    return map.size();
}

template <typename K, typename V>
typename Map<K,V>::Iterator Map<K,V>::begin()
{
    return map.begin();
}
template <typename K, typename V>
typename Map<K,V>::Iterator Map<K,V>::end()
{
    return map.end();
}

template <typename K, typename V>
typename Map<K,V>::Iterator Map<K,V>::find(const K& key)
{
    //typename map_base::Iterator it; return it;
    return map.find(key);
}

template <typename K, typename V>
bool Map<K,V>::contains(const Pair<K, V>& target) const
{
    return map.contains(target);
} 

template <typename K, typename V>
bool Map<K,V>::empty() const
{
    return map.empty();
}
template <typename K, typename V>
void Map<K,V>::insert(const K& k, const V& v)
{
    Pair<K,V> p;
    p.key=k;
    p.value=v;
    map.insert(p);
}
template <typename K, typename V>
void Map<K,V>::erase(const K& key)
{
    map.remove(find(key)._it);
}
template <typename K, typename V>
void Map<K,V>::clear()
{
    map.clear_tree();
}

#endif
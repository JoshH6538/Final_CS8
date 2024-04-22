#ifndef B_PLUS_TREE_H
#define B_PLUS_TREE_H
#include <cmath>
#include <iostream>
#include <iomanip>
#include <set>
#include <vector>
#include "btree_array_funcs.h"
#include "../stl_utils/vector_utilities.h"
using namespace std;

template <class T>
class BPlusTree
{
public:
    class Iterator{
    public:
        friend class BPlusTree;
        Iterator(BPlusTree<T>* _it=NULL, int _key_ptr = 0):
                                                it(_it), key_ptr(_key_ptr){}

        T operator *(){return it->data[key_ptr];}
        const T operator *() const {return it->data[key_ptr];}
        Iterator operator++(int un_used)
        {
            BPlusTree<T>::Iterator temp=it;
            key_ptr++;
            if(key_ptr>=it->data_count)
            {
                it=it->next;
                key_ptr=0;
            }
            return temp;
        }
        Iterator operator++()
        {
            key_ptr++;
            if(key_ptr>=it->data_count)
            {
                it=it->next;
                key_ptr=0;
            }
            return it;
        }
        friend bool operator ==(const Iterator& lhs, const Iterator& rhs)
        {
            if(lhs.is_null() || rhs.is_null()) return (lhs.is_null() && rhs.is_null());
            return (*lhs==*rhs);
        }
        friend bool operator !=(const Iterator& lhs, const Iterator& rhs){return !(lhs==rhs);}
        void print_Iterator(){
            if (it){
                cout<<"iterator: ";
                print_array(it->data, it->data_count, key_ptr);
            }
            else{
                cout<<"iterator: NULL, key_ptr: "<<key_ptr<<endl;
            }
        }
        bool is_null(){return !it;}
        const bool is_null() const {return !it;}
        void info(){
            cout<<endl<<"Iterator info:"<<endl;
            cout<<"key_ptr: "<<key_ptr<<endl;
            cout<<"it: "<<*it<<endl;
        }

    private:
        BPlusTree<T>* it;
        int key_ptr;
    };

    BPlusTree(bool dups = false);
    BPlusTree(T *a, int size, bool dups = false);

    //big three:
    BPlusTree(const BPlusTree<T>& other);
    ~BPlusTree();
    BPlusTree<T>& operator =(const BPlusTree<T>& RHS);

    void copy_tree(const BPlusTree<T>& other);  //copy other into this object
    void copy_tree(const BPlusTree<T>& other, BPlusTree<T>*& last_node);

    void insert(const T& entry);   //insert entry into the tree
    void remove(const T& entry);   //remove entry from the tree
    void clear_tree();             //clear this object (delete all nodes etc.)

    bool contains(const T& entry) const; //true if entry can be found
    T& get(const T& entry);              //return a reference to entry
    const T& get(const T& entry)const;   //return a reference to entry
    T& get_existing(const T& entry);     //return a reference to entry
    Iterator find(const T& key);         //return an iterator to this key.
                                         //     NULL if not there.
    Iterator lower_bound(const T& key);  //return first that goes NOT BEFORE
                                         // key entry or next if does not
                                         // exist: >= entry
    Iterator upper_bound(const T& key);  //return first that goes AFTER key
                                         //exist or not, the next entry  >entry

    int size() const;                    //count the number of elements

    bool empty() const;                  //true if the tree is empty

    void print_tree(int level = 0,
                    ostream &outs=cout) const;
    friend ostream& operator<<(ostream& outs,
                               const BPlusTree<T>& print_me){
        print_me.print_tree(0, outs);
        return outs;
    }

    bool is_valid();
    string in_order();
    string pre_order();
    string post_order();

    Iterator begin();
    Iterator end();
    ostream& list_keys(Iterator from = NULL, Iterator to = NULL){
        if (from == NULL) from = begin();
        if (to == NULL) to = end();
        for (Iterator it = from; it != to; it++)
            cout<<*it<<" ";
        return cout;

    }
    //personal functions
    bool verify_tree();

private:
    static const int MINIMUM = 1;
    static const int MAXIMUM = 2 * MINIMUM;

    bool dups_ok;                        //true if duplicate keys are allowed
    int data_count;                      //number of data elements
    T data[MAXIMUM + 1];                 //holds the keys
    int child_count;                     //number of children
    BPlusTree* subset[MAXIMUM + 2];      //suBPlusTrees
    BPlusTree* next;
    int _size;
    bool is_leaf() const
    {return child_count==0;}             //true if this is a leaf node

    T* find_ptr(const T& entry);         //return a pointer to this key.
                                         // NULL if not there.

    //insert element functions
    void loose_insert(const T& entry);   //allows MAXIMUM+1 data elements in
                                         //   the root
    void fix_excess(int i);              //fix excess in child i

    //remove element functions:
    void loose_remove(const T& entry);  //allows MINIMUM-1 data elements
                                        //  in the root

    BPlusTree<T>* fix_shortage(int i);  //fix shortage in child i
    // and return the smallest key in this suBPlusTree
    BPlusTree<T>* get_smallest_node();
    BPlusTree<T>* get_biggest_node();
    void get_smallest(T& entry);      //entry := leftmost leaf
    void get_biggest(T& entry);       //entry := rightmost leaf
    void remove_biggest(T& entry);    //remove the biggest child of tree->entry
    void remove_smallest(T& entry);    //remove the biggest child of tree->entry
    void transfer_left(int i);        //transfer one element LEFT from child i
    void transfer_right(int i);       //transfer one element RIGHT from child i
    BPlusTree<T> *merge_with_next_subset(int i); //merge subset i with  i+1

    //personal functions
    bool verify_node();

};

//---------------------------------------------------------------------
//            C T O R  /  B I G   T H R E E
//---------------------------------------------------------------------

template <typename T>
BPlusTree<T>::BPlusTree(bool dups):dups_ok(dups), data_count(0),
                                   child_count(0), next(NULL),_size(0){
    //left empty
}

template <typename T>
BPlusTree<T>::BPlusTree(T *a, int size, bool dups)
{
    data_count=0;
    child_count=0;
    _size=0;
    next=nullptr;
    for(int i=0;i<size;i++) insert(a[i]);
}

template <typename T>
BPlusTree<T>::BPlusTree(const BPlusTree<T>& other):
                        data_count(0), child_count(0), next(NULL)
{
    copy_tree(other);
}

template <typename T>
BPlusTree<T>::~BPlusTree(){
    const bool debug = false;
    if (debug) cout<<"** destructor fired!"<<endl;
    clear_tree();
}

template <typename T>
BPlusTree<T>& BPlusTree<T>::operator =(const BPlusTree<T>& RHS){
    const bool debug = false;
    if (debug) cout<<"** assignment operator fired!"<<endl;
    data_count=0;
    child_count=0;
    next=nullptr;
    copy_tree(RHS);
    return *this;
}

//---------------------------------------------------------------------
//            B E G I N ( )   &   E N D ( )
//---------------------------------------------------------------------

template <typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::begin()
{
    if(!is_leaf()) return subset[0]->begin();
    else
    {
        BPlusTree<T>::Iterator value=this;
        value.key_ptr=0;
        return value;
    }
}
template <typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::end(){
    return BPlusTree<T>::Iterator(NULL);
}

template <typename T>
bool BPlusTree<T>::contains(const T& entry) const
{
    int index=first_ge(data, data_count,entry);
    //if data is found
    if(index<data_count&& data[index]==entry)
        return true;
    if(is_leaf()) return false;
    //if child exists, call find with child
    if(index<child_count && index>=0)
        return subset[index]->contains(entry);
} 

template <typename T>
T& BPlusTree<T>::get(const T& entry) {
    //If entry is not in the tree, add it to the tree
    //assert(contains(entry));
    const bool debug = false;
    if (!contains(entry))
        insert(entry);
    return get_existing(entry);
}
template <typename T>
const T& BPlusTree<T>::get(const T& entry) const{
    //If entry is not in the tree, add it to the tree
    //assert(contains(entry));
    const bool debug = false;
    return get_existing(entry);
}

template <typename T>
T& BPlusTree<T>::get_existing(const T& entry) {

    //assert that entry is not in the tree.
    //assert(contains(entry));

    const bool debug = false;
    int i = first_ge(data, data_count, entry);
    bool found = (i<data_count && data[i]==entry);
    if (is_leaf())
        if (found){
            return data[i];
        }
        else{
            if (debug) cout<<"get_existing was called with nonexistent entry"<<endl;
            //assert(found);
        }
    if (found) //inner node
        return subset[i+1]->get_existing(entry);
        //or just return true?
    else //not found yet...
        return subset[i]->get_existing(entry);
}

template <typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::find(const T& key)
{
    int index=first_ge(data, data_count,key);
    //if data is found
    if(index<data_count&& data[index]==key)
    {
        BPlusTree<T>::Iterator it;
        if(!is_leaf()) 
        {
            it.it=subset[index+1]->get_smallest_node();
            it.key_ptr=0;
            return it;
        }
        it.it=this;
        it.key_ptr=index;
        return it;
    }
        
    if(is_leaf()) return BPlusTree<T>::Iterator(NULL);
    //if child exists, call find with child
    if(index<child_count && index>=0)
        return subset[index]->find(key);
}

template <typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::lower_bound(const T& key)
{
    int index=first_ge(data, data_count,key);
    BPlusTree<T>::Iterator it(NULL);
    if(!is_leaf())
    {
        //will go to right child if largest in current index is too small
        if(index<child_count-1)
        {
            T value;
            subset[index]->get_biggest(value);
            if(value < key)
                return subset[index+1]->lower_bound(key);
        }
        return subset[index]->lower_bound(key);
    }
    //if there are no children
    else
    {
        //did not find large enough value, return null
        if(data[index]<key) return  BPlusTree<T>::Iterator(NULL);
        it.it=this;
    }
    it.key_ptr=index;
    return it;
}

template <typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::upper_bound(const T& key)
{
    int index=first_g(data, data_count,key);
    BPlusTree<T>::Iterator it(NULL);
    if(!is_leaf())
    {
        //will go to right child if largest in current index is too small
        if(index<child_count-1)
        {
            T value;
            subset[index]->get_biggest(value);
            if(value <= key)
                return subset[index+1]->upper_bound(key);
        }
        return subset[index]->upper_bound(key);
    }
    //if there are no children
    else
    {
        //did not find large enough value, return null
        if(data[index]<=key) return  BPlusTree<T>::Iterator(NULL);
        it.it=this;
    }
    it.key_ptr=index;
    return it;
}

//---------------------------------------------------------------------
//                         V A L I D A T I O N
//---------------------------------------------------------------------

template <typename T>
bool BPlusTree<T>::is_valid()
{
    //should check that every data[i] < data[i+1]
    if (is_leaf())
        return true;
    return true;
}

template <typename T>
string BPlusTree<T>::in_order()
{
     string value="";
    //go to left most leaf
    if(!is_leaf()) value+=subset[0]->in_order();
    //add data
    for(int i=0;i<data_count;i++)
    {
        // if(is_leaf())
        // {
            value+=to_string(data[i]);
            value+="|";
        // }
        //go right if not leaf
        if(!is_leaf()) value+=subset[i+1]->in_order();
    }
    return value;
}

template <typename T>
string BPlusTree<T>::pre_order()
{
    string value="";
    //go to left most leaf
    //add data
    for(int i=0;i<data_count;i++)
    {
        value+=to_string(data[i]);
        value+="|";
        if(!is_leaf()) value+=subset[i]->pre_order();
        //go right if not leaf
        if(!is_leaf()) value+=subset[i+1]->pre_order();
    }
    return value;
}

template <typename T>
string BPlusTree<T>::post_order()
{
     string value="";
    //go to left most leaf
    if(!is_leaf()) value+=subset[0]->post_order();
    //add data
    for(int i=0;i<data_count;i++)
    {
        if(!is_leaf()) value+=subset[i+1]->post_order();
        value+=to_string(data[i]);
        value+="|";  
    }
    return value;
}

//------------------------------------------------
//                 R E M O V E
//------------------------------------------------
template <typename T>
void BPlusTree<T>::remove(const T& entry)
{
    //print_tree();
    //cout<<"======================="<<endl;
    loose_remove(entry);
    if(data_count<MINIMUM && !is_leaf())
    {
        cout<<"Shrinking tree"<<endl;
        BPlusTree<T>* shrink_ptr=subset[0];
        child_count=0;
        data_count=0;
        copy_array(data,shrink_ptr->data,data_count,shrink_ptr->data_count);
        copy_array(subset,shrink_ptr->subset,child_count,shrink_ptr->child_count);
        shrink_ptr->child_count=0;
        shrink_ptr->data_count=0;
        delete shrink_ptr;
    }
    if(data_count<MINIMUM && child_count>0 && subset[0]->data_count==0)
    {
        cout<<"clearing empty tree"<<endl;
        data_count=0;
        child_count=0;
    }
}
template <typename T>
void BPlusTree<T>::loose_remove(const T& entry)
{
    // cout<<"Attempting to remove "<<entry<<endl;
    // print_tree();
    int index=first_ge(data, data_count, entry);
    if(index<data_count && data[index]==entry)
    {
        // cout<<"Removing "<<entry<<endl;
        T value;
        //first case(easy): remove from a leaf
        if(is_leaf())
        {
            // cout<<"leaf"<<endl;
            delete_item(data,index,data_count,value);
        }
        else{
            subset[index+1]->loose_remove(entry);
            fix_shortage(index+1);
            print_tree();
            return;
        }
    }
    if(is_leaf()) return;
    //if child exists, call find with child
    if(index<child_count && index>=0)
    {
        T value;
        subset[index]->loose_remove(entry);
        // print_tree();
        fix_shortage(index);
        print_tree();
        // cout<<"==========\n";
        // print_tree();
        // cout<<"==========\n";
        // if(data[index]==entry)
        // {
        //     //remove biggest item from left child
        //     subset[index+1]->remove_smallest(value);
        //     fix_shortage(index+1);
        //     T removed;
        //     delete_item(data,index,data_count,removed);
        //     insert_item(data,index,data_count,value);
        //     fix_shortage(index);
        // }
    }
}

template <typename T>
BPlusTree<T>* BPlusTree<T>::fix_shortage(int i)
{
    const bool debug = false;
    if(i>=child_count || i<0)
    {
        cout<<"out of bounds: "<<i<<endl;
        return nullptr;
    }
    //base case
    if(subset[i]->data_count >= MINIMUM) return nullptr;
    //print_tree();
    //4 cases
    // cout<<"fixing index: "<<i<<endl;
    //borrow from left(rotate_right)
    // cout<<"borrow left condition"<<endl;
    if(i>0 && subset[i-1]->data_count>MINIMUM)
    {
        cout<<"borrowing from left"<<endl;
        transfer_right(i);
        return NULL;
    }
    // cout<<"borrow right condition"<<endl;
    //borrow from right(rotate_left)
    if(i<child_count-1 && subset[i+1]->data_count>MINIMUM)
    {
        // cout<<"borrowing from right"<<endl;
        transfer_left(i);
        return NULL;
    }
    // cout<<"merge left condition"<<endl;
    //merge with left
    // cout<<"here: "<<subset[i-1]->data_count<<endl;
    if(i>0 && subset[i-1]->data_count<MAXIMUM)
    {
        cout<<"merging with left"<<endl;
        merge_with_next_subset(i-1);
        // print_tree();
        return nullptr;
    }
    // cout<<"merge right condition"<<endl;
    //merge with right
    
    cout<<"merging with right"<<endl;
    merge_with_next_subset(i);
    return NULL;
}
template <typename T>
BPlusTree<T>* BPlusTree<T>::get_smallest_node()
{
    if(!is_leaf())
        return subset[0]->get_smallest_node();
    else return this;
}

template <typename T>
BPlusTree<T>* BPlusTree<T>::get_biggest_node()
{
    if(!is_leaf())
        return subset[child_count-1]->get_biggest_node();
    else return this;
}

template <typename T>
void BPlusTree<T>::get_smallest(T& entry){
    if(!is_leaf())
        subset[0]->get_smallest(entry);
    else
        entry=data[0];
    const bool debug = false;
}

template <typename T>
void BPlusTree<T>::get_biggest(T& entry){
    if(!is_leaf())
        subset[child_count-1]->get_biggest(entry);
    else
        entry=data[data_count-1];
    const bool debug = false;
}

template <typename T>
void BPlusTree<T>::remove_biggest(T& entry)
{
    //go to right-most child
    if(!is_leaf())
    {
        subset[child_count-1]->remove_biggest(entry);
        fix_shortage(child_count-1);
    }
    else 
    {
        detach_item(data,data_count,entry);
    }
}

template <typename T>
void BPlusTree<T>::remove_smallest(T& entry)
{
    //go to left-most child
    if(!is_leaf())
    {
        subset[0]->remove_smallest(entry);
        fix_shortage(0);
    }
    else 
    {
        delete_item(data,0,data_count,entry);
    }
}

template <typename T>
BPlusTree<T>* BPlusTree<T>::merge_with_next_subset(int i)
{
    T value;
    T child;
    //move element i  of parent down to child at  i
    delete_item(data,i,data_count,value);
    // if(!subset[i]->is_leaf()) attach_item(subset[i]->data,subset[i]->data_count,value);
    //merge child at i+1 into child at i
    merge(subset[i]->data,subset[i]->data_count,subset[i+1]->data,subset[i+1]->data_count);
    merge(subset[i]->subset,subset[i]->child_count,subset[i+1]->subset,subset[i+1]->child_count);
    subset[i+1]->data_count=0;
    subset[i+1]->child_count=0;
    //delete empty child
    BPlusTree<T>* temp;
    delete_item(subset,i+1,child_count,temp);
    delete temp;
    temp=NULL;

    return subset[i];

}
template <typename T>
void BPlusTree<T>::transfer_left(int i)
{
    T value;
    BPlusTree* child=nullptr;
    if(!subset[i+1]->is_leaf())
    {
        //move first child  of next child over to end of prev child
        delete_item(subset[i+1]->subset,0,subset[i+1]->child_count,child);
        attach_item(subset[i]->subset,subset[i]->child_count,child);
    }
    //move parent data over to empty child (i)
    delete_item(data,i,data_count,value);
    attach_item(subset[i]->data,subset[i]->data_count,value);
    //move leftmost data over to parent at index i
    delete_item(subset[i+1]->data,0,subset[i+1]->data_count,value);
    insert_item(data,i,data_count,value);
}

template <typename T>
void BPlusTree<T>::transfer_right(int i)
{
    const bool debug = false;
    T value;
    BPlusTree<T>* child=nullptr;
    if(!subset[i-1]->is_leaf())
    {
        //move rightmost child over to left of next child
        detach_item(subset[i-1]->subset,subset[i-1]->child_count,child);
        insert_item(subset[i]->subset,0,subset[i]->child_count,child);
    }
    //move parent data over to empty child (i)
    delete_item(data,i-1,data_count, value);
    insert_item(subset[i]->data,0,subset[i]->data_count,value);
    //move rightmost data over to parent at index i - 1
    detach_item(subset[i-1]->data,subset[i-1]->data_count,value);
    insert_item(data,i-1,data_count,value);
}

//------------------------------------------------
//          I N S E R T
//------------------------------------------------
template <typename T>
void BPlusTree<T>::insert(const T& entry)
{
    //if(find(entry)) return;
    loose_insert(entry);
    _size++;
    //check root excess
    if(data_count>MAXIMUM)
    {
        //grow tree
        //shallow copy
        BPlusTree<T>* ins = new BPlusTree;
        copy_array(ins->data,data,ins->data_count,data_count);
        copy_array(ins->subset,subset,ins->child_count,child_count);
        //clear root
        data_count=0;
        child_count=0;
        insert_item(subset,0,child_count,ins);
        //fix excess
        fix_excess(0);
    }
}

template <typename T>
T* BPlusTree<T>::find_ptr(const T& entry)
{
    int index=first_ge(data, data_count,entry);
    //if data is found
    if(index<data_count&& data[index]==entry)
    {
        if(!is_leaf()) return &subset[index+1]->get_smallest_node();
        return &data[index];
    }
        
    if(is_leaf()) return nullptr;
    //if child exists, call find with child
    if(index<child_count && index>=0)
        return subset[index]->find_ptr(entry);
}

template <typename T>
void BPlusTree<T>::loose_insert(const T& entry)
{
    const bool debug = false;
    int index=first_ge(data, data_count,entry);
    // assert(index >=0);
    // assert(index<=data_count);

    bool found = index<data_count && data[index]==entry;

    if(is_leaf())
    {
        //duplicate case
        if (found)
        {
            if(dups_ok)
            {
                data[index]=data[index]+entry;
            }
        }
        //no duplicate, insert
        else insert_item(data, index, data_count, entry);
    }
    //didn't find item in parent
    else { //not a leaf:
        if(!found)
        {
            subset[index]->loose_insert(entry);
            fix_excess(index);
        }
        else //found and not lea
        {
            subset[index+1]->loose_insert(entry);
            fix_excess(index+1);
        }
    }
}

template <typename T>
void BPlusTree<T>::fix_excess(int i)
{
    //base case
    if(subset[i]->data_count <= MAXIMUM) return;

    //make new node
    BPlusTree<T>* ins = new BPlusTree;
    insert_item(subset,i+1,child_count, ins);

    //move second half of child data into new sibling
    split(subset[i]->data,subset[i]->data_count,ins->data,ins->data_count);

    //move second half of child children into new sibling
    split(subset[i]->subset,subset[i]->child_count,ins->subset,ins->child_count);
    
    //grad middle data and put at correct index of parent data
    
    //remove middle item from child
    T mid;
    detach_item(subset[i]->data,subset[i]->data_count,mid);
    if(subset[i]->is_leaf())
    {
        insert_item(subset[i+1]->data,0,subset[i+1]->data_count,mid);
        subset[i+1]->next=subset[i]->next;
        subset[i]->next=subset[i+1];
    }
    //insert middle item into parent, increase data count
    int parent_ins=first_ge(data,data_count,mid);
    insert_item(data, parent_ins, data_count, mid);
}

//---------------------------------------------------------------
//      S I Z E  /  E M P T Y
//---------------------------------------------------------------
template <typename T>
int BPlusTree<T>::size() const{
    //this function counts the number of keys in the BPlusTree
    return _size;
}

template <typename T>
bool BPlusTree<T>::empty() const{
    return (data_count ==0);
}
//---------------------------------------------------------------
//             P R I N T
//---------------------------------------------------------------

template <typename T>
void BPlusTree<T>::print_tree(int level,
                    ostream &outs) const
{
    //goes to right most child
    if(!is_leaf())  
    {
        subset[child_count-1]->print_tree(level+1, outs);
    }
    if(data_count<1) return;
        //prints all of data in current structure
        outs<<setw(10*level)<<"^"<<endl;
        for(int i=data_count-1;i>=0;i--)
        {
            
            outs<<setw(10*level)<<data[i]<<endl;
            //prints children after structure's own data
            
            if(child_count>0) subset[i]->print_tree(level+1, outs);
        }
        outs<<setw(10*level)<<"V"<<endl;
}

//---------------------------------------------------------------
//      C L E A R   /  C O P Y
//---------------------------------------------------------------

template <typename T>
void BPlusTree<T>::clear_tree(){
    //set data count to zero for all leaves
    data_count=0;
    for(int i=0;i<child_count;i++) 
    {
        subset[i]->clear_tree();
        delete subset[i];
    }
    child_count=0;
}

template <typename T>
void BPlusTree<T>::copy_tree(const BPlusTree<T>& other)
{
    BPlusTree<T>* last_leaf = nullptr;
    copy_tree(other,last_leaf);
}

template <typename T>
void BPlusTree<T>::copy_tree(const BPlusTree<T>& other, BPlusTree<T>*& last_node)
{
    child_count=0;
    data_count=0;
    //copy data over
    copy_array(data,other.data,data_count,other.data_count);
    //copy each child over
    for(int i=0;i<other.child_count;i++)
    {
        child_count++;
        subset[i]= new BPlusTree<T>;
        //call function recursively to copy lower leaves
        subset[i]->copy_tree(*other.subset[i]);
    }
    if(is_leaf())
    {
        if(last_node) last_node->next=this;
        last_node=this; 
    }
}

//---------------------------------------------------------------
//                       V E R I F Y
//---------------------------------------------------------------

template <typename T>
bool BPlusTree<T>::verify_node()
{
    bool verify=true;
    if(data_count<MINIMUM)
    {
        cout<<"============================================================="<<endl
        <<"ERROR: DATA COUNT IS LESS THAN MINIMUM"<<endl
        <<"============================================================="<<endl;
        verify=false;
    }
    if(data_count>MAXIMUM)
    {
        cout<<"============================================================="<<endl
        <<"ERROR: DATA COUNT IS GREATER THAN MAXIMUM"<<endl
        <<"============================================================="<<endl;
        verify=false;
    }
    if(child_count<0)
    {
        cout<<"============================================================="<<endl
        <<"ERROR: CHILD COUNT IS LESS THAN MINIMUM"<<endl
        <<"============================================================="<<endl;
        verify=false;
    }
    if(child_count  > (MAXIMUM + 1))
    {
        cout<<"============================================================="<<endl
        <<"ERROR: CHILD COUNT IS GREATER THAN MAXIMUM"<<endl
        <<"============================================================="<<endl;
        verify=false;
    }
    if( !is_leaf() && (child_count != (data_count + 1)))
    {
        cout<<"============================================================="<<endl
        <<"ERROR: CHILD COUNT IS NOT ONE LARGER THAN DATA COUNT"<<endl
        <<"============================================================="<<endl;
        verify=false;
    }
    if(!verify) exit(1);
    return verify;
}

template <typename T>
bool BPlusTree<T>::verify_tree()
{
    if(!verify_node()) return false;
    if(!is_leaf())
        for(int i=0;i<child_count;i++)
            if(!subset[i]->verify_tree()) return false;
    return true;
}

#endif
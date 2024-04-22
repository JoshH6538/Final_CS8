#ifndef MY_QUEUE_H
#define MY_QUEUE_H
#include <iostream>
#include "../linked_list_functions/linkedlistfunctionsTemplated.h"
using namespace std;

template <typename T>
class Queue
{
public:
    class Iterator{
    public:
        friend class Queue;                               //give access to list to access _ptr
        Iterator(){_ptr = NULL;}                          //default ctor
        Iterator(node<T>* p){_ptr=p;}                            //Point Iterator to where
                                                          //  p is pointing to
        T operator *(){return _ptr->_item;}    //dereference operator
        bool is_null(){if(_ptr == NULL) return true; else return false;}            //true if _ptr is NULL
        friend bool operator !=(const Iterator& left, const Iterator& right)  //true if left != right
        {
            if(left._ptr != right._ptr) return true;
            return false;
        }

        friend bool operator ==(const Iterator& left,
                                const Iterator& right)  //true if left == right
        {
            if(left._ptr == right._ptr) return true;
            else return false;
        }

        Iterator& operator++()
        {                         //member operator:++it
            // or ++it = new_value
            _ptr=_ptr->_next;
            return _ptr;
        }

        friend Iterator operator++(Iterator& it,
                                   int unused)
        {         //friend operator: it++
            assert(it._ptr!=NULL);
            node<T>* store_ptr=it._ptr;
            it._ptr=it._ptr->_next;
            return it._ptr;
        }

    private:
        node<T>* _ptr;                          //pointer being encapsulated
    };

    Queue();
    Queue(const Queue<T>& copyMe);
    ~Queue();
    Queue& operator=(const Queue<T>& RHS);
    bool empty();
    T front();
    T back();
    void push(T item);
    T pop();

    Iterator begin() const;                                  //Iterator to the head node
    Iterator end() const;                               //Iterator to NULL
    void print_pointers();
    int size() const;
    template<typename TT>
    friend ostream& operator << (ostream& outs, const Queue<TT>& printMe)
    {
        node<TT>* ptr=printMe._front;
        while(ptr!=NULL)
        {
            outs<<ptr->_item<<" ";
            ptr=ptr->_next;
        }
        return outs;
    }
private:
    node<T>* _front;
    node<T>* _rear;
    int _size;
};

//declaration ends here


template <typename T>
Queue<T>::Queue()
{
    _front=NULL;
    _rear=NULL;
    _size=0;
}

template <typename T>
Queue<T>::Queue(const Queue<T>& copyMe)
{
    if(copyMe._front==NULL)
    {
        _front=NULL;
        _rear=NULL;
        _size=0;
    }
    else
    {
        _front=_copy_list(copyMe._front);
        _rear=copyMe._rear;
        _size=copyMe._size;
    }
    
}

template <typename T>
Queue<T>::~Queue()
{
    _clear_list(_front);
}

template <typename T>
Queue<T>& Queue<T>::operator=(const Queue<T>& RHS)
 {
    if(RHS._front==NULL)
    {
        _front=NULL;
        _rear=NULL;
        _size=0;
        return *this;
    }
    _front=_copy_list(RHS._front);
    _size=RHS._size;
    return *this;
}

template <typename T>
bool Queue<T>::empty()
{
    if(_front==NULL) return true;
    else return false;
}

template <typename T>
T Queue<T>::front()
{return _front->_item;}

template <typename T>
T Queue<T>::back()
{return _rear->_item;}

template <typename T>
void Queue<T>::push(T item)
{
    //cout<<"pushing after "<<_rear<<endl;
    _rear=_insert_after(_front, _rear, item);
    _size++;
}

template <typename T>
T Queue<T>::pop()
{
    T value=_at(_front, 0);
    node<T>* remove = _front;
    _front=_front->_next;
    _delete_head(remove);
    _size--;
    return value;
}
template <typename T>
typename Queue<T>::Iterator Queue<T>::begin() const
{
    return Iterator(_front);
}   

template <typename T>
typename Queue<T>::Iterator Queue<T>::end() const
{
    return Iterator(NULL);
}

template <typename T>
void Queue<T>::print_pointers()
{
    for( node<T>* curr=_front;curr!=NULL;curr=curr->_next)
    {
        cout<<curr<<" ";
    }
    cout<<endl;
}

template <typename T>
int Queue<T>::size() const
{ return _size; }

#endif
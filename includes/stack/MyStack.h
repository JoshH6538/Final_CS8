#ifndef MY_STACK_H
#define MY_STACK_H
#include <iostream>
using namespace std;

template <typename ITEM_TYPE>
class Stack{
public:
    class Iterator{
    public:
        friend class Stack;                 //give access to list to access _ptr
        Iterator(){_ptr = NULL;}            //default ctor
        Iterator(node<ITEM_TYPE>* p){_ptr=p;}      //Point Iterator to where
                                            //  p is pointing to
        ITEM_TYPE operator *(){return _ptr->_item;}    //dereference operator
        bool is_null(){if(_ptr == NULL) return true; else return false;}            //true if _ptr is NULL
        friend bool operator !=(const Iterator& left,
                                const Iterator& right)  //true if left != right
        {
            if(left._ptr != right._ptr) return true;
            return false;
        }

        friend bool operator ==(const Iterator& left,
                                const Iterator& right)  //true if left == right
        {if(left._ptr == right._ptr) return true;
            else return false;}


        Iterator& operator++(){                        //member operator:++it
                                                       // or ++it = new_value
            _ptr=_ptr->_next;
            return _ptr;
        }

        friend Iterator operator++(Iterator& it,
                                   int unused){        //friend operator: it++
            assert(it._ptr!=NULL);
            node<ITEM_TYPE>* store_ptr=it._ptr;
            it._ptr=it._ptr->_next;
            return it._ptr;
        }

    private:
        node<ITEM_TYPE>* _ptr;    //pointer being encapsulated
    };

    Stack();
    Stack(const Stack<ITEM_TYPE>& copyMe);
    ~Stack();
    Stack<ITEM_TYPE>& operator=(const Stack<ITEM_TYPE>& RHS);
    ITEM_TYPE top();
    bool empty();
    void push(ITEM_TYPE item);
    ITEM_TYPE pop();
    template<typename T>
    friend ostream& operator<<(ostream& outs, const Stack<T>& printMe)
    {
        node<ITEM_TYPE>* ptr=printMe._top;
        while(ptr!=NULL)
        {
            outs<<ptr->_item<<" ";
            ptr=ptr->_next;
        }
        return outs;
    }
    Iterator begin() const;                 //Iterator to the head node
    Iterator end() const;             //Iterator to NULL
    int size() const;

private:
    node<ITEM_TYPE>* _top;
    int _size;
};


//declaration ends here


template <typename ITEM_TYPE>
Stack<ITEM_TYPE>::Stack()
{_top=NULL;_size=0;}

template <typename ITEM_TYPE>
Stack<ITEM_TYPE>::Stack(const Stack<ITEM_TYPE>& copyMe)
{
    if(copyMe._top==NULL)
    {
        _top=NULL;
        _size=0;
    }
    else{
        _top=_copy_list(copyMe._top);
        _size=copyMe._size;
    }
    
}

template <typename ITEM_TYPE>
Stack<ITEM_TYPE>::~Stack()
{ _clear_list(_top); }

template <typename ITEM_TYPE>
Stack<ITEM_TYPE>& Stack<ITEM_TYPE>::operator=(const Stack<ITEM_TYPE>& RHS)
{
    if(RHS._top==NULL)
    {
        _top=NULL;
        _size=0;
    }
    else{
        _top=_copy_list(RHS._top);
        _size=RHS._size;
    }
    
    return *this;
}

template <typename ITEM_TYPE>
ITEM_TYPE Stack<ITEM_TYPE>::top()
{return _top->_item;}

template <typename ITEM_TYPE>
bool Stack<ITEM_TYPE>::empty()
{
    if(_top==NULL) return true;
    else return false;
}

template <typename ITEM_TYPE>
void Stack<ITEM_TYPE>::push(ITEM_TYPE item)
{
    _insert_head(_top, item);
    _size++;
}

template <typename ITEM_TYPE>
ITEM_TYPE Stack<ITEM_TYPE>::pop()
{
    ITEM_TYPE value=_at(_top,0);
    node<ITEM_TYPE>* remove =_top;
    _top=_top->_next;
    _delete_head(remove);
    _size--;
    return value;
}

template <typename ITEM_TYPE>
typename Stack<ITEM_TYPE>::Iterator Stack<ITEM_TYPE>::begin() const
{ return Iterator(_top); } 

template <typename ITEM_TYPE>
typename Stack<ITEM_TYPE>::Iterator Stack<ITEM_TYPE>::end() const
{return Iterator(NULL);}  

template <typename ITEM_TYPE>
int Stack<ITEM_TYPE>::size() const
{ return _size; }

#endif
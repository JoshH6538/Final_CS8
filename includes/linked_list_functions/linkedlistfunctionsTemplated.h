#ifndef LINKED_LIST_FUNCTIONS_TEMPLATED_H
#define LINKED_LIST_FUNCTIONS_TEMPLATED_H
#include <iostream>
#include "../node/node.h"
using namespace std;

//Linked List General Functions:
template <typename T>
void _print_list(node<T>* head);

//recursive fun! :)
template <typename T>
void _print_list_backwards(node<T> *head);

//return ptr to key or NULL
template <typename T>
node<T>* _search_list(node<T>* head,
                            T key);


template <typename T>
node<T>* _insert_head(node<T> *&head,
                            T insert_this);

//insert after ptr: insert head if marker null
template <typename T>
node<T>* _insert_after(node<T>*& head,
                                node<T> *after_this,
                                T insert_this);

//insert before ptr: insert head if marker null
template <typename T>
node<T>* _insert_before(node<T>*& head,
                                node<T>* before_this,
                                T insert_this);

//ptr to previous node
template <typename T>
node<T>* _previous_node(node<T>* prev_to_this);

//delete, return item
template <typename T>
T _delete_node(node<T>*& head, node<T>* delete_this);

//duplicate the list...
template <typename T>
node<T>* _copy_list(node<T>* head);

//duplicate the list, return pointer to last node in dest... 
//     use this function in your queue big three 
template <typename T>
node<T> *_copy_list(node<T> *&dest, node<T> *src);



//delete all the nodes
template <typename T>
void _clear_list(node<T>*& head);

//_item at this position
template <typename T>
T& _at(node<T>* head, int pos);

#endif

//Linked List General Functions:
template <typename T>
void _print_list(node<T>* head)
{
    //for each node in list
    for(node<T>* ptr=head;ptr!=NULL;ptr=ptr->_next)
    {
        //print item of node
        cout<<ptr->_item<<" ";
    }
    cout<<endl;
}

//recursive fun! :)
template <typename T>
void _print_list_backwards(node<T> *head)
{
    //search for tail by going through list
    for(node<T>* ptr=head;ptr!=NULL;ptr=ptr->_next)
    {
        if(ptr->_next==NULL)
        {
            node<T>* tail=ptr;
            //for each node in list going backwards from tail
            for(node<T>* ptr=tail;ptr!=NULL;ptr=ptr->_prev)
            {
                //print item of node
                cout<<ptr->_item<<" ";
            }
            cout<<endl;
        }
    }
    cout<<endl;
}

//return ptr to key or NULL
template <typename T>
node<T>* _search_list(node<T>* head, T key)
{
    //start at the head and move to next node until NULL
    for(node<T>* ptr=head;ptr!=NULL;ptr=ptr->_next)
    {
        //return ptr if match with key
        if(ptr->_item==key) 
        {
            cout<<ptr->_item<<endl;
            return ptr;
        }
    }
    return NULL;
}


template <typename T>
node<T>* _insert_head(node<T> *&head, T insert_this)
{
    //make new head node
    node<T>* head_new = new node<T>;
    //assign item
    head_new->_item=insert_this;
    //new head will point to old head
    head_new->_next=head;
    //old head prev will point to new head
    if (head) {
    head->_prev=head_new;
    //no node before head
    head_new->_prev=NULL;
    }
    // assign the new head as head
    head=head_new;
    return head;
    }

template <typename T>
node<T>* _delete_head(node<T> *&head)
{
    if(head->_next!=NULL)
    {
    //store the node after head
    node<T>* store_next=head->_next;
    //assign this store node as the new head
    head=store_next;
    //delete original head
    delete(store_next->_prev);
    //cut link to prev node
    store_next->_prev=NULL;
    }
    else{head=NULL;}
    //return new head
    return head;
}

//insert after ptr: insert head if marker null
template <typename T>
node<T>* _insert_after(node<T>*& head,
node<T> *after_this, T insert_this)
{
    if(head==NULL)
    {
        // cout<<"inserting at head: "<<insert_this<<endl;
        _insert_head(head, insert_this);
        return head;
    };
    //cout<<"head: "<<head->_item<<endl;
    if(after_this==NULL){
        _insert_head(head, insert_this);
        return head;
    }
    else if(after_this->_next!=NULL){
        // cout<<"inserting in middle: "<<insert_this<<endl;
    //store node after insertion place
    node<T>* store_next= after_this->_next;
    //create node to be inserted
    node<T>* node_ins = new node<T>;
    //make destination node point to new node
    after_this->_next=node_ins;  
    //insert item into new node          
    node_ins->_item=insert_this;
    //make new node point back to prev node
    node_ins->_prev=after_this;
    //make new node point to the next node
    node_ins->_next=store_next;
    //make node after new node point back to new node
    store_next->_prev=node_ins;
    //return the new node
    return node_ins;
    }
    else
    {
        // cout<<"inserting at end: "<<insert_this<<endl;
        //create node to be inserted
        node<T>* node_ins = new node<T>;
         //make destination node point to new node
        after_this->_next=node_ins;  
        //insert item into new node          
        node_ins->_item=insert_this;
        //make new node point back to prev node
        node_ins->_prev=after_this;
        node_ins->_next=NULL;
        return node_ins;
    }

    // cout<<"end of prog\n";
}

//insert before ptr: insert head if marker null
template <typename T>
node<T>* _insert_before(node<T>*& head,
node<T>* before_this, T insert_this)
{
    if(before_this==NULL) _insert_head(head, insert_this);
    else{
    //create node to be inserted
    node<T>* node_ins = new node<T>;
    //insert item into new node
    node_ins->_item=insert_this;
    //store the node before destination
    node<T>* ptr=before_this->_prev;
    //store the node after destination
    node<T>* store_next= before_this;
    //replace the node after destination with new node
    before_this=node_ins;
    //make new node point to node previous to destination
    node_ins->_prev=ptr;
    //make new node point to node after destination
    node_ins->_next=store_next;
    //make nodes point back to new node
    store_next->_prev=node_ins;
    ptr->_next=node_ins;
    //return head of list
    }
    return head;
}

//ptr to previous node
template <typename T>
node<T>* _previous_node(node<T>* prev_to_this){
    return prev_to_this->_prev;
}

//delete, return item
template <typename T>
T _delete_node(node<T>*& head, node<T>* delete_this)
{
    node<T>* temp=delete_this;
    T value;
    //delete node in middle of list
    if(delete_this->_prev!=NULL && delete_this->_next!=NULL)
    {
        node<T>* temp2=temp->_prev;
        temp2->_next=temp->_next;
        temp->_next->_prev=temp2;
        T value=temp->_item;
        delete temp;
        temp=NULL;
    }
    //delete tail
    if(delete_this->_prev!=NULL && delete_this->_next==NULL)
    {
        node<T>* temp2=temp->_prev;
        temp2->_next=NULL;
        T value=temp->_item;
        delete temp;
        temp=NULL;
    }
    return value;
}
//duplicate the list...
template <typename T>
node<T>* _copy_list(node<T>* head)
{
    //get value of head and assign first node as new head and tail
    node<T> *head2, *temp, *tail2;
    temp=new node<T>;
    temp->_item=head->_item;
    temp->_next=NULL;
    temp->_prev=NULL;
    head2=temp;
    tail2=temp;
    //for each node in the original list, create a new node in the duplicate with its value and assign the latest as the new tail
    for(node<T>* ptr=head->_next;ptr!=NULL;ptr=ptr->_next)
    {
        temp=new node<T>;
        temp->_item=ptr->_item;
        temp->_next=NULL;
        temp->_prev=tail2;
        tail2->_next=temp;
        tail2=temp;
    }
    //return duplicate list
    return head2;
}

//duplicate the list, return pointer to last node in dest... 
//     use this function in your queue big three 
template <typename T>
node<T> *_copy_list(node<T> *&dest, node<T> *src)
{
    //get value of head and assign first node as new head and tail
    node<T> *temp, *tail2;
    temp=new node<T>;
    temp->_item=dest->_item;
    temp->_next=NULL;
    temp->_prev=NULL;
    src=temp;
    tail2=temp;
    //for each node in the original list, create a new node in the duplicate with its value and assign the latest as the new tail
    for(node<T>* ptr=dest->_next;ptr!=NULL;ptr=ptr->_next)
    {
        temp=new node<T>;
        temp->_item=ptr->_item;
        temp->_next=NULL;
        temp->_prev=tail2;
        tail2->_next=temp;
        tail2=temp;
    }
    //return the tail of the new list
    return tail2;
}

//delete all the nodes
template <typename T>
void _clear_list(node<T>*& head)
{
    node<T>* ptr1, *ptr2;
    ptr1=head;
    while(ptr1!=NULL)
    {
        ptr2=ptr1;
        ptr1=ptr1->_prev;
        ptr2->_prev=NULL;
        delete ptr2;
    }
}

//_item at this position
template <typename T>
T& _at(node<T>* head, int pos)
{
    int i=0;
    for(node<T>* ptr=head;i<pos;ptr=ptr->_next)
    {
        //return ptr if match with key
        if(i==pos && ptr!=NULL) return ptr->_item;
        i++;
        if(ptr->_next==NULL) break;
    }
    return head->_item;
}
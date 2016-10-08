#include "list.h"
#include <iostream>
using namespace std;


// List node constructors provided by Prof. Campbell

list_node::list_node() : prev(NULL), next(NULL) {}

list_node::list_node(list_node *p, list_node *n) : prev(p), next(n) {}

list_node::list_node(list_node *p, const list_element & d, list_node * n):
  prev(p), data(d), next(n) {}

// List constructors/destructor/assignment also provided.  
// No need to change these.

list::list() 
{
  _init();
}

list::~list()
{
  _destroy();
}

list::list(const list & orig)
{
  _copy(orig);
}

list & list::operator=(const list & rhs)
{
  if (this != &rhs) {
    _destroy();
    _copy(rhs);
  }
  return *this;
}

// List method stubs provided.  Here's where you come in. You delete this
// comment and fill in the method bodies.

// initializes an empty list by creating a front and rear sentinel and
// connecting the two in order to form an empty list. 
void list::_init()
{
  list_node * sentinel_front = new list_node();
  _front = sentinel_front;
  list_node * sentinel_rear = new list_node();
  _rear = sentinel_rear;
  sentinel_front -> next = sentinel_rear;
  sentinel_rear -> prev = sentinel_front;
  // sets _current to rear sentinel, current index to 0, and size to 0
  _current = _rear;
  _size = 0;
  _current_index = 0;
}

// copy constructor—used to make copies of a list
void list::_copy(const list & orig)
{
  // initializes a new list on which to copy the old list
  _init();
  // copies the data of each individual element of the old list to the new list
  for(size_t i = 0; i < orig.size(); i += 1){
    // copy only the data, not the node in the old list
    list_element data_copy = orig.get(i);
    // place data into new node and place node in list
    list_node * p = new list_node(_front -> next, data_copy, _rear -> prev);
    // adds the first element in the list 
    if (_front == NULL){
      _front -> next = p;
      _rear -> next = p;
    }
    // adds all the elements following the first element
    else{
      p -> prev = _rear;
      _rear -> next = p;
      _rear = _rear -> next = p;
    }
  }
}

// destructor, used to get rid of the list after completion
void list::_destroy()
{
  list_node * p = new list_node;
  // set p equal to _front and as _front cycles through the list, p gets deleted
  while (_front != NULL){
    p = _front;
    _front = _front -> next;
    delete p;
  }
}

// get obtains the data in a node by using index by using _set_current_index to
// set _current pointer to the node indicated by index and return the data
list_element list::get(size_t index) const
{
  _set_current_index(index);
  return _current -> data;
}

// adds element to the list
void list::add(const list_element & item, size_t index)
{
  // set _current to index, where you want to place the node
  _set_current_index(index);
  // create node
  list_node * p = new list_node(_current -> prev, item, _current);
  // connect node to the list
  _current -> prev -> next = p;
  _current -> prev = p;
  // increase _size by 1
  _size = _size + 1;
}

// removes the node at a given index
void list::remove_at(size_t index)
{
  // uses _current to find node that you want to remove
  _set_current_index(index);
  // connect the nodes around _current
  _current -> prev -> next = _current -> next;
  _current -> next -> prev = _current -> prev;
  // delete _current
  delete _current;
  // decrease _size
  _size = _size - 1;
  // reset _current to point to the rear sentinel
  _set_current_index(_size);
}

// removes the first node containing a given item
void list::remove(const list_element & item)
{
  // rotates through list to find item
  for (size_t i; i < _size; i += 1){
    if (list().get(i) == item){
      // removes the item by assigning _current and removing that node while
      // reconnecting the adjacent nodes
      _set_current_index(i);
      _current -> prev -> next = _current -> next;
      _current -> next -> prev = _current -> prev;
      delete _current;
      // decreases size
      _size = _size - 1;
      _set_current_index(_size);
      return;
    }
  }
}

// finds the node containing a certain item and returns the index. 
// if item does not exist, returns _size
size_t list::find(const list_element & item) const
{
  for (size_t i = 0; i < _size; i += 1){
    if (get(i) == item){
      return i;
    }
  }
  return _size;
}

// returns size
size_t list::size() const
{
  return _size;
}

// outputs the list by rotating the list with _current
void list::output(std::ostream & ostr) const
{
  // sets _current to the first element in the list
  _current = _front -> next;
  // sets up and prints first item
  ostr << '<' << _current -> data;
  // rotates through the list and prints each item
  while (_current -> next -> next){
    _current = _current -> next;
    ostr << ", "<< _current -> data;
  }
  // ends list
  ostr << '>';
}

// sets _current to a given index and sets _current_index to index
void list::_set_current_index(size_t index) const
{
  // only works if index is less that _size
  if (index <= _size){
    // sets _current_index to index and sets _current to first element of list
    _current_index = index;
    _current = _front -> next;
    // rotates through list with _current
    for (size_t i = 0; i < index; i += 1)
      _current = _current -> next;
  }
  // if _size = 0, set _current to _rear
  if (_size == 0)
    _current = _rear;
}

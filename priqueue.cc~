#include <cassert>
#include <cstdlib>

template <typename T>
priqueue<T>::priqueue(priorityfn pri)
{
  _pri = pri;
  _size = 0;
}

template <typename T>
T priqueue<T>::front() const
{
  assert(not empty());
  return _data[0];
}

template <typename T>
void priqueue<T>::add(const element_type & item)
{
  assert(_size != MAX);
  _data[_size++] = item;
  _reheap_up(_size-1);
}

template <typename T>
void priqueue<T>::remove_front()
{
  assert(not empty());
  _data[0] = _data[--_size];
  _reheap_down(0);
}

template <typename T>
bool priqueue<T>::empty() const
{
  return _size == 0;
}

template <typename T>
size_t priqueue<T>::size() const
{
  return _size;
}

size_t parent(size_t i)
{
  return (i-1)/2;
}

size_t left_child(size_t i)
{
  return 2*i + 1;
}

size_t right_child(size_t i)
{
  return 2*i + 2;
}


template <typename T>
void swap(T & item1, T & item2)
{
  T temp = item1;
  item1 = item2;
  item2 = temp;
}

template <typename T>
void priqueue<T>::_reheap_up(size_t i) 
{
  // base case: root.  nothing to do
  if (i==0) return;
  // otherwise, compare with parent.
  size_t p = parent(i);
  // if the child's priority is less than the parent's priority
  if (_pri(_data[i]) < _pri(_data[p])) {
    // swap and continue...
    swap(_data[i],_data[p]);
    _reheap_up(p);
  }
  // otherwise, stop.
}

template <typename T>
void priqueue<T>::_reheap_down(size_t i)
{
  size_t left = left_child(i);
  size_t right = right_child(i);
  // if at a leaf... stop.
  if (left >= _size) return;
  // otherwise choose a child...
  size_t child = left;
  if (right < _size and _pri(_data[right]) < _pri(_data[left]))
    child = right;
  // if parent's priority is greater than the child's, 
  // swap and continue 
  if (_pri(_data[i]) > _pri(_data[child])) {
    swap(_data[i],_data[child]);
    _reheap_down(child);
  }
  // or stop.
}

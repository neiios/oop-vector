#pragma once

#include <iostream>
#include <memory>
#include <string>

template <class T>
class Vector {
 public:
  typedef T* iterator;
  typedef const T* const_iterator;
  typedef size_t size_type;
  typedef T value_type;

  // rule of three
  Vector() { create(); }
  explicit Vector(size_type n, const T& t = T{}) { create(n, t); }
  Vector(const Vector& v) { create(v.begin(), v.end()); }
  Vector& operator=(const Vector&);
  ~Vector() { uncreate(); }

  T& operator[](size_type i) { return data[i]; }
  const T& operator[](size_type i) const { return data[i]; }
  void push_back(const T& t) {
    if (avail == limit)
      grow();
    unchecked_append(t);
  }

  size_type size() const { return avail - data; }
  iterator begin() { return data; }
  const_iterator begin() const { return data; }
  iterator end() { return avail; }
  const_iterator end() const { return avail; }

  // likusi interfeiso realizacija
 private:
  iterator data;   // kaip buvo anksčiau
  iterator avail;  // pirmasis po paskutiniojo sukonstruoto Vector elementas
  iterator limit;  // pirmasis po paskutiniojo Vector elementas
  // atminties išskyrimo valdymui
  std::allocator<T> alloc;  // objektas atminties valdymui
  // išskirti atmintį (array) ir ją inicializuoti
  void create();
  void create(size_type, const T&);
  void create(const_iterator, const_iterator);
  // sunaikinti elementus array ir atlaisvinti atmintį
  void uncreate();
  // pagalbinės funkcijos push_back realizacijai
  void grow();
  void unchecked_append(const T&);
};

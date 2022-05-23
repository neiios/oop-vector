#pragma once

#include <cmath>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

template <class T>
class Vector {
 public:
  // typedefs
  typedef T* iterator;
  typedef const T* const_iterator;
  typedef size_t size_type;
  typedef T value_type;
  typedef std::allocator<T> allocator_type;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  typedef std::reverse_iterator<iterator> reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

  /**
   * @brief Construct a new Vector object.
   *
   */
  Vector() { create(); }
  /**
   * @brief Construct a new Vector object
   *
   * @param n
   * @param t
   */
  explicit Vector(size_type n, const T& t = T{}) { create(n, t); }
  /**
   * @brief Copy constructor.
   *
   * @param v Source object.
   */
  Vector(const Vector& v) { create(v.begin(), v.end()); }
  /**
   * @brief Destroy the Vector object.
   *
   */
  ~Vector() { uncreate(); }
  /**
   * @brief Move constructor.
   *
   * @param vector
   */
  Vector(Vector&& vector) noexcept {
    create(vector.begin(), vector.end());
    vector.swap(*this);
  }
  /**
   * @brief List initializer.
   *
   * @param il
   */
  Vector(const std::initializer_list<T>& il) { create(il.begin(), il.end()); }
  /**
   * @brief Copy assignment operator. Replaces the contents with a copy of the
   * contents of other.
   *
   * @param rhs Container to use as a data source.
   * @return Vector&
   */
  Vector& operator=(const Vector& rhs) {
    // patikriname ar ne lygu
    if (&rhs != this) {
      // atlaisviname seną this objekto atmintį
      uncreate();
      // perkopijuojame elementus iš rhs į lhs (this)
      create(rhs.begin(), rhs.end());
    }
    return *this;
  }
  /**
   * @brief Assigns new contents to the vector, replacing its current contents,
   * and modifying its size accordingly.
   *
   * @param count New size.
   * @param value Value to fill the container with.
   */
  void assign(size_type count, const T& value) {
    uncreate();
    create(count, value);
  }
  /**
   * @brief Get the allocator object.
   *
   * @return allocator_type Returns allocator object.
   */
  allocator_type get_allocator() const { return alloc; }

  // Element access
  /**
   * @brief Returns a reference to the element at specified location pos, with
   * bounds checking.
   *
   * @param pos Position of the element to return.
   * @return reference
   */
  reference at(size_type pos) {
    // range check
    if (pos >= this->size()) {
      throw std::out_of_range(
          "Range check failed. Pos argument is out of bounds.");
    }
    return (*this)[pos];
  }
  /**
   * @brief Access specified element
   *
   * @param pos Position of the element to return
   * @return T&
   */
  T& operator[](size_type pos) { return data[pos]; }
  /**
   * @brief Const version of [] operator.
   *
   * @param pos Position of the element to return
   * @return const T&
   */
  const T& operator[](size_type pos) const { return data[pos]; }
  /**
   * @brief Return the first element.
   *
   * @return reference
   */
  reference front() { return *(begin()); }
  /**
   * @brief Const version of front().
   *
   * @return const_reference
   */
  const_reference front() const { return *(begin()); }
  /**
   * @brief Return the last element.
   *
   * @return reference
   */
  reference back() { return *(end() - 1); }
  /**
   * @brief Const version of back().
   *
   * @return const_reference
   */
  const_reference back() const { return *(end() - 1); }
  /**
   * @brief Return pointer to the underlying array (basically data() function
   * from std::vector).
   *
   * @return T*
   */
  T* directData() { return data; }
  /**
   * @brief Const version of directData() function.
   *
   * @return const T*
   */
  const T* directData() const { return data; }

  // Iterators
  // variations of begin
  iterator begin() { return data; }
  const_iterator begin() const { return data; }
  const_iterator cbegin() const { return begin(); }
  reverse_iterator rbegin() { return reverse_iterator(end()); }
  const_reverse_iterator rbegin() const {
    return const_reverse_iterator(end());
  }
  const_reverse_iterator crbegin() const { return rbegin(); }
  // variations of end
  iterator end() { return avail; }
  const_iterator end() const { return avail; }
  const_iterator cend() const { return end(); }
  reverse_iterator rend() { return reverse_iterator(begin()); }
  const_reverse_iterator rend() const {
    return const_reverse_iterator(begin());
  }
  const_reverse_iterator crend() const { return rend(); }

  // Capacity
  /**
   * @brief Returns true if vector is empty.
   *
   * @return true
   * @return false
   */
  bool empty() const { return begin() == end(); }
  /**
   * @brief Returns the number of elements
   *
   * @return size_type
   */
  size_type size() const { return avail - data; }
  /**
   * @brief Should return the largest possible size (though you can calculate it
   * differently)
   *
   * @return size_type
   */
  size_type max_size() const {
    return pow(2, sizeof(void*) * 8) / sizeof(T) - 1;
  }
  /**
   * @brief Increase the capacity of the vector.
   *
   * @param new_cap New capacity of the vector (number of elements).
   */
  void reserve(size_type new_cap) {
    if (new_cap > size_type(limit - data)) {
      size_type new_size = new_cap;
      // išskirti naują vietą ir perkopijuoti egzistuojančius elementus
      iterator new_data = alloc.allocate(new_size);
      iterator new_avail = std::uninitialized_copy(data, avail, new_data);
      // atlaisvinti seną vietą
      uncreate();
      // reset'int rodykles į naujai išskirtą vietą
      data = new_data;
      avail = new_avail;
      limit = data + new_size;
    }
  }
  /**
   * @brief Return the number of elements the container has allocated space for.
   *
   * @return size_type
   */
  size_type capacity() const { return size_type(limit - data); }

  /**
   * @brief Requests the removal of unused capacity.
   * Will ALWAYS reallocate memory on function call.
   */
  void shrink_to_fit() {
    size_type new_size = size_type(avail - data);
    // išskirti naują vietą ir perkopijuoti egzistuojančius elementus
    iterator new_data = alloc.allocate(new_size);
    iterator new_avail = std::uninitialized_copy(data, avail, new_data);
    // atlaisvinti seną vietą
    uncreate();
    // reset'int rodykles į naujai išskirtą vietą
    data = new_data;
    avail = new_avail;
    limit = data + new_size;
  }

  // Modifiers
  /**
   * @brief Erases all elements from the container.
   *
   */
  void clear() {
    iterator it = avail;
    while (it != data)
      alloc.destroy(--it);
    avail = data;
  }
  /**
   * @brief Inserts n copies of the val before position.
   *
   * @param position Iterator before which the content will be inserted.
   * @param n Number of elements to insert
   * @param val Element value to insert
   * @return iterator
   */
  iterator insert(iterator position, size_type n, const value_type& val) {
    if (position < begin() || position >= end()) {
      throw std::out_of_range("Index out of range");
    }

    if (avail + n >= limit) {
      size_type index = position - begin();
      grow();
      position = begin() + index;
    }

    int i = 0;
    for (iterator it(avail + n - 1); it != (position + n - 1); it--, i++) {
      *it = data[size() - i - 1];
    }

    std::uninitialized_fill(position, position + n, val);
    avail += n;

    return position;
  }
  /**
   * @brief Insert a single element.
   *
   * @param position Iterator before which the content will be inserted.
   * @param val Element value to insert.
   * @return iterator.
   */
  iterator insert(iterator position, const value_type& val) {
    return insert(position, 1, val);
  }
  /**
   * @brief Removes the element at pos.
   *
   * @param pos Iterator to the element to remove.
   * @return iterator
   */
  iterator erase(iterator pos) {
    if (pos < data || pos > avail) {
      throw std::out_of_range("Index out of range");
    }

    iterator new_available = std::uninitialized_copy(pos + 1, avail, pos);
    alloc.destroy(avail + 1);

    avail = new_available;

    return pos;
  }
  /**
   * @brief Removes the elements in the range [first, last).
   *
   * @param first
   * @param last
   * @return iterator
   */
  iterator erase(iterator first, iterator last) {
    if (first < data || last > avail) {
      throw std::out_of_range("Index out of range");
    }

    iterator new_available = std::uninitialized_copy(last, avail, first);

    iterator it = avail;
    while (it != new_available) {
      alloc.destroy(--it);
    }

    avail = new_available;
    return last;
  }
  /**
   * @brief Appends the given element value to the end of the container. The new
   * element is initialized as a copy of value.
   *
   * @param t Value to append.
   */
  void push_back(const T& t) {
    if (avail == limit)
      grow();
    unchecked_append(t);
  }
  /**
   * @brief Appends the given element value to the end of the container. Value
   * is moved into the new element.
   *
   * @param t
   */
  void push_back(T&& t) {
    if (avail == limit)
      grow();
    unchecked_append(t);
  }
  /**
   * @brief Removes the last element of the container. Calling pop_back on an
   * empty container results in undefined behavior.
   *
   */
  void pop_back() {
    iterator it = avail;
    alloc.destroy(--it);
    avail = it;
  }
  /**
   * @brief Resizes the container to contain n elements.
   *
   * @param n New size of the vector.
   */
  void resize(size_type n) {
    if (n < size()) {
      avail = data + n;
      iterator it = limit;
      while (it != avail) {
        alloc.destroy(--it);
      }
    } else if (n > capacity()) {
      grow();
      resize(n);
    } else if (n > size()) {
      avail += n - size();
    }
  }
  /**
   * @brief Resizes the container to contain n elements. If n is larger than
   * size() fills new elements with value.
   *
   * @param n New size of the container.
   * @param value The value to initialize the new elements with.
   */
  void resize(size_type n, const value_type& value) {
    if (n > capacity()) {
      grow();
      resize(n, value);
    } else if (n > size()) {
      size_type i = size();
      avail += n - size();

      for (; i < n; i++) {
        data[i] = value;
      }
    } else {
      resize(n);
    }
  }
  /**
   * @brief Exchanges the contents of the container with those of other.
   *
   * @param other
   */
  void swap(Vector& other) {
    std::swap(data, other.data);
    std::swap(avail, other.avail);
    std::swap(limit, other.limit);
  }

  // Non-member functions
  // Comparison operators
  bool operator==(const Vector<T>& rhs) const {
    return size() == rhs.size() && std::equal(begin(), end(), rhs.begin());
  }
  bool operator!=(const Vector<T>& rhs) { return !(*this == rhs); }
  bool operator<(const Vector<T>& rhs) {
    return std::lexicographical_compare(begin(), end(), rhs.begin(), rhs.end());
  }
  bool operator>(const Vector<T>& rhs) {
    return std::lexicographical_compare(rhs.begin(), rhs.end(), begin(), end());
  }
  bool operator>=(const Vector<T>& rhs) { return !(*this < rhs); }
  bool operator<=(const Vector<T>& rhs) { return !(*this > rhs); }

  void swap(Vector<T>& x, Vector<T>& y) { std::swap(x, y); }

 private:
  iterator data;
  iterator avail;
  iterator limit;
  std::allocator<T> alloc;

  void create() { data = avail = limit = nullptr; }
  void create(size_type n, const T& val) {
    data = alloc.allocate(n);
    limit = avail = data + n;
    std::uninitialized_fill(data, limit, val);
  }
  void create(const_iterator i, const_iterator j) {
    data = alloc.allocate(j - i);
    limit = avail = std::uninitialized_copy(i, j, data);
  }

  void uncreate() {
    if (data) {
      iterator it = avail;
      while (it != data)
        alloc.destroy(--it);
      alloc.deallocate(data, limit - data);
    }
    data = limit = avail = nullptr;
  }

  void grow() {
    size_type new_size = std::max(2 * (limit - data), ptrdiff_t(1));
    iterator new_data = alloc.allocate(new_size);
    iterator new_avail = std::uninitialized_copy(data, avail, new_data);
    uncreate();
    data = new_data;
    avail = new_avail;
    limit = data + new_size;
  }

  void unchecked_append(const T& val) { alloc.construct(avail++, val); }
};

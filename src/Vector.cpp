#include "Vector.hpp"

// priskyrimo operatoriaus realizacija
template <class T>
Vector<T>& Vector<T>::operator=(const Vector& rhs) {
  // patikriname ar ne lygu
  if (&rhs != this) {
    // atlaisviname seną this objekto atmintį
    uncreate();
    // perkopijuojame elementus iš rhs į lhs (this)
    create(rhs.begin(), rhs.end());
  }
  return *this;
}

template <class T>
void Vector<T>::create() {
  data = avail = limit = nullptr;
}
template <class T>
void Vector<T>::create(size_type n, const T& val) {
  data = alloc.allocate(n);  // grąžina ptr į array pirmą elementą
  limit = avail = data + n;  // sustato rodykles į vietas
  uninitialized_fill(data, limit, val);  // inicializuoja elementus val reikšme
}
template <class T>
void Vector<T>::create(const_iterator i, const_iterator j) {
  data = alloc.allocate(j - i);  // išskirti vietos j-i elementams
  limit = avail =
      uninitialized_copy(i, j, data);  // nukopijuoja elementus iš intervalo
}

template <class T>
void Vector<T>::uncreate() {
  if (data) {
    // sunaikinti (atbuline tvarka) sukonstruotus elementus
    iterator it = avail;
    while (it != data)
      alloc.destroy(--it);
    // atlaisvinti išskirtą atmintį. Turi būti data != nullptr
    alloc.deallocate(data, limit - data);
  }
  // reset'inam pointer'iuss - Vector'ius tuščias
  data = limit = avail = nullptr;
}

template <class T>
void Vector<T>::grow() {
  // dvigubai daugiau vietos, nei buvo
  size_type new_size = max(2 * (limit - data), ptrdiff_t(1));
  // išskirti naują vietą ir perkopijuoti egzistuojančius elementus
  iterator new_data = alloc.allocate(new_size);
  iterator new_avail = uninitialized_copy(data, avail, new_data);
  // atlaisvinti seną vietą
  uncreate();
  // reset'int rodykles į naujai išskirtą vietą
  data = new_data;
  avail = new_avail;
  limit = data + new_size;
}
// tariame, kad `avail` point'ina į išskirtą, bet neinicializuotą vietą
template <class T>
void Vector<T>::unchecked_append(const T& val) {
  alloc.construct(avail++, val);
}

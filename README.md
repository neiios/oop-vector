# Vector klasė

## Projekto struktūra

```
vector
├── CMakeLists.txt
├── Makefile
├── README.md
├── src
│   ├── CMakeLists.txt
│   ├── main.cpp
│   └── Vector.hpp
└── test
    ├── CMakeLists.txt
    ├── main.cpp
    └── VectorTests.cpp
```

## 1. Vector::at() function

### Code:

```c++
reference at(size_type pos) {
  // range check
  if (pos >= this->size()) {
    throw std::out_of_range(
        "Range check failed. Pos argument is out of bounds.");
  }
  return (*this)[pos];
}
```
### Unit test:

```c++
TEST(VectorTests, testAtFunction) {
  Vector<string> myVector;
  EXPECT_THROW(myVector.at(0), std::out_of_range);

  myVector.push_back("Pedro");
  myVector.push_back("Gonzales");
  EXPECT_EQ("Pedro", myVector.at(0));
  EXPECT_EQ("Gonzales", myVector.at(1));

  EXPECT_THROW(myVector.at(2), std::out_of_range);
  EXPECT_THROW(myVector.at(-10), std::out_of_range);
}
```

## 2. Vector::size() function

### Code:

```c++
size_type size() const { return avail - data; }
```
### Unit test:

```c++
TEST(VectorTests, testSize) {
  unsigned int sz = 10;
  std::vector<int> stdVector;
  Vector<int> myVector;

  EXPECT_EQ(stdVector.size(), myVector.size());

  // fill with values
  for (int i = 0; i <= sz; ++i) {
    stdVector.push_back(i);
    myVector.push_back(i);
  }

  EXPECT_EQ(stdVector.size(), myVector.size());
}
```

## 3. Vector::push_back() function

### Code:

```c++
void push_back(const T& t) {
  if (avail == limit)
    grow();
  unchecked_append(t);
}
```
### Unit test:

```c++
TEST(VectorTests, testPushBack) {
  unsigned int sz = 10;
  std::vector<int> stdVector;
  Vector<int> myVector;
  for (int i = 0; i <= sz; ++i) {
    stdVector.push_back(i);
    myVector.push_back(i);
    EXPECT_EQ(stdVector[i], myVector[i]);
  }
}
```

## 4. Vector::capacity() function

### Code:

```c++
size_type capacity() const { return size_type(limit - data); }
```
### Unit test:

```c++
TEST(VectorTests, testCapacity) {
  unsigned int sz = 10;
  Vector<int> myVector;

  EXPECT_EQ(0, myVector.capacity());

  // fill with values
  for (int i = 0; i <= sz; ++i) {
    myVector.push_back(i);
  }

  EXPECT_EQ(16, myVector.capacity());
}
```

## 5. Vector::empty() function

### Code:

```c++
bool empty() const { return begin() == end(); }
```
### Unit test:

```c++
TEST(VectorTests, testEmpty) {
  Vector<int> myVector;
  EXPECT_EQ(true, myVector.empty());
}
```

## Spartos analizė:

|                                | 10 000      | 100 000    | 1 000 000 | 10 000 000 | 100 000 000 | Total time |
|--------------------------------|-------------|------------|-----------|------------|-------------|------------|
| Time (Vector)                  | 0.000235475 | 0.00226962 | 0.0214833 | 0.247347   | 2.31198     | 2.58329    |
| Capacity changes (Vector)      | 14          | 17         | 20        | 24         | 27          | -          |
| Time (std::vector)             | 0.000247481 | 0.00236621 | 0.0239906 | 0.247632   | 2.43998     | 2.95956    |
| Capacity changes (std::vector) | 14          | 17         | 20        | 24         | 27          | -          |

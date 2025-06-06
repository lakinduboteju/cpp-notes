# C++ Class Member Access: `public`, `private`, `protected`

## Access Specifiers

C++ uses three main access specifiers to control the visibility of class/struct members:

- **`public`**: Accessible from anywhere.
- **`protected`**: Accessible to the class itself and its derived classes.
- **`private`**: Accessible only to the class itself.

**Syntax:**
```cpp
class MyClass {
public:
    // Accessible from anywhere
    int publicVar;
protected:
    // Accessible only in MyClass and derived classes
    int protectedVar;
private:
    // Accessible only in MyClass
    int privateVar;
};
```

---

## Struct vs. Class: Defaults

- **`struct`**: Members are `public` by default.
- **`class`**: Members are `private` by default.

**Example:**
```cpp
struct MyStruct {
    int x; // public by default
};

class MyClass {
    int y; // private by default
};
```

---

## Access Specifiers and Inheritance

### Inheritance Types

- **`public` inheritance:**  
  Derived class keeps the same access control for base members.
- **`protected` inheritance:**  
  `public` and `protected` base members become `protected` in the derived class.
- **`private` inheritance:**  
  `public` and `protected` base members become `private` in the derived class.

**Example Table:**

| Base Member    | public inheritance | protected inheritance | private inheritance |
|----------------|-------------------|----------------------|--------------------|
| `public`       | public            | protected            | private            |
| `protected`    | protected         | protected            | private            |
| `private`      | not accessible    | not accessible       | not accessible     |


**Example:**
```cpp
class Base {
public:    int a;
protected: int b;
private:   int c;
};

class Pub : public Base     { /* a is public, b protected, c inaccessible */ };
class Prot: protected Base  { /* a, b are protected, c inaccessible */ };
class Priv: private Base    { /* a, b are private, c inaccessible */ };
```

---

## Practical Examples

```cpp
struct Data {
    int x;              // public by default
};

class Item {
    int y;              // private by default
public:
    int getY() const { return y; }
};

class Parent {
protected:
    int val;
};
class Child : public Parent {
    void foo() { val = 5; }    // OK: val is protected in base, accessible here
};
```

---

## Summary

- Use `public` for API, `protected` for internals accessible to subclasses, `private` to encapsulate data.
- `struct` members are public by default, `class` members are private by default.
- Inheritance type changes member access in derived classes (public/protected/private).
- **`private` members are never accessible in derived classes.**

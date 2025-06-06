# C++ Object-Oriented Programming (OOP) Concepts

## 1. Core OOP Principles

- **Encapsulation:** Bundle data and methods operating on that data within a class.
- **Abstraction:** Hide implementation details and show only essential features (using public APIs, abstract base classes).
- **Inheritance:** Derive new classes from existing ones, reusing and extending behavior.
- **Polymorphism:** Code using base types can manipulate derived types.

---

## 2. Polymorphism in C++

### a. Compile-Time Polymorphism

- **Also known as:** Static polymorphism
- **Achieved by:** Function Overloading, Operator Overloading, Templates

**Examples:**

```cpp
// Function Overloading
void print(int);
void print(double);
void print(std::string);

// Operator Overloading
struct Vec2 { int x, y; };
Vec2 operator+(const Vec2&, const Vec2&);

// Templates
template<typename T>
void swap(T& a, T& b) { T tmp = a; a = b; b = tmp; }
```
- Chosen at compile time—no runtime overhead.

---

### b. Run-Time Polymorphism

- **Also known as:** Dynamic polymorphism
- **Achieved by:** Inheritance + Virtual Functions + Pointers/References

**Example:**

```cpp
class Shape {
public:
    virtual void draw() const = 0;
};
class Circle : public Shape {
public:
    void draw() const override { std::cout << "Circle\n"; }
};

void drawShape(const Shape& s) { s.draw(); } // Dynamic dispatch
```
- Virtual function mechanism (vtable) enables dynamic dispatch at runtime.

---

### c. Template Specialization and Polymorphism

- Templates provide **compile-time polymorphism**; specializations allow custom behavior for specific types.

```cpp
template<typename T>
void print(T x) { std::cout << x << "\n"; }

template<>
void print<bool>(bool x) { std::cout << (x ? "true" : "false") << "\n"; }
```
- All resolved at compile time.

---

### d. Universal References and Polymorphism

- **Universal (forwarding) references** are parameters declared as `T&&` in a deduced context (templates).  
- They enable *perfect forwarding* (can forward both lvalues and rvalues).
- Universal references provide **flexibility and generic programming**, but do **not** provide runtime polymorphism.

**Example:**
```cpp
template<typename T>
void wrapper(T&& arg) {
    // Accepts lvalues and rvalues
}

// Usage
int x = 42;
wrapper(x);    // T = int&, arg is int&
wrapper(42);   // T = int,   arg is int&&
```

#### Difference from OOP Polymorphism

- Universal references allow code to work with any type and value category (resolved at compile time), but **do not select function implementations at runtime**.
- Classic OOP (virtual functions, inheritance) **does** provide runtime polymorphism.

**Comparison Example:**
```cpp
struct Base { virtual void foo() const { std::cout << "Base\n"; } };
struct Derived : Base { void foo() const override { std::cout << "Derived\n"; } };

template<typename T>
void call_foo(T&& t) { t.foo(); }     // compile-time flexibility

void runtime_poly(const Base& base) { base.foo(); } // runtime, via vtable

Derived d;
call_foo(d);          // Resolved at compile time
runtime_poly(d);      // Dynamic dispatch at runtime
```

---

## 3. Other Important C++ OOP Features

### Abstract Classes and Interfaces

- **Abstract class:** Contains at least one pure virtual function
- **Interface:** All functions pure virtual, no data.

```cpp
class Interface {
public:
    virtual void foo() = 0;
    virtual ~Interface() = default;
};
```

### Multiple Inheritance

- Supported (danger of “diamond problem” solved via virtual inheritance).

### Access Control

- `public`, `protected`, `private` members and inheritance modes.

### Friend Classes/Functions

- Allow external access to private/protected data.

---

## 4. Summary Table

| Polymorphism Type         | Mechanism                        | Example                     | When Resolved    |
|---------------------------|----------------------------------|-----------------------------|------------------|
| Compile-time (static)     | Overloading, templates           | `swap<int>(a, b)`           | Compile time     |
| Template specialization   | Specialized templates            | `print<bool>(true)`         | Compile time     |
| Runtime (dynamic)         | Virtual functions (vtable)       | `virtual void foo()`        | Run time         |
| Universal refs/templates  | Generic programming (T&&, etc.)  | `call_foo(t)`               | Compile time     |

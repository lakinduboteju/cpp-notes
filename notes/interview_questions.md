# C++ OOP Technical Interview Questions

## 1. Core OOP Concepts

### Question 1: Explain the four pillars of OOP with C++ examples

**Answer:**
The four pillars are:

1. **Encapsulation**: Bundling data and methods together, controlling access
```cpp
class BankAccount {
private:
    double balance_;  // Hidden data
public:
    void deposit(double amount) { balance_ += amount; }  // Controlled access
    double getBalance() const { return balance_; }
};
```

2. **Inheritance**: Creating new classes based on existing ones
```cpp
class Vehicle { protected: int speed_; };
class Car : public Vehicle { /* inherits speed_ */ };
```

3. **Polymorphism**: Using base class interface to work with derived objects
```cpp
virtual void draw() = 0;  // Base interface
Circle c; Rectangle r;
Shape* shapes[] = {&c, &r};
for(auto* s : shapes) s->draw();  // Polymorphic calls
```

4. **Abstraction**: Hiding implementation details, showing only essential features
```cpp
class FileProcessor {
public:
    void processFile(const string& filename);  // Public interface
private:
    void parseHeader();  // Hidden implementation
    void validateData();
};
```

---

### Question 2: What's the difference between compile-time and runtime polymorphism?

**Answer:**

**Compile-time Polymorphism (Static)**:
- Resolved during compilation
- Zero runtime overhead
- Achieved through: function overloading, operator overloading, templates

```cpp
template<typename T>
void process(T data) { /* resolved at compile time */ }

void print(int x);     // Overloaded functions
void print(double x);  // Compiler chooses based on argument type
```

**Runtime Polymorphism (Dynamic)**:
- Resolved during execution
- Uses virtual function table (vtable)
- Enables true OOP polymorphism

```cpp
class Base { virtual void foo() = 0; };
class Derived : public Base { void foo() override {} };

Base* ptr = new Derived();
ptr->foo();  // Resolved at runtime via vtable lookup
```

**When to use which:**
- Compile-time: Performance critical, type known at compile time
- Runtime: Need true polymorphic behavior, type determined at runtime

---

## 2. Virtual Functions and Inheritance

### Question 3: Why do we need virtual destructors? What happens without them?

**Answer:**

**Problem without virtual destructors:**
```cpp
class Base {
public:
    ~Base() { cout << "Base destructor\n"; }  // Non-virtual!
};

class Derived : public Base {
    int* data_;
public:
    Derived() : data_(new int[1000]) {}
    ~Derived() { delete[] data_; cout << "Derived destructor\n"; }
};

Base* ptr = new Derived();
delete ptr;  // UNDEFINED BEHAVIOR! Only ~Base() called, memory leak!
```

**Solution with virtual destructor:**
```cpp
class Base {
public:
    virtual ~Base() { cout << "Base destructor\n"; }  // Virtual!
};
// Now: delete ptr correctly calls ~Derived() then ~Base()
```

**Rule**: Always make destructor virtual in classes intended for inheritance.

---

### Question 4: Explain the diamond problem and how virtual inheritance solves it

**Answer:**

**Diamond Problem:**
```cpp
class Animal { public: void eat(); };
class Mammal : public Animal { };
class Bird : public Animal { };
class Platypus : public Mammal, public Bird { };  // TWO Animal copies!

Platypus p;
p.eat();  // ERROR: Ambiguous! Which Animal::eat()?
```

**Solution with Virtual Inheritance:**
```cpp
class Mammal : public virtual Animal { };  // virtual inheritance
class Bird : public virtual Animal { };
class Platypus : public Mammal, public Bird { };  // ONE Animal copy

Platypus p;
p.eat();  // OK: Only one Animal::eat()
```

**How it works**: Virtual inheritance ensures only one copy of the base class exists in the inheritance hierarchy.

---

### Question 5: What's the difference between function overriding, overloading, and hiding?

**Answer:**

**Function Overriding** (Virtual functions):
```cpp
class Base { virtual void func(int x) {} };
class Derived : public Base { 
    void func(int x) override {}  // Overrides Base::func
};
```

**Function Overloading** (Same class, different signatures):
```cpp
class Calculator {
    int add(int a, int b);
    double add(double a, double b);     // Overloaded
    int add(int a, int b, int c);       // Overloaded
};
```

**Function Hiding** (Non-virtual inheritance):
```cpp
class Base {
    void func(int x) {}
    void func(double x) {}
};
class Derived : public Base {
    void func(int x) {}  // HIDES all Base::func versions!
};

Derived d;
d.func(42);    // OK: calls Derived::func(int)
d.func(3.14);  // ERROR: Base::func(double) is hidden

// Solution: Use 'using' declaration
class Better : public Base {
    using Base::func;  // Bring all Base::func into scope
    void func(int x) {}  // Only hides the int version
};
```

---

## 3. Memory Management and RAII

### Question 6: Explain RAII and provide a real-world example

**Answer:**

**RAII (Resource Acquisition Is Initialization)**:
- Acquire resources in constructor
- Release resources in destructor
- Resource lifetime tied to object lifetime

**Example - File Management:**
```cpp
class FileManager {
    FILE* file_;
public:
    explicit FileManager(const char* filename) {
        file_ = fopen(filename, "r");
        if (!file_) throw std::runtime_error("Cannot open file");
    }
    
    ~FileManager() {
        if (file_) fclose(file_);  // Automatic cleanup
    }
    
    // Prevent copying to avoid double-close
    FileManager(const FileManager&) = delete;
    FileManager& operator=(const FileManager&) = delete;
    
    FILE* get() const { return file_; }
};

void processFile() {
    FileManager fm("data.txt");  // File automatically opened
    // ... use fm.get() ...
    // File automatically closed when fm goes out of scope
    // Even if exceptions are thrown!
}
```

**Benefits:**
- Exception safety
- Automatic cleanup
- No memory leaks
- Clear ownership semantics

---

### Question 7: Compare unique_ptr, shared_ptr, and weak_ptr

**Answer:**

| Smart Pointer | Ownership | Copy | Use Case | Overhead |
|---------------|-----------|------|----------|----------|
| `unique_ptr` | Exclusive | Move only | Single owner | Zero |
| `shared_ptr` | Shared | Yes | Multiple owners | Reference counting |
| `weak_ptr` | None | Yes | Break cycles | Minimal |

**unique_ptr Example:**
```cpp
auto ptr = std::make_unique<Widget>();  // Exclusive ownership
auto ptr2 = std::move(ptr);  // Transfer ownership, ptr becomes null
// Automatic cleanup when ptr2 goes out of scope
```

**shared_ptr Example:**
```cpp
auto ptr1 = std::make_shared<Widget>();
auto ptr2 = ptr1;  // Both own the object, reference count = 2
// Object destroyed when last shared_ptr is destroyed
```

**weak_ptr Example (Breaking Cycles):**
```cpp
class Node {
public:
    std::shared_ptr<Node> next;
    std::weak_ptr<Node> parent;  // Weak to break cycle
};

auto node1 = std::make_shared<Node>();
auto node2 = std::make_shared<Node>();
node1->next = node2;
node2->parent = node1;  // No cycle created!

// Check if parent still exists
if (auto parent = node2->parent.lock()) {
    // Use parent safely
}
```

**When to use:**
- `unique_ptr`: Default choice, exclusive ownership
- `shared_ptr`: Multiple owners needed
- `weak_ptr`: Observer pattern, breaking cycles

---

## 4. Class Design and Special Members

### Question 8: Explain the Rule of Five and when to use it

**Answer:**

**Rule of Five**: If you define any of these five special member functions, you should probably define all five:
1. Destructor
2. Copy constructor
3. Copy assignment operator
4. Move constructor
5. Move assignment operator

**When needed**: Classes that manage resources (memory, file handles, network connections, etc.)

**Example:**
```cpp
class ResourceManager {
    Resource* resource_;
    
public:
    // Constructor
    explicit ResourceManager(const std::string& name) 
        : resource_(new Resource(name)) {}
    
    // 1. Destructor
    ~ResourceManager() { delete resource_; }
    
    // 2. Copy constructor
    ResourceManager(const ResourceManager& other) 
        : resource_(new Resource(*other.resource_)) {}
    
    // 3. Copy assignment
    ResourceManager& operator=(const ResourceManager& other) {
        if (this != &other) {
            delete resource_;
            resource_ = new Resource(*other.resource_);
        }
        return *this;
    }
    
    // 4. Move constructor
    ResourceManager(ResourceManager&& other) noexcept 
        : resource_(other.resource_) {
        other.resource_ = nullptr;
    }
    
    // 5. Move assignment
    ResourceManager& operator=(ResourceManager&& other) noexcept {
        if (this != &other) {
            delete resource_;
            resource_ = other.resource_;
            other.resource_ = nullptr;
        }
        return *this;
    }
};
```

**Alternative - Rule of Zero**: For classes that don't manage resources, let the compiler generate all special members:
```cpp
class SimpleClass {
    std::string name_;
    std::vector<int> data_;
    std::unique_ptr<Widget> widget_;
    // Compiler-generated special members are perfect!
};
```

---

### Question 9: Why use noexcept with move operations?

**Answer:**

**Key Reason**: STL containers only use move operations if they're marked `noexcept`. Otherwise, they fall back to copying for exception safety.

**Example:**
```cpp
class MyClass {
    std::string data_;
public:
    // Without noexcept
    MyClass(MyClass&& other) : data_(std::move(other.data_)) {}
    
    // With noexcept - enables std::vector optimizations
    MyClass(MyClass&& other) noexcept : data_(std::move(other.data_)) {}
};

std::vector<MyClass> vec;
vec.push_back(MyClass{});  // Only uses move if move constructor is noexcept
```

**Rule**: Mark move constructor and move assignment as `noexcept` if they truly cannot throw.

**STL Example:**
```cpp
// std::vector::push_back implementation (simplified)
void push_back(T&& value) {
    if (size_ == capacity_) {
        // If T's move is noexcept, use move during reallocation
        if constexpr (std::is_nothrow_move_constructible_v<T>) {
            reallocate_and_move();  // Efficient
        } else {
            reallocate_and_copy();  // Safe but slower
        }
    }
    // ...
}
```

---

## 5. Templates and Generic Programming

### Question 10: What is SFINAE and how is it used?

**Answer:**

**SFINAE**: "Substitution Failure Is Not An Error"
- Template substitution failures are silently ignored
- Enables conditional template instantiation

**Example - Enable function only for integral types:**
```cpp
#include <type_traits>

// Method 1: std::enable_if
template<typename T>
typename std::enable_if<std::is_integral<T>::value, T>::type
process(T value) {
    return value * 2;  // Only available for integral types
}

// Method 2: C++14 simplified
template<typename T>
std::enable_if_t<std::is_integral_v<T>, T>
process(T value) {
    return value * 2;
}

// Method 3: C++20 concepts (cleaner)
template<std::integral T>
T process(T value) {
    return value * 2;
}

// Usage
auto result = process(42);     // OK: int is integral
// auto bad = process(3.14);      // Compilation error: double is not integral
```

**Advanced SFINAE - Function Overloading:**
```cpp
// Overload for types with begin() method (containers)
template<typename T>
auto print(const T& container) 
    -> decltype(container.begin(), void()) {
    for (const auto& item : container) {
        std::cout << item << " ";
    }
}

// Overload for other types
template<typename T>
auto print(const T& value) 
    -> decltype(std::cout << value, void()) {
    std::cout << value;
}

// Usage
std::vector<int> vec{1, 2, 3};
print(vec);  // Calls container version
print(42);   // Calls value version
```

---

### Question 11: Explain the difference between template specialization and overloading

**Answer:**

**Template Specialization**: Providing a different implementation for specific template arguments

```cpp
// Primary template
template<typename T>
class Container {
public:
    void process() { std::cout << "Generic processing\n"; }
};

// Full specialization for bool
template<>
class Container<bool> {
public:
    void process() { std::cout << "Optimized bool processing\n"; }
};

// Partial specialization for pointers
template<typename T>
class Container<T*> {
public:
    void process() { std::cout << "Pointer processing\n"; }
};

Container<int> c1;      // Uses primary template
Container<bool> c2;     // Uses bool specialization
Container<int*> c3;     // Uses pointer specialization
```

**Function Template Overloading**: Providing different template functions

```cpp
// Overload 1: Single parameter
template<typename T>
void process(T value) {
    std::cout << "Single value: " << value << "\n";
}

// Overload 2: Two parameters
template<typename T, typename U>
void process(T first, U second) {
    std::cout << "Two values: " << first << ", " << second << "\n";
}

// Overload 3: Container
template<typename Container>
void process(const Container& c) {
    std::cout << "Container with " << c.size() << " elements\n";
}

// Usage - compiler chooses best match
process(42);              // Calls overload 1
process(1, 2.5);          // Calls overload 2
process(std::vector<int>{1,2,3});  // Calls overload 3
```

**Key Differences:**
- **Specialization**: Same template, different implementation for specific types
- **Overloading**: Different templates, compiler chooses based on arguments

---

## 6. Advanced Concepts

### Question 12: What is CRTP (Curiously Recurring Template Pattern)?

**Answer:**

**CRTP**: A class derives from a template instantiation of itself
- Provides static polymorphism
- No virtual function overhead
- Compile-time interface enforcement

**Example:**
```cpp
template<typename Derived>
class Base {
public:
    void interface() {
        static_cast<Derived*>(this)->implementation();
    }
    
    void commonFunction() {
        std::cout << "Common logic\n";
        interface();  // Calls derived implementation
    }
};

class ConcreteA : public Base<ConcreteA> {
public:
    void implementation() {
        std::cout << "ConcreteA implementation\n";
    }
};

class ConcreteB : public Base<ConcreteB> {
public:
    void implementation() {
        std::cout << "ConcreteB implementation\n";
    }
};

// Usage
ConcreteA a;
ConcreteB b;
a.commonFunction();  // Calls ConcreteA::implementation
b.commonFunction();  // Calls ConcreteB::implementation
```

**Benefits:**
- No virtual function overhead
- Type safety at compile time
- Interface enforcement
- Better optimization opportunities

**Real-world usage**: `std::enable_shared_from_this`, iterator implementations

---

### Question 13: Explain move semantics and perfect forwarding

**Answer:**

**Move Semantics**: Transfer resources instead of copying them

```cpp
class MyString {
    char* data_;
    size_t size_;
    
public:
    // Move constructor
    MyString(MyString&& other) noexcept 
        : data_(other.data_), size_(other.size_) {
        other.data_ = nullptr;  // Leave other in valid state
        other.size_ = 0;
    }
    
    // Move assignment
    MyString& operator=(MyString&& other) noexcept {
        if (this != &other) {
            delete[] data_;     // Clean up current resource
            data_ = other.data_;
            size_ = other.size_;
            other.data_ = nullptr;  // Steal other's resource
            other.size_ = 0;
        }
        return *this;
    }
};

MyString s1("Hello");
MyString s2 = std::move(s1);  // Move, not copy - much faster!
```

**Perfect Forwarding**: Forward arguments exactly as they were passed

```cpp
template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

// Usage
auto ptr1 = make_unique<MyString>("Hello");        // Forward rvalue
MyString temp("World");
auto ptr2 = make_unique<MyString>(temp);           // Forward lvalue
auto ptr3 = make_unique<MyString>(std::move(temp)); // Forward rvalue
```

**Universal References**: `T&&` in template context can bind to both lvalues and rvalues
```cpp
template<typename T>
void wrapper(T&& arg) {  // Universal reference
    process(std::forward<T>(arg));  // Perfect forwarding
}

int x = 42;
wrapper(x);           // T = int&, arg is lvalue reference
wrapper(42);          // T = int, arg is rvalue reference
wrapper(std::move(x)); // T = int, arg is rvalue reference
```

---

## 7. Common Pitfalls and Debugging

### Question 14: What are some common C++ OOP pitfalls and how to avoid them?

**Answer:**

**1. Forgetting Virtual Destructor**
```cpp
// Problem
class Base { ~Base() {} };  // Non-virtual destructor
Base* ptr = new Derived();
delete ptr;  // Undefined behavior!

// Solution
class Base { virtual ~Base() {} };  // Virtual destructor
```

**2. Slicing Problem**
```cpp
// Problem
class Base { virtual void func() {} };
class Derived : public Base { int extra_data; };

Derived d;
Base b = d;  // SLICING! extra_data is lost
b.func();    // Calls Base::func, not Derived::func

// Solution
Base& ref = d;   // Use reference
Base* ptr = &d;  // Or pointer
```

**3. Multiple Inheritance Ambiguity**
```cpp
// Problem
class A { public: void func() {} };
class B { public: void func() {} };
class C : public A, public B {};

C c;
c.func();  // ERROR: Ambiguous

// Solutions
c.A::func();  // Explicit qualification
// Or use virtual inheritance
// Or redesign hierarchy
```

**4. Resource Leaks in Constructors**
```cpp
// Problem
class Widget {
    Resource* r1_;
    Resource* r2_;
public:
    Widget() : r1_(new Resource()), r2_(new Resource()) {
        if (someCondition) throw std::exception();  // Leak r1_ and r2_!
    }
};

// Solutions
class Widget {
    std::unique_ptr<Resource> r1_;
    std::unique_ptr<Resource> r2_;
public:
    Widget() : r1_(std::make_unique<Resource>()), 
               r2_(std::make_unique<Resource>()) {
        // Exception safe - automatic cleanup
    }
};
```

**5. Circular Dependencies with shared_ptr**
```cpp
// Problem
class Node {
    std::shared_ptr<Node> next;
    std::shared_ptr<Node> parent;  // Creates cycle!
};

// Solution
class Node {
    std::shared_ptr<Node> next;
    std::weak_ptr<Node> parent;  // Break cycle with weak_ptr
};
```

---

### Question 15: How would you debug a memory leak in a C++ application?

**Answer:**

**Tools and Techniques:**

**1. Static Analysis:**
```bash
# Compile with warnings
g++ -Wall -Wextra -Wpedantic -fsanitize=address -g program.cpp

# Use static analyzers
clang-static-analyzer program.cpp
cppcheck program.cpp
```

**2. Runtime Tools:**
```bash
# Valgrind (Linux/Mac)
valgrind --leak-check=full --track-origins=yes ./program

# AddressSanitizer
g++ -fsanitize=address -g program.cpp
./a.out

# Microsoft Visual Studio (Windows)
# Enable CRT debug heap
```

**3. Code Review Checklist:**
```cpp
// Check for matching new/delete
Resource* r = new Resource();
// ... is there a corresponding delete r; ?

// Check for exception safety
try {
    Resource* r = new Resource();
    riskyOperation();  // If this throws, r is leaked!
    delete r;
} catch (...) {
    // Resource leaked!
}

// Better approach - RAII
auto r = std::make_unique<Resource>();
riskyOperation();  // Safe - automatic cleanup
```

**4. Common Leak Patterns:**
```cpp
// Pattern 1: Forgot to delete
void leak1() {
    int* ptr = new int(42);
    // Missing: delete ptr;
}

// Pattern 2: Exception before delete
void leak2() {
    int* ptr = new int(42);
    throw std::exception();  // Leak!
    delete ptr;  // Never reached
}

// Pattern 3: Early return
void leak3() {
    int* ptr = new int(42);
    if (condition) return;  // Leak!
    delete ptr;
}

// Pattern 4: Circular references
std::shared_ptr<Node> createCycle() {
    auto n1 = std::make_shared<Node>();
    auto n2 = std::make_shared<Node>();
    n1->next = n2;
    n2->next = n1;  // Cycle - never destroyed!
    return n1;
}
```

**Prevention Strategies:**
- Use smart pointers (`unique_ptr`, `shared_ptr`)
- Follow RAII principles
- Use containers instead of raw arrays
- Enable compiler warnings and static analysis
- Write unit tests for resource management

---

## 8. Design Patterns in C++

### Question 16: Implement the Singleton pattern and discuss its pros/cons

**Answer:**

**Thread-Safe Singleton (C++11):**
```cpp
class Singleton {
private:
    Singleton() = default;
    
public:
    // Delete copy constructor and assignment
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    
    static Singleton& getInstance() {
        static Singleton instance;  // Thread-safe in C++11
        return instance;
    }
    
    void doSomething() {
        std::cout << "Singleton doing work\n";
    }
};

// Usage
Singleton::getInstance().doSomething();
```

**Pros:**
- Guaranteed single instance
- Global access point
- Lazy initialization
- Thread-safe (C++11 static local)

**Cons:**
- Hidden dependencies (hard to test)
- Global state (breaks encapsulation)
- Difficult to unit test
- Can create initialization order problems

**Alternative - Dependency Injection:**
```cpp
class Service {
public:
    virtual void process() = 0;
    virtual ~Service() = default;
};

class ConcreteService : public Service {
public:
    void process() override { /* implementation */ }
};

class Client {
    Service& service_;
public:
    explicit Client(Service& service) : service_(service) {}
    void doWork() { service_.process(); }
};

// Usage - better testability
ConcreteService service;
Client client(service);  // Dependency injected
```

---

### Question 17: Implement the Factory pattern for creating different types of objects

**Answer:**

**Abstract Factory Pattern:**
```cpp
// Product interface
class Shape {
public:
    virtual void draw() const = 0;
    virtual double area() const = 0;
    virtual ~Shape() = default;
};

// Concrete products
class Circle : public Shape {
    double radius_;
public:
    explicit Circle(double r) : radius_(r) {}
    void draw() const override { std::cout << "Drawing Circle\n"; }
    double area() const override { return 3.14159 * radius_ * radius_; }
};

class Rectangle : public Shape {
    double width_, height_;
public:
    Rectangle(double w, double h) : width_(w), height_(h) {}
    void draw() const override { std::cout << "Drawing Rectangle\n"; }
    double area() const override { return width_ * height_; }
};

// Factory interface
class ShapeFactory {
public:
    virtual std::unique_ptr<Shape> createShape() const = 0;
    virtual ~ShapeFactory() = default;
};

// Concrete factories
class CircleFactory : public ShapeFactory {
    double radius_;
public:
    explicit CircleFactory(double r) : radius_(r) {}
    std::unique_ptr<Shape> createShape() const override {
        return std::make_unique<Circle>(radius_);
    }
};

class RectangleFactory : public ShapeFactory {
    double width_, height_;
public:
    RectangleFactory(double w, double h) : width_(w), height_(h) {}
    std::unique_ptr<Shape> createShape() const override {
        return std::make_unique<Rectangle>(width_, height_);
    }
};

// Usage
void drawShapes(const std::vector<std::unique_ptr<ShapeFactory>>& factories) {
    for (const auto& factory : factories) {
        auto shape = factory->createShape();
        shape->draw();
        std::cout << "Area: " << shape->area() << "\n";
    }
}

// Client code
std::vector<std::unique_ptr<ShapeFactory>> factories;
factories.push_back(std::make_unique<CircleFactory>(5.0));
factories.push_back(std::make_unique<RectangleFactory>(4.0, 6.0));
drawShapes(factories);
```

**Benefits:**
- Decouples object creation from usage
- Easy to extend with new types
- Follows Open/Closed Principle
- Testable (can inject mock factories)

---

## 9. Performance and Optimization

### Question 18: How do virtual functions affect performance and when should you avoid them?

**Answer:**

**Performance Impact:**

1. **Memory Overhead:**
   - vtable pointer per object (typically 8 bytes on 64-bit)
   - vtable per class (one per class, not per object)

2. **Runtime Overhead:**
   - Indirect function call through vtable
   - Cannot be inlined by compiler
   - Potential cache misses

**Benchmark Example:**
```cpp
class NonVirtual {
public:
    void process() { /* simple operation */ }
};

class Virtual {
public:
    virtual void process() { /* same operation */ }
};

// Typical performance difference: 10-20% for simple functions
void benchmark() {
    NonVirtual nv;
    Virtual v;
    
    // Direct call - can be inlined
    for (int i = 0; i < 1000000; ++i) {
        nv.process();  // Fast
    }
    
    // Virtual call - cannot be inlined
    for (int i = 0; i < 1000000; ++i) {
        v.process();   // Slower
    }
}
```

**When to Avoid Virtual Functions:**
- Performance-critical inner loops
- Value types (mathematical vectors, points)
- Classes that will never be inherited
- Real-time systems with strict timing

**When Virtual Functions Are Worth It:**
- Clear polymorphic design benefit
- Extensible framework design
- Plugin architectures
- When design clarity outweighs performance cost

**Alternatives:**
```cpp
// 1. Template-based polymorphism (CRTP)
template<typename Derived>
class Base {
    void process() { static_cast<Derived*>(this)->processImpl(); }
};

// 2. std::variant for limited set of types
using Shape = std::variant<Circle, Rectangle, Triangle>;
void draw(const Shape& shape) {
    std::visit([](const auto& s) { s.draw(); }, shape);
}

// 3. Function pointers/std::function
class Processor {
    std::function<void()> process_func_;
public:
    template<typename Func>
    Processor(Func f) : process_func_(f) {}
    void process() { process_func_(); }
};
```

---

### Question 19: What are the performance implications of different smart pointer types?

**Answer:**

**Performance Comparison:**

| Pointer Type | Size | Copy Cost | Dereference Cost | Thread Safety |
|--------------|------|-----------|------------------|---------------|
| Raw pointer | 8 bytes | O(1) | O(1) | No |
| unique_ptr | 8 bytes | Move only | O(1) | No |
| shared_ptr | 16 bytes | O(1) atomic | O(1) | Yes |
| weak_ptr | 16 bytes | O(1) | O(1) + validity check | Yes |

**Detailed Analysis:**

**unique_ptr:**
```cpp
// Zero overhead abstraction
std::unique_ptr<Widget> ptr = std::make_unique<Widget>();
ptr->method();  // Same as raw pointer dereference

// Optimizes to same assembly as raw pointer
Widget* raw = new Widget();
raw->method();
delete raw;
```

**shared_ptr:**
```cpp
// Higher overhead due to control block
std::shared_ptr<Widget> ptr1 = std::make_shared<Widget>();
std::shared_ptr<Widget> ptr2 = ptr1;  // Atomic increment

// Control block contains:
struct ControlBlock {
    std::atomic<size_t> ref_count;    // Strong references
    std::atomic<size_t> weak_count;   // Weak references
    Widget* object_ptr;               // Actual object
    // Custom deleter storage (if any)
};
```

**Performance Best Practices:**

1. **Prefer unique_ptr by default:**
```cpp
// Good: Zero overhead
std::unique_ptr<Resource> createResource() {
    return std::make_unique<Resource>();
}

// Avoid: Unnecessary shared_ptr overhead
std::shared_ptr<Resource> createResourceBad() {
    return std::make_shared<Resource>();  // Overhead when not needed
}
```

2. **Use make_shared for shared_ptr:**
```cpp
// Good: Single allocation for object + control block
auto ptr = std::make_shared<Widget>(args);

// Bad: Two allocations (object + control block separately)
auto ptr = std::shared_ptr<Widget>(new Widget(args));
```

3. **Avoid shared_ptr in performance-critical code:**
```cpp
// If you need shared ownership in hot path
class PerformanceCritical {
    Widget* widget_;  // Raw pointer for performance
    std::shared_ptr<Widget> owner_;  // Keep ownership elsewhere
public:
    PerformanceCritical(std::shared_ptr<Widget> w) 
        : widget_(w.get()), owner_(std::move(w)) {}
    
    void hotFunction() {
        widget_->fastMethod();  // No atomic operations
    }
};
```

---

## 10. Modern C++ Features

### Question 20: How do C++11/14/17/20 features improve OOP design?

**Answer:**

**C++11 Improvements:**

1. **Move Semantics:**
```cpp
class Resource {
    std::vector<int> data_;
public:
    // Move constructor - avoid expensive copies
    Resource(Resource&& other) noexcept : data_(std::move(other.data_)) {}
    
    Resource& operator=(Resource&& other) noexcept {
        data_ = std::move(other.data_);
        return *this;
    }
};
```

2. **Smart Pointers:**
```cpp
class Widget {
    std::unique_ptr<Resource> resource_;  // Automatic memory management
    std::shared_ptr<Cache> cache_;        // Shared ownership
public:
    Widget() : resource_(std::make_unique<Resource>()) {}
    // No need for custom destructor!
};
```

3. **override and final:**
```cpp
class Base {
    virtual void func() {}
    virtual void finalFunc() final {}  // Cannot be overridden
};

class Derived final : public Base {  // Cannot be inherited
    void func() override {}  // Explicit override - compiler checks
};
```

**C++14 Improvements:**

1. **Generic Lambdas:**
```cpp
auto processor = [](auto& container) {
    for (auto& item : container) {
        item.process();
    }
};
// Works with any container type
```

2. **make_unique:**
```cpp
auto widget = std::make_unique<Widget>(args);  // Exception safe
```

**C++17 Improvements:**

1. **Class Template Argument Deduction:**
```cpp
std::vector vec{1, 2, 3, 4};  // Deduced as vector<int>
std::pair p{42, "hello"};     // Deduced as pair<int, const char*>
```

2. **if constexpr for Template Metaprogramming:**
```cpp
template<typename T>
void process(T value) {
    if constexpr (std::is_arithmetic_v<T>) {
        // Arithmetic-specific code
    } else {
        // General code
    }
}
```

**C++20 Improvements:**

1. **Concepts for Better Templates:**
```cpp
template<std::integral T>  // Clear constraint
T multiply(T a, T b) {
    return a * b;
}

// Custom concept
template<typename T>
concept Drawable = requires(T t) {
    t.draw();
};

template<Drawable T>
void renderObject(const T& obj) {
    obj.draw();
}
```

2. **Coroutines for Async Design:**
```cpp
class AsyncProcessor {
public:
    std::future<Result> processAsync(const Data& data) {
        co_return co_await computeResult(data);
    }
};
```

3. **Modules (better than headers):**
```cpp
// math_module.cppm
export module math;

export class Calculator {
public:
    int add(int a, int b) { return a + b; }
};

// main.cpp
import math;
Calculator calc;  // Clean, fast compilation
```

**Key Benefits:**
- **Safer code**: Smart pointers, move semantics, concepts
- **Cleaner syntax**: Auto, range-based for, CTAD
- **Better performance**: Move semantics, constexpr
- **Easier generic programming**: Concepts, if constexpr
- **Modern async patterns**: Coroutines, std::async

These features make C++ OOP code more expressive, safer, and performant while maintaining backwards compatibility.

---

## Summary

This collection covers the most common C++ OOP interview questions across different experience levels. Key areas to focus on:

1. **Fundamentals**: OOP principles, virtual functions, inheritance
2. **Memory Management**: RAII, smart pointers, move semantics  
3. **Templates**: Generic programming, SFINAE, concepts
4. **Design Patterns**: Singleton, Factory, CRTP
5. **Performance**: Virtual function overhead, optimization techniques
6. **Modern C++**: Features that improve OOP design and safety

Remember to:
- Provide concrete code examples
- Explain trade-offs and alternatives
- Discuss real-world applications
- Demonstrate understanding of performance implications
- Show knowledge of modern C++ best practices

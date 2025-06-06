# C++ Virtual Functions and Inheritance Deep Dive

## 1. Virtual Functions Mechanism

### How Virtual Functions Work

Virtual functions enable **runtime polymorphism** through a mechanism called the **virtual table (vtable)**.

```cpp
class Base {
public:
    virtual void foo() { std::cout << "Base::foo\n"; }
    virtual void bar() { std::cout << "Base::bar\n"; }
    void nonVirtual() { std::cout << "Base::nonVirtual\n"; }
};

class Derived : public Base {
public:
    void foo() override { std::cout << "Derived::foo\n"; }
    void bar() override { std::cout << "Derived::bar\n"; }
    void nonVirtual() { std::cout << "Derived::nonVirtual\n"; }
};
```

### Memory Layout with vtable

```
Base object:
+------------------+
| vtable pointer   |  -> Points to Base's vtable
+------------------+
| member data...   |
+------------------+

Derived object:
+------------------+
| vtable pointer   |  -> Points to Derived's vtable
+------------------+
| Base member data |
+------------------+
| Derived data...  |
+------------------+

Base vtable:           Derived vtable:
+--------------+       +--------------+
| &Base::foo   |       | &Derived::foo|
| &Base::bar   |       | &Derived::bar|
+--------------+       +--------------+
```

### Dynamic Dispatch Example

```cpp
void demonstrateVtable() {
    Base* basePtr = new Derived();
    
    basePtr->foo();         // Calls Derived::foo (virtual dispatch)
    basePtr->bar();         // Calls Derived::bar (virtual dispatch)
    basePtr->nonVirtual();  // Calls Base::nonVirtual (static dispatch)
    
    delete basePtr;
}
```

---

## 2. Pure Virtual Functions and Abstract Classes

### Abstract Base Classes

```cpp
class Shape {  // Abstract class
public:
    // Pure virtual function - must be implemented by derived classes
    virtual double area() const = 0;
    virtual double perimeter() const = 0;
    
    // Virtual destructor for proper cleanup
    virtual ~Shape() = default;
    
    // Non-pure virtual function (can be overridden)
    virtual void display() const {
        std::cout << "Area: " << area() << ", Perimeter: " << perimeter() << "\n";
    }
    
    // Regular member function
    void printType() const {
        std::cout << "This is a shape\n";
    }
};

class Circle : public Shape {
    double radius_;
public:
    explicit Circle(double r) : radius_(r) {}
    
    double area() const override {
        return 3.14159 * radius_ * radius_;
    }
    
    double perimeter() const override {
        return 2 * 3.14159 * radius_;
    }
};

class Rectangle : public Shape {
    double width_, height_;
public:
    Rectangle(double w, double h) : width_(w), height_(h) {}
    
    double area() const override {
        return width_ * height_;
    }
    
    double perimeter() const override {
        return 2 * (width_ + height_);
    }
    
    // Override the virtual function with custom behavior
    void display() const override {
        std::cout << "Rectangle - ";
        Shape::display();  // Call base implementation
    }
};
```

### Interface Design Pattern

```cpp
// Pure interface - no data, all functions pure virtual
class Drawable {
public:
    virtual ~Drawable() = default;
    virtual void draw() const = 0;
    virtual void erase() const = 0;
};

class Serializable {
public:
    virtual ~Serializable() = default;
    virtual std::string serialize() const = 0;
    virtual void deserialize(const std::string& data) = 0;
};

// Multiple inheritance from interfaces
class DrawableShape : public Shape, public Drawable {
public:
    void draw() const override {
        std::cout << "Drawing shape with area " << area() << "\n";
    }
    
    void erase() const override {
        std::cout << "Erasing shape\n";
    }
};

class SerializableCircle : public Circle, public Serializable {
public:
    using Circle::Circle;  // Inherit constructors
    
    std::string serialize() const override {
        return "Circle:" + std::to_string(radius_);
    }
    
    void deserialize(const std::string& data) override {
        // Parse data and set radius_
    }
    
private:
    double radius_;
};
```

---

## 3. Virtual Destructors

### Why Virtual Destructors Are Critical

```cpp
class Base {
public:
    Base() { std::cout << "Base constructor\n"; }
    // Non-virtual destructor - DANGEROUS!
    ~Base() { std::cout << "Base destructor\n"; }
};

class Derived : public Base {
    int* data_;
public:
    Derived() : data_(new int[1000]) {
        std::cout << "Derived constructor\n";
    }
    
    ~Derived() {
        delete[] data_;
        std::cout << "Derived destructor\n";
    }
};

void demonstrateDestructorProblem() {
    Base* ptr = new Derived();  // Creates Derived object
    delete ptr;  // UNDEFINED BEHAVIOR! Only calls ~Base()
                 // ~Derived() is never called -> memory leak!
}
```

### Correct Implementation

```cpp
class Base {
public:
    Base() { std::cout << "Base constructor\n"; }
    virtual ~Base() { std::cout << "Base destructor\n"; }  // Virtual destructor
};

class Derived : public Base {
    int* data_;
public:
    Derived() : data_(new int[1000]) {
        std::cout << "Derived constructor\n";
    }
    
    ~Derived() override {  // Can use override for destructors too
        delete[] data_;
        std::cout << "Derived destructor\n";
    }
};

void demonstrateCorrectDestruction() {
    Base* ptr = new Derived();
    delete ptr;  // Correctly calls ~Derived() then ~Base()
}
```

---

## 4. Multiple Inheritance

### Basic Multiple Inheritance

```cpp
class Flyable {
public:
    virtual void fly() { std::cout << "Flying\n"; }
    virtual ~Flyable() = default;
};

class Swimmable {
public:
    virtual void swim() { std::cout << "Swimming\n"; }
    virtual ~Swimmable() = default;
};

class Duck : public Flyable, public Swimmable {
public:
    void fly() override { std::cout << "Duck flying\n"; }
    void swim() override { std::cout << "Duck swimming\n"; }
    void quack() { std::cout << "Quack!\n"; }
};

void multipleInheritanceExample() {
    Duck duck;
    duck.fly();
    duck.swim();
    duck.quack();
    
    // Can be used as either interface
    Flyable* flyable = &duck;
    Swimmable* swimmable = &duck;
    
    flyable->fly();
    swimmable->swim();
}
```

### Diamond Problem and Virtual Inheritance

```cpp
class Animal {
public:
    Animal(const std::string& name) : name_(name) {}
    virtual void makeSound() = 0;
    const std::string& getName() const { return name_; }
protected:
    std::string name_;
};

// Without virtual inheritance - creates diamond problem
class Mammal : public Animal {
public:
    Mammal(const std::string& name) : Animal(name) {}
    void breathe() { std::cout << "Breathing air\n"; }
};

class Bird : public Animal {
public:
    Bird(const std::string& name) : Animal(name) {}
    void layEggs() { std::cout << "Laying eggs\n"; }
};

// PROBLEM: Platypus would have TWO Animal base classes!
// class Platypus : public Mammal, public Bird { ... }  // Ambiguous!

// SOLUTION: Virtual inheritance
class VirtualMammal : public virtual Animal {
public:
    VirtualMammal(const std::string& name) : Animal(name) {}
    void breathe() { std::cout << "Breathing air\n"; }
};

class VirtualBird : public virtual Animal {
public:
    VirtualBird(const std::string& name) : Animal(name) {}
    void layEggs() { std::cout << "Laying eggs\n"; }
};

class Platypus : public VirtualMammal, public VirtualBird {
public:
    Platypus() : Animal("Platypus"), VirtualMammal("Platypus"), VirtualBird("Platypus") {}
    
    void makeSound() override {
        std::cout << "Platypus sound\n";
    }
};
```

---

## 5. Function Overriding vs Overloading vs Hiding

### Function Overriding (Virtual Functions)

```cpp
class Base {
public:
    virtual void func(int x) { std::cout << "Base::func(int): " << x << "\n"; }
};

class Derived : public Base {
public:
    void func(int x) override { std::cout << "Derived::func(int): " << x << "\n"; }
};
```

### Function Overloading (Same Class)

```cpp
class Calculator {
public:
    int add(int a, int b) { return a + b; }
    double add(double a, double b) { return a + b; }
    int add(int a, int b, int c) { return a + b + c; }
};
```

### Function Hiding (Non-Virtual in Inheritance)

```cpp
class Base {
public:
    void func(int x) { std::cout << "Base::func(int): " << x << "\n"; }
    void func(double x) { std::cout << "Base::func(double): " << x << "\n"; }
};

class Derived : public Base {
public:
    void func(int x) { std::cout << "Derived::func(int): " << x << "\n"; }
    // This HIDES all Base::func versions, not just the int version!
};

void demonstrateHiding() {
    Derived d;
    d.func(42);     // Calls Derived::func(int)
    // d.func(3.14);   // ERROR! Base::func(double) is hidden
    
    // To access hidden functions:
    d.Base::func(3.14);  // Explicit qualification
}

// Solution: Use 'using' declaration to bring all overloads into scope
class BetterDerived : public Base {
public:
    using Base::func;  // Bring all Base::func overloads into scope
    void func(int x) { std::cout << "BetterDerived::func(int): " << x << "\n"; }
};
```

---

## 6. Advanced Virtual Function Concepts

### Virtual Function Call from Constructor/Destructor

```cpp
class Base {
public:
    Base() {
        init();  // CAREFUL! Calls Base::init(), not Derived::init()
    }
    
    virtual ~Base() {
        cleanup();  // CAREFUL! Calls Base::cleanup(), not Derived::cleanup()
    }
    
    virtual void init() { std::cout << "Base::init\n"; }
    virtual void cleanup() { std::cout << "Base::cleanup\n"; }
};

class Derived : public Base {
public:
    Derived() {
        // Base constructor already called, now Derived constructor
        init();  // This WILL call Derived::init()
    }
    
    void init() override { std::cout << "Derived::init\n"; }
    void cleanup() override { std::cout << "Derived::cleanup\n"; }
};

void demonstrateConstructorVirtual() {
    Derived d;  // Output: "Base::init", "Derived::init"
    // When d is destroyed: "Base::cleanup" (not "Derived::cleanup")
}
```

### Final and Override Keywords (C++11)

```cpp
class Base {
public:
    virtual void func1() {}
    virtual void func2() {}
    virtual void func3() final {}  // Cannot be overridden further
};

class Derived : public Base {
public:
    void func1() override {}           // Good: explicitly overriding
    void func2() {}                    // Works but not recommended
    // void func3() override {}        // ERROR: func3 is final
    
    // void func4() override {}        // ERROR: no Base::func4 to override
};

class Final final : public Base {  // This class cannot be inherited from
public:
    void func1() override final {}     // Final override
};

// class CannotInherit : public Final {};  // ERROR: Final is final
```

---

## 7. Performance Considerations

### Virtual Function Overhead

```cpp
class NonVirtual {
public:
    void func() { /* implementation */ }  // Direct call
};

class Virtual {
public:
    virtual void func() { /* implementation */ }  // Indirect call through vtable
};

// Benchmark comparison:
void benchmarkCalls() {
    NonVirtual nv;
    Virtual v;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    // Direct calls - can be inlined
    for (int i = 0; i < 1000000; ++i) {
        nv.func();
    }
    
    auto middle = std::chrono::high_resolution_clock::now();
    
    // Virtual calls - cannot be inlined, vtable lookup
    for (int i = 0; i < 1000000; ++i) {
        v.func();
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    
    // Virtual calls are typically 10-20% slower
}
```

### When to Use Virtual Functions

**Use Virtual Functions When:**
- You need runtime polymorphism
- You have a hierarchy where derived classes override behavior
- You're designing interfaces or abstract base classes

**Avoid Virtual Functions When:**
- Performance is critical and polymorphism isn't needed
- The class hierarchy is shallow and unlikely to change
- You're designing value types (like mathematical vectors)

---

## 8. Best Practices

### Virtual Function Design Guidelines

```cpp
// Good: Interface design
class IProcessor {
public:
    virtual ~IProcessor() = default;
    virtual void process(const Data& data) = 0;
    virtual bool canProcess(const Data& data) const = 0;
};

// Good: Template method pattern
class Algorithm {
public:
    void execute() {  // Non-virtual public interface
        if (validate()) {
            performStep1();
            performStep2();
            finalize();
        }
    }
    
    virtual ~Algorithm() = default;

protected:
    virtual bool validate() const = 0;
    virtual void performStep1() = 0;
    virtual void performStep2() = 0;
    virtual void finalize() {}  // Optional override
};

// Good: Non-virtual interface (NVI) idiom
class Widget {
public:
    void show() {  // Non-virtual public interface
        doShow();  // Virtual implementation
    }
    
private:
    virtual void doShow() = 0;  // Derived classes implement this
};
```

---

## 9. Summary

| Concept | Purpose | Key Points |
|---------|---------|------------|
| **Virtual Functions** | Runtime polymorphism | vtable lookup, overridable |
| **Pure Virtual** | Abstract interfaces | Must be implemented in derived classes |
| **Virtual Destructor** | Proper cleanup | Required for polymorphic deletion |
| **Multiple Inheritance** | Multiple capabilities | Watch for diamond problem |
| **Virtual Inheritance** | Solve diamond problem | Shared base class |
| **final/override** | Code clarity/safety | Explicit intent, compiler checks |

**Key Takeaways:**
- Always use virtual destructors in polymorphic base classes
- Prefer pure virtual functions for interfaces
- Use `override` keyword for clarity and safety
- Be aware of virtual function call overhead
- Consider virtual inheritance for complex hierarchies
- Virtual functions called in constructors/destructors don't dispatch to derived classes

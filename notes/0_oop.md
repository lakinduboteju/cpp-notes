# C++ Object-Oriented Programming (OOP) Concepts

## 1. Core OOP Principles

- **Encapsulation:** Bundle data and methods operating on that data within a class.
- **Abstraction:** Hide implementation details and show only essential features (using public APIs, abstract base classes).
- **Inheritanc### Multiple Inheritance and Diamond Problem

C++ supports multiple inheritance, but it can lead to the **Diamond Problem**.

#### Diamond Problem Example:

```cpp
class Animal {
protected:
    std::string name;
public:
    Animal(const std::string& n) : name(n) {}
    virtual void speak() { std::cout << name << " makes a sound\n"; }
};

class Mammal : public Animal {
public:
    Mammal(const std::string& n) : Animal(n) {}
    void giveBirth() { std::cout << name << " gives birth\n"; }
};

class Bird : public Animal {
public:
    Bird(const std::string& n) : Animal(n) {}
    void layEggs() { std::cout << name << " lays eggs\n"; }
};

// Diamond Problem: Bat inherits from both Mammal and Bird
// Both inherit from Animal, creating two Animal subobjects
class Bat : public Mammal, public Bird {
public:
    // ERROR: Ambiguous - which Animal constructor?
    // Bat(const std::string& n) : Mammal(n), Bird(n) {} 
    
    // Must specify which path to use
    Bat(const std::string& n) : Mammal(n), Bird(n) {}
    
    void fly() { std::cout << name << " flies\n"; }  // ERROR: Ambiguous name!
};
```

#### Problems:
1. **Ambiguous Access**: `name` exists in two Animal subobjects
2. **Memory Waste**: Two copies of Animal data
3. **Inconsistent State**: Two Animal subobjects can have different values

#### Solution: Virtual Inheritance

```cpp
class Animal {
protected:
    std::string name;
public:
    Animal(const std::string& n) : name(n) {}
    virtual void speak() { std::cout << name << " makes a sound\n"; }
};

// Virtual inheritance ensures only one Animal subobject
class Mammal : public virtual Animal {
public:
    Mammal(const std::string& n) : Animal(n) {}
    void giveBirth() { std::cout << name << " gives birth\n"; }
};

class Bird : public virtual Animal {
public:
    Bird(const std::string& n) : Animal(n) {}
    void layEggs() { std::cout << name << " lays eggs\n"; }
};

class Bat : public Mammal, public Bird {
public:
    // Most derived class must call virtual base constructor directly
    Bat(const std::string& n) : Animal(n), Mammal(n), Bird(n) {}
    
    void fly() { std::cout << name << " flies\n"; }  // OK: only one name
};

// Usage
Bat bat("Bruce");
bat.speak();     // OK: calls Animal::speak()
bat.giveBirth(); // OK: calls Mammal::giveBirth()
bat.layEggs();   // OK: calls Bird::layEggs()
bat.fly();       // OK: calls Bat::fly()
```

#### How Virtual Inheritance Works:
- Creates only **one shared instance** of the virtual base class
- **Most derived class** is responsible for initializing the virtual base
- Slightly more complex object layout and construction, but resolves ambiguity* Derive new classes from existing ones, reusing and extending behavior.
- **Polymorphism:** The ability to use a single interface to represent different underlying forms (data types). It allows the same code to work with objects of different types, providing a unified way to interact with different implementations. Polymorphism enables "one interface, multiple implementations."

---

## 2. Polymorphism in C++

### a. Compile-Time Polymorphism

- **Also known as:** Static polymorphism
- **Achieved by:** Function Overloading, Operator Overloading, Templates

**Examples:**

```cpp
// Function Overloading - multiple functions with same name, different parameters
void print(int x) { std::cout << "Integer: " << x << "\n"; }
void print(double x) { std::cout << "Double: " << x << "\n"; }
void print(const std::string& x) { std::cout << "String: " << x << "\n"; }

// Operator Overloading - multiple operators for same symbol
struct Vec2 { 
    int x, y; 
    Vec2(int x = 0, int y = 0) : x(x), y(y) {}
};

// Overloading + operator for Vec2
Vec2 operator+(const Vec2& a, const Vec2& b) {
    return Vec2(a.x + b.x, a.y + b.y);
}

// Overloading += operator for Vec2 
Vec2& operator+=(Vec2& a, const Vec2& b) {
    a.x += b.x;
    a.y += b.y;
    return a;
}

// Overloading << operator for Vec2
std::ostream& operator<<(std::ostream& os, const Vec2& v) {
    return os << "(" << v.x << ", " << v.y << ")";
}

// Templates - single code works with multiple types
template<typename T>
void swap(T& a, T& b) { T tmp = a; a = b; b = tmp; }
```
- Chosen at compile time—no runtime overhead.

#### How Template Code Generation Works

When the compiler encounters template usage, it performs **template instantiation**:

1. **Code Generation Phase**: For each unique template argument, the compiler generates a separate copy of the template code with type substitution.

```cpp
template<typename T>
T add(T a, T b) { return a + b; }

// Usage
int result1 = add(5, 3);        // Instantiates add<int>
double result2 = add(2.5, 1.7); // Instantiates add<double>
```

2. **Compilation to Binary**: The compiler generates separate functions:
```cpp
// Generated by compiler:
int add_int(int a, int b) { return a + b; }      // for add<int>
double add_double(double a, double b) { return a + b; } // for add<double>
```

3. **Binary Layout**: Each instantiation becomes a separate function in the object file, with mangled names for linking:
   - `add<int>` → `_Z3addIiET_S0_S0_` (mangled name)
   - `add<double>` → `_Z3addIdET_S0_S0_` (mangled name)

4. **Optimization**: Modern compilers can inline these functions, eliminating function call overhead entirely.

---

### b. Run-Time Polymorphism

- **Also known as:** Dynamic polymorphism
- **Achieved by:** Inheritance + Virtual Functions + Pointers/References

**Example:**

```cpp
class Shape {
public:
    virtual void draw() const = 0;
    virtual double area() const = 0;
    virtual ~Shape() = default;  // Virtual destructor important!
};

class Circle : public Shape {
private:
    double radius;
public:
    Circle(double r) : radius(r) {}
    void draw() const override { std::cout << "Drawing Circle\n"; }
    double area() const override { return 3.14159 * radius * radius; }
};

class Rectangle : public Shape {
private:
    double width, height;
public:
    Rectangle(double w, double h) : width(w), height(h) {}
    void draw() const override { std::cout << "Drawing Rectangle\n"; }
    double area() const override { return width * height; }
};

// Dynamic dispatch in action
void processShape(const Shape& s) { 
    s.draw();                    // Function determined at runtime
    std::cout << "Area: " << s.area() << "\n";
}
```

#### How Runtime Function Selection Works (Dynamic Dispatch)

**Dynamic Dispatch** is the process of selecting which function implementation to call at runtime based on the actual object type, not the pointer/reference type.

##### Virtual Table (vtable) Mechanism:

1. **vtable Creation**: Each class with virtual functions gets a virtual table (vtable) - an array of function pointers.

```cpp
// Conceptual vtable layout:
// Shape vtable: [nullptr, nullptr, ~Shape]  // Pure virtual = nullptr
// Circle vtable: [Circle::draw, Circle::area, Circle::~Circle]
// Rectangle vtable: [Rectangle::draw, Rectangle::area, Rectangle::~Rectangle]
```

2. **vptr (Virtual Pointer)**: Each object contains a hidden pointer (vptr) to its class's vtable.

```cpp
Circle c(5.0);
// c's memory layout: [vptr → Circle vtable] [radius = 5.0]
```

3. **Dynamic Dispatch Process**:
   - When `s.draw()` is called on a `Shape&`:
   - Compiler generates: `s.vptr[0](&s)` (call function at vtable index 0)
   - At runtime: Follow vptr → vtable → get actual function → call it

4. **Performance**: One extra indirection (pointer dereference) compared to direct calls.

##### What "Dynamic" Means:
- **Static/Compile-time**: Function call destination known at compile time
- **Dynamic/Runtime**: Function call destination determined by examining object's actual type at runtime through vtable lookup
- Virtual function mechanism (vtable) enables dynamic dispatch at runtime.
- **Cost**: One additional pointer dereference per virtual function call.
- **Benefit**: True polymorphic behavior - same interface, different implementations selected at runtime.

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

### d. Universal References and Perfect Forwarding

#### Are Universal References Compile-Time Polymorphism?

**Yes, but with caveats.** Universal references provide a form of compile-time polymorphism because:
- Single template function can accept any type and value category
- Behavior is resolved at compile time through template instantiation
- However, they don't provide different implementations like function overloading does

- **Universal (forwarding) references** are parameters declared as `T&&` in a deduced context (templates).  
- They enable *perfect forwarding* (preserving value category when forwarding arguments).
- Universal references provide **flexibility and generic programming**, but do **not** provide runtime polymorphism.

**Complete Example with Perfect Forwarding:**
```cpp
#include <iostream>
#include <utility>

// Target functions to forward to
void process(int& x) { 
    std::cout << "Processing lvalue: " << x << "\n"; 
    x *= 2;  // Can modify
}

void process(const int& x) { 
    std::cout << "Processing const lvalue: " << x << "\n"; 
}

void process(int&& x) { 
    std::cout << "Processing rvalue: " << x << "\n"; 
    x *= 3;  // Can modify temporary
}

// Perfect forwarding wrapper
template<typename T>
void wrapper(T&& arg) {
    std::cout << "Wrapper called with: ";
    
    // Perfect forward: preserves value category
    process(std::forward<T>(arg));
    
    // Without std::forward, would always call lvalue version
    // process(arg);  // arg is always lvalue inside function
}

// Usage examples
int main() {
    int x = 10;
    const int y = 20;
    
    wrapper(x);        // T = int&, perfect forwards as lvalue
    wrapper(y);        // T = const int&, perfect forwards as const lvalue  
    wrapper(30);       // T = int, perfect forwards as rvalue
    wrapper(std::move(x)); // T = int, perfect forwards as rvalue
    
    return 0;
}
```

**Output:**
```
Wrapper called with: Processing lvalue: 10
Wrapper called with: Processing const lvalue: 20
Wrapper called with: Processing rvalue: 30
Wrapper called with: Processing rvalue: 20
```

#### How Perfect Forwarding Works:

1. **Reference Collapsing Rules**:
   - `T& &&` → `T&` (lvalue reference)
   - `T&& &&` → `T&&` (rvalue reference)

2. **std::forward Magic**:
   - `std::forward<T&>(arg)` → returns `T&`
   - `std::forward<T>(arg)` → returns `T&&`

3. **Template Argument Deduction**:
   ```cpp
   int x = 42;
   wrapper(x);    // T deduced as int&
   wrapper(42);   // T deduced as int
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

### Access Control and Inheritance Modes

C++ provides three access levels that work differently for class members and inheritance.

#### Member Access Levels:

```cpp
class MyClass {
private:
    int private_data;      // Only accessible within MyClass
    void private_method(); // Only accessible within MyClass

protected:
    int protected_data;      // Accessible in MyClass and derived classes
    void protected_method(); // Accessible in MyClass and derived classes

public:
    int public_data;      // Accessible from anywhere
    void public_method(); // Accessible from anywhere
};
```

#### Inheritance Access Modes:

The inheritance mode controls how base class members are accessible in the derived class:

```cpp
class Base {
public:
    int pub_member;
protected:
    int prot_member;
private:
    int priv_member;
};

// Public Inheritance (most common)
class PublicDerived : public Base {
    // pub_member  → public    (accessible everywhere)
    // prot_member → protected (accessible in further derived classes)
    // priv_member → not accessible
};

// Protected Inheritance
class ProtectedDerived : protected Base {
    // pub_member  → protected (accessible in further derived classes only)
    // prot_member → protected (accessible in further derived classes only)
    // priv_member → not accessible
};

// Private Inheritance
class PrivateDerived : private Base {
    // pub_member  → private (accessible only within PrivateDerived)
    // prot_member → private (accessible only within PrivateDerived)
    // priv_member → not accessible
};
```

#### Access Control Rules Table:

| Base Access | Public Inheritance | Protected Inheritance | Private Inheritance |
|-------------|-------------------|----------------------|-------------------|
| `public`    | `public`          | `protected`          | `private`         |
| `protected` | `protected`       | `protected`          | `private`         |
| `private`   | not accessible    | not accessible       | not accessible    |

#### Practical Example:

```cpp
class Vehicle {
public:
    void start() { std::cout << "Vehicle started\n"; }
protected:
    int engine_power;
private:
    std::string vin_number;
};

// IS-A relationship: Car is a Vehicle
class Car : public Vehicle {  // Public inheritance
public:
    void drive() {
        start();        // OK: inherited as public
        engine_power = 200; // OK: inherited as protected
        // vin_number = "123"; // ERROR: private not inherited
    }
};

// HAS-A relationship: CarDealer has a Vehicle (composition preferred)
class CarDealer : private Vehicle {  // Private inheritance
public:
    void testDrive() {
        start();        // OK: accessible within CarDealer
        engine_power = 150; // OK: accessible within CarDealer
    }
};

// Usage
Car my_car;
my_car.start();       // OK: start() is public in Car
my_car.engine_power = 250; // ERROR: protected not accessible outside

CarDealer dealer;
// dealer.start();    // ERROR: start() is private in CarDealer
dealer.testDrive();   // OK: public method of CarDealer
```

#### When to Use Each Mode:
- **Public Inheritance**: "IS-A" relationship (most common)
- **Protected Inheritance**: Rarely used, similar to private but allows further inheritance
- **Private Inheritance**: "IMPLEMENTED-IN-TERMS-OF" relationship (prefer composition)

### Friend Classes and Functions

Friend declarations grant external classes or functions access to private and protected members, breaking encapsulation in a controlled way.

#### Friend Function Example:

```cpp
class BankAccount {
private:
    double balance;
    std::string account_number;

public:
    BankAccount(const std::string& acc_num, double initial_balance) 
        : account_number(acc_num), balance(initial_balance) {}

    // Declare friend function
    friend void auditAccount(const BankAccount& account);
    friend double calculateInterest(const BankAccount& account, double rate);

    // Regular public methods
    void deposit(double amount) { balance += amount; }
    double getBalance() const { return balance; }  // Controlled access
};

// Friend functions can access private members
void auditAccount(const BankAccount& account) {
    std::cout << "Auditing Account: " << account.account_number 
              << ", Balance: $" << account.balance << "\n";
}

double calculateInterest(const BankAccount& account, double rate) {
    return account.balance * rate;  // Direct access to private balance
}

// Usage
BankAccount acc("12345", 1000.0);
auditAccount(acc);  // Can access private members
double interest = calculateInterest(acc, 0.05);
```

#### Friend Class Example:

```cpp
class Engine {
private:
    int horsepower;
    double fuel_efficiency;
    bool is_running;

public:
    Engine(int hp, double mpg) : horsepower(hp), fuel_efficiency(mpg), is_running(false) {}
    
    // Declare Car as friend class
    friend class Car;
    friend class Mechanic;  // Multiple friends allowed
    
    void start() { is_running = true; }
};

class Car {
private:
    Engine engine;
    std::string model;

public:
    Car(const std::string& m, int hp, double mpg) 
        : model(m), engine(hp, mpg) {}

    void displaySpecs() {
        // Can access Engine's private members because Car is friend
        std::cout << model << " - " << engine.horsepower << "HP, "
                  << engine.fuel_efficiency << "MPG\n";
        
        if (engine.is_running) {
            std::cout << "Engine is running\n";
        }
    }
    
    void tuneEngine() {
        // Direct manipulation of friend's private data
        engine.horsepower += 50;  // Turbocharge!
        engine.fuel_efficiency *= 0.9;  // Less efficient now
    }
};

class Mechanic {
public:
    void diagnose(Engine& engine) {
        // Mechanic can access Engine's private members
        std::cout << "Diagnosing engine: " << engine.horsepower << "HP\n";
        if (!engine.is_running) {
            std::cout << "Engine is not running\n";
            engine.is_running = true;  // Can modify private state
        }
    }
};

// Usage
Car sports_car("Ferrari", 500, 15.0);
sports_car.displaySpecs();
sports_car.tuneEngine();
sports_car.displaySpecs();
```

#### Friend Function for Operator Overloading:

```cpp
class Complex {
private:
    double real, imag;

public:
    Complex(double r = 0, double i = 0) : real(r), imag(i) {}
    
    // Friend function for symmetric operations
    friend Complex operator+(const Complex& a, const Complex& b);
    friend std::ostream& operator<<(std::ostream& os, const Complex& c);
    
    // Could also be implemented as member, but friend is cleaner for symmetric ops
};

// Friend functions can access private members directly
Complex operator+(const Complex& a, const Complex& b) {
    return Complex(a.real + b.real, a.imag + b.imag);
}

std::ostream& operator<<(std::ostream& os, const Complex& c) {
    return os << c.real << " + " << c.imag << "i";
}

// Usage
Complex c1(3, 4);
Complex c2(1, 2);
Complex result = c1 + c2;  // Uses friend operator+
std::cout << result;       // Uses friend operator<<
```

#### Important Notes:
- **Friendship is not inherited**: If `Base` declares `Foo` as friend, derived classes don't automatically have `Foo` as friend
- **Friendship is not symmetric**: If `A` declares `B` as friend, `B` doesn't automatically have access to `A`'s privates
- **Friendship is not transitive**: If `A` is friend of `B`, and `B` is friend of `C`, `A` is not automatically friend of `C`
- **Use sparingly**: Breaks encapsulation, use only when necessary (operators, testing, closely coupled classes)

---

## 4. Summary Table

| Polymorphism Type         | Mechanism                        | Example                     | When Resolved    |
|---------------------------|----------------------------------|-----------------------------|------------------|
| Compile-time (static)     | Overloading, templates           | `swap<int>(a, b)`           | Compile time     |
| Template specialization   | Specialized templates            | `print<bool>(true)`         | Compile time     |
| Runtime (dynamic)         | Virtual functions (vtable)       | `virtual void foo()`        | Run time         |
| Universal refs/templates  | Generic programming (T&&, etc.)  | `call_foo(t)`               | Compile time     |

# C++ Templates and Generic Programming

## 1. Function Templates

### Basic Function Templates

```cpp
// Generic function template
template<typename T>
T max(T a, T b) {
    return (a > b) ? a : b;
}

// Multiple template parameters
template<typename T, typename U>
auto add(T a, U b) -> decltype(a + b) {  // C++11 trailing return type
    return a + b;
}

// C++14 simplified
template<typename T, typename U>
auto add14(T a, U b) {
    return a + b;  // Return type deduced
}

void functionTemplateExample() {
    int maxInt = max(10, 20);           // T = int
    double maxDouble = max(3.14, 2.71); // T = double
    
    auto result1 = add(5, 3.14);        // T = int, U = double
    auto result2 = add14(5, 3.14f);     // Same but cleaner syntax
}
```

### Template Specialization

```cpp
// Primary template
template<typename T>
void print(T value) {
    std::cout << "Generic: " << value << "\n";
}

// Full specialization for bool
template<>
void print<bool>(bool value) {
    std::cout << "Bool: " << (value ? "true" : "false") << "\n";
}

// Full specialization for const char*
template<>
void print<const char*>(const char* value) {
    std::cout << "String: \"" << value << "\"\n";
}

void specializationExample() {
    print(42);        // Generic: 42
    print(true);      // Bool: true
    print("hello");   // String: "hello"
}
```

---

## 2. Class Templates

### Basic Class Templates

```cpp
template<typename T>
class Stack {
private:
    std::vector<T> elements_;
    
public:
    void push(const T& element) {
        elements_.push_back(element);
    }
    
    void pop() {
        if (elements_.empty()) {
            throw std::runtime_error("Stack is empty");
        }
        elements_.pop_back();
    }
    
    const T& top() const {
        if (elements_.empty()) {
            throw std::runtime_error("Stack is empty");
        }
        return elements_.back();
    }
    
    bool empty() const {
        return elements_.empty();
    }
    
    size_t size() const {
        return elements_.size();
    }
};

void classTemplateExample() {
    Stack<int> intStack;
    intStack.push(10);
    intStack.push(20);
    
    Stack<std::string> stringStack;
    stringStack.push("hello");
    stringStack.push("world");
    
    std::cout << intStack.top() << "\n";     // 20
    std::cout << stringStack.top() << "\n";  // world
}
```

### Template Parameters

```cpp
// Multiple template parameters with defaults
template<typename T, typename Allocator = std::allocator<T>, size_t InitialSize = 10>
class Container {
private:
    T data_[InitialSize];
    Allocator alloc_;
    size_t size_ = 0;
    
public:
    void add(const T& item) {
        if (size_ < InitialSize) {
            data_[size_++] = item;
        }
    }
    
    size_t size() const { return size_; }
    const T& get(size_t index) const { return data_[index]; }
};

// Non-type template parameters
template<int N>
class FixedArray {
private:
    int data_[N];
    
public:
    constexpr size_t size() const { return N; }
    int& operator[](size_t index) { return data_[index]; }
    const int& operator[](size_t index) const { return data_[index]; }
};

void templateParametersExample() {
    Container<int> container1;                           // Uses defaults
    Container<double, std::allocator<double>, 5> container2;  // Custom parameters
    
    FixedArray<10> arr;  // Compile-time size
    arr[0] = 42;
    static_assert(arr.size() == 10);  // Compile-time check
}
```

---

## 3. Template Metaprogramming

### SFINAE (Substitution Failure Is Not An Error)

```cpp
#include <type_traits>

// Enable function only for integral types
template<typename T>
typename std::enable_if<std::is_integral<T>::value, T>::type
process(T value) {
    return value * 2;
}

// Enable function only for floating-point types
template<typename T>
typename std::enable_if<std::is_floating_point<T>::value, T>::type
process(T value) {
    return value / 2.0;
}

// C++14 version using std::enable_if_t
template<typename T>
std::enable_if_t<std::is_integral_v<T>, T>  // C++17 _v suffix
processModern(T value) {
    return value * 2;
}

void sfinaeExample() {
    auto result1 = process(10);     // Calls integral version
    auto result2 = process(3.14);   // Calls floating-point version
    // auto result3 = process("hi");   // Compilation error - no matching function
}
```

### Constexpr and Template Metaprogramming

```cpp
// Compile-time factorial
template<int N>
struct Factorial {
    static constexpr int value = N * Factorial<N-1>::value;
};

template<>
struct Factorial<0> {
    static constexpr int value = 1;
};

// C++14 constexpr function (simpler)
constexpr int factorial(int n) {
    return (n <= 1) ? 1 : n * factorial(n - 1);
}

// Type traits example
template<typename T>
struct IsPointer {
    static constexpr bool value = false;
};

template<typename T>
struct IsPointer<T*> {
    static constexpr bool value = true;
};

void metaprogrammingExample() {
    constexpr int fact5_old = Factorial<5>::value;  // Computed at compile time
    constexpr int fact5_new = factorial(5);         // Also computed at compile time
    
    static_assert(fact5_old == 120);
    static_assert(fact5_new == 120);
    
    static_assert(IsPointer<int>::value == false);
    static_assert(IsPointer<int*>::value == true);
}
```

---

## 4. Variadic Templates (C++11)

### Basic Variadic Templates

```cpp
// Base case for recursion
void print() {
    std::cout << "\n";
}

// Variadic template function
template<typename T, typename... Args>
void print(T first, Args... args) {
    std::cout << first << " ";
    print(args...);  // Recursive call with remaining arguments
}

// C++17 fold expressions (much simpler!)
template<typename... Args>
void printFold(Args... args) {
    ((std::cout << args << " "), ...);  // Fold expression
    std::cout << "\n";
}

void variadicExample() {
    print(1, 2.5, "hello", 'c');        // 1 2.5 hello c
    printFold(1, 2.5, "hello", 'c');    // Same output, simpler code
}
```

### Variadic Class Templates

```cpp
template<typename... Types>
class Tuple;  // Forward declaration

// Base case
template<>
class Tuple<> {};

// Recursive case
template<typename Head, typename... Tail>
class Tuple<Head, Tail...> : private Tuple<Tail...> {
    Head head_;
    
public:
    Tuple() = default;
    
    Tuple(Head h, Tail... tail) : Tuple<Tail...>(tail...), head_(h) {}
    
    Head& head() { return head_; }
    const Head& head() const { return head_; }
    
    Tuple<Tail...>& tail() { return static_cast<Tuple<Tail...>&>(*this); }
    const Tuple<Tail...>& tail() const { return static_cast<const Tuple<Tail...>&>(*this); }
};

// Perfect forwarding with variadic templates
template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

void variadicClassExample() {
    Tuple<int, double, std::string> t(42, 3.14, "hello");
    std::cout << t.head() << "\n";  // 42
    
    auto ptr = make_unique<std::vector<int>>(10, 42);  // Vector with 10 elements of value 42
}
```

---

## 5. Template Argument Deduction

### Automatic Type Deduction

```cpp
template<typename T>
void func(T param) {
    // T is deduced from argument
}

template<typename T>
void funcRef(T& param) {
    // T is deduced, but param is always a reference
}

template<typename T>
void funcUniversal(T&& param) {
    // Universal reference - T&& can bind to lvalues and rvalues
}

void deductionExample() {
    int x = 42;
    const int cx = x;
    const int& rx = x;
    
    func(x);   // T = int
    func(cx);  // T = int (const stripped)
    func(rx);  // T = int (const and reference stripped)
    
    funcRef(x);   // T = int, param type is int&
    funcRef(cx);  // T = const int, param type is const int&
    // funcRef(42);  // Error! Can't bind rvalue to non-const reference
    
    funcUniversal(x);   // T = int&, param type is int&
    funcUniversal(42);  // T = int, param type is int&&
}
```

### Class Template Argument Deduction (C++17)

```cpp
template<typename T>
class MyVector {
    std::vector<T> data_;
public:
    MyVector(std::initializer_list<T> init) : data_(init) {}
    
    template<typename Iterator>
    MyVector(Iterator begin, Iterator end) : data_(begin, end) {}
};

// Deduction guide (optional, but helpful)
template<typename Iterator>
MyVector(Iterator, Iterator) -> MyVector<typename std::iterator_traits<Iterator>::value_type>;

void ctadExample() {
    // C++17: No need to specify template arguments
    MyVector vec1{1, 2, 3, 4};           // Deduced as MyVector<int>
    MyVector vec2{"a", "b", "c"};        // Deduced as MyVector<const char*>
    
    std::vector<double> source{1.1, 2.2, 3.3};
    MyVector vec3(source.begin(), source.end());  // Deduced as MyVector<double>
}
```

---

## 6. Concepts (C++20)

### Basic Concepts

```cpp
#include <concepts>

// Define a concept
template<typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;

// Use concept to constrain template
template<Numeric T>
T multiply(T a, T b) {
    return a * b;
}

// Alternative syntax
template<typename T>
requires Numeric<T>
T add(T a, T b) {
    return a + b;
}

// Custom concept
template<typename T>
concept Printable = requires(T t) {
    std::cout << t;  // T must be printable to std::cout
};

template<Printable T>
void print(const T& value) {
    std::cout << value << "\n";
}

void conceptsExample() {
    auto result1 = multiply(5, 10);        // OK: int is Numeric
    auto result2 = multiply(3.14, 2.71);   // OK: double is Numeric
    // auto result3 = multiply("a", "b");     // Error: string is not Numeric
    
    print(42);        // OK: int is Printable
    print("hello");   // OK: const char* is Printable
}
```

---

## 7. Template Best Practices

### Avoid Template Bloat

```cpp
// Bad: Template instantiated for every type
template<typename T>
class BadLogger {
    std::vector<T> logs_;
public:
    void log(const T& message) {
        logs_.push_back(message);
        std::cout << "Log: " << message << "\n";
    }
};

// Better: Non-template base class for common functionality
class LoggerBase {
protected:
    void logToConsole(const std::string& message) {
        std::cout << "Log: " << message << "\n";
    }
};

template<typename T>
class Logger : private LoggerBase {
    std::vector<T> logs_;
public:
    void log(const T& message) {
        logs_.push_back(message);
        std::ostringstream oss;
        oss << message;
        logToConsole(oss.str());  // Reuse non-template code
    }
};
```

### Use Type Traits

```cpp
#include <type_traits>

template<typename T>
class Container {
public:
    void process() {
        if constexpr (std::is_arithmetic_v<T>) {
            // Arithmetic types: use optimized algorithm
            processArithmetic();
        } else {
            // General types: use generic algorithm
            processGeneric();
        }
    }
    
private:
    void processArithmetic() { /* optimized for numbers */ }
    void processGeneric() { /* works for any type */ }
};
```

### Template Alias for Cleaner Code

```cpp
// Type aliases for complex template types
template<typename T>
using UniquePtr = std::unique_ptr<T>;

template<typename Key, typename Value>
using HashMap = std::unordered_map<Key, Value>;

template<typename T>
using Vector = std::vector<T>;

// Even more complex aliases
template<typename T>
using ProcessorFunction = std::function<void(const T&)>;

template<typename T>
using ProcessorMap = HashMap<std::string, ProcessorFunction<T>>;

void aliasExample() {
    UniquePtr<int> ptr = std::make_unique<int>(42);
    HashMap<std::string, int> map;
    Vector<double> vec{1.1, 2.2, 3.3};
    
    ProcessorMap<int> processors;
    processors["double"] = [](const int& x) { std::cout << x * 2 << "\n"; };
}
```

---

## 8. Common Template Patterns

### CRTP (Curiously Recurring Template Pattern)

```cpp
template<typename Derived>
class Base {
public:
    void interface() {
        static_cast<Derived*>(this)->implementation();
    }
    
    void commonFunction() {
        std::cout << "Common functionality\n";
        interface();
    }
};

class Derived1 : public Base<Derived1> {
public:
    void implementation() {
        std::cout << "Derived1 implementation\n";
    }
};

class Derived2 : public Base<Derived2> {
public:
    void implementation() {
        std::cout << "Derived2 implementation\n";
    }
};

void crtpExample() {
    Derived1 d1;
    Derived2 d2;
    
    d1.commonFunction();  // Calls Derived1::implementation
    d2.commonFunction();  // Calls Derived2::implementation
}
```

### Policy-Based Design

```cpp
// Storage policies
template<typename T>
class VectorStorage {
    std::vector<T> data_;
public:
    void store(const T& item) { data_.push_back(item); }
    T& get(size_t index) { return data_[index]; }
    size_t size() const { return data_.size(); }
};

template<typename T>
class ArrayStorage {
    static constexpr size_t MAX_SIZE = 100;
    T data_[MAX_SIZE];
    size_t size_ = 0;
public:
    void store(const T& item) { 
        if (size_ < MAX_SIZE) data_[size_++] = item; 
    }
    T& get(size_t index) { return data_[index]; }
    size_t size() const { return size_; }
};

// Container that uses policy
template<typename T, template<typename> class StoragePolicy>
class Container : private StoragePolicy<T> {
public:
    using StoragePolicy<T>::store;
    using StoragePolicy<T>::get;
    using StoragePolicy<T>::size;
    
    void addItem(const T& item) {
        store(item);
    }
};

void policyExample() {
    Container<int, VectorStorage> dynamicContainer;
    Container<int, ArrayStorage> fixedContainer;
    
    dynamicContainer.addItem(42);
    fixedContainer.addItem(42);
}
```

---

## 9. Summary

| Concept | Purpose | Key Benefits |
|---------|---------|--------------|
| **Function Templates** | Generic functions | Code reuse, type safety |
| **Class Templates** | Generic classes | Type-safe containers/algorithms |
| **Specialization** | Custom behavior for specific types | Optimization, special cases |
| **Variadic Templates** | Variable number of arguments | Flexible interfaces |
| **SFINAE** | Conditional compilation | Enable/disable based on type properties |
| **Concepts (C++20)** | Template constraints | Better error messages, clearer intent |
| **CRTP** | Static polymorphism | Performance benefits of virtual functions without overhead |

**Key Takeaways:**
- Templates provide compile-time polymorphism with zero runtime overhead
- Use concepts (C++20) or SFINAE for template constraints
- Avoid template bloat by extracting non-template base classes
- Variadic templates enable flexible, type-safe interfaces
- Template metaprogramming can compute values at compile time
- Type traits help write generic, efficient code

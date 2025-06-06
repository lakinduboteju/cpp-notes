# C++ Rule of Five, `noexcept`, and Default Special Member Functions

## Rule of Five

If your C++ class manages resources (memory, file handles, etc.) and you define **any** of the following, you should probably define **all five**:

1. Destructor  
2. Copy Constructor  
3. Copy Assignment Operator  
4. Move Constructor  
5. Move Assignment Operator  

This ensures resource safety and predictable behavior.

### The Five Special Functions

| Function              | When is it called?                                        | Example                        |
|-----------------------|-----------------------------------------------------------|--------------------------------|
| Destructor            | When object goes out of scope or is deleted               | `{ MyType a; }`                |
| Copy Constructor      | Creating a new object from an existing one                | `MyType b = a;`                |
| Copy Assignment       | Assigning to an existing object from another object       | `b = a;`                       |
| Move Constructor      | Creating object from a temporary/rvalue                   | `MyType b = std::move(a);`     |
| Move Assignment       | Assigning from a temporary/rvalue to an existing object   | `b = std::move(a);`            |

---

## Why Use `noexcept`?

The `noexcept` specifier promises a function will **never throw exceptions**.  
**STL containers (like `std::vector`) only use move operations if they are marked `noexcept`.**  
If not marked, containers may fall back to copy semantics, which can be less efficient.

```cpp
struct MyType {
    MyType(MyType&&) noexcept;          // Enables efficient moving!
    MyType& operator=(MyType&&) noexcept;
};
```
**Recommendation:**  
Mark move constructors and move assignment as `noexcept` if all member variables' moves are also `noexcept`.

---

## Default Special Member Functions

If you **don’t** declare these special functions, C++ will automatically generate them for you.

- For **primitive types**: field-wise copy/move/assignment/destruction is safe and efficient.
- For **objects**: compiler will invoke each member’s respective copy/move/assign/destructor.
- For **resource-managing classes** (e.g., raw pointers), always implement the Rule of Five manually!

```cpp
struct Simple {
    int x;
    double y;
    // All special functions are safe as default
};

struct Wrapper {
    std::string s;
    // Default copy/move/assignment uses std::string's implementations
};
```

**Tip:**  
If no special resource handling needed, rely on the **Rule of Zero**—let the compiler generate all special members.

### Explicit Default/Deletion

```cpp
struct Defaulted {
    std::string s;
    Defaulted(const Defaulted&) = default;     // explicit defaulting
    Defaulted& operator=(const Defaulted&) = default;
    Defaulted(Defaulted&&) = delete;           // disabling move
};
```

---

## Example: Class with All Five Special Functions

```cpp
#include <string>
#include <iostream>
#define LOGI(msg) std::cout << msg << std::endl

struct MyType {
    std::string value;

    MyType(const std::string& v = "") : value(v) { LOGI("Constructor"); }
    MyType(const MyType& other) : value(other.value) { LOGI("Copy Constructor"); }
    MyType(MyType&& other) noexcept : value(std::move(other.value)) { LOGI("Move Constructor"); }
    MyType& operator=(const MyType& other) {
        LOGI("Copy Assignment");
        if (this != &other) value = other.value;
        return *this;
    }
    MyType& operator=(MyType&& other) noexcept {
        LOGI("Move Assignment");
        if (this != &other) value = std::move(other.value);
        return *this;
    }
    ~MyType() { LOGI("Destructor"); }
};
```
**Usage Examples:**
```cpp
MyType a("A");
MyType b = a;                // Copy constructor
MyType c = std::move(a);     // Move constructor
MyType d("D");
d = b;                       // Copy assignment
d = std::move(c);            // Move assignment
// Destructors called at end of scope
```

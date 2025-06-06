# C++ Class Members: `const` and Other Essentials

## Types of Class Members

1. **Data Members**  
   - Variables that hold state/data of the object.
2. **Member Functions (Methods)**  
   - Functions defined inside the class to operate on data members.
3. **Static Members**  
   - Shared among all instances of the class.
4. **Const Members**  
   - Data or functions that cannot be modified.

---

## `const` and Class Members

### 1. **Const Data Members**

- Declared as `const` inside the class.
- Must be initialized using **constructor initializer list** (cannot be assigned in constructor body).
- Cannot be modified after initialization.

**Example:**
```cpp
class MyClass {
public:
    const int id;
    MyClass(int i) : id(i) {} // OK: Initialization in initializer list
};
```

### 2. **Const Member Functions**

- Declared with `const` **after** the parameter list.
- Cannot modify any non-`mutable` data members of the class.
- Can be called on `const` objects.

**Example:**
```cpp
class MyClass {
    int x;
public:
    int getX() const { return x; } // Won't modify the object
    void setX(int v) { x = v; }    // Can modify x
};
```

### 3. **Const Objects**

- An object declared as `const` can only call `const` member functions.
- Data members of a `const` object cannot be modified (unless they are `mutable`).

**Example:**
```cpp
const MyClass obj(10);
obj.getX();        // OK
obj.setX(5);       // Error! Cannot call non-const function on const object
```

---

## Other Important Points

### Static Members

- Declared with `static`, belong to the class itself, not objects.
- Must be defined outside the class if not inline.

**Example:**
```cpp
class Counter {
public:
    static int count;
};
int Counter::count = 0;
```

### Mutable Members

- Declared with `mutable`, can be modified even in `const` member functions and for `const` objects.
- Useful for caching and reference counting.

**Example:**
```cpp
class MyClass {
    mutable int cache;
public:
    int getCached() const { cache = 1; return cache; } // Legal due to mutable
};
```

### Member Initialization

- Use initializer lists in constructors to initialize `const`, `reference`, and member objects.
- Order of initialization is **the order in which members are declared** in the class, not the order in the initializer list.

---

## Summary Table

| Feature        | Use/Notes                                                      |
|----------------|---------------------------------------------------------------|
| `const` data   | Must be initialized in constructor initializer list            |
| `const` method | Cannot modify object; can call on `const` object              |
| `const` object | Can only call `const` member functions                        |
| `static`       | Shared among all objects; single storage for the class        |
| `mutable`      | Allows modification in `const` functions or on `const` object |

# C++ Memory Management and RAII

## 1. Memory Allocation Types

### Stack vs Heap Memory

| Aspect | Stack | Heap |
|--------|-------|------|
| **Speed** | Very fast (single instruction) | Slower (involves allocator) |
| **Size** | Limited (~1-8MB typically) | Large (limited by system RAM) |
| **Management** | Automatic (RAII) | Manual or smart pointers |
| **Fragmentation** | None | Possible |
| **Access Pattern** | LIFO (Last In, First Out) | Random access |

**Example:**
```cpp
void stackVsHeap() {
    int stackVar = 42;                    // Stack allocation
    int* heapVar = new int(42);          // Heap allocation (manual)
    std::unique_ptr<int> smartPtr = std::make_unique<int>(42); // Heap (automatic)
    
    // stackVar automatically destroyed when function ends
    delete heapVar;                      // Manual cleanup required
    // smartPtr automatically cleans up when going out of scope
}
```

---

## 2. RAII (Resource Acquisition Is Initialization)

RAII is a fundamental C++ idiom where **resource lifetime is tied to object lifetime**.

### Core RAII Principles

1. **Acquire resources in constructor**
2. **Release resources in destructor**
3. **Objects manage their own resources**
4. **Automatic cleanup when objects go out of scope**

**Example:**
```cpp
class FileHandler {
    FILE* file_;
public:
    explicit FileHandler(const char* filename) 
        : file_(fopen(filename, "r")) {
        if (!file_) throw std::runtime_error("Failed to open file");
    }
    
    ~FileHandler() {
        if (file_) fclose(file_);  // Automatic cleanup
    }
    
    // Prevent copying to avoid double-free
    FileHandler(const FileHandler&) = delete;
    FileHandler& operator=(const FileHandler&) = delete;
    
    FILE* get() const { return file_; }
};

void useFile() {
    FileHandler fh("data.txt");  // File opened
    // ... use fh.get() ...
    // File automatically closed when fh goes out of scope
}
```

---

## 3. Smart Pointers

Smart pointers provide automatic memory management and are the modern C++ way to handle dynamic memory.

### std::unique_ptr

- **Exclusive ownership** of a resource
- **Cannot be copied**, only moved
- **Zero overhead** compared to raw pointers
- **Automatic cleanup** when destroyed

```cpp
class Resource {
public:
    Resource() { std::cout << "Resource created\n"; }
    ~Resource() { std::cout << "Resource destroyed\n"; }
    void use() { std::cout << "Using resource\n"; }
};

void uniquePtrExample() {
    // Creation
    std::unique_ptr<Resource> ptr1 = std::make_unique<Resource>();
    auto ptr2 = std::make_unique<Resource>();  // Preferred
    
    // Usage
    ptr1->use();
    (*ptr1).use();
    
    // Transfer ownership
    auto ptr3 = std::move(ptr1);  // ptr1 is now nullptr
    
    // Custom deleter
    auto ptr4 = std::unique_ptr<Resource, std::function<void(Resource*)>>(
        new Resource(), 
        [](Resource* r) { 
            std::cout << "Custom cleanup\n"; 
            delete r; 
        }
    );
    
    // Automatic cleanup when ptr2, ptr3, ptr4 go out of scope
}
```

### std::shared_ptr

- **Shared ownership** with reference counting
- **Thread-safe** reference counting
- **Higher overhead** due to control block
- **Automatic cleanup** when last reference is destroyed

```cpp
void sharedPtrExample() {
    // Creation
    std::shared_ptr<Resource> ptr1 = std::make_shared<Resource>();  // Preferred
    auto ptr2 = std::shared_ptr<Resource>(new Resource());          // Less efficient
    
    std::cout << "Reference count: " << ptr1.use_count() << "\n";  // 1
    
    {
        auto ptr3 = ptr1;  // Copy increases reference count
        std::cout << "Reference count: " << ptr1.use_count() << "\n";  // 2
        
        auto ptr4 = ptr1;  // Another copy
        std::cout << "Reference count: " << ptr1.use_count() << "\n";  // 3
    }  // ptr3 and ptr4 destroyed, reference count decreases
    
    std::cout << "Reference count: " << ptr1.use_count() << "\n";  // 1
    
    // Resource destroyed when ptr1 goes out of scope
}
```

### std::weak_ptr

- **Non-owning** reference to shared_ptr managed object
- **Breaks circular references**
- **Can check if object still exists**
- **Must be converted to shared_ptr to access object**

```cpp
class Node {
public:
    std::shared_ptr<Node> next;
    std::weak_ptr<Node> parent;  // Weak to break cycles
    int data;
    
    Node(int d) : data(d) {}
    ~Node() { std::cout << "Node " << data << " destroyed\n"; }
};

void weakPtrExample() {
    auto node1 = std::make_shared<Node>(1);
    auto node2 = std::make_shared<Node>(2);
    
    node1->next = node2;
    node2->parent = node1;  // Weak reference - no cycle!
    
    std::weak_ptr<Node> weak = node1;
    
    // Check if object still exists
    if (auto shared = weak.lock()) {
        std::cout << "Node data: " << shared->data << "\n";
    } else {
        std::cout << "Object has been destroyed\n";
    }
    
    // node1 and node2 automatically destroyed (no memory leak)
}
```

---

## 4. Memory Leak Prevention

### Common Memory Leak Scenarios

```cpp
// 1. Forgot to delete
void memoryLeak1() {
    int* ptr = new int(42);
    // Forgot: delete ptr;  // LEAK!
}

// 2. Exception before delete
void memoryLeak2() {
    int* ptr = new int(42);
    // Some function that might throw
    riskyFunction();  // If this throws, delete is never reached
    delete ptr;
}

// 3. Circular references with shared_ptr
class BadNode {
public:
    std::shared_ptr<BadNode> next;
    std::shared_ptr<BadNode> parent;  // Creates cycles!
};
```

### Solutions

```cpp
// 1. Use smart pointers
void solution1() {
    auto ptr = std::make_unique<int>(42);
    // Automatic cleanup, no delete needed
}

// 2. RAII with exception safety
void solution2() {
    auto ptr = std::make_unique<int>(42);
    riskyFunction();  // Even if this throws, ptr is cleaned up
}

// 3. Break cycles with weak_ptr
class GoodNode {
public:
    std::shared_ptr<GoodNode> next;
    std::weak_ptr<GoodNode> parent;  // Weak reference breaks cycles
};
```

---

## 5. Custom Memory Management

### Placement New

```cpp
#include <new>

void placementNewExample() {
    // Allocate raw memory
    void* memory = std::malloc(sizeof(int));
    
    // Construct object in that memory
    int* ptr = new(memory) int(42);  // Placement new
    
    // Use object
    std::cout << *ptr << "\n";
    
    // Explicitly call destructor
    ptr->~int();
    
    // Free raw memory
    std::free(memory);
}

class MyClass {
public:
    MyClass(int x) : value(x) { std::cout << "Constructed: " << value << "\n"; }
    ~MyClass() { std::cout << "Destroyed: " << value << "\n"; }
private:
    int value;
};

void placementNewClass() {
    alignas(MyClass) char buffer[sizeof(MyClass)];
    
    // Construct in buffer
    MyClass* obj = new(buffer) MyClass(42);
    
    // Use object
    // ...
    
    // Explicitly destroy
    obj->~MyClass();
    
    // No delete needed - buffer is automatic storage
}
```

### Custom Allocators

```cpp
template<typename T>
class DebugAllocator {
public:
    using value_type = T;
    
    T* allocate(size_t n) {
        std::cout << "Allocating " << n << " objects of size " << sizeof(T) << "\n";
        return static_cast<T*>(std::malloc(n * sizeof(T)));
    }
    
    void deallocate(T* ptr, size_t n) {
        std::cout << "Deallocating " << n << " objects\n";
        std::free(ptr);
    }
};

void customAllocatorExample() {
    std::vector<int, DebugAllocator<int>> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    // Allocation/deallocation messages will be printed
}
```

---

## 6. Memory Management Best Practices

### Prefer Stack to Heap

```cpp
// Good: Stack allocation when possible
void processData() {
    std::array<int, 1000> data;  // Stack allocation
    // ... process data ...
}

// Avoid: Heap allocation for small, short-lived objects
void processDataBad() {
    auto data = std::make_unique<std::array<int, 1000>>();  // Unnecessary heap
    // ... process data ...
}
```

### Use make_unique and make_shared

```cpp
// Preferred: Exception-safe, more efficient
auto ptr1 = std::make_unique<MyClass>(args);
auto ptr2 = std::make_shared<MyClass>(args);

// Avoid: Exception unsafe, less efficient for shared_ptr
auto ptr3 = std::unique_ptr<MyClass>(new MyClass(args));
auto ptr4 = std::shared_ptr<MyClass>(new MyClass(args));
```

### Follow the Rule of Zero

```cpp
// Good: Let standard containers manage memory
class GoodClass {
    std::vector<int> data_;      // Automatic memory management
    std::string name_;           // Automatic memory management
    std::unique_ptr<Resource> resource_;  // Smart pointer manages resource
    // No need for custom destructor, copy/move operations
};

// Avoid: Manual memory management when not necessary
class BadClass {
    int* data_;
    size_t size_;
    // Would need Rule of Five implementation
};
```

---

## 7. Summary

| Technique | Use Case | Pros | Cons |
|-----------|----------|------|------|
| **Stack allocation** | Small, short-lived objects | Fast, automatic cleanup | Limited size |
| **unique_ptr** | Exclusive ownership | Zero overhead, exception safe | No sharing |
| **shared_ptr** | Shared ownership | Reference counting, thread-safe | Higher overhead |
| **weak_ptr** | Breaking cycles | Prevents circular references | Must check validity |
| **RAII** | Resource management | Automatic, exception safe | Requires careful design |

**Key Takeaways:**
- Prefer stack allocation when possible
- Use smart pointers instead of raw pointers for dynamic memory
- Follow RAII principles for all resources
- Use `make_unique` and `make_shared` for exception safety
- Break circular references with `weak_ptr`
- Follow the Rule of Zero when possible

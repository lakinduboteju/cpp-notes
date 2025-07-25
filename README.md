# cpp-notes
C++ Notes and Examples

## Notes

### 0. C++ Object-Oriented Programming (OOP) Concepts

A foundational guide covering core OOP principles and polymorphism mechanisms in C++.

**Location**: `notes/0_oop.md`

**Key Topics Covered**:
- **Core OOP principles**: Encapsulation, abstraction, inheritance, and polymorphism
- **Compile-time polymorphism**: Function overloading, operator overloading, and templates
- **Runtime polymorphism**: Virtual functions, inheritance, and dynamic dispatch
- **Template specialization**: Custom behavior for specific types
- **Universal references**: Perfect forwarding and generic programming

**Concepts Demonstrated**:
- Static vs. dynamic polymorphism trade-offs
- Virtual function mechanism and vtable usage
- Abstract classes and interface design
- Multiple inheritance and virtual inheritance
- Template-based generic programming vs. OOP polymorphism

This guide provides the theoretical foundation for understanding C++ object-oriented design patterns and polymorphic behavior.

### 1. C++ Class Implementation with Special Member Functions

A comprehensive guide covering C++ class design best practices and resource management.

**Location**: `notes/1_class_design.md`

**Key Topics Covered**:
- **Rule of Five**: Complete implementation of destructor, copy constructor, copy assignment, move constructor, and move assignment
- **`noexcept` specification**: Enabling efficient move operations in STL containers
- **Default special member functions**: Understanding when and how the compiler generates special members
- **Resource management**: Safe handling of memory and other resources in custom classes

**Concepts Demonstrated**:
- RAII (Resource Acquisition Is Initialization) principles
- Move semantics for performance optimization
- Copy vs. move operations and when each is called
- Explicit defaulting and deletion of special member functions
- Rule of Zero for simple classes without resource management

This guide is essential for understanding modern C++ class design and avoiding common pitfalls in resource management.

### 2. C++ Class Member Access Control and Inheritance

A detailed explanation of C++ access specifiers and how they interact with inheritance patterns.

**Location**: `notes/2_class_access.md`

**Key Topics Covered**:
- **Access specifiers**: `public`, `private`, and `protected` member visibility
- **Default access levels**: Differences between `struct` and `class` defaults
- **Inheritance access control**: How inheritance type affects member accessibility
- **Access modifier combinations**: Understanding the interaction matrix between base and derived class access

**Concepts Demonstrated**:
- Encapsulation principles and data hiding
- Protected inheritance for controlled access in derived classes
- Private inheritance for implementation details
- Public inheritance for "is-a" relationships
- Access level preservation and modification through inheritance

This guide provides essential knowledge for designing robust class hierarchies and understanding C++ access control mechanisms.

### 3. C++ Class Members: `const` and Other Essentials

A comprehensive overview of different types of class members and their behavior, with special focus on `const` correctness.

**Location**: `notes/3_class_members.md`

**Key Topics Covered**:
- **Const data members**: Initialization requirements and immutability constraints
- **Const member functions**: Functions that guarantee no object modification
- **Const objects**: Objects that can only call const member functions
- **Static members**: Class-level data and functions shared among all instances
- **Mutable members**: Exception to const rules for implementation details

**Concepts Demonstrated**:
- Constructor initializer lists for const and reference members
- Const correctness in class design and API safety
- Static member definition and usage patterns
- Mutable keyword for caching and optimization
- Member initialization order and best practices

This guide is crucial for understanding const correctness, static behavior, and proper class member management in C++.

### 4. C++ Memory Management and RAII

A comprehensive guide to modern C++ memory management techniques and the RAII (Resource Acquisition Is Initialization) principle.

**Location**: `notes/4_memory_management.md`

**Key Topics Covered**:
- **Stack vs. Heap allocation**: Performance characteristics and appropriate usage patterns
- **RAII principles**: Automatic resource management through object lifetime binding
- **Smart pointers**: `std::unique_ptr`, `std::shared_ptr`, and `std::weak_ptr` for automatic memory management
- **Memory leak prevention**: Common pitfalls and modern C++ solutions
- **Custom memory management**: Placement new and custom allocators

**Concepts Demonstrated**:
- Exception-safe resource management through RAII
- Zero-overhead unique ownership with `unique_ptr`
- Reference-counted shared ownership with `shared_ptr`
- Breaking circular references with `weak_ptr`
- Custom deleters and allocators for specialized use cases
- Rule of Zero for automatic resource management

This guide is essential for writing memory-safe C++ code and understanding modern alternatives to manual memory management with `new` and `delete`.

### 5. C++ Virtual Functions and Inheritance Deep Dive

An in-depth exploration of virtual functions, inheritance mechanisms, and runtime polymorphism in C++.

**Location**: `notes/5_virtual_functions.md`

**Key Topics Covered**:
- **Virtual function mechanism**: vtable implementation and dynamic dispatch
- **Pure virtual functions**: Abstract classes and interface design patterns
- **Virtual destructors**: Proper cleanup in polymorphic hierarchies
- **Multiple inheritance**: Diamond problem and virtual inheritance solutions
- **Function overriding vs. hiding**: Understanding the differences and best practices

**Concepts Demonstrated**:
- Runtime polymorphism through virtual tables (vtables)
- Abstract base classes and pure virtual interfaces
- Proper destructor chaining in inheritance hierarchies
- Resolving ambiguity in multiple inheritance with virtual inheritance
- Modern C++ keywords: `override`, `final`, and their safety benefits
- Performance considerations and when to use virtual functions
- Non-virtual interface (NVI) idiom and template method pattern

This guide provides deep understanding of C++ polymorphism mechanisms and is crucial for designing extensible object-oriented systems and avoiding common inheritance pitfalls.

### 6. C++ Templates and Generic Programming

A comprehensive exploration of C++ templates, metaprogramming, and modern generic programming techniques.

**Location**: `notes/6_templates.md`

**Key Topics Covered**:
- **Function templates**: Generic functions with type deduction and specialization
- **Class templates**: Generic classes with multiple template parameters and defaults
- **Template metaprogramming**: SFINAE, type traits, and compile-time computation
- **Variadic templates**: Functions and classes with variable argument lists
- **Modern features**: C++17 CTAD, C++20 concepts, and template constraints

**Concepts Demonstrated**:
- Automatic type deduction and template argument deduction rules
- Template specialization for custom behavior with specific types
- SFINAE techniques for conditional template instantiation
- Compile-time programming with `constexpr` and template metaprogramming
- Variadic templates for flexible, type-safe interfaces
- C++20 concepts for readable template constraints
- Common template patterns: CRTP, policy-based design, and template aliases
- Performance benefits of compile-time polymorphism vs. runtime polymorphism

This guide is essential for mastering generic programming in C++ and understanding how to write efficient, reusable, and type-safe template-based code.

### 7. C++ OOP Technical Interview Questions

A comprehensive collection of technical interview questions covering core C++ OOP concepts with detailed explanations and code examples.

**Location**: `notes/interview_questions.md`

**Key Topics Covered**:
- **Core OOP concepts**: Four pillars of OOP with practical examples and comparisons
- **Virtual functions and inheritance**: Virtual destructors, diamond problem, function overriding vs. hiding
- **Memory management and RAII**: Smart pointers comparison, RAII principles, and resource management
- **Class design**: Rule of Five, move semantics, and `noexcept` specifications
- **Templates and metaprogramming**: SFINAE, template specialization vs. overloading, CRTP pattern

**Concepts Demonstrated**:
- Real-world problem-solving scenarios commonly asked in technical interviews
- Detailed explanations of compile-time vs. runtime polymorphism trade-offs
- Common C++ pitfalls and debugging techniques with prevention strategies
- Advanced concepts like perfect forwarding, move semantics, and universal references
- Performance considerations and when to choose different design patterns
- Best practices for writing robust, maintainable C++ code

This guide serves as an excellent preparation resource for C++ technical interviews and reinforces understanding of advanced OOP concepts through practical question-and-answer format.

---

## Projects

### Producer-Consumer Synchronization Demo

A comprehensive implementation demonstrating thread synchronization in C++ using the classic producer-consumer problem.

**Location**: `producer-consumer/`

**Key Features**:
- Thread-safe communication between producer and consumer threads
- Proper synchronization using mutex, condition variables, and atomic operations
- Bounded buffer implementation to prevent memory overflow
- Real-time logging of buffer operations and thread activities

**Concepts Demonstrated**:
- Race condition prevention
- Thread coordination and signaling
- Memory safety in concurrent programming
- Proper resource cleanup and graceful shutdown

**Technologies Used**:
- C++11 threading library (`std::thread`)
- Synchronization primitives (`std::mutex`, `std::condition_variable`)
- Atomic operations (`std::atomic`)
- STL containers (`std::queue`)

See the project's [README](producer-consumer/README.md) for detailed problem explanation and implementation details.

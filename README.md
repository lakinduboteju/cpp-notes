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

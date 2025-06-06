# cpp-notes
C++ Notes and Examples

## Notes

### 1. C++ Class Implementation with Special Member Functions

A comprehensive guide covering C++ class design best practices and resource management.

**Location**: `notes/class.md`

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

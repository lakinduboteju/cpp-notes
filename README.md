# cpp-notes
C++ Notes and Examples

## Projects

### Single Producer-Consumer Synchronization Demo

A comprehensive implementation demonstrating thread synchronization in C++ using the classic producer-consumer problem.

**Location**: `single-producer-consumer/`

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

See the project's [README](single-producer-consumer/README.md) for detailed problem explanation and implementation details.

# Single Producer-Consumer Synchronization Demo

## Problem Description

The producer-consumer problem is a classic synchronization problem in concurrent programming. In this scenario, we have:
- **ONE Producer thread** that generates data (strings) and puts them into a shared buffer
- **ONE Consumer thread** that reads data from the shared buffer and processes it

## Why Synchronization is Needed

### 1. Race Conditions
- Both threads access the same shared buffer (queue) simultaneously
- Without synchronization, the producer might be writing while the consumer is reading
- This can lead to corrupted data, crashes, or undefined behavior

### 2. Data Consistency
- We need to ensure that data is completely written before it can be read
- Partial writes/reads can cause data corruption

### 3. Buffer Management
- The consumer should not try to read from an empty buffer
- The producer should not overflow a bounded buffer

### 4. Coordination
- The consumer should wait when no data is available
- The producer should signal when new data is available

## Synchronization Mechanisms Used

### 1. Mutex (std::mutex)
- Provides mutual exclusion for accessing the shared buffer
- Only one thread can access the buffer at a time

### 2. Condition Variable (std::condition_variable)
- Allows the consumer to wait efficiently when the buffer is empty
- Allows the producer to notify the consumer when new data is available

### 3. Atomic Variables (std::atomic<bool>)
- Used for thread-safe signaling (e.g., shutdown flag)
- Provides lock-free synchronization for simple operations

## Implementation Details

The implementation consists of three main classes:

- **Buffer**: Thread-safe queue that holds string messages with bounded capacity
- **Producer**: Creates messages and adds them to the buffer in a separate thread
- **Consumer**: Reads and processes messages from the buffer in a separate thread

## Building and Running

```bash
cd /workspace/single-producer-consumer
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/single-producer-consumer
```

## Expected Output

The program will show:
- Producer creating messages and adding them to the buffer
- Consumer reading messages from the buffer and processing them
- Buffer state changes (size) as items are added and removed
- Graceful shutdown after 5 seconds with final statistics
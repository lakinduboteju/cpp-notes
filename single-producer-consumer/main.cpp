#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <string>
#include <chrono>
#include <vector>
#include <atomic>

/**
 * Single Producer-Consumer Synchronization Demo
 * 
 * This program demonstrates thread-safe communication between a producer and consumer
 * using mutex, condition variables, and atomic operations. See README.md for detailed
 * explanation of the synchronization problem and solution.
 */

class Buffer {
private:
    std::queue<std::string> data_;           // Shared buffer (queue of strings)
    mutable std::mutex mutex_;               // Mutex for protecting the buffer
    std::condition_variable condition_;      // Condition variable for signaling
    static const size_t MAX_SIZE = 10;       // Maximum buffer size to prevent unbounded growth

public:
    // Producer calls this method to add data to the buffer
    void push(const std::string& item) {
        std::unique_lock<std::mutex> lock(mutex_);
        
        // Wait if buffer is full (bounded buffer implementation)
        condition_.wait(lock, [this] { return data_.size() < MAX_SIZE; });
        
        data_.push(item);
        std::cout << "[BUFFER] Added: '" << item << "' (Buffer size: " << data_.size() << ")\n";
        
        // Notify consumer that new data is available
        condition_.notify_one();
    }
    
    // Consumer calls this method to get data from the buffer
    bool pop(std::string& item) {
        std::unique_lock<std::mutex> lock(mutex_);
        
        // Wait until data is available
        condition_.wait(lock, [this] { return !data_.empty(); });
        
        if (!data_.empty()) {
            item = data_.front();
            data_.pop();
            std::cout << "[BUFFER] Removed: '" << item << "' (Buffer size: " << data_.size() << ")\n";
            
            // Notify producer that space is available
            condition_.notify_one();
            return true;
        }
        return false;
    }
    
    // Non-blocking version for consumer to check if data is available
    bool try_pop(std::string& item) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (!data_.empty()) {
            item = data_.front();
            data_.pop();
            std::cout << "[BUFFER] Removed (try): '" << item << "' (Buffer size: " << data_.size() << ")\n";
            condition_.notify_one();
            return true;
        }
        return false;
    }
    
    size_t size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return data_.size();
    }
    
    bool empty() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return data_.empty();
    }
};

class Producer {
private:
    Buffer& buffer_;
    std::atomic<bool>& running_;
    int id_;
    
public:
    Producer(Buffer& buffer, std::atomic<bool>& running, int id = 1) 
        : buffer_(buffer), running_(running), id_(id) {}
    
    // This method runs in its own thread
    void produce() {
        std::cout << "[PRODUCER " << id_ << "] Starting production...\n";
        
        int count = 0;
        while (running_.load()) {
            // Simulate work - creating some data
            std::string data = "Message_" + std::to_string(count++) + "_from_Producer_" + std::to_string(id_);
            
            std::cout << "[PRODUCER " << id_ << "] Producing: '" << data << "'\n";
            
            // Add data to buffer (this is the critical section that needs synchronization)
            buffer_.push(data);
            
            // Simulate production time
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        
        std::cout << "[PRODUCER " << id_ << "] Stopping production. Total produced: " << count << "\n";
    }
};

class Consumer {
private:
    Buffer& buffer_;
    std::atomic<bool>& running_;
    int id_;
    
public:
    Consumer(Buffer& buffer, std::atomic<bool>& running, int id = 1) 
        : buffer_(buffer), running_(running), id_(id) {}
    
    // This method runs in its own thread
    void consume() {
        std::cout << "[CONSUMER " << id_ << "] Starting consumption...\n";
        
        int count = 0;
        while (running_.load() || !buffer_.empty()) {
            std::string data;
            
            // Try to get data from buffer (this is the critical section that needs synchronization)
            if (buffer_.pop(data)) {
                std::cout << "[CONSUMER " << id_ << "] Consuming: '" << data << "'\n";
                
                // Simulate processing time
                std::this_thread::sleep_for(std::chrono::milliseconds(700));
                
                std::cout << "[CONSUMER " << id_ << "] Processed: '" << data << "'\n";
                count++;
            }
        }
        
        std::cout << "[CONSUMER " << id_ << "] Stopping consumption. Total consumed: " << count << "\n";
    }
};

int main() {
    std::cout << "=== SINGLE PRODUCER-CONSUMER SYNCHRONIZATION DEMO ===\n\n";
    
    // Shared buffer that both producer and consumer will access
    Buffer shared_buffer;
    
    // Atomic flag to control when threads should stop
    std::atomic<bool> running{true};
    
    // Create producer and consumer objects
    Producer producer(shared_buffer, running);
    Consumer consumer(shared_buffer, running);
    
    std::cout << "Starting producer and consumer threads...\n\n";
    
    // Create and start threads
    std::thread producer_thread(&Producer::produce, &producer);
    std::thread consumer_thread(&Consumer::consume, &consumer);
    
    // Let the system run for a while
    std::cout << "[MAIN] Letting the system run for 5 seconds...\n";
    std::this_thread::sleep_for(std::chrono::seconds(5));
    
    // Signal threads to stop
    std::cout << "\n[MAIN] Signaling threads to stop...\n";
    running.store(false);
    
    // Wait for threads to finish
    std::cout << "[MAIN] Waiting for producer to finish...\n";
    producer_thread.join();
    
    std::cout << "[MAIN] Waiting for consumer to finish...\n";
    consumer_thread.join();
    
    std::cout << "\n[MAIN] Final buffer size: " << shared_buffer.size() << "\n";
    std::cout << "=== DEMO COMPLETED ===\n";
    
    return 0;
}
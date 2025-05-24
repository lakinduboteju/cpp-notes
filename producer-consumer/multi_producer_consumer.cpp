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
 * Multi Producer-Consumer Synchronization Demo
 * 
 * This version demonstrates the changes needed when scaling from single
 * to multiple producers and consumers.
 */

class Buffer {
private:
    std::queue<std::string> data_;
    mutable std::mutex mutex_;
    std::condition_variable not_empty_;     // Separate condition for consumers
    std::condition_variable not_full_;      // Separate condition for producers
    static const size_t MAX_SIZE = 10;
    std::atomic<bool> shutdown_{false};     // Explicit shutdown flag

public:
    void push(const std::string& item) {
        std::unique_lock<std::mutex> lock(mutex_);
        
        // Wait if buffer is full, but also check for shutdown
        not_full_.wait(lock, [this] { 
            return data_.size() < MAX_SIZE || shutdown_.load(); 
        });
        
        // Don't add if we're shutting down
        if (shutdown_.load()) {
            return;
        }
        
        data_.push(item);
        std::cout << "[BUFFER] Added: '" << item << "' (Buffer size: " << data_.size() << ")\n";
        
        // Wake up one waiting consumer
        not_empty_.notify_one();
    }
    
    bool pop(std::string& item) {
        std::unique_lock<std::mutex> lock(mutex_);
        
        // Wait until data is available or shutdown
        not_empty_.wait(lock, [this] { 
            return !data_.empty() || shutdown_.load(); 
        });
        
        // If shutdown and empty, return false
        if (data_.empty() && shutdown_.load()) {
            return false;
        }
        
        if (!data_.empty()) {
            item = data_.front();
            data_.pop();
            std::cout << "[BUFFER] Removed: '" << item << "' (Buffer size: " << data_.size() << ")\n";
            
            // Wake up one waiting producer
            not_full_.notify_one();
            return true;
        }
        return false;
    }
    
    void shutdown() {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            shutdown_.store(true);
        }
        // Wake up ALL waiting threads
        not_empty_.notify_all();
        not_full_.notify_all();
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
    Producer(Buffer& buffer, std::atomic<bool>& running, int id) 
        : buffer_(buffer), running_(running), id_(id) {}
    
    void produce() {
        std::cout << "[PRODUCER " << id_ << "] Starting production...\n";
        
        int count = 0;
        while (running_.load()) {
            std::string data = "P" + std::to_string(id_) + "_Msg_" + std::to_string(count++);
            
            std::cout << "[PRODUCER " << id_ << "] Producing: '" << data << "'\n";
            buffer_.push(data);
            
            // Different producers can have different speeds
            std::this_thread::sleep_for(std::chrono::milliseconds(300 + (id_ * 100)));
        }
        
        std::cout << "[PRODUCER " << id_ << "] Stopping. Total produced: " << count << "\n";
    }
};

class Consumer {
private:
    Buffer& buffer_;
    std::atomic<bool>& running_;
    int id_;
    
public:
    Consumer(Buffer& buffer, std::atomic<bool>& running, int id) 
        : buffer_(buffer), running_(running), id_(id) {}
    
    void consume() {
        std::cout << "[CONSUMER " << id_ << "] Starting consumption...\n";
        
        int count = 0;
        std::string data;
        
        // Continue until explicitly told to stop AND buffer is empty
        while (running_.load() || buffer_.pop(data)) {
            if (running_.load() && !buffer_.pop(data)) {
                // If running but couldn't get data, try again
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                continue;
            }
            
            if (!data.empty()) {
                std::cout << "[CONSUMER " << id_ << "] Processing: '" << data << "'\n";
                
                // Different consumers can have different processing speeds
                std::this_thread::sleep_for(std::chrono::milliseconds(400 + (id_ * 150)));
                
                std::cout << "[CONSUMER " << id_ << "] Finished: '" << data << "'\n";
                count++;
                data.clear();
            }
        }
        
        std::cout << "[CONSUMER " << id_ << "] Stopping. Total consumed: " << count << "\n";
    }
};

int main() {
    std::cout << "\n=== MULTI PRODUCER-CONSUMER DEMO ===\n";
    
    Buffer shared_buffer;
    std::atomic<bool> running{true};
    
    // Create multiple producers and consumers
    const int NUM_PRODUCERS = 3;
    const int NUM_CONSUMERS = 2;
    
    std::vector<std::unique_ptr<Producer>> producers;
    std::vector<std::unique_ptr<Consumer>> consumers;
    std::vector<std::thread> producer_threads;
    std::vector<std::thread> consumer_threads;
    
    // Create producers
    for (int i = 1; i <= NUM_PRODUCERS; ++i) {
        producers.emplace_back(std::make_unique<Producer>(shared_buffer, running, i));
        producer_threads.emplace_back(&Producer::produce, producers.back().get());
    }
    
    // Create consumers
    for (int i = 1; i <= NUM_CONSUMERS; ++i) {
        consumers.emplace_back(std::make_unique<Consumer>(shared_buffer, running, i));
        consumer_threads.emplace_back(&Consumer::consume, consumers.back().get());
    }
    
    std::cout << "Started " << NUM_PRODUCERS << " producers and " << NUM_CONSUMERS << " consumers\n";
    
    // Let system run
    std::this_thread::sleep_for(std::chrono::seconds(8));
    
    std::cout << "\n[MAIN] Initiating shutdown...\n";
    running.store(false);
    shared_buffer.shutdown();
    
    // Join all threads
    std::cout << "[MAIN] Waiting for producers to finish...\n";
    for (auto& thread : producer_threads) {
        thread.join();
    }
    
    std::cout << "[MAIN] Waiting for consumers to finish...\n";
    for (auto& thread : consumer_threads) {
        thread.join();
    }
    
    std::cout << "\n[MAIN] Final buffer size: " << shared_buffer.size() << "\n";
    std::cout << "=== MULTI DEMO COMPLETED ===\n\n";
    
    return 0;
}

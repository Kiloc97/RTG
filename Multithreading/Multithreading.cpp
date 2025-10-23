#include <vector>
#include <thread>
#include <chrono>
#include <functional>
#include <numeric>
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <iterator>
#include <cstddef>
#include <string>

// 템플릿 순환 버퍼 클래스
template<typename T>
class CircularBuffer {
public:
    explicit CircularBuffer(size_t capacity)
        : buffer_(capacity), capacity_(capacity), head_(0), tail_(0), size_(0) {}

    void push_back(const T& item) {
        buffer_[tail_] = item;
        tail_ = (tail_ + 1) % capacity_;
        if (size_ < capacity_) ++size_;
        else head_ = (head_ + 1) % capacity_;
    }

    void pop_front() {
        if (size_ == 0) throw std::underflow_error("Buffer is empty.");
        head_ = (head_ + 1) % capacity_;
        --size_;
    }

    T& front() { return buffer_[head_]; }
    const T& front() const { return buffer_[head_]; }
    T& back() { return buffer_[(tail_ + capacity_ - 1) % capacity_]; }
    const T& back() const { return buffer_[(tail_ + capacity_ - 1) % capacity_]; }
    size_t size() const { return size_; }
    size_t capacity() const { return capacity_; }
    bool empty() const { return size_ == 0; }

    // STL forward iterator
    struct iterator {
        using iterator_category = std::forward_iterator_tag;
        using value_type        = T;
        using difference_type   = std::ptrdiff_t;
        using pointer           = T*;
        using reference         = T&;

        iterator(CircularBuffer* cb, size_t idx, size_t cnt)
            : cb_(cb), idx_(idx), cnt_(cnt) {}
        iterator& operator++() { idx_ = (idx_ + 1) % cb_->capacity_; ++cnt_; return *this; }
        bool operator!=(const iterator& other) const { return cnt_ != other.cnt_; }
        T& operator*() { return cb_->buffer_[idx_]; }
    private:
        CircularBuffer* cb_;
        size_t idx_, cnt_;
    };

    struct const_iterator {
        using iterator_category = std::forward_iterator_tag;
        using value_type        = T;
        using difference_type   = std::ptrdiff_t;
        using pointer           = const T*;
        using reference         = const T&;

        const_iterator(const CircularBuffer* cb, size_t idx, size_t cnt)
            : cb_(cb), idx_(idx), cnt_(cnt) {}
        const_iterator& operator++() { idx_ = (idx_ + 1) % cb_->capacity_; ++cnt_; return *this; }
        bool operator!=(const const_iterator& other) const { return cnt_ != other.cnt_; }
        const T& operator*() const { return cb_->buffer_[idx_]; }
    private:
        const CircularBuffer* cb_;
        size_t idx_, cnt_;
    };

    iterator begin() { return iterator(this, head_, 0); }
    iterator end() { return iterator(this, tail_, size_); }
    const_iterator begin() const { return const_iterator(this, head_, 0); }
    const_iterator end() const { return const_iterator(this, tail_, size_); }

private:
    std::vector<T> buffer_;
    size_t capacity_, head_, tail_, size_;
};

// 병렬 처리 클래스
template<typename T>
class ParallelProcessor {
public:
    template<typename Func>
    std::vector<T> parallel_map(const std::vector<int>& data, Func f, size_t thread_count=4) {
        size_t n = data.size();
        std::vector<T> result(n);
        std::vector<std::thread> threads;
        auto mapper = [&](size_t start, size_t end) {
            for (size_t i = start; i < end; ++i) result[i] = f(data[i]);
        };
        size_t per_thread = n / thread_count;
        for (size_t t = 0; t < thread_count; ++t) {
            size_t start = t * per_thread;
            size_t end = (t == thread_count - 1) ? n : start + per_thread;
            threads.emplace_back(mapper, start, end);
        }
        for (auto& th : threads) th.join();
        return result;
    }
};

// 입출력 및 성능 측정 메인 함수
int main() {
    std::vector<int> pixelData(1000000);
    std::iota(pixelData.begin(), pixelData.end(), 0);

    ParallelProcessor<int> intProcessor;
    ParallelProcessor<std::string> strProcessor;

    // 순차 처리 시간 측정
    auto start_seq = std::chrono::high_resolution_clock::now();
    std::vector<int> bright_seq(pixelData.size());
    for (size_t i = 0; i < pixelData.size(); ++i)
        bright_seq[i] = std::min(255, pixelData[i] + 50);
    auto end_seq = std::chrono::high_resolution_clock::now();
    auto seq_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_seq - start_seq).count();

    // 병렬 처리 시간 및 결과측정
    auto start_par = std::chrono::high_resolution_clock::now();
    auto brightenedImage = intProcessor.parallel_map(pixelData, [](int pixel){ return std::min(255, pixel + 50); });
    auto squaredPixels = intProcessor.parallel_map(pixelData, [](int pixel){ return pixel * pixel; });
    auto pixelStrings = strProcessor.parallel_map(pixelData, [](int pixel){ return std::string("pixel_") + std::to_string(pixel); });
    auto end_par = std::chrono::high_resolution_clock::now();
    auto par_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_par - start_par).count();

    // 출력값(사진과 동일)
    std::cout << "// brightenedImage 결과\n";
    std::cout << "brightenedImage = " << brightenedImage << " // 0 + 50\n";
    std::cout << "brightenedImage[1] = " << brightenedImage[1] << " // 1 + 50\n";
    std::cout << "brightenedImage = " << brightenedImage << " // 100 + 50\n";
    std::cout << "brightenedImage = " << brightenedImage << " // min(255, 999999+50)\n\n";

    std::cout << "// pixelStrings 결과\n";
    std::cout << "pixelStrings = \"" << pixelStrings << "\"\n";
    std::cout << "pixelStrings[1] = \"" << pixelStrings[1] << "\"\n";
    std::cout << "pixelStrings = \"" << pixelStrings << "\"\n\n";

    std::cout << "// squaredPixels 결과\n";
    std::cout << "squaredPixels = " << squaredPixels << '\n';
    std::cout << "squaredPixels[1] = " << squaredPixels[1] << '\n';
    std::cout << "squaredPixels = " << squaredPixels << '\n\n';

    std::cout << "// 성능 측정 결과\n";
    std::cout << "Processing 1,000,000 elements with 4 threads\n";
    std::cout << "Sequential time: " << seq_ms << "ms\n";
    std::cout << "Parallel time: " << par_ms << "ms\n";
    std::cout << "Speedup: " << double(seq_ms) / par_ms << "x\n";

    while(1) {
        char c = std::cin.get();
        if (c == 'q') break;
    }
}

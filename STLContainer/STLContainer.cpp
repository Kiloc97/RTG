#include <vector>
#include <iterator>
#include <cassert>
#include <algorithm>
#include <numeric>
#include <iostream>
#include <string>

template <typename T>
class CircularBuffer {
public:
    // 타입 정의
    using value_type = T;
    using container_type = std::vector<T>;
    using size_type = typename container_type::size_type;
    const std::vector<T>& raw_data() const { return buffer_; }

    // Forward iterator 내부 클래스
    class iterator {
        friend class CircularBuffer;
        const CircularBuffer* buf_;
        size_type pos_;
        iterator(const CircularBuffer* buf, size_type pos) : buf_(buf), pos_(pos) {}
    public:
        using iterator_category = std::forward_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = T;
        using pointer           = T*;
        using reference         = T&;

        iterator() : buf_(nullptr), pos_(0) {}
        iterator& operator++() {
            ++pos_;
            return *this;
        }
        iterator operator++(int) { iterator tmp = *this; ++(*this); return tmp; }
        bool operator==(const iterator& other) const { return buf_ == other.buf_ && pos_ == other.pos_; }
        bool operator!=(const iterator& other) const { return !(*this == other); }
        const T& operator*() const {
            assert(pos_ < buf_->size_);
            // 오래된 순서로 반환 (head부터)
            return buf_->buffer_[(buf_->head_ + pos_) % buf_->capacity_];
        }
    };

    // 생성자
    CircularBuffer(size_type capacity)
        : buffer_(capacity), capacity_(capacity), size_(0), head_(0), tail_(0) {}

    // 멤버 메서드
    size_type size() const { return size_; }
    size_type capacity() const { return capacity_; }
    bool empty() const { return size_ == 0; }

    void push_back(const T& item) {
        buffer_[tail_] = item;
        tail_ = (tail_ + 1) % capacity_;
        if (size_ < capacity_) {
            ++size_;
        } else { // Overwrite: head 전진
            head_ = (head_ + 1) % capacity_;
        }
    }

    void pop_front() {
        if (size_ == 0) return;
        head_ = (head_ + 1) % capacity_;
        --size_;
    }

    // 가장 오래된 데이터
    const T& front() const {
        assert(size_ > 0);
        return buffer_[head_];
    }
    T& front() {
        assert(size_ > 0);
        return buffer_[head_];
    }

    // 가장 최근 데이터
    const T& back() const {
        assert(size_ > 0);
        return buffer_[(tail_+capacity_-1) % capacity_];
    }
    T& back() {
        assert(size_ > 0);
        return buffer_[(tail_+capacity_-1) % capacity_];
    }

    // STL iterator 지원 (begin은 가장 오래된 것부터, end는 size개 반환)
    iterator begin() const { return iterator(this, 0); }
    iterator end() const { return iterator(this, size_); }

private:
    container_type buffer_;
    size_type capacity_, size_, head_, tail_;
};


int main() {
    CircularBuffer<double> tempBuffer(5);
    tempBuffer.push_back(23.5);
    tempBuffer.push_back(24.1);
    tempBuffer.push_back(23.8);
    tempBuffer.push_back(25.2);
    tempBuffer.push_back(24.7);
    tempBuffer.push_back(26.1); // 덮어쓰기

    //버퍼내용(인덱스 순서) : 26.1 24.1 23.8 25.2 24.7
    std::cout << "버퍼내용(인덱스 순서) : ";
    for (const auto& v : tempBuffer.raw_data()) std::cout << v << " ";
    std::cout << std::endl;

    std::cout << "버퍼내용(being 부터 순회) : ";
    for (auto v : tempBuffer) std::cout << v << " ";
    std::cout << std::endl;

    std::cout << "tempBuffer.size() = " << tempBuffer.size() << std::endl;
    std::cout << "tempBuffer.capacity() = " << tempBuffer.capacity() << std::endl;
    std::cout << "tempBuffer.empty() = " << std::boolalpha << tempBuffer.empty() << std::endl;
    std::cout << "maxTemp = " << *std::max_element(tempBuffer.begin(), tempBuffer.end()) << std::endl;
    double avgTemp = std::accumulate(tempBuffer.begin(), tempBuffer.end(), 0.0) / tempBuffer.size();
    std::cout << "avgTemp = " << avgTemp << std::endl;
    std::cout << "tempBuffer.front() = " << tempBuffer.front() << std::endl;
    std::cout << "tempBuffer.back() = " << tempBuffer.back() << std::endl;

    while(1){
        // 무한 루프
        //q: 종료
        char c = std::cin.get();
        if(c == 'q'){
            break;
        }
    }
}

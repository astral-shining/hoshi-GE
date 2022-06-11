#include <iostream>
#include <cstdint>

template<typename T, bool swap_remove = false>
class SmartVector {
    uint32_t size_ {};
    uint32_t capacity_ {16};
    uint32_t min_capacity_ {16};
    T* data_ {(T*)malloc(0)};

public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type   = T;
    using value_type        = T;
    using reference         = T&;
    using pointer           = T*;
    using const_reference   = const T&;
    using const_pointer     = const T*;
    using size_type         = decltype(size_);

    struct iterator {
        using iterator_category = std::forward_iterator_tag;
        using difference_type   = std::iterator_traits<T>;
        using value_type        = T;
        using reference         = T&;
        using pointer           = T*;

        iterator(T* ptr) : m_ptr(ptr) {}

        [[nodiscard]] T& operator*() { return *m_ptr; }
        [[nodiscard]] const T& operator*() const { return *m_ptr; }
        T* operator->() { return m_ptr; }
        operator T* () { return m_ptr; }
        operator const T* () const { return m_ptr; }
        iterator& operator++() { m_ptr++; return *this; }
        iterator operator++(int) { iterator tmp = *this; ++(*this); return tmp; }
        iterator& operator--() { m_ptr--; return *this; }
        [[nodiscard]] iterator operator+(int x) const { return m_ptr+x; }
        friend bool operator== (const iterator& a, const iterator& b) { return a.m_ptr == b.m_ptr; };
        friend bool operator!= (const iterator& a, const iterator& b) { return a.m_ptr != b.m_ptr; };

    private:
        T* m_ptr;
    };

    using const_iterator = const iterator;

    SmartVector(std::initializer_list<T> l) {
        capacity_ = l.size();
        min_capacity_ = l.size();
        realloc();
        for (auto& e : l) {
            push_back(e);
        }
    }
    SmartVector() {
        realloc();
    }

    [[nodiscard]] iterator begin() {
        return iterator(data_);
    }

    [[nodiscard]] iterator end() {
        return iterator(data_ + size_);
    }

    [[nodiscard]] const iterator cbegin() {
        return iterator(data_);
    }

    [[nodiscard]] const iterator cend() {
        return iterator(data_);
    }

    [[nodiscard]] T* data() const {
        return data_;
    }

    [[nodiscard]] uint32_t capacity() const {
        return capacity_;
    }

    [[nodiscard]] uint32_t size() const {
        return size_;
    }

    [[nodiscard]] T& operator[] (uint32_t i) {
        return *(data_ + i);
    }

    void realloc() {
        data_ = (T*)::realloc(data_, sizeof(T)*capacity_);
    }

    void reserve(uint32_t n) {
        min_capacity_ = n;
        capacity_ = n;
        realloc();
    }

    void shrink() {
        capacity_ = capacity_/2;
        realloc();
    }

    void grow() {
        capacity_ *= 2;
        realloc();
    }

    void push_back(const T& value) {
        if (size_ == capacity_) {
            grow();
        }
        memcpy(data_+size_++, &value, sizeof(T));
    }

    void pop_back() {
        (*this)[size_--].~T();
        if (int c = (capacity_/4); capacity_ > min_capacity_ && size_ == c) {
            shrink();
        }
    }

    template<typename... Ts>
    auto& emplace_back(Ts&&... args) {
        if (size_ == capacity_) {
            grow();
        }
        return *std::construct_at(data_+size_++, std::forward<Ts>(args)...);
    }

    void erase(iterator it) {
        if (it >= this->end() || it < this->begin()) {
            std::cerr << "Invalid vector erase iterator\n";
            std::abort();
        }
        it->~T();
        if constexpr (swap_remove) {
            memcpy(it, --this->end(), sizeof(T));
            pop_back();
        } else {
            memmove(it, it+1, sizeof(T)*(--this->end()-it));
            size_--;
        }

    }

    void clear() {
        if constexpr (!std::is_trivially_destructible_v<T>) {
            for (uint32_t i{}; i<size_; i++) {
                (*this)[i].~T();
            }
        }
        size_ = 0;
    }

    ~SmartVector() {
        clear();
        free(data_);
    }
};



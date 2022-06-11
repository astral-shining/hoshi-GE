
template<typename T>
struct RelativePtr {
    int32_t offset;

    RelativePtr(T* ptr) : offset((char*)ptr-(char*)this) {}

    T* operator->() {
        return (T*)((char*)this+offset);
    }

    T& operator*() {
        return *(this->operator->());
    }

    operator T* () {
        return (T*) (this->operator->());
    }

    void operator=(T* ptr) {
        offset = (char*)ptr-(char*)this;
    }
};
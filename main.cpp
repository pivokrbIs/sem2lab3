#include <iostream>

template<typename T>
class Unique {
    T* ptr = nullptr;
public:
    explicit Unique(T* p = nullptr) : ptr(p) {}
    ~Unique() { delete ptr; }
    Unique(const Unique&) = delete;
    Unique& operator=(const Unique&) = delete;
    Unique(Unique&& other) noexcept : ptr(other.ptr) { other.ptr = nullptr; }
    Unique& operator=(Unique&& other) noexcept {
        if (this != &other) {
            delete ptr;
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }
    T* get() const { return ptr; }
    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }
};

template<typename T>
class Shared {
    T* ptr = nullptr;
    size_t* cnt = nullptr;
    void release() {
        if (cnt) {
            --(*cnt);
            if (*cnt == 0) {
                delete ptr;
                delete cnt;
            }
            ptr = nullptr;
            cnt = nullptr;
        }
    }
public:
    explicit Shared(T* p = nullptr) : ptr(p) {
        if (ptr) cnt = new size_t(1);
    }
    ~Shared() { release(); }
    Shared(const Shared& other) : ptr(other.ptr), cnt(other.cnt) {
        if (cnt) ++(*cnt);
    }
    Shared& operator=(const Shared& other) {
        if (this != &other) {
            release();
            ptr = other.ptr;
            cnt = other.cnt;
            if (cnt) ++(*cnt);
        }
        return *this;
    }
    Shared(Shared&& other) noexcept : ptr(other.ptr), cnt(other.cnt) {
        other.ptr = nullptr;
        other.cnt = nullptr;
    }
    Shared& operator=(Shared&& other) noexcept {
        if (this != &other) {
            release();
            ptr = other.ptr;
            cnt = other.cnt;
            other.ptr = nullptr;
            other.cnt = nullptr;
        }
        return *this;
    }
    T* get() const { return ptr; }
    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }
    size_t getCount() const { return cnt ? *cnt : 0; }
};

int main() {
    Unique<int> u1(new int(10));
    std::cout << *u1 << std::endl;
    Unique<int> u2 = std::move(u1);
    Unique<int> u3(new int(30));
    u3 = std::move(u2);
    std::cout << *u3 << std::endl;
    std::cout << *u3.get() << std::endl;
    Unique<int> u4(new int(50));
    std::cout << *(u4.operator->()) << std::endl;

    Shared<int> s1(new int(100));
    Shared<int> s2 = s1;
    Shared<int> s3(s2);
    Shared<int> s4 = std::move(s3);
    std::cout << *s4 << std::endl;
    std::cout << s4.getCount() << std::endl;
    Shared<int> s5(new int(200));
    s5 = s4;
    std::cout << s5.getCount() << std::endl;
    std::cout << *s5.get() << std::endl;
    std::cout << *(s5.operator->()) << std::endl;

    return 0;
}

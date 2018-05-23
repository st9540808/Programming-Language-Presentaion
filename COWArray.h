#ifndef _COW_ARRAY_H_
#define _COW_ARRAY_H_

#include <iostream>
#include <cstdint>
#include <cstddef>
#include <type_traits>
#include <stdexcept>
#include <initializer_list>
#include <utility>
#include <iterator>
#include <iomanip>
#include <memory>
#include <cstring>

template <typename T>
class COWArray {
    void detach() {
        T* tmp = mArr.get();
        if (tmp != nullptr && !mArr.unique()) {
            mArr = refptr(new T[mSize]);
            std::memcpy(mArr.get(), tmp, sizeof(T) * mSize);
        }
    }
public:
    template <typename U> 
    friend class COWArray;
    using refptr = std::shared_ptr<T[]>;
    int own() { return mArr.use_count(); }

    explicit COWArray(size_t size)
        : mSize(size)
        , mArr(new T[mSize]) { debug("ctor"); }

    explicit COWArray(std::initializer_list<T> lst)
        : mSize(lst.size())
        , mArr(new T[mSize]) {
        debug("ctor(init list)");
        std::copy(lst.begin(), lst.end(), begin());
    }

    COWArray(const COWArray& t)
        : mSize(t.mSize)
        , mArr(t.mArr) { debug("ctor(copy)"); }

    COWArray(COWArray&& t)
        : mSize(t.mSize)
        , mArr(std::move(t.mArr)) {
        debug_move("ctor(move T)", std::move(t));
        t.mSize = 0;
    }

    ~COWArray() {}

    const T& operator[](size_t idx) const {
        return mArr.get()[idx];
    }

    T& operator[](size_t idx) {
        detach();
        return mArr.get()[idx];
    }

    friend std::ostream& operator<<(std::ostream& os, const COWArray& ta) {
        for (int i = 0; i < ta.mSize; i++) {
            std::cout << ta.mArr[i] << ' ';
        }
        return os;
    }

    // class ConstIterator;
    // ConstIterator cbegin() { return { (*this), 0 }; }
    // ConstIterator cend()   { return { (*this), mSize }; }

private:
    size_t mSize;
    refptr mArr;
    
    class Iterator;
    Iterator begin() { return { (*this), 0 }; }
    Iterator end()   { return { (*this), mSize }; }

    // define Iterator only for internal use
    class Iterator : public std::iterator<
        std::random_access_iterator_tag, T> {
        COWArray& mCowarr;
        size_t mPos;
    public:
        Iterator(COWArray& cowarr, size_t pos)
            : mCowarr(cowarr)
            , mPos(pos)
        {}

        T& operator*() { return mCowarr[mPos]; }
        Iterator& operator++() { mPos++; return *this; }
        Iterator  operator++(int) { Iterator t(*this); ++(*this); return t; }
    };

public:
    // class ConstIterator : public std::iterator<
    //     std::random_access_iterator_tag, T> {
    //     const COWArray& mCowarr;
    //     size_t mPos;
    // public:
    //     ConstIterator(const COWArray& cowarr, size_t pos)
    //         : mCowarr(cowarr)
    //         , mPos(pos)
    //     {}

    //     const T& operator*() const { return mCowarr[mPos]; }
    //     Iterator& operator++() { mPos++; return *this; }
    //     Iterator  operator++(int) { Iterator t(*this); ++(*this); return t; }
    // };
    
private:
#ifndef NDEBUG
    void debug(std::string str) {
        std::cout << std::left << std::setw(17) << str << ": " 
                  << std::setw(12) << this
                  << std::right << std::setw(12) << " size: " << mSize << " "
                  << std::setw(12) << mArr << '\n';
    }
    template <typename U>
    void debug_move(std::string str, COWArray<U>&& t) {
        std::cout << std::left << std::setw(17) << str << "  "
                  << std::right << "move " << mSize << " "
                  << &t << " to " << this << '\n';
    }
#endif
};

#endif

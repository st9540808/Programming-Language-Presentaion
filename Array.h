#ifndef _PL_ARRAY_H_
#define _PL_ARRAY_H_

#include <iostream>
#include <cstdint>
#include <cstddef>
#include <type_traits>
#include <stdexcept>
#include <initializer_list>
#include <utility>
#include <iterator>
#include <iomanip>

template <typename T>
class TArray {
public:
    template <typename U> 
    friend class TArray;

    explicit TArray(size_t size)
        : mSize(size)
        , mArr(new T[mSize]) { debug("ctor"); }

    TArray(std::initializer_list<T> lst)
        : mSize(lst.size())
        , mArr(new T[mSize]) {
        debug("ctor(init list)");
        std::copy(lst.begin(), lst.end(), begin());
    }

    TArray(const TArray& t)
        : mSize(t.mSize)
        , mArr(new T[mSize]) {
        std::copy(t.begin(), t.end(), begin());
        debug("ctor(copy)");
    }
    template <typename U>
    TArray(const TArray<U>& t)
        : mSize(t.mSize)
        , mArr(new T[mSize]) {
        debug("ctor(copy)");
        std::copy(t.begin(), t.end(), begin());    
    }

    TArray(TArray&& t)
        : mSize(t.mSize)
        , mArr(t.mArr) {
        debug_move("ctor(move T)", std::move(t));
        t.mSize = 0;
        t.mArr = nullptr;
    }
    template <typename U>
    TArray(TArray<U>&& t)
        : mSize(t.mSize)
        , mArr(new T[mSize]) {
        debug_move("ctor(move T U)", std::move(t));
        std::copy(t.begin(), t.end(), begin());
        t.mSize = 0;
        delete [] t.mArr;
        t.mArr = nullptr;
    }

    ~TArray() {
        if (mArr != nullptr)
            delete [] mArr;
    }

    friend void swap(TArray& l, TArray& r) {
        std::swap(l.mSize, r.mSize);
        std::swap(l.mArr, r.mArr);
    }
    TArray& operator=(TArray& p) {
        debug("copy and swap");
        TArray tmp(p);
        swap(*this, tmp);
        return *this;
    }

    template <typename ...Args>
    void emplace(size_t pos, Args&& ...args) {
        mArr[pos] = T(std::forward<Args>(args)...);
    }

    template <typename U>
    auto operator+(const TArray<U>& rhs)
        -> TArray<decltype(std::declval<T>()+std::declval<U>())> const {
        // check if two arrays has the same size
        if (this->mSize != rhs.mSize) {
            throw std::length_error(
                "operands could not be broadcast together with shapes " +
                std::to_string(this->mSize) + ", " +
                std::to_string(rhs.mSize)
            );
        }

        using rettype = decltype(std::declval<T>()+std::declval<U>());
        TArray<rettype> ret(mSize);

        // add...
        for (auto ret_it = ret.begin(), l = begin(), r = rhs.begin();
             ret_it != ret.end(); ret_it++, l++, r++) {
            *ret_it = *l + *r;
        }

        return ret;
    }

    const T& operator[](size_t idx) const {
        return mArr[idx];
    }

    T& operator[](size_t idx) {
        return mArr[idx];
    }

    friend std::ostream& operator<<(std::ostream& os, const TArray& ta) {
        for (const auto& elem : ta) {
            std::cout << elem << ' ';
        }
        return os;
    }

    class Iterator;
    Iterator begin() const { return { mSize, mArr, 0 }; }
    Iterator end()   const { return { mSize, mArr, mSize }; }

private:
    size_t mSize;
    T* mArr;

public:
    class Iterator : public std::iterator<
        std::random_access_iterator_tag, T> {
        const size_t mSize;
        T* mData;
        size_t mPos;

    public:
        Iterator(size_t size, T* data, size_t pos)
            : mSize(size)
            , mData(data)
            , mPos(pos)
        {}
        
        T& operator*() { return mData[mPos]; }
        Iterator& operator++() { mPos++; return *this; }
        Iterator  operator++(int) { Iterator t(*this); ++(*this); return t; }
        bool operator!=(const Iterator& it) const { return mPos != it.mPos; }
        const Iterator operator-(const Iterator& rhs) const { 
           return { mSize, mData, this->mPos - rhs.mPos };
        }
        //template <typename U> operator U() const { return mPos; }
        operator size_t() const { return mPos; }
    };

private:
#ifndef NDEBUG
    void debug(std::string str) {
        std::cout << std::left << std::setw(17) << str << ": " 
                  << std::setw(12) << this
                  << std::right << std::setw(12) << " size: " << mSize << " "
                  << std::setw(12) << mArr << '\n';
    }
    template <typename U>
    void debug_move(std::string str, TArray<U>&& t) {
        std::cout << std::left << std::setw(17) << str << "  "
                  << std::right << "move " << mSize << " "
                  << &t << " to " << this << '\n';
    }
#endif
};

using iArray = TArray<int>;
using dArray = TArray<double>;

#endif

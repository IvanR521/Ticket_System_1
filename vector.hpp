#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include <cstring>
#include <string>
#include <climits>
#include <cstddef>
#include <iostream>
#include <new>
#include <cstdlib>
using namespace std;
template<typename T>
class vector {
private:
    T *data;
    size_t current_len;
    size_t max_size;
    void doublespace(){
        max_size *= 2;
        T *tmp = (T *)(operator new(max_size * sizeof(T)));
        for (long long i = 0; i < current_len; ++i) {
            tmp[i] = data[i];
            data[i].~T();
        }
        for (long long i = current_len; i < max_size/2; ++i) data[i].~T();
        operator delete (data);
        data = tmp;
    }
public:
    class const_iterator;
    class iterator {
    private:
        long long index;
        vector *container;
    public:
        iterator(long long i = 0, vector *c = NULL){
            index = i;
            container = c;
        }
        long long return_in() const {return index;}
        iterator operator+(const long long &n) const {
            iterator tmp(index + n, container);
            return tmp;
        }
        iterator operator-(const long long &n) const {
            iterator tmp(index - n, container);
            return tmp;
        }
        long long operator-(const iterator &rhs) const {
            return index - rhs.index;
        }
        iterator& operator+=(const long long &n) {
            index += n;
            return *this;
        }
        iterator& operator-=(const long long &n) {
            index -= n;
            return *this;
        }
        iterator operator++(int n) {
            iterator tmp(index, container);
            index ++;
            return tmp;
        }
        iterator& operator++() {
            index ++;
            return *this;
        }
        iterator operator--(int n) {
            iterator tmp(index, container);
            index --;
            return tmp;
        }
        iterator& operator--() {
            index --;
            return *this;
        }
        T& operator*() const {return (*container)[index];}
        bool operator==(const iterator &rhs) const {return (index == rhs.index) && (container == rhs.container);}
        bool operator==(const const_iterator &rhs) const {return (index == rhs.index) && (container == rhs.container);}
        bool operator!=(const iterator &rhs) const {return (index != rhs.index) || (container != rhs.container);}
        bool operator!=(const const_iterator &rhs) const {return (index != rhs.index) || (container != rhs.container);}
    };
    class const_iterator {
    private:
        long long index;
        const vector *container;
    public:
        const_iterator(long long i = 0, const vector *c = NULL): index(i), container(c){}
        const long long return_in() const {return index;}
        const_iterator operator+(const long long &n) const {
            const_iterator tmp(index + n, container);
            return tmp;
        }
        const_iterator operator-(const long long &n) const {
            const_iterator tmp(index - n, container);
            return tmp;
        }
        long long operator-(const const_iterator &rhs) const {
            return index - rhs.index;
        }
        const_iterator& operator+=(const long long &n) {
            index += n;
            return *this;
        }
        const_iterator& operator-=(const long long &n) {
            index -= n;
            return *this;
        }
        const_iterator operator++(int n) {
            const_iterator tmp(index, container);
            index ++;
            return tmp;
        }
        const_iterator& operator++() {
            index ++;
            return *this;
        }
        const_iterator operator--(int n) {
            const_iterator tmp(index, container);
            index --;
            return tmp;
        }
        const_iterator& operator--() {
            index --;
            return *this;
        }
        const T& operator*() const {return (*container)[index];}
        bool operator==(const iterator &rhs) const {return (index == rhs.index) && (container == rhs.container);}
        bool operator==(const const_iterator &rhs) const {return (index == rhs.index) && (container == rhs.container);}
        bool operator!=(const iterator &rhs) const {return (index != rhs.index) || (container != rhs.container);}
        bool operator!=(const const_iterator &rhs) const {return (index != rhs.index) || (container != rhs.container);}
    };
    vector(size_t max_s = 10) {
        data = (T *)(operator new(max_s * sizeof(T)));
        current_len = 0;
        max_size = max_s;
    }
    vector(const vector &other) {
        data = (T *)(operator new(other.max_size * sizeof(T)));
        current_len = other.current_len;
        for (long long i = 0; i < current_len; ++i) data[i] = other.data[i];
        max_size = other.max_size;
    }
    ~vector() {
        for (long long i = 0; i < max_size; ++i) data[i].~T();
        operator delete (data);
    }
    vector &operator=(const vector &other) {
        for (long long i = 0; i < max_size; ++i) data[i].~T();
        operator delete (data);
        data = (T *)(operator new(other.max_size * sizeof(T)));
        current_len = other.current_len;
        for (long long i = 0; i < current_len; ++i) data[i] = other.data[i];
        max_size = other.max_size;
        return *this;
    }
    T & at(const size_t &pos) {
        return data[pos];
    }
    const T & at(const size_t &pos) const {
        return data[pos];
    }
    T & operator[](const size_t &pos) {
        return data[pos];
    }
    const T & operator[](const size_t &pos) const {
        return data[pos];
    }
    const T & front() const {
        return data[0];
    }
    const T & back() const {
        return data[current_len - 1];
    }
    iterator begin() {
        iterator tmp(0, this);
        return tmp;
    }
    const_iterator cbegin() const {
        const_iterator tmp(0, this);
        return tmp;
    }
    iterator end() {
        iterator tmp(current_len, this);
        return tmp;
    }
    const_iterator cend() const {
        const_iterator tmp(current_len, this);
        return tmp;
    }
    bool empty() const {return current_len == 0;}
    size_t size() const {return current_len;}
    void clear() {current_len = 0;}
    iterator insert(iterator pos, const T &value) {
        if (current_len == max_size) doublespace();
        long long tmp = pos.return_in();
        for (long long i = current_len - 1; i >= tmp; --i) data[i + 1] = data[i];
        new (data + tmp) T(value);
        current_len ++;
        return pos;
    }
    iterator insert(const size_t &ind, const T &value) {
        if (current_len == max_size) doublespace();
        for (long long i = current_len - 1; i >= ind; --i) data[i + 1] = data[i];
        new (data + ind) T(value);
        current_len ++;
        iterator tmp(ind, this);
        return tmp;
    }
    iterator erase(iterator pos) {
        long long tmp = pos.return_in();
        for (long long i = tmp; i < current_len - 1; ++i) data[i] = data[i + 1];
        current_len --;
        return pos;
    }
    iterator erase(const size_t &ind) {
        for (long long i = ind; i < current_len - 1; ++i) data[i] = data[i + 1];
        current_len --;
        iterator tmp(ind, this);
        return tmp;
    }
    void push_back(const T &value) {
        if (current_len == max_size) doublespace();
        new (data + current_len) T(value);
        current_len ++;
    }
    void pop_back() {
        current_len --;
    }
};

#endif

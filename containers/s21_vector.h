#ifndef SRC_CONTAINERS_S21_VECTOR_H_
#define SRC_CONTAINERS_S21_VECTOR_H_

namespace s21
{
    template <typename T>
    class vector
    {
    public:
        // Vector Member Type
        using value_type = T;
        using reference = T &;
        using const_reference = const T &;
        using iterator = T *;
        using const_iterator = const T *;
        using size_type = size_t;
        using difference_type = std::ptrdiff_t;

        // Vector Member functions
        vector() noexcept;
        explicit vector(size_type n);
        vector(std::initializer_list<value_type> const &items);
        vector(const vector &v);
        vector(vector &&v) noexcept;
        ~vector();

        vector &operator=(const vector &v);
        vector &operator=(vector &&v) noexcept;
        vector &operator=(std::initializer_list<value_type> const &items);

        // Vector Element access
        reference at(size_type pos);
        reference operator[](size_type pos);
        const_reference front();
        const_reference back();
        T *data() noexcept;

        // Vector iterators
        iterator begin() noexcept;
        iterator end() noexcept;
        const_iterator cbegin() const noexcept;
        const_iterator cend() const noexcept;

        // Vector capacity
        bool empty() noexcept;
        size_type size() noexcept;
        size_type max_size() noexcept;
        void reserve(size_type size);
        size_type capacity() noexcept;
        void shrink_to_fit();

        // Vector modifiers
        void clear() noexcept;
        iterator insert(iterator pos, const_reference value);
        void erase(iterator pos);
        void push_back(const_reference value);
        void pop_back();
        void swap(vector &other);

        template <typename... Args>
        iterator insert_many(const_iterator pos, Args &&...args);
        template <typename... Args>
        void insert_many_back(Args &&...args);

    private:
        T *arr_;
        size_t size_;
        size_t capacity_;

        void expandArray(size_type incoming_amount = 1);
        void increaseCapacity();
        void reallocateArray(size_type capacity);
        void copyToArray(value_type *arr, size_type size);
        void copyFromArray(const value_type *arr, size_type size);
        void freeArray();
    };


    // Vector Member functions
    template <typename T>
    vector<T>::vector() noexcept : arr_(nullptr), size_(0), capacity_(0) {}

    template <typename T>
    vector<T>::vector(size_type n)
        : arr_(new value_type[n]()), size_(n), capacity_(n) {}

    template <typename T>
    vector<T>::vector(std::initializer_list<value_type> const &items)
    {
        size_ = items.size();
        capacity_ = size_;
        arr_ = new value_type[size_];
        copyFromArray(items.begin(), items.size());
    }

    template <typename T>
    vector<T>::vector(const vector &v)
    {
        size_ = v.size_;
        capacity_ = v.capacity_;
        arr_ = new value_type[v.capacity_]();
        copyFromArray(v.arr_, v.size_);
    }

    template <typename T>
    vector<T>::vector(vector &&v) noexcept
    {
        size_ = v.size_;
        capacity_ = v.capacity_;
        arr_ = v.arr_;

        v.size_ = 0;
        v.capacity_ = 0;
        v.arr_ = nullptr;
    }

    template <typename T>
    vector<T>::~vector()
    {
        freeArray();
    }

    template <typename T>
    vector<T> &vector<T>::operator=(const vector<T> &v)
    {
        if (this == &v)
        {
            return *this;
        }
        clear();
        reserve(v.size_);

        copyFromArray(v.arr_, v.size_);
        return *this;
    }

    template <typename T>
    vector<T> &vector<T>::operator=(vector<T> &&v) noexcept
    {
        if (this == &v)
        {
            return *this;
        }
        freeArray();

        size_ = v.size_;
        capacity_ = v.capacity_;
        arr_ = v.arr_;

        v.size_ = 0;
        v.capacity_ = 0;
        v.arr_ = nullptr;
        return *this;
    }

    template <typename T>
    vector<T> &vector<T>::operator=(
        std::initializer_list<value_type> const &items)
    {
        clear();
        reserve(items.size());
        copyFromArray(items.begin(), items.size());
        return *this;
    }

    // Vector Element access
    template <typename T>
    typename vector<T>::reference vector<T>::at(size_type pos)
    {
        if (pos >= size_)
        {
            throw std::out_of_range("accessing vector element out of range");
        }
        return arr_[pos];
    }

    template <typename T>
    typename vector<T>::reference vector<T>::operator[](size_type pos)
    {
        return arr_[pos];
    }

    template <typename T>
    typename vector<T>::const_reference vector<T>::front()
    {
        return arr_[0];
    }

    template <typename T>
    typename vector<T>::const_reference vector<T>::back()
    {
        return arr_[size_ - 1];
    }

    template <typename T>
    T *vector<T>::data() noexcept
    {
        return arr_;
    }

    // Vector iterators
    template <typename T>
    typename vector<T>::iterator vector<T>::begin() noexcept
    {
        return arr_;
    }

    template <typename T>
    typename vector<T>::const_iterator vector<T>::cbegin() const noexcept
    {
        return arr_;
    }

    template <typename T>
    typename vector<T>::iterator vector<T>::end() noexcept
    {
        return arr_ + size_;
    }

    template <typename T>
    typename vector<T>::const_iterator vector<T>::cend() const noexcept
    {
        return arr_ + size_;
    }

    // Vector capacity
    template <typename T>
    bool vector<T>::empty() noexcept
    {
        return size_ == 0;
    }

    template <typename T>
    typename vector<T>::size_type vector<T>::size() noexcept
    {
        return size_;
    }

    template <typename T>
    typename vector<T>::size_type vector<T>::max_size() noexcept
    {
        return SIZE_MAX / sizeof(value_type);
    }

    template <typename T>
    void vector<T>::reserve(size_type size)
    {
        if (size <= capacity_)
        {
            return;
        }
        auto expanded_arr = new value_type[size]();
        copyToArray(expanded_arr, size);
        freeArray();
        arr_ = expanded_arr;
        capacity_ = size;
    }

    template <typename T>
    typename vector<T>::size_type vector<T>::capacity() noexcept
    {
        return capacity_;
    }

    template <typename T>
    void vector<T>::shrink_to_fit()
    {
        reallocateArray(size_);
    }

    // Vector modifiers
    template <typename T>
    void vector<T>::clear() noexcept
    {
        size_ = 0;
    }

    template <typename T>
    typename vector<T>::iterator vector<T>::insert(iterator pos,
                                                   const_reference value)
    {
        if (pos == end())
        {
            push_back(value);
            pos = end();
            return --pos;
        }

        difference_type diff = pos - begin();

        expandArray();

        auto insert_pos = begin() + diff;
        pos = insert_pos;

        value_type evicted_item = *pos;
        *pos = value;
        pos++;

        for (; pos <= end(); ++pos)
        {
            std::swap(*pos, evicted_item);
        }
        ++size_;
        return insert_pos;
    }

    template <typename T>
    template <typename... Args>
    typename vector<T>::iterator vector<T>::insert_many(const_iterator pos,
                                                        Args &&...args)
    {
        difference_type diff = pos - begin();

        expandArray(sizeof...(args));

        auto return_pos = begin() + diff;

        [[maybe_unused]] auto insert_pos = return_pos - 1;

        (insert(++insert_pos, value_type(std::forward<Args>(args))), ...);
        return return_pos;
    }

    template <typename T>
    template <typename... Args>
    void vector<T>::insert_many_back(Args &&...args)
    {
        expandArray(sizeof...(args));
        (push_back(value_type(std::forward<Args>(args))), ...);
    }

    template <typename T>
    void vector<T>::erase(iterator pos)
    {
        auto next = pos;
        next++;

        for (; next < end(); ++next, ++pos)
        {
            std::swap(*next, *pos);
        }
        --size_;
    }

    template <typename T>
    void vector<T>::push_back(const_reference value)
    {
        expandArray();
        arr_[size_++] = value;
    }

    template <typename T>
    void vector<T>::pop_back()
    {
        --size_;
    }

    template <typename T>
    void vector<T>::swap(vector &other)
    {
        std::swap(*this, other);
    }

    template <typename T>
    void vector<T>::expandArray(size_type incoming_amount)
    {
        if (capacity_ > size_ + incoming_amount)
        {
            return;
        }

        if (capacity_ == 0)
        {
            capacity_ = 8; 
            // Vector is initialized with an 
            // initial capacity of 8 elements.
        }

        while (capacity_ < (size_ + incoming_amount))
        {
            increaseCapacity();
        }

        auto expanded_arr = new value_type[capacity_]();
        copyToArray(expanded_arr, capacity_);
        freeArray();
        arr_ = expanded_arr;
    }

    template <typename T>
    void vector<T>::increaseCapacity()
    {
        capacity_ = 1 + static_cast<size_type>(1.618 *
                                               static_cast<double>(capacity_)); 
        // 1.618 is the golden ratio for optimal expansion
    }

    template <typename T>
    void vector<T>::reallocateArray(size_type capacity)
    {
        auto reallocatedArray = new value_type[capacity]();
        copyToArray(reallocatedArray, capacity);
        freeArray();
        capacity_ = capacity;
        if (capacity_ < size_)
        {
            size_ = capacity_;
        }
        arr_ = reallocatedArray;
    }

    template <typename T>
    void vector<T>::copyToArray(value_type *arr, size_type size)
    {
        for (size_type i = 0; i < size && i < size_; ++i)
        {
            arr[i] = arr_[i];
        }
    }

    template <typename T>
    void vector<T>::copyFromArray(const value_type *arr, size_type size)
    {
        clear();
        if (capacity_ < size)
        {
            reallocateArray(size);
        }
        for (size_type i = 0; i < size; ++i)
        {
            arr_[i] = arr[i];
        }
        size_ = size;
    }

    template <typename T>
    void vector<T>::freeArray()
    {
        delete[] arr_;
    }
} // namespace s21

#endif // SRC_CONTAINERS_S21_VECTOR_H_
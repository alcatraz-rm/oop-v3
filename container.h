#include <iostream>

template <typename T>
class Container
{
private:
    struct Element
    {
        T data;
        Element *next;
        Element *prev;

        Element() : next(nullptr), prev(nullptr) {}
        Element(
            T const &data,
            Element *next,
            Element *prev) : data(data), next(next), prev(prev) {}
    };

    Element *_limiter{nullptr};
    size_t _size;

    Element *create_limiter()
    {
        Element *result = new Element();
        result->next = result;
        result->prev = result;
        return result;
    }

public:
    Container() : _size(0) {}

    ~Container()
    {
        clear();
    }

    class Iterator
    {
    private:
        Element *current;

    public:
        Iterator(Element *_current) : current(_current) {}

        bool operator!=(const Iterator &other)
        {
            return current != other.current;
        }

        Iterator &operator++()
        {
            current = current->next;
            return *this;
        }

        Iterator &operator--()
        {
            current = current->prev;
            return *this;
        }

        T const &value()
        {
            return current->data;
        }
    };

    void pop()
    {
        if (_size == 0)
        {
            return;
        }

        Element *first_element = _limiter->next;
        Element *second_element = first_element->next;

        second_element->prev = _limiter;
        _limiter->next = second_element;

        delete first_element;

        --_size;

        if (_size == 0 && _limiter)
        {
            delete _limiter;
            _limiter = nullptr;
        }
    }

    void pop_back()
    {
        if (_size == 0)
        {
            return;
        }

        Element *last_element = _limiter->prev;
        Element *second_last_element = last_element->prev;

        second_last_element->next = _limiter;
        _limiter->prev = second_last_element;

        delete last_element;
        --_size;

        if (_size == 0 && _limiter)
        {
            delete _limiter;
            _limiter = nullptr;
        }
    }

    void push(T const &data)
    {
        if (_size == 0)
        {
            _limiter = create_limiter();
        }
        Element *old_first = _limiter->next;
        Element *new_element = new Element(data, old_first, _limiter);

        old_first->prev = new_element;
        _limiter->next = new_element;
        ++_size;
    }

    void push_back(T const &data)
    {
        if (_size == 0)
        {
            _limiter = create_limiter();
        }
        Element *old_last = _limiter->prev;
        Element *new_element = new Element(data, _limiter, old_last);

        old_last->next = new_element;
        _limiter->prev = new_element;
        ++_size;
    }

    Iterator begin()
    {
        if (_size == 0)
        {
            return end();
        }
        return Iterator(this->_limiter->next);
    }

    Iterator end()
    {
        return Iterator(this->_limiter);
    }

    T const &first() const
    {
        if (_size == 0)
        {
            throw std::out_of_range("Container is empty.");
        }
        return _limiter->next->data;
    }

    T const &last() const
    {
        if (_size == 0)
        {
            throw std::out_of_range("Container is empty.");
        }
        return _limiter->prev->data;
    }

    void reverse()
    {
        if (_size < 2)
        {
            return;
        }

        Element *current = _limiter;

        while (current)
        {
            std::swap(current->next, current->prev);
            current = current->prev;
        }
    }

    size_t size() const
    {
        return _size;
    }

    bool empty() const
    {
        return _size == 0;
    }

    void clear()
    {
        while (_size > 0)
        {
            pop();
        }
    }

    void switch_containers(Container &container_1, Container &container_2)
    {
        std::swap(container_1._limiter, container_2._limiter);
        std::swap(container_1._size, container_2._size);
    }
};

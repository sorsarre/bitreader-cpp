#pragma once
#include <cstdint>
#include <cstddef>
#include <algorithm>
#include <memory>

namespace brcpp
{
    //--------------------------------------------------------------------------
    class shared_buffer
    {
    public:
        //----------------------------------------------------------------------
        shared_buffer();

        //----------------------------------------------------------------------
        shared_buffer(shared_buffer&& other) = default;
        shared_buffer(const shared_buffer& other) = default;
        shared_buffer& operator=(const shared_buffer& other) = default;
        ~shared_buffer() = default;

        //----------------------------------------------------------------------
        using iterator = uint8_t*;
        using const_iterator = const uint8_t*;

        size_t size() const { return _state->size; }
        size_t capacity() const { return _state->capacity; }
        const uint8_t* get() const { return _state->data; }
        uint8_t* get() { return _state->data; }

        iterator begin() { return get(); }
        iterator end() { return get() + size(); }
        const_iterator begin() const { return get(); }
        const_iterator end() const { return get() + size(); }
        const_iterator cbegin() const { return get(); }
        const_iterator cend() const { return get() + size(); }

        //----------------------------------------------------------------------
        static shared_buffer wrap_mem(uint8_t* data, size_t size);
        static shared_buffer copy_mem(const uint8_t* data, size_t size);
        static shared_buffer clone(const shared_buffer& buffer);
        static shared_buffer allocate(size_t size);
        void realloc(size_t new_size);
        void resize(size_t new_size);
        operator bool() const;

    private:
        //----------------------------------------------------------------------
        struct _internal
        {
            uint8_t* data = nullptr;
            size_t capacity = 0;
            size_t size = 0;
        };

        //----------------------------------------------------------------------
        static std::shared_ptr<_internal> make_internal(_internal state);
        static std::shared_ptr<_internal> make_internal(
                uint8_t* data,
                size_t size,
                size_t capacity);

        explicit shared_buffer(_internal state);
        explicit shared_buffer(std::shared_ptr<_internal> state);

        std::shared_ptr<_internal> _state;
    };
}

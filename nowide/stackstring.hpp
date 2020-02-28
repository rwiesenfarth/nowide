//
//  Copyright (c) 2012 Artyom Beilis (Tonkikh)
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef NOWIDE_STACKSTRING_HPP_INCLUDED
#define NOWIDE_STACKSTRING_HPP_INCLUDED

#include <nowide/convert.hpp>
#include <cstring>


namespace nowide {

    ///
    /// \brief A class that allows to create a temporary wide or narrow UTF strings from
    /// wide or narrow UTF source.
    ///
    /// It uses a stack buffer if the string is short enough
    /// otherwise allocates a buffer on the heap.
    ///
    /// Invalid UTF characters are replaced by the substitution character, see #NOWIDE_REPLACEMENT_CHARACTER
    ///
    /// If a NULL pointer is passed to the constructor or convert method, NULL will be returned by c_str.
    /// Similarily a default constructed stackstring will return NULL on calling c_str.
    ///
    template<typename CharOut = wchar_t, typename CharIn = char, size_t BufferSize = 256>
    class basic_stackstring
    {
    public:
        static const size_t buffer_size = BufferSize;
        typedef CharOut output_char;
        typedef CharIn input_char;

        basic_stackstring() : data_(NULL)
        {
            buffer_[0] = 0;
        }
        explicit basic_stackstring(const input_char* input) : data_(NULL)
        {
            convert(input);
        }
        basic_stackstring(const input_char* begin, const input_char* end) : data_(NULL)
        {
            convert(begin, end);
        }

        basic_stackstring(const basic_stackstring& other) : data_(NULL)
        {
            *this = other;
        }
        basic_stackstring& operator=(const basic_stackstring& other)
        {
            if(this != &other)
            {
                clear();
                const size_t len = other.length();
                if(other.uses_stack_memory())
                    data_ = buffer_;
                else if(other.data_)
                    data_ = new output_char[len + 1];
                else
                {
                    data_ = NULL;
                    return *this;
                }
                std::memcpy(data_, other.data_, sizeof(output_char) * (len + 1));
            }
            return *this;
        }

        ~basic_stackstring()
        {
            clear();
        }

        output_char* convert(const input_char* input)
        {
            if(input)
                return convert(input, input + detail::strlen(input));
            clear();
            return get();
        }
        output_char* convert(const input_char* begin, const input_char* end)
        {
            clear();

            if(begin)
            {
                size_t space = get_space(sizeof(input_char), sizeof(output_char), end - begin) + 1;
                if(space <= buffer_size)
                {
                    data_ = buffer_;
                    detail::convert_buffer(buffer_, buffer_size, begin, end);
                } else
                {
                    data_ = new output_char[space];
                    detail::convert_buffer(data_, space, begin, end);
                }
            }
            return get();
        }
        /// Return the converted, NULL-terminated string or NULL if no string was converted
        output_char* get()
        {
            return data_;
        }
        /// Return the converted, NULL-terminated string or NULL if no string was converted
        const output_char* get() const
        {
            return data_;
        }
        void clear()
        {
            if(!uses_stack_memory())
                delete[] data_;
            data_ = NULL;
        }

        friend void swap(basic_stackstring& lhs, basic_stackstring& rhs)
        {
            if(lhs.uses_stack_memory())
            {
                if(rhs.uses_stack_memory())
                {
                    for(size_t i = 0; i < buffer_size; i++)
                        std::swap(lhs.buffer_[i], rhs.buffer_[i]);
                } else
                {
                    lhs.data_ = rhs.data_;
                    rhs.data_ = rhs.buffer_;
                    for(size_t i = 0; i < buffer_size; i++)
                        rhs.buffer_[i] = lhs.buffer_[i];
                }
            } else if(rhs.uses_stack_memory())
            {
                rhs.data_ = lhs.data_;
                lhs.data_ = lhs.buffer_;
                for(size_t i = 0; i < buffer_size; i++)
                    lhs.buffer_[i] = rhs.buffer_[i];
            } else
                std::swap(lhs.data_, rhs.data_);
        }

    private:
        /// True if the stack memory is used
        bool uses_stack_memory() const
        {
            return data_ == buffer_;
        }
        size_t length() const
        {
            if(!data_)
                return 0;
            size_t len = 0;
            while(data_[len])
                len++;
            return len;
        }
        static size_t get_space(size_t insize, size_t outsize, size_t in)
        {
            if(insize <= outsize)
                return in;
            else if(insize == 2 && outsize == 1)
                return 3 * in;
            else if(insize == 4 && outsize == 1)
                return 4 * in;
            else // if(insize == 4 && outsize == 2)
                return 2 * in;
        }
        output_char buffer_[buffer_size];
        output_char* data_;
    }; // basic_stackstring

    ///
    /// Convenience typedef
    ///
    typedef basic_stackstring<wchar_t, char, 256> wstackstring;
    ///
    /// Convenience typedef
    ///
    typedef basic_stackstring<char, wchar_t, 256> stackstring;
    ///
    /// Convenience typedef
    ///
    typedef basic_stackstring<wchar_t, char, 16> wshort_stackstring;
    ///
    /// Convenience typedef
    ///
    typedef basic_stackstring<char, wchar_t, 16> short_stackstring;

} // namespace nowide


#endif

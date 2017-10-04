#ifndef __COMPONENT_BUFFER_H__
#define __COMPONENT_BUFFER_H__

#include "misc_utils.h"

class component_buffer {
public:
    component_buffer();
    component_buffer(void* pStart, size_t len, size_t inc = sizeof(wxUint8),
                     formatEndian endianness = endian_little);
    virtual ~component_buffer();

    size_t  size() const { return m_buf_len; }
    void*   ptr() const { return m_ptr_start; }

    void    set_fetch_size(size_t inc = sizeof(wxUint8));
    size_t  get_fetch_size() const;

    /**
     *  Iterator class.
     */

    class component_iterator {
    public:
        component_iterator();
        virtual ~component_iterator();

        wxUint32 operator *() const;

        component_iterator& operator++(int);
        component_iterator& operator--();
        bool                operator != (const component_iterator& iter);

    private:
        friend class component_buffer;

        component_iterator(component_buffer* pParent, void* pStart,
                           size_t inc                     = sizeof(wxUint8),
                           formatEndian endianness = endian_little);

        component_buffer*   m_parent;
        void*               m_ptr;
        size_t              m_inc;
        formatEndian m_endian;
    };

    component_iterator begin();
    component_iterator end();
    component_iterator begin(size_t inc, formatEndian endian = endian_little);
    component_iterator end(size_t inc, formatEndian endian = endian_little);

private:
    void*               m_ptr_start;
    void*               m_ptr_end;
    size_t              m_buf_len;
    size_t              m_inc;
    formatEndian m_endian;
};

#endif // __COMPONENT_BUFFER_H__

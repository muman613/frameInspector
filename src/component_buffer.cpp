#ifdef  wxDEBUG_LEVEL
#undef wxDEBUG_LEVEL
#define wxDEBUG_LEVEL 0
#endif
#undef __WXDEBUG__

#include <wx/wx.h>

#include "component_buffer.h"
#include "misc_utils.h"

component_buffer::component_buffer()
    :   m_ptr_start(0),
        m_ptr_end(0),
        m_buf_len(0),
        m_inc(sizeof(wxUint8)),
        m_endian(endian_little)
{
    wxLogDebug("component_buffer::component_buffer() default constructor");
}

component_buffer::component_buffer(void* pStart, size_t len, size_t inc,
                                   formatEndian endianness)
    :   m_ptr_start(pStart),
        m_ptr_end((void *)((wxUint8*)pStart + len)),
        m_buf_len(len),
        m_inc(inc),
        m_endian(endianness)
{
    wxLogDebug("component_buffer::component_buffer(%p, %ld, %ld, %s)", pStart,
               len, inc, (endianness == endian_little)?"le":"be");
}

/**
 *
 */

component_buffer::~component_buffer()
{
    // dtor
    wxLogDebug("component_buffer::~component_buffer()");
}

/**
 *
 */

component_buffer::component_iterator component_buffer::begin()
{
    wxLogDebug("component_buffer::begin() inc %ld endian %s", m_inc, (m_endian == endian_little)?"le":"be");
    component_buffer::component_iterator iter(this, m_ptr_start, m_inc, m_endian);
    return iter;
}

/**
 *
 */

component_buffer::component_iterator component_buffer::end() {
    wxLogDebug("component_buffer::end() inc %ld endian %s", m_inc, (m_endian == endian_little)?"le":"be");
    component_buffer::component_iterator iter(this, m_ptr_end, m_inc, m_endian);
    return iter;
}

/**
 *
 */

component_buffer::component_iterator component_buffer::begin(size_t inc, formatEndian endian)
{
    wxLogDebug("component_buffer::begin(%zu, %s)", inc, (endian == endian_little)?"le":"be");
    component_buffer::component_iterator iter(this, m_ptr_start, inc, endian);
    return iter;
}

/**
 *
 */

component_buffer::component_iterator component_buffer::end(size_t inc, formatEndian endian) {
    wxLogDebug("component_buffer::end(%zu, %s)", inc, (endian == endian_little)?"le":"be");
    component_buffer::component_iterator iter(this, m_ptr_end, inc, endian);
    return iter;
}

/**
 *
 */

void    component_buffer::set_fetch_size(size_t inc)
{
    wxLogDebug("component_buffer::set_fetch_size(%zu)", inc);
    m_inc = inc;
}

/**
 *
 */

size_t  component_buffer::get_fetch_size() const
{
    wxLogDebug("component_buffer::get_fetch_size()");
    return m_inc;
}

/*----------------------------------------------------------------------------*/

/**
 *
 */

component_buffer::component_iterator::component_iterator()
    :   m_ptr(0),
        m_inc(sizeof(wxUint8))
{
    // ctor
    wxLogDebug("component_buffer::component_iterator::component_iterator() default constructor");
}

/**
 *
 */

component_buffer::component_iterator::component_iterator(component_buffer*      parent,
        void*                  pStart,
        size_t                 inc,
        formatEndian    endianness)
    :   m_parent(parent),
        m_ptr(pStart),
        m_inc(inc),
        m_endian(endianness)
{
    // ctor
    wxLogDebug("component_buffer::component_iterator::component_iterator(%p, %p, %zu)", parent, pStart, inc);
}

/**
 *
 */

component_buffer::component_iterator::~component_iterator()
{
    // dtor
    wxLogDebug("component_buffer::component_iterator::~component_iterator()");
}

/**
 *
 */

wxUint32 component_buffer::component_iterator::operator *() const
{
    if (m_inc == sizeof(wxUint8)) {
        return (wxUint32)(*(wxUint8 *)m_ptr);
    } else if (m_inc == sizeof(wxUint16)) {
        if (m_endian == endian_little) {
            return (wxUint32)(*(wxUint16*)m_ptr);
        } else {
            return (wxUint32)(wxUINT16_SWAP_ALWAYS(*(wxUint16*)m_ptr));
        }
    }

    return 0;
}

/**
 *
 */

component_buffer::component_iterator& component_buffer::component_iterator::operator++(int)
{
    m_ptr = (void *)((wxUint8*)m_ptr + m_inc);
    return *this;
}

/**
 *
 */

component_buffer::component_iterator& component_buffer::component_iterator::operator--()
{
    m_ptr = (void *)((wxUint8*)m_ptr - m_inc);
    return *this;
}

bool component_buffer::component_iterator::operator != (const component_buffer::component_iterator& iter)
{
    return (m_ptr != iter.m_ptr);
}


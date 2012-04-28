//          Copyright Allan W. Nielsen 2011, 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#ifndef __COMPRESSED_ENUMS_HXX__
#define __COMPRESSED_ENUMS_HXX__

#include <assert.h>
#include <string>
#include <ostream>
#include <sstream>
#include <iostream>
#include <iterator>
#include <type_traits>
#include <boost/utility.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/seq/reverse.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include "consoleColor.h"

#define GMB_ENUM_ELEMENT(r, data, i, elem)                          \
    BOOST_PP_COMMA_IF(i) elem = i

#define GMB_ENUM_CASE_TO_STRING_SHORT(r, data, i, elem)             \
    case data::elem: return BOOST_PP_STRINGIZE(elem);

#define GMB_ENUM_CASE_TO_STRING_LONG(r, data, i, elem)              \
    case data::elem: return BOOST_PP_STRINGIZE(data) "::"           \
        BOOST_PP_STRINGIZE(elem);

#define GMB_ENUM_CASE_OSTREAM(r, data, i, elem)                     \
    case data::elem: o << BOOST_PP_STRINGIZE(elem); return o;

#define GMB_TAG(name)                                               \
    struct name {};                                                 \
    inline std::ostream& operator<<(std::ostream& o, name ) {       \
        o << BOOST_PP_STRINGIZE(name);                              \
        return o;                                                   \
    }

#define GMB_ENUM(name, seq)                                         \
    struct name {                                                   \
        static const size_t size = BOOST_PP_SEQ_SIZE(seq);          \
        enum inner_type {                                           \
            BOOST_PP_SEQ_FOR_EACH_I(GMB_ENUM_ELEMENT, _, seq)       \
        };                                                          \
        static const inner_type first = BOOST_PP_SEQ_ELEM(0, seq);  \
        static const inner_type last =                              \
            BOOST_PP_SEQ_ELEM(0, BOOST_PP_SEQ_REVERSE(seq));        \
        typedef inner_type E;                                       \
        static E from_uint(size_t i) {                              \
            assert(i < size);                                       \
            return (E)i;                                            \
        }                                                           \
    };                                                              \
    inline std::string to_s(name::E e) {                            \
        switch(e) {                                                 \
            BOOST_PP_SEQ_FOR_EACH_I(                                \
                GMB_ENUM_CASE_TO_STRING_SHORT, name, seq)           \
        }                                                           \
        assert(0);                                                  \
        return "";                                                  \
    }                                                               \
                                                                    \
    inline std::string to_ns_s(name::E e) {                         \
        switch(e) {                                                 \
            BOOST_PP_SEQ_FOR_EACH_I(                                \
                GMB_ENUM_CASE_TO_STRING_LONG, name, seq)            \
        }                                                           \
        assert(0);                                                  \
        return "";                                                  \
    }                                                               \
                                                                    \
    inline std::ostream& operator<<(std::ostream& o, const name ) { \
        o << BOOST_PP_STRINGIZE(name);                              \
        return o;                                                   \
    }                                                               \
    inline std::ostream& operator<<(std::ostream& o,                \
                                    const name::E e) {              \
        switch(e) {                                                 \
            BOOST_PP_SEQ_FOR_EACH_I(                                \
                GMB_ENUM_CASE_OSTREAM, name, seq)                   \
        }                                                           \
        assert(0);                                                  \
        return o;                                                   \
    }

#define GMB_SEQUENCE_ELEMENT(z, i, data) (_##i##_)
#define GMB_ENUM_CNT(name, count)                                   \
    GMB_ENUM(name, BOOST_PP_REPEAT(count, GMB_SEQUENCE_ELEMENT, _))

#define GMB_ENUM_MPL_ELEMENT_VECTOR(r, data, i, tuple)              \
    BOOST_PP_COMMA_IF(i) BOOST_PP_TUPLE_ELEM(2, 0, tuple)

#define GMB_ENUM_MPL_ELEMENT_STRUCT(r, data, i, tuple)              \
    struct BOOST_PP_TUPLE_ELEM(2, 0, tuple){};

#define GMB_ENUM_MPL_ELEMENT_STRUCT_TD(r, data, i, tuple)           \
    typedef data##_detail::BOOST_PP_TUPLE_ELEM(2, 0, tuple)         \
            BOOST_PP_TUPLE_ELEM(2, 0, tuple);

#define GMB_ENUM_MPL_ELEMENT_ENUM_CONST(r, data, i, tuple)          \
    static const data##_detail::E BOOST_PP_TUPLE_ELEM(2, 1, tuple)  \
         = data##_detail::BOOST_PP_TUPLE_ELEM(2, 1, tuple);

#define GMB_ENUM_MPL_ELEMENT_GET_NAME(r, data, i, tuple)            \
    template<> inline std::string to_name<                          \
            data::BOOST_PP_TUPLE_ELEM(2, 0, tuple)>() {             \
        return BOOST_PP_STRINGIZE(                                  \
            BOOST_PP_TUPLE_ELEM(2, 1, tuple));                      \
    }

#define GMB_ENUM_MPL_ELEMENT_GET_ENUM(r, data, i, tuple)            \
    template<> inline data::E to_enum<                              \
            data::E, data::BOOST_PP_TUPLE_ELEM(2, 0, tuple)>() {    \
        return data::BOOST_PP_TUPLE_ELEM(2, 1, tuple);              \
    }

#define GMB_ENUM_MPL_ELEMENT_PRINT_TAG(r, data, i, tuple)           \
    inline std::ostream& operator<<(                                \
        std::ostream& o,                                            \
        data::BOOST_PP_TUPLE_ELEM(2, 0, tuple)                      \
    ) {                                                             \
        o << BOOST_PP_STRINGIZE(BOOST_PP_TUPLE_ELEM(2, 0, tuple));  \
        return o;                                                   \
    }

#define GMB_ENUM_MPL_ELEMENT(r, data, i, tuple)                     \
    GMB_ENUM_ELEMENT(r, data, i, BOOST_PP_TUPLE_ELEM(2, 1, tuple))

#define GMB_ENUM_MPL_CASE_TO_STRING_SHORT(r, data, i, tuple)        \
    GMB_ENUM_CASE_TO_STRING_SHORT(                                  \
        r, data, i, BOOST_PP_TUPLE_ELEM(2, 1, tuple))

#define GMB_ENUM_MPL_CASE_TO_STRING_LONG(r, data, i, tuple)         \
    GMB_ENUM_CASE_TO_STRING_LONG(                                   \
        r, data, i, BOOST_PP_TUPLE_ELEM(2, 1, tuple))

#define GMB_ENUM_MPL_CASE_OSTREAM(r, data, i, tuple)                \
    GMB_ENUM_CASE_OSTREAM(                                          \
        r, data, i, BOOST_PP_TUPLE_ELEM(2, 1, tuple))

#define GMB_ENUM_MPL(name, seq)                                     \
    namespace name##_detail {                                       \
        template< typename E, typename T > E to_enum();             \
        template< typename T > std::string to_name();               \
    };                                                              \
    struct name {                                                   \
        static const size_t size = BOOST_PP_SEQ_SIZE(seq);          \
        enum inner_type {                                           \
            BOOST_PP_SEQ_FOR_EACH_I(GMB_ENUM_MPL_ELEMENT, _, seq)   \
        };                                                          \
        typedef inner_type E;                                       \
        static const inner_type first =                             \
            BOOST_PP_TUPLE_ELEM(2, 1, BOOST_PP_SEQ_ELEM(0, seq));   \
        static const inner_type last =                              \
            BOOST_PP_TUPLE_ELEM(                                    \
                2, 1, BOOST_PP_SEQ_ELEM(                            \
                    0, BOOST_PP_SEQ_REVERSE(seq)));                 \
        BOOST_PP_SEQ_FOR_EACH_I(GMB_ENUM_MPL_ELEMENT_STRUCT,        \
                                _, seq)                             \
        typedef boost::mpl::vector<                                 \
            BOOST_PP_SEQ_FOR_EACH_I(                                \
                GMB_ENUM_MPL_ELEMENT_VECTOR, _, seq) > Typelist;    \
        template< typename T > static E to_enum() {                 \
            return name##_detail::to_enum<E, T>();                  \
        }                                                           \
        template< typename T > static std::string to_name() {       \
            return name##_detail::to_name<T>();                     \
        }                                                           \
        static E from_uint(size_t i) {                              \
            assert(i < size); return (E)i;                          \
        }                                                           \
    };                                                              \
    namespace name##_detail {                                       \
        BOOST_PP_SEQ_FOR_EACH_I(                                    \
            GMB_ENUM_MPL_ELEMENT_GET_ENUM, name, seq)               \
        BOOST_PP_SEQ_FOR_EACH_I(                                    \
            GMB_ENUM_MPL_ELEMENT_GET_NAME, name, seq)               \
    };                                                              \
    inline std::string to_s(name::E e) {                            \
        switch(e) {                                                 \
            BOOST_PP_SEQ_FOR_EACH_I(                                \
                GMB_ENUM_MPL_CASE_TO_STRING_SHORT, name, seq)       \
        }                                                           \
        assert(0);                                                  \
        return "";                                                  \
    }                                                               \
                                                                    \
    inline std::string to_ns_s(name::E e) {                         \
        switch(e) {                                                 \
            BOOST_PP_SEQ_FOR_EACH_I(                                \
                GMB_ENUM_MPL_CASE_TO_STRING_LONG, name, seq)        \
        }                                                           \
        assert(0);                                                  \
        return "";                                                  \
    }                                                               \
    inline std::ostream& operator<<(std::ostream& o, const name ) { \
        o << BOOST_PP_STRINGIZE(name);                              \
        return o;                                                   \
    }                                                               \
    BOOST_PP_SEQ_FOR_EACH_I(                                        \
        GMB_ENUM_MPL_ELEMENT_PRINT_TAG, name, seq)                  \
                                                                    \
    inline std::ostream& operator<<(std::ostream& o,                \
                                    const name::E e) {              \
        switch(e) {                                                 \
            BOOST_PP_SEQ_FOR_EACH_I(                                \
                GMB_ENUM_MPL_CASE_OSTREAM, name, seq)               \
        }                                                           \
        assert(0);                                                  \
        return o;                                                   \
    }

template< typename _tag,
          typename _inner_type = _tag >
struct Tag;

template< typename super_t,
          typename base_config,
          typename type >
struct GetImplementation;

template< typename _type >
struct GetAbstractType;

template< typename _storage_type,
          typename _inner_type,
          typename _nested_configuration >
struct Proxy;

template< typename SuperType >
struct GetStorageType;

template< typename _storage_type,
          typename _inner_type,
          typename _nested_configuration >
struct GetStorageType<
    Proxy<_storage_type, _inner_type, _nested_configuration >
> {
    typedef _storage_type type;
};

template <
    typename _storage_type,
    size_t _max
>
struct BasicRangedUIntDescriptor {
    typedef _storage_type inner_type;
    const static size_t size = _max + 1;
    const static size_t max = _max;
};

template< typename _storage_type,
          typename _inner_type >
struct Container;


struct Terminal{}; // T as in terminal

template< typename _storage_type,
          size_t _base,
          size_t _offset >
struct NestedConfiguration {
    typedef _storage_type storage_type;
    static const size_t base = _base;
    static const size_t offset = _offset;
};

template< size_t size >
struct ElementConfig { };
template< size_t size, typename tail = Terminal >
struct ElementConfig_ : public tail {
    ElementConfig_( const tail& tail_value ) :
        tail( tail_value ) { }
};

template< size_t size >
struct NodeElementConfig { };
template< size_t size, typename tail = Terminal >
struct NodeElementConfig_: public tail {
    NodeElementConfig_( const tail& tail_value ) :
        tail( tail_value ) { }
};

template< size_t _base >
struct StructConfig {
    static const size_t base = _base;
};

template< size_t _base,
          typename _tail = Terminal >
struct StructConfig_ : public _tail {
    typedef _tail tail;
    static const size_t base = _base;

    StructConfig_ ( const tail& tail_value ) :
        tail( tail_value ) { }
};

template< size_t _length, size_t _base >
struct ArrayConfig {
    static const size_t length = _length;
    static const size_t base = _base;
    const size_t index;

    ArrayConfig( size_t _index ) : index(_index) { }
};

template< size_t _length, size_t _base, typename _tail = Terminal >
struct ArrayConfig_ : public _tail {
    typedef _tail tail;
    static const size_t length = _length;
    static const size_t base = _base;
    const size_t index;

    ArrayConfig_ ( size_t _index, const tail& tail_value ) :
        tail( tail_value ),
        index(_index) { }
};

template< size_t _size, size_t _offset >
struct UnionConfig {
    static const size_t size = _size;
    static const size_t offset = _offset;
};

template< size_t _size,
          size_t _offset,
          typename _tail = Terminal>
struct UnionConfig_ : public _tail {
    typedef _tail tail;
    static const size_t size = _size;
    static const size_t offset = _offset;

    UnionConfig_ ( const tail& tail_value ) :
        tail( tail_value ) { }
};

template< typename BaseConfig,
          typename NoDependConfig
        >
struct CalculateConfiguration;

// Optimizations configurations
#define OPTIMIZE
#define NODE_UNION_OPTIMIZE
#define UNION_UNION_OPTIMIZE
#define ELEMENT_UNION_OPTIMIZE
#define NODE_TERMINAL_OPTIMIZE
#define STRUCT_NODE_STRUCT_NODE_OPTIMIZE
#define STRUCT_NODE_STRUCT_TERMINAL_OPTIMIZE
#define STRUCT_NODE_STRUCT_UNION_OPTIMIZE

#if (defined OPTIMIZE) && (defined NODE_TERMINAL_OPTIMIZE)
// N(size)(TERMINAL) -> (TERMINAL)
template< size_t size >
struct CalculateConfiguration<
    Terminal, NodeElementConfig<size>
> {
    typedef Terminal type;
};
#endif

#if (defined OPTIMIZE) && (defined NODE_UNION_OPTIMIZE)
// N(size)U(size, offset) -> U(size, offset)
template< size_t size,
          size_t offset,
          typename History >
struct CalculateConfiguration<
    UnionConfig_<
        size,
        offset,
        History
    >,
    NodeElementConfig<size>
> {
    typedef UnionConfig_<
        size,
        offset,
        History
    > type;

    type operator()( const NodeElementConfig<size>& head,
                     const History& tail ) {
        return type(tail);
    }
};
#endif

template< typename History, size_t size >
struct CalculateConfiguration<
    History, NodeElementConfig<size>
> {
    typedef NodeElementConfig_<size, History> type;

    type operator()( const NodeElementConfig<size>& head,
                     const History& tail ) {
        return type(tail);
    }
};

#if (defined OPTIMIZE) && (defined ELEMENT_UNION_OPTIMIZE)
// E(size)U(size, offset) -> U(size, offset)
template< size_t size,
          size_t offset,
          typename History >
struct CalculateConfiguration<
    UnionConfig_<
        size,
        offset,
        History
    >,
    ElementConfig<size>
> {
    typedef UnionConfig_<
        size,
        offset,
        History
    > type;

    type operator()( const ElementConfig<size>& head,
                     const History& tail ) {
        return type(tail);
    }
};
#endif

template< typename History, size_t size >
struct CalculateConfiguration<
    History, ElementConfig<size>
> {
    typedef ElementConfig_<size, History> type;

    type operator()( const ElementConfig<size>& head,
                     const History& tail ) {
        return type(tail);
    }
};

#if (defined OPTIMIZE) && (defined STRUCT_NODE_STRUCT_NODE_OPTIMIZE)
// S(base_1)N(size_1)S(base_2)N(size_2)(History) ->
// S(base_1 * base_2)N(size_2)(History)
template< size_t base_1,
          size_t size_1,
          size_t base_2,
          size_t size_2,
          typename History >
struct CalculateConfiguration<
    NodeElementConfig_<
        size_1,
        StructConfig_<
            base_2,
            NodeElementConfig_<
                size_2,
                History
            >
        >
    >,
    StructConfig<base_1>
> {
    typedef typename CalculateConfiguration<
        History,
        NodeElementConfig<size_2>
    >::type history_pushed_size;

    typedef StructConfig_<
        base_1 * base_2,
        history_pushed_size
    > type;

    type operator()( const StructConfig<base_1>& head,
                     const History& tail ) {
        return type(tail);
    }
};
#endif

#if (defined OPTIMIZE) && \
    (defined STRUCT_NODE_STRUCT_UNION_OPTIMIZE)
// S(base_1)N(size_1)S(base_2)U(union_size, union_offset)(History) ->
// S(base_1 * base_2)U(union_size, union_offset)(History)
template< size_t base_1,
          size_t size_1,
          size_t base_2,
          size_t union_size,
          size_t union_offset,
          typename History >
struct CalculateConfiguration<
    NodeElementConfig_<
        size_1,
        StructConfig_<
            base_2,
            UnionConfig_<
                union_size,
                union_offset,
                History
            >
        >
    >,
    StructConfig<base_1>
> {
    typedef typename CalculateConfiguration<
        History,
        UnionConfig<union_size, union_offset>
    >::type history_pushed_union;

    typedef StructConfig_<
        base_1 * base_2,
        history_pushed_union
    > type;

    type operator()( const StructConfig<base_1>& head,
                     const History& tail ) {
        return type(tail);
    }
};
#endif

#if (defined OPTIMIZE) && \
    (defined STRUCT_NODE_STRUCT_TERMINAL_OPTIMIZE)
// S(base_1)N(size_1)S(base_2)(TERMINAL) ->
// S(base_1 * base_2)(TERMINAL)
template< size_t base_1,
          size_t size_1,
          size_t base_2 >
struct CalculateConfiguration<
    NodeElementConfig_<
        size_1,
        StructConfig_<
            base_2,
            Terminal
        >
    >,
    StructConfig<base_1>
> {
    typedef StructConfig_<
        base_1 * base_2,
        Terminal
    > type;

    type operator()( const StructConfig<base_1>& head,
                     const Terminal& tail ) {
        return type(tail);
    }
};
#endif

template< typename History, size_t base >
struct CalculateConfiguration<
    History, StructConfig<base>
> {
    typedef StructConfig_<base, History> type;

    type operator()( const StructConfig<base>& head,
                     const History& tail ) {
        return type(tail);
    }
};

template< typename History, size_t length, size_t base >
struct CalculateConfiguration<
    History, ArrayConfig<length, base>
> {
    typedef ArrayConfig_<length, base, History> type;

    type operator()( const ArrayConfig<length, base>& head,
                     const History& tail ) {
        return type(head.index, tail);
    }
};


#if (defined OPTIMIZE) && (defined UNION_UNION_OPTIMIZE)
// U(union_size_1, union_offset_1)
// U(union_size_2, union_offset_2)(History)
//
// ->
//
// U(union_size_1, offset_1 + offset_2)
// (History)
template< size_t union_size_1,
          size_t union_offset_1,
          size_t union_size_2,
          size_t union_offset_2,
          typename History >
struct CalculateConfiguration<
    UnionConfig_<
        union_size_2,
        union_offset_2,
        History
    >,
    UnionConfig<union_size_1, union_offset_1>
> {
    typedef UnionConfig_<
        union_size_1,
        union_offset_1 + union_offset_2,
        History
    > type;

    type operator()( const UnionConfig<
                        union_size_1, union_offset_1
                    >& head,
                     const History& tail ) {
        return type(tail);
    }
};
#endif

template< typename History, size_t size, size_t offset >
struct CalculateConfiguration<
    History, UnionConfig<size, offset>
> {
    typedef UnionConfig_<size, offset, History> type;

    type operator()( const UnionConfig<size, offset>& head,
                     const History& tail ) {
        return type(tail);
    }
};

template< typename _storage_type,
          size_t _base,
          size_t _offset >
std::ostream& operator<<( std::ostream& o,
                          const NestedConfiguration<
                              _storage_type, _base, _offset>& ) {
    o << "Base: " << _base << ", Offset: " << _offset;
    return o;
}

inline std::ostream& operator<<( std::ostream& o, const Terminal& v ) {
    o << "(TERMINAL)"; return o;
}

template< size_t size,
          typename tail >
std::ostream& operator<<( std::ostream& o,
                          const ElementConfig_<size, tail>& v ) {
    o << "E(Size: " << size << ")";
    o << static_cast<const tail&>(v);
    return o;
}

template< size_t size,
          typename tail >
std::ostream& operator<<( std::ostream& o,
                          const NodeElementConfig_<size, tail>& v ) {
    o << "N(Size: " << size << ")";
    o << static_cast<const tail&>(v);
    return o;
}

template< size_t base,
          typename tail >
std::ostream& operator<<( std::ostream& o,
                          const StructConfig_<base, tail>& v ) {
    o << "S(Base: " << base << ")";
    o << static_cast<const tail&>(v);
    return o;
}

template< size_t lenght,
          size_t base,
          typename tail >
std::ostream& operator<<(
    std::ostream& o,
    const ArrayConfig_<lenght, base, tail>& v
) {
    o << "A(Index: " << v.index << " Base: " << base << ")";
    o << static_cast<const tail&>(v);
    return o;
}

template< size_t size,
          size_t offset,
          typename tail >
std::ostream& operator<<( std::ostream& o,
                          const UnionConfig_<size, offset, tail>& v
) {
    o << "U(Size: " << size << " Offset: " << offset << ")";
    o << static_cast<const tail&>(v);
    return o;
}

/////////////////////////////////////////////////////////////////////

#undef ENABLE_DEBUG_BUFFER

#ifdef ENABLE_DEBUG_BUFFER
std::stringstream ss;
struct SIndent;
struct Indent {
    friend struct SIndent;
    Indent() : cnt_ (0) { }
    Indent(int i) : cnt_ (i) {}
    Indent(const Indent& rhs) : cnt_ (rhs.cnt_) {}

    Indent& operator= (const Indent& rhs) {
        cnt_ = rhs.cnt_;
        return *this;
    }

    int cnt() const  { return cnt_; }

private:
    int cnt_;
    Indent& operator++ () { ++cnt_; return *this; }
    Indent& operator-- () { --cnt_; return *this; }
} ss_i;
std::ostream& operator<<( std::ostream& o, const Indent& v ) {
    for( int i = 0; i < v.cnt(); i ++ )
        o << "  ";
    return o;
}
struct SIndent {
    SIndent(std::string s) : cnt( ss_i.cnt() + 1 ) {
        ss << ss_i << s << std::endl;
        ++ss_i;
        ++ss_i;
    }

    ~SIndent() {
        --ss_i;
        --ss_i;
    }

    const int cnt;
};
std::ostream& operator<<( std::ostream& o, const SIndent& v ) {
    for( int i = 0; i < v.cnt; i ++ )
        o << "  ";
    return o;
}
#endif

void inline debug_buf_sink(const std::string& s) {
#ifdef ENABLE_DEBUG_BUFFER
    std::cout << bright_green_black "debug_buf_sink: " << s <<
        clr_color_nl;

    std::copy( std::istreambuf_iterator<char>(ss),
               std::istreambuf_iterator<char>(),
               std::ostreambuf_iterator<char>(std::cout) );
#endif
}

void inline debug_buf_clear(const std::string& s) {
#ifdef ENABLE_DEBUG_BUFFER
    std::cout << bright_yellow_black "debug_buf_clear: " << s <<
        clr_color_nl;
    ss.clear();
    ss.str(std::string());
#endif
}

template< typename storage_type,
          typename config >
struct BasicGet;

#define CONF StructConfig_< size, tail>
template< typename storage_type, size_t size, typename tail >
struct BasicGet< storage_type, CONF > {
    storage_type operator()(storage_type data, const CONF& conf ) {
        storage_type new_data = BasicGet<storage_type, tail>()(
            data, static_cast<const tail&>(conf));
#ifdef ENABLE_DEBUG_BUFFER
        ss << ss_i << "GET S(" << size << "): " << new_data
            << " / " << size << " = " << new_data / size << std::endl;
#endif
        return new_data / size;
    }
};
#undef CONF

#define CONF StructConfig_< size, Terminal>
template< typename storage_type, size_t size >
struct BasicGet< storage_type, CONF > {
    storage_type operator()(storage_type data, const CONF& conf ) {
#ifdef ENABLE_DEBUG_BUFFER
        ss << ss_i << "GET S(" << size << "): " << data
            << " / " << size << " = " << data / size << std::endl;
#endif
        return data / size;
    }
};
#undef CONF

#define CONF ArrayConfig_< length, size, tail>
template< typename storage_type,
          size_t length,
          size_t size,
          typename tail >
struct BasicGet< storage_type, CONF > {
    storage_type operator()(storage_type data, const CONF& conf ) {
        storage_type new_data = BasicGet<storage_type, tail>() (
            data, static_cast<const tail&>(conf) );
        size_t rt_base = 1;
        for(size_t i = 0; i < conf.index; i ++ ) rt_base *= size;
#ifdef ENABLE_DEBUG_BUFFER
        ss << ss_i << "GET A(" << length << ", " << size << "): " <<
            new_data << " / (" << size << " ^ " << conf.index <<
            ") = " << new_data / rt_base << std::endl;
#endif
        return new_data / rt_base;
    }
};
#undef CONF

#define CONF ArrayConfig_< length, size, Terminal>
template< typename storage_type,
          size_t length,
          size_t size >
struct BasicGet< storage_type, CONF > {
    storage_type operator()(storage_type data, const CONF& conf ) {
        assert(conf.index < length);

        size_t rt_base = 1;
        for(size_t i = 0; i < conf.index; i ++ ) rt_base *= size;
#ifdef ENABLE_DEBUG_BUFFER
        ss << ss_i << "GET A(" << length << ", " << size << "): " <<
            data << " / (" << size << " ^ " << conf.index <<
            ") = " << data / rt_base << std::endl;
#endif

        return data / rt_base;
    }
};
#undef CONF

#define CONF UnionConfig_< size, offset, tail>
template< typename storage_type,
          size_t size,
          size_t offset,
          typename tail >
struct BasicGet< storage_type, CONF > {
    storage_type operator()(storage_type data, const CONF& conf ) {
        storage_type new_data = BasicGet<storage_type, tail>()(
            data, static_cast<const tail&>(conf) );
#ifdef ENABLE_DEBUG_BUFFER
        ss << ss_i << "GET U(" << size << ", " << offset << "): " <<
            new_data << " - " << offset << " = " <<
            new_data - offset << std::endl;
#endif
        //assert( new_data >= offset );
        //assert( new_data - offset < size );
        return new_data - offset;
    }
};
#undef CONF

#define CONF UnionConfig_< size, offset, Terminal >
template< typename storage_type,
          size_t size,
          size_t offset>
struct BasicGet< storage_type, CONF > {
    storage_type operator()(storage_type data, const CONF& conf ) {
#ifdef ENABLE_DEBUG_BUFFER
        ss << ss_i << "GET U(" << size << ", " << offset << "): " <<
            data  << " - " << offset << " = " <<
            data - offset << std::endl;
#endif
        //assert( data >= offset );
        //assert( data - offset < size );
        return data - offset;
    }
};
#undef CONF

#define CONF ElementConfig_< size, tail>
template< typename storage_type, size_t size, typename tail >
struct BasicGet< storage_type, CONF > {
    storage_type operator()(storage_type data, const CONF& conf ) {
        storage_type new_data = BasicGet<storage_type, tail>()(
            data, static_cast<const tail&>(conf) );
#ifdef ENABLE_DEBUG_BUFFER
        ss << ss_i << "GET E(" << size << "): " << new_data <<
            " % " << size << " = " << new_data % size << std::endl;
#endif
        return new_data % size;
    }
};
#undef CONF

#define CONF ElementConfig_< size, Terminal >
template< typename storage_type, size_t size >
struct BasicGet< storage_type, CONF > {
    storage_type operator()(storage_type data, const CONF& conf ) {
#ifdef ENABLE_DEBUG_BUFFER
        ss << ss_i << "GET E(" << size << "): " << data  << " % " <<
            size << " = " << data % size << std::endl;
#endif
        return data % size;
    }
};
#undef CONF

#define CONF NodeElementConfig_< size, tail>
template< typename storage_type, size_t size, typename tail >
struct BasicGet< storage_type, CONF > {
    storage_type operator()(storage_type data, const CONF& conf ) {
        storage_type new_data = BasicGet<storage_type, tail>()(
            data, static_cast<const tail&>(conf) );
#ifdef ENABLE_DEBUG_BUFFER
        ss << ss_i << "GET N(" << size << ")" << new_data << " % " <<
            size << " = " << new_data % size << std::endl;
#endif
        return new_data % size;
    }
};
#undef CONF

#define CONF NodeElementConfig_< size, Terminal >
template< typename storage_type, size_t size >
struct BasicGet< storage_type, CONF > {
    storage_type operator()(storage_type data, const CONF& conf ) {
#ifdef ENABLE_DEBUG_BUFFER
        ss << ss_i << "GET N(" << size << "): " << data  << " % " <<
            size << " = " << data % size << std::endl;
#endif
        return data % size;
    }
};
#undef CONF

template< typename storage_type,
          typename config >
struct BasicSet;

#define CONF StructConfig_< size, tail>
template< typename storage_type, size_t size, typename tail >
struct BasicSet< storage_type, CONF >{
    storage_type operator()(storage_type value, const CONF& conf ) {
#ifdef ENABLE_DEBUG_BUFFER
        ss << ss_i << "SET S(" << size << "): " << value << " * " <<
            size << " = " << (value * size) << std::endl;
#endif
        return BasicSet<storage_type, tail>()(
            value * size, static_cast<const tail&>(conf) );
    }
};
#undef CONF

#define CONF StructConfig_< size, Terminal >
template< typename storage_type, size_t size >
struct BasicSet< storage_type, CONF >{
    storage_type operator()(storage_type value, const CONF& conf ) {
#ifdef ENABLE_DEBUG_BUFFER
        ss << ss_i << "SET S(" << size << "): " << value << " * " <<
            size << " = " << (value * size) << std::endl;
#endif
        return value * size;
    }
};
#undef CONF

#define CONF ArrayConfig_< length, size, tail>
template< typename storage_type,
          size_t length,
          size_t size,
          typename tail >
struct BasicSet< storage_type, CONF >{
    storage_type operator()(storage_type value, const CONF& conf ) {
        size_t rt_base = 1;
        for(size_t i = 0; i < conf.index; i ++ ) rt_base *= size;

#ifdef ENABLE_DEBUG_BUFFER
        ss << ss_i << "SET A(" << length << ", " << size << ")[" <<
            conf.index << "]: " << value << " * " << rt_base << " = "
            << (value * rt_base) << std::endl;
#endif
        return BasicSet<storage_type, tail>()(
            value * rt_base, static_cast<const tail&>(conf) );
    }
};
#undef CONF

#define CONF ArrayConfig_< length, size, Terminal >
template< typename storage_type,
          size_t length,
          size_t size >
struct BasicSet< storage_type, CONF >{
    storage_type operator()(storage_type value, const CONF& conf ) {
        size_t rt_base = 1;
        for(size_t i = 0; i < conf.index; i ++ ) rt_base *= size;

#ifdef ENABLE_DEBUG_BUFFER
        ss << ss_i << "SET A(" << length << ", " << size << ")[" <<
            conf.index << "]: " << value << " * " << rt_base << " = "
            << (value * rt_base) << std::endl;
#endif

        return value * rt_base;
    }
};
#undef CONF

#define CONF UnionConfig_< size, offset, tail>
template< typename storage_type,
          size_t size,
          size_t offset,
          typename tail >
struct BasicSet< storage_type, CONF >{
    storage_type operator()(storage_type value, const CONF& conf ) {
#ifdef ENABLE_DEBUG_BUFFER
        ss << ss_i << "SET U(" << size << ", " << offset << "): " <<
            value << " + " << offset << " = " << (value + offset) <<
            std::endl;
#endif

        return BasicSet<storage_type, tail>()(
            value + offset, static_cast<const tail&>(conf) );
    }
};
#undef CONF

#define CONF UnionConfig_< size, offset, Terminal >
template< typename storage_type,
          size_t size,
          size_t offset>
struct BasicSet< storage_type, CONF >{
    storage_type operator()(storage_type value, const CONF& conf ) {
#ifdef ENABLE_DEBUG_BUFFER
        ss << ss_i << "SET U(" << size << ", " << offset << "): " <<
            value << " + " << offset << " = " << (value + offset) <<
            std::endl;
#endif

        return value + offset;
    }
};
#undef CONF

#define CONF ElementConfig_< size, tail>
template< typename storage_type, size_t size, typename tail >
struct BasicSet< storage_type, CONF >{
    storage_type operator()(storage_type value, const CONF& conf ) {
#ifdef ENABLE_DEBUG_BUFFER
        ss << ss_i << "SET E(" << size << "): " <<
            value << " = " << value <<
            std::endl;
#endif
        return BasicSet<storage_type, tail>()(
            value, static_cast<const tail&>(conf) );
    }
};
#undef CONF

#define CONF ElementConfig_< size, Terminal >
template< typename storage_type, size_t size >
struct BasicSet< storage_type, CONF >{
    storage_type operator()(storage_type value, const CONF& conf ) {
#ifdef ENABLE_DEBUG_BUFFER
        ss << ss_i << "SET E(" << size << "): " <<
            value << " = " << value <<
            std::endl;
#endif
        return value;
    }
};
#undef CONF

#define CONF NodeElementConfig_<size, tail>
template< typename storage_type, size_t size, typename tail >
struct BasicSet< storage_type, CONF >{
    storage_type operator()(storage_type value, const CONF& conf ) {
#ifdef ENABLE_DEBUG_BUFFER
        ss << ss_i << "SET N(" << size << "): " <<
            value << " = " << value << std::endl;
#endif
        return BasicSet<storage_type, tail>()(
            value, static_cast<const tail&>(conf) );
    }
};
#undef CONF

#define CONF NodeElementConfig_< size, Terminal>
template< typename storage_type, size_t size >
struct BasicSet< storage_type, CONF >{
    storage_type operator()(storage_type value, const CONF& conf ) {
#ifdef ENABLE_DEBUG_BUFFER
        ss << ss_i << "SET N(" << size << "): " <<
            value << " = " << value <<
            std::endl;
#endif
        return value;
    }
};
#undef CONF

template<typename storage_type,
         typename nested_configuration>
void basic_clear( storage_type& data,
                  const nested_configuration& conf ){
#ifdef ENABLE_DEBUG_BUFFER
    SIndent i("Basic clear");
    ss << i << "Data in: " << data << std::endl;
    ss << i << "conf: " << conf << std::endl;
#endif

    storage_type base_clear_value =
        BasicGet<storage_type, nested_configuration>()(
            data, conf);

#ifdef ENABLE_DEBUG_BUFFER
    ss << i << "base clear value: " <<
        base_clear_value << std::endl;
#endif

    storage_type clear_value =
        BasicSet<storage_type, nested_configuration>()(
            base_clear_value, conf);

#ifdef ENABLE_DEBUG_BUFFER
    ss << i << "Clear value: " << clear_value <<
        std::endl;
#endif

    data -= clear_value;

#ifdef ENABLE_DEBUG_BUFFER
    ss << i << "Data out: " << data << std::endl;
#endif
}

template<typename storage_type,
         typename nested_configuration>
void basic_set_no_clear( storage_type& data,
                         const storage_type& new_data,
                         const nested_configuration& conf ){
#ifdef ENABLE_DEBUG_BUFFER
    SIndent i("Basic set_no_clear");
    ss << i << "Data in: " << data << " new_data: " << new_data <<
        std::endl;
    ss << i << "conf: " << conf << std::endl;
#endif
    storage_type set_value =
        BasicSet<storage_type, nested_configuration>()(
            new_data, conf );

#ifdef ENABLE_DEBUG_BUFFER
    ss << i << "set_value: " << set_value <<
        std::endl;
#endif

    data += set_value;

#ifdef ENABLE_DEBUG_BUFFER
    ss << i << "Data out: " << data <<
        std::endl;
#endif
}


template<typename storage_type,
         typename nested_configuration>
void basic_set( storage_type& data,
                const storage_type& new_data,
                const nested_configuration& conf ){

#ifdef ENABLE_DEBUG_BUFFER
    SIndent i("Basic set");
    ss << i << "Data in: " << data << " new_data: " << new_data <<
        std::endl;
    ss << i << "conf: " << conf << std::endl;
#endif

    storage_type base_clear_value =
        BasicGet<storage_type, nested_configuration>()(
            data, conf);

#ifdef ENABLE_DEBUG_BUFFER
    ss << i << "base clear value: " <<
        base_clear_value << std::endl;
#endif

    storage_type clear_value =
        BasicSet<storage_type, nested_configuration>()(
            base_clear_value, conf);

#ifdef ENABLE_DEBUG_BUFFER
    ss << i << "Clear value: " << clear_value << std::endl;
#endif

    data -= clear_value;

#ifdef ENABLE_DEBUG_BUFFER
    ss << i << "cleared data: " << data << std::endl;
#endif

    storage_type set_value =
        BasicSet<storage_type, nested_configuration>()(
            new_data, conf );

#ifdef ENABLE_DEBUG_BUFFER
    ss << i << "set_value: " << set_value << std::endl;
#endif

    data += set_value;

#ifdef ENABLE_DEBUG_BUFFER
    ss << i << "Data out: " << data << std::endl;
#endif
}



/////////////////////////////////////////////////////////////////////
template < typename element_descriptor >
struct Element {
    static const size_t size = element_descriptor::size;
    static const bool container_type = false;

    typedef typename GetAbstractType<
        typename element_descriptor::inner_type
    >::type inner_type;
};

template < typename super_t,
           typename base_config,
           typename element_descriptor >
struct ElementImpl {
    static const size_t size = element_descriptor::size;
    typedef Element<
       element_descriptor
    > element_type;

    typedef typename Element<
       element_descriptor
    >::inner_type standalone_type;

    typedef typename GetStorageType<super_t>::type storage_type;

    typedef CalculateConfiguration<
        base_config, ElementConfig<size>
    > conf_calc;
    typedef typename conf_calc::type nested_configuration;

    nested_configuration get_conf() const {
        super_t const& self = static_cast<super_t const&>(*this);
        return conf_calc()(ElementConfig<size>(), self.conf);
    }

    standalone_type to_standalone_type() const {
        super_t const& self = static_cast<super_t const&>(*this);
#ifdef ENABLE_DEBUG_BUFFER
        SIndent i("Element to_standalone_type");
#endif
        storage_type data =
            BasicGet<storage_type, nested_configuration>()(
                self.data, get_conf());
        return (standalone_type)data;
    }

    void clear( ) {
#ifdef ENABLE_DEBUG_BUFFER
        SIndent i("Element clear");
#endif
        super_t& self = static_cast<super_t&>(*this);
        basic_clear<storage_type, nested_configuration>(
            self.data, get_conf() );
    }

    void set_no_clear(const standalone_type& new_value) {
#ifdef ENABLE_DEBUG_BUFFER
        SIndent i("Element set_no_clear");
#endif
        super_t& self = static_cast<super_t&>(*this);
        basic_set_no_clear<storage_type, nested_configuration>(
            self.data, (storage_type)new_value, get_conf() );
    }

    void set(const standalone_type& new_value) {
#ifdef ENABLE_DEBUG_BUFFER
        SIndent i("Element set");
#endif
        super_t& self = static_cast<super_t&>(*this);
        basic_set<storage_type, nested_configuration>(
            self.data, (storage_type)new_value, get_conf() );
    }

#define PROXY_TYPE                    \
Proxy< storage_type,                  \
       Element<element_descriptor>,   \
       nested_configuration >
    PROXY_TYPE next_no_wrap() {
        super_t& self = static_cast<super_t&>(*this);
#ifdef ENABLE_DEBUG_BUFFER
        SIndent i("Element next_no_wrap");
#endif
        storage_type current_value = (storage_type)
            BasicGet<storage_type, nested_configuration>()(
                self.data, get_conf() );
        if( current_value < ( size - 1 ) )
            set( (standalone_type) (current_value + 1) );
        return get();
    }

    PROXY_TYPE next_wrap() {
        super_t& self = static_cast<super_t&>(*this);
#ifdef ENABLE_DEBUG_BUFFER
        SIndent i("Element next_wrap");
#endif
        storage_type current_value = (storage_type)
            BasicGet<storage_type, nested_configuration>()(
                self.data, get_conf() );
        if( current_value < ( size - 1 ) )
            set( (standalone_type) (current_value + 1) );
        else
            set( (standalone_type) (0) );
        return get();
    }

    PROXY_TYPE prev_no_wrap() {
        super_t& self = static_cast<super_t&>(*this);
#ifdef ENABLE_DEBUG_BUFFER
        SIndent i("Element prev_no_wrap");
#endif
        storage_type current_value = (storage_type)
            BasicGet<storage_type, nested_configuration>()(
                self.data, get_conf() );
        if( current_value > 0 )
            set( (standalone_type) (current_value - 1) );
        return get();
    }

    PROXY_TYPE prev_wrap() {
        super_t& self = static_cast<super_t&>(*this);
#ifdef ENABLE_DEBUG_BUFFER
        SIndent i("Element prev_wrap");
#endif
        storage_type current_value = (storage_type)
            BasicGet<storage_type, nested_configuration>()(
                self.data, get_conf() );
        if( current_value > 0 )
            set( (standalone_type) (current_value - 1) );
        else
            set( (standalone_type) (size - 1) );
        return get();
    }

    PROXY_TYPE get() {
        super_t& self = static_cast<super_t&>(*this);
        return PROXY_TYPE( self.data, get_conf() );
    }
#undef PROXY_TYPE

#define PROXY_TYPE                    \
Proxy< const storage_type,            \
       Element<element_descriptor>,   \
       nested_configuration >
    PROXY_TYPE get() const {
        const super_t& self = static_cast<const super_t&>(*this);
        return PROXY_TYPE( self.data, get_conf() );
    }
#undef PROXY_TYPE
};


/////////////////////////////////////////////////////////////////////
template< size_t start_size,
          template< size_t prev_size,
                    class type
          > class CalcSize,
          typename... _args >
struct CalculateSize;

template< size_t start_size,
          template< size_t prev_size,
                    class type
          > class CalcSize,
          typename head,
          typename... tail >
struct CalculateSize<
    start_size,
    CalcSize,
    head,
    tail...
> : public CalculateSize<
    CalcSize< start_size,
              head
    >::new_size,
    CalcSize,
    tail...
> { };

template< size_t start_size,
          template< size_t prev_size,
                    class type
          > class CalcSize >
struct CalculateSize<
    start_size,
    CalcSize
> {
    static const size_t size = start_size;
};

/////////////////////////////////////////////////////////////////////
template< typename Tag,      // The tag we are looking for
          typename BaseConf,
          typename TailConf,
          template< class _BaseConf,
                    class _TailConf,
                    class type > class NextConf,
          template< class _BaseConf,
                    class _TailConf,
                    class type > class ThisConf,
          typename... Tags >
struct Tag2Conf;

template< typename BaseConf,
          typename TailConf,
          template< class _BaseConf,
                    class _TailConf,
                    class _type > class ThisConf,
          typename Head>
struct DefineType {
    typedef typename Head::tag_inner_type type;

    typedef ThisConf<
        BaseConf,
        TailConf,
        type
    > conf_calc;

    typedef typename conf_calc::new_conf conf;

    conf operator()(const TailConf& tail) {
        return conf(tail);
    }
};

template< typename _tag >
struct TagNotFound {
    struct _void {};
    typedef _void conf;
    static_assert( std::is_same<_void, _tag>::value,
                   "Tag not found in type list!" );
};

template< typename Tag,
          typename BaseConf,
          typename TailConf,
          template< class _BaseConf,
                    class _TailConf,
                    class type > class NextConf,
          template< class _BaseConf,
                    class _TailConf,
                    class type > class ThisConf,
          typename Head,
          typename... Tail >
struct Tag2Conf<
    Tag, BaseConf, TailConf, NextConf, ThisConf, Head, Tail...
> : public std::conditional<
    std::is_same<typename Head::tag, Tag>::value, // condition
    DefineType< BaseConf, // conditional-then
                TailConf,
                ThisConf,
                Head
    >,
    Tag2Conf<             // conditional-else
        Tag,
        typename NextConf<
            BaseConf,
            TailConf,
            Head
        >::next_conf,
        TailConf,
        NextConf,
        ThisConf,
        Tail...
    >
>::type { };

template< typename Tag,
          typename BaseConf,
          typename TailConf,
          template< class _BaseConf,
                    class _TailConf,
                    class type > class NextConf,
          template< class _BaseConf,
                    class _TailConf,
                    class type > class ThisConf,
          typename Head >
struct Tag2Conf<
    Tag, BaseConf, TailConf, NextConf, ThisConf, Head
> : public std::conditional<
    std::is_same<typename Head::tag, Tag>::value,
    DefineType<BaseConf, TailConf, ThisConf, Head>,
    TagNotFound< Tag >
>::type { };

/////////////////////////////////////////////////////////////////////
template < typename... args>
struct EnumStruct {
    template< size_t prev_size, class type >
    struct SizeFunction {
        static const size_t new_size = type::size * prev_size;
    };

    static const size_t size = CalculateSize<
        1, SizeFunction, args...
    >::size;

    static const bool container_type = true;

    typedef EnumStruct<
        typename GetAbstractType<
            args
        >::type...
    > inner_type;
};

template < typename super_t,
           typename base_config,
           typename... args >
struct EnumStructImpl {
    typedef typename GetStorageType<super_t>::type storage_type;
    static const size_t size = EnumStruct<args...>::size;

    typedef Container<
        typename std::remove_reference<
            typename std::remove_cv<storage_type>::type
        >::type,
        typename EnumStruct<args...>::inner_type
    > standalone_type;

    typedef CalculateConfiguration<
        base_config, ElementConfig<size>
    > conf_calc;
    typedef typename conf_calc::type nested_configuration;

    nested_configuration get_conf() const {
        super_t const& self = static_cast<super_t const&>(*this);
        return conf_calc()(ElementConfig<size>(), self.conf);
    }

    typedef typename CalculateConfiguration<
        base_config, NodeElementConfig<size>
    >::type childs_config;

    template< class BaseConf, class TailConf, class type >
    struct NextConf {
        typedef StructConfig< BaseConf::base * type::size > next_conf;
    };

    template< class BaseConf, class TailConf, class type >
    struct ThisConf {
        typedef typename CalculateConfiguration<
            TailConf, BaseConf
        >::type new_conf;

        new_conf operator()(const BaseConf& tail) {
            return new_conf(tail);
        }
    };

    standalone_type to_standalone_type() const {
        super_t const& self = static_cast<super_t const&>(*this);
#ifdef ENABLE_DEBUG_BUFFER
        SIndent i("Struct to_standalone_type");
#endif
        return standalone_type (
            BasicGet<storage_type, nested_configuration>()(
                self.data, get_conf() ) );
    }

    void clear() {
#ifdef ENABLE_DEBUG_BUFFER
        SIndent i("Struct clear");
#endif
        super_t& self = static_cast<super_t&>(*this);
        basic_clear<storage_type, nested_configuration>(
            self.data, get_conf());
    }

    template< typename T >
    void set(const T& new_value,
             typename std::enable_if<
                std::is_same<
                    typename T::inner_type,
                    typename EnumStruct<args...>::inner_type
                >::value
             >::type* = 0 ) {
        // new_value may either be a proxy or a container.
        storage_type new_value_data = (storage_type)
            (new_value.to_standalone_type().data);

        super_t& self = static_cast<super_t&>(*this);

#ifdef ENABLE_DEBUG_BUFFER
        SIndent i("Struct set");
#endif
        basic_set<storage_type, nested_configuration>(
            self.data, new_value_data, get_conf() );
    }

#define TAG2CONF                 \
Tag2Conf<                        \
    tag,                         \
    StructConfig<1>,             \
    childs_config,               \
    NextConf,                    \
    ThisConf,                    \
    args...  >
#define PROXY_TYPE               \
Proxy< storage_type,             \
       typename TAG2CONF::type,  \
       typename TAG2CONF::conf >
    template<typename tag>
    PROXY_TYPE
    get() {
        super_t& self = static_cast<super_t&>(*this);
        return PROXY_TYPE( self.data, TAG2CONF()(self.conf) );
    }
#undef PROXY_TYPE

#define PROXY_TYPE               \
Proxy< const storage_type,       \
       typename TAG2CONF::type,  \
       typename TAG2CONF::conf >
    template<typename tag>
    PROXY_TYPE
    get() const {
        const super_t& self = static_cast<const super_t&>(*this);
        return PROXY_TYPE( self.data, TAG2CONF()(self.conf) );
    }
#undef PROXY_TYPE
#undef TAG2CONF
};



/////////////////////////////////////////////////////////////////////
template<size_t X, size_t P> struct StaticPowerOf {
    const static size_t result = X*StaticPowerOf<X,P-1>::result;
};
template<size_t X> struct StaticPowerOf<X,0> {
    const static size_t result = 1;
};
template<size_t X> struct StaticPowerOf<X,1> {
    const static size_t result = X;
};

template < size_t length, typename element_descriptor >
struct Array {
    typedef typename GetAbstractType<
        element_descriptor
    >::type abstract_type;

    const static size_t size =
        StaticPowerOf<abstract_type::size, length>::result;

    static const bool container_type = true;

    typedef Array<
        length,
        typename GetAbstractType<
            element_descriptor
        >::type
    > inner_type;
};

template < typename super_t,
           typename base_config,
           size_t   _length,
           typename element_descriptor >
struct ArrayImpl {
    typedef typename GetStorageType<super_t>::type storage_type;
    static const size_t size =
        Array<_length, element_descriptor>::size;
    static const size_t length = _length;

    typedef typename Array<
        length, element_descriptor
    >::inner_type inner_type;

    typedef Container<
        typename std::remove_reference<
            typename std::remove_cv<storage_type>::type
        >::type,
        typename Array<length, element_descriptor>::inner_type
    > standalone_type;

    typedef CalculateConfiguration<
        base_config, ElementConfig<size>
    > calc_conf;

    typedef typename calc_conf::type nested_configuration;

    nested_configuration get_conf() const {
        super_t const& self = static_cast<super_t const&>(*this);
        return calc_conf()(ElementConfig<size>(), self.conf);
    }

    standalone_type to_standalone_type() const {
        super_t const& self = static_cast<super_t const&>(*this);
#ifdef ENABLE_DEBUG_BUFFER
        SIndent i("Array to_standalone_type");
#endif
        return (standalone_type)
            BasicGet<storage_type, nested_configuration>()(
                self.data, get_conf() );
    }

    template< typename T >
    void set(const T& new_value,
             typename std::enable_if<
                std::is_same<
                    typename T::inner_type,
                    inner_type
                >::value
             >::type* = 0 ) {
        // new_value may either be a proxy or a container.
        storage_type new_value_data = (storage_type)
            (new_value.to_standalone_type().data);

        super_t& self = static_cast<super_t&>(*this);

#ifdef ENABLE_DEBUG_BUFFER
        SIndent i("Array set");
#endif
        basic_set<storage_type, nested_configuration>(
            self.data, new_value_data, get_conf() );
    }

#define ARRAY_CONF                   \
ArrayConfig<                         \
    length,                          \
    element_descriptor::size         \
>
#define CONF_CALC                    \
CalculateConfiguration<              \
    typename CalculateConfiguration< \
        base_config,                 \
        NodeElementConfig<size>      \
    >::type,                         \
    ARRAY_CONF                       \
>
#define PROXY_TYPE                   \
Proxy< CONST storage_type,           \
       element_descriptor,           \
       typename CONF_CALC ::type >
#define GET_RUNTIME_INDEX                                         \
    PROXY_TYPE                                                    \
    get( size_t idx ) CONST {                                     \
        assert (idx < length);                                    \
        CONST super_t& self = static_cast<CONST super_t&>(*this); \
        return PROXY_TYPE(                                        \
            self.data,                                            \
            CONF_CALC()(ARRAY_CONF(idx), self.conf) );            \
    }

#define CONST
    GET_RUNTIME_INDEX
#undef CONST
#define CONST const
    GET_RUNTIME_INDEX
#undef CONST

#undef GET_RUNTIME_INDEX
#undef ARRAY_CONF
#undef PROXY_TYPE
#undef CONF_CALC

#define BASE                         \
StaticPowerOf<                       \
    element_descriptor::size, index  \
>::result
#define CONF_CALC                    \
CalculateConfiguration<              \
    typename CalculateConfiguration< \
        base_config,                 \
        NodeElementConfig<size>      \
    >::type,                         \
    StructConfig< BASE >             \
>
#define CONF typename CONF_CALC ::type
#define PROXY_TYPE                   \
Proxy< storage_type,                 \
       element_descriptor,           \
       CONF >
    template< size_t index >
    PROXY_TYPE
    get() {
        super_t& self = static_cast<super_t&>(*this);
        return PROXY_TYPE(self.data, CONF_CALC()(
                StructConfig< BASE >(), self.conf) );
    }
#undef PROXY_TYPE

#define PROXY_TYPE                   \
Proxy< const storage_type,           \
        element_descriptor,          \
        CONF >
    template< size_t index >
    PROXY_TYPE
    get() const {
        const super_t& self = static_cast<const super_t&>(*this);
        return PROXY_TYPE(self.data, CONF_CALC()(
                StructConfig< BASE >(), self.conf) );
    }
#undef PROXY_TYPE
#undef CONF_CALC
#undef CONF
#undef BASE
};




/////////////////////////////////////////////////////////////////////
template < typename... args>
struct Union {
    template< size_t prev_size, class type >
    struct SizeFunction {
        static const size_t new_size = type::size + prev_size;
    };

    static const size_t size = CalculateSize<
        0, SizeFunction, args...
    >::size;

    static const bool container_type = true;

    typedef Union<
        typename GetAbstractType<
            args
        >::type...
    > inner_type;
};

template < typename... args >
struct UnionVisitor2;

template < typename base,
           typename head,
           typename... tail >
struct UnionVisitor2< base, head, tail... > :
public UnionVisitor2< base, tail... > {
////// DECLARE IMPLEMENTATIONS OF NORMAL VISITOR ////////////////////
#define NORMAL_VISITOR                                              \
template< size_t offset,                                            \
          typename storage_type,                                    \
          typename Visitor,                                         \
          typename... VisitorArgs >                                 \
auto __visit ( CONST storage_type& value,                           \
               Visitor& visitor,                                    \
               VisitorArgs&&... visitor_args ) CONST ->             \
decltype( std::declval<Visitor>().template operator()<              \
            typename head::tag                                      \
          > ( std::declval<                                         \
                Proxy< CONST storage_type,                          \
                       typename head::tag_inner_type,               \
                       Terminal                                     \
                >                                                   \
              >(),                                                  \
              std::declval<VisitorArgs&&>()...                      \
            )                                                       \
)                                                                   \
{                                                                   \
    CONST base& self = static_cast<CONST base&>(*this);             \
    if( value >= ( head::size + offset ) ) {                        \
        return                                                      \
            UnionVisitor2< base, tail... >::template __visit<       \
                offset + head::size, storage_type,                  \
                Visitor, VisitorArgs&&...                           \
        >( value,                                                   \
           visitor,                                                 \
           std::forward<VisitorArgs>(visitor_args)...);             \
    } else {                                                        \
        auto union_value =                                          \
            self.template union_get<typename head::tag>();          \
        return                                                      \
            visitor.template operator()<typename head::tag>(        \
                union_value,                                        \
                std::forward<VisitorArgs>(visitor_args)...);        \
    }                                                               \
}

////// DECLARE IMPLEMENTATIONS OF CLASS VISITOR /////////////////////
#define CLASS_VISITOR                                               \
template< size_t offset,                                            \
          typename storage_type,                                    \
          template<class... _tag> class Visitor,                    \
          typename... VisitorArgs >                                 \
auto __visit_class( CONST storage_type& value,                      \
                    VisitorArgs&&... visitor_args ) CONST ->        \
decltype( std::declval<Visitor<typename head::tag> >()(             \
              std::declval< Proxy< CONST storage_type,              \
                                   typename head::tag_inner_type,   \
                                   Terminal >& >(),                 \
              std::declval<VisitorArgs&&>()... ) )                  \
{                                                                   \
    if( value >= ( head::size + offset ) ) {                        \
        return                                                      \
            UnionVisitor2< base, tail... >::template                \
                __visit_class<                                      \
                    offset + head::size,                            \
                    storage_type,                                   \
                    Visitor,                                        \
                    VisitorArgs&&...                                \
                >( value,                                           \
                   std::forward<VisitorArgs>(visitor_args)...);     \
    } else {                                                        \
        CONST base& self = static_cast<CONST base&>(*this);         \
        Visitor<typename head::tag> v;                              \
        auto union_value =                                          \
            self.template union_get<typename head::tag>();          \
        return v( union_value,                                      \
                  std::forward<VisitorArgs>(visitor_args)...);      \
    }                                                               \
}

#define CONST const
CLASS_VISITOR;
NORMAL_VISITOR;
#undef CONST
#define CONST
CLASS_VISITOR;
NORMAL_VISITOR;
#undef CONST
#undef CLASS_VISITOR
#undef NORMAL_VISITOR
};

template < typename base,
           typename head >
struct UnionVisitor2 < base, head > {
    typedef typename head::tag SomeTag;

////// DECLARE IMPLEMENTATIONS OF NORMAL VISITOR ////////////////////
#define NORMAL_VISITOR                                              \
template< size_t offset,                                            \
          typename storage_type,                                    \
          typename Visitor,                                         \
          typename... VisitorArgs >                                 \
auto __visit ( CONST storage_type& value,                           \
               Visitor& visitor,                                    \
               VisitorArgs&&... visitor_args ) CONST ->             \
decltype( std::declval<Visitor>().template operator()<              \
            typename head::tag                                      \
          > ( std::declval<                                         \
                Proxy< CONST storage_type,                          \
                       typename head::tag_inner_type,               \
                       Terminal                                     \
                >                                                   \
              >(),                                                  \
              std::declval<VisitorArgs&&>()...                      \
            )                                                       \
)                                                                   \
{                                                                   \
    CONST base& self = static_cast<CONST base&>(*this);             \
    if( value >= ( head::size + offset ) ) {                        \
        std::cerr << "union error" << std::endl;                    \
        abort();                                                    \
    }                                                               \
    auto union_value =                                              \
        self.template union_get<typename head::tag>();              \
    return                                                          \
        visitor.template operator()<typename head::tag>(            \
            union_value,                                            \
            std::forward<VisitorArgs>(visitor_args)... );           \
}


////// DECLARE IMPLEMENTATIONS OF CLASS VISITOR /////////////////////
#define CLASS_VISITOR                                               \
template< size_t offset,                                            \
          typename storage_type,                                    \
          template<class... _tag> class Visitor,                    \
          typename... VisitorArgs >                                 \
auto __visit_class( CONST storage_type& value,                      \
                   VisitorArgs&&... visitor_args ) CONST ->         \
decltype( std::declval<Visitor<typename head::tag> >()(             \
              std::declval< Proxy< CONST storage_type,              \
                                   typename head::tag_inner_type,   \
                                   Terminal >& >(),                 \
              std::declval<VisitorArgs&&>()... ) )                  \
{                                                                   \
    if( value >= ( head::size + offset ) ) {                        \
        std::cerr << "union error" << std::endl;                    \
        abort();                                                    \
    }                                                               \
    CONST base& self = static_cast<CONST base&>(*this);             \
    Visitor<typename head::tag> v;                                  \
    auto union_value =                                              \
        self.template union_get<typename head::tag>();              \
    return v( union_value,                                          \
              std::forward<VisitorArgs>(visitor_args)... );         \
}

#define CONST const
CLASS_VISITOR;
NORMAL_VISITOR;
#undef CONST
#define CONST
CLASS_VISITOR;
NORMAL_VISITOR;
#undef CONST
#undef CLASS_VISITOR
#undef NORMAL_VISITOR
};

template < typename super_t,
           typename base_config,
           typename... args >
struct EnumUnionImpl :
public UnionVisitor2 <
    EnumUnionImpl<super_t, base_config, args...>, args...
> {
    typedef UnionVisitor2 <
        EnumUnionImpl<super_t, base_config, args...>, args...
    > Base;

    typedef typename GetStorageType<super_t>::type storage_type;
    static const size_t size = Union<args...>::size;

    typedef Container<
        typename std::remove_reference<
            typename std::remove_cv<storage_type>::type
        >::type,
        typename Union<args...>::inner_type
    > standalone_type;

    typedef CalculateConfiguration<
        base_config, ElementConfig<size>
    > conf_calc;
    typedef typename conf_calc::type nested_configuration;

    nested_configuration get_conf() const {
        super_t const& self = static_cast<super_t const&>(*this);
        return conf_calc()(ElementConfig<size>(), self.conf);
    }

    typedef typename CalculateConfiguration<
        base_config, NodeElementConfig<size>
    >::type childs_config;

    template< class BaseConf, class TailConf, class type >
    struct NextConf {
        typedef UnionConfig<
            0, BaseConf::offset + type::size
        > next_conf;
    };

    template< class BaseConf, class TailConf, class type >
    struct ThisConf {
        typedef typename CalculateConfiguration<
            TailConf, UnionConfig<type::size, BaseConf::offset>
        >::type new_conf;
    };

    standalone_type to_standalone_type() const {
        super_t const& self = static_cast<super_t const&>(*this);
#ifdef ENABLE_DEBUG_BUFFER
        SIndent i("Union to_standalone_type");
#endif
        return standalone_type (
            BasicGet<storage_type, nested_configuration>()(
                self.data, get_conf() ) );
    }

    void clear() {
        super_t& self = static_cast<super_t&>(*this);
#ifdef ENABLE_DEBUG_BUFFER
        SIndent i("Union clear");
#endif
        basic_clear<storage_type, nested_configuration>(
            self.data, get_conf() );
    }

    template< typename T >
    void set(const T& new_value,
             typename std::enable_if<
                std::is_same<
                    typename T::inner_type,
                    typename Union<args...>::inner_type
                >::value
             >::type* = 0 ) {
        // new_value may either be a proxy or a container.
        storage_type new_value_data = (storage_type)
            (new_value.to_standalone_type().data);

        super_t& self = static_cast<super_t&>(*this);

#ifdef ENABLE_DEBUG_BUFFER
        SIndent i("Union set");
#endif
        basic_set<storage_type, nested_configuration>(
            self.data, new_value_data, get_conf() );
    }

#define TAG2CONF           \
typename Tag2Conf<         \
    TAG,                   \
    UnionConfig<0, 0>,     \
    childs_config,         \
    NextConf,              \
    ThisConf,              \
    args...  >
#define PROXY_TYPE         \
Proxy< CONST storage_type, \
       TAG2CONF::type,     \
       TAG2CONF::conf >

#define CONST
#define TAG tag
    template<typename tag>
    PROXY_TYPE
    union_clear() {
        super_t& self = static_cast<super_t&>(*this);
#ifdef ENABLE_DEBUG_BUFFER
        SIndent i("Union clear");
#endif
        //typedef TAG2CONF::conf ConfType;
        //const size_t temporary_data = ConfType::offset;

        basic_set<storage_type, nested_configuration>(
            self.data, (storage_type)0, get_conf());

        auto xx = union_get<tag>();
        return xx;
    }
#undef CONST

////// DECLARE IMPLEMENTATIONS OF UNION_GET /////////////////////////
#define UNION_GET                                                   \
template<typename tag>                                              \
PROXY_TYPE                                                          \
union_get() CONST {                                                 \
    CONST super_t& self = static_cast<CONST super_t&>(*this);       \
    return PROXY_TYPE(                                              \
        self.data, TAG2CONF::conf(self.conf) );                     \
}
#define CONST const
UNION_GET
#undef CONST
#define CONST
UNION_GET
#undef CONST
#undef UNION_GET

////// DECLARE IMPLEMENTATIONS OF STATIC_VISITOR ////////////////////
#ifdef ENABLE_DEBUG_BUFFER
#define DEBUG_BUF_PRINT SIndent i("Union_get");
#else
#define DEBUG_BUF_PRINT
#endif

#undef  TAG
#define TAG typename Base::SomeTag
#define STATIC_VISITOR                                              \
template< typename Visitor, typename... VisitorArgs >               \
auto                                                                \
static_visit( Visitor& visitor,                                     \
              VisitorArgs&&... visitor_args ) CONST ->              \
decltype( std::declval<Visitor>().template operator()<TAG>(         \
        std::declval<PROXY_TYPE>(),                                 \
        std::declval<VisitorArgs&&>()...) )                         \
{                                                                   \
    super_t CONST& self = static_cast<super_t CONST&>(*this);       \
    DEBUG_BUF_PRINT                                                 \
    storage_type value = BasicGet<                                  \
        storage_type,                                               \
        nested_configuration                                        \
    >()(self.data, get_conf() );                                    \
    return this->template __visit<0>(                               \
        value, visitor,                                             \
        std::forward<VisitorArgs>(visitor_args)... );               \
}

////// DECLARE IMPLEMENTATION OF STATIC_VISITOR_CLASS ///////////////
#define STATIC_VISITOR_CLASS                                        \
template< template<class... _tag> class Visitor,                    \
          typename... VisitorArgs >                                 \
auto                                                                \
static_visit_class( VisitorArgs&&... visitor_args ) CONST ->        \
decltype(                                                           \
    std::declval< Visitor<typename Base::SomeTag> >().operator()(   \
        std::declval<PROXY_TYPE&>(),                                \
        std::declval<VisitorArgs&&>()...) )                         \
{                                                                   \
    CONST super_t & self = static_cast<CONST super_t &>(*this);     \
    DEBUG_BUF_PRINT                                                 \
    storage_type value = BasicGet<                                  \
        storage_type,                                               \
        nested_configuration                                        \
    >()(self.data, get_conf() );                                    \
    return this->template __visit_class<                            \
        0, storage_type, Visitor, VisitorArgs&&...                  \
    >(  value,                                                      \
        std::forward<VisitorArgs>(visitor_args)... );               \
}

#define CONST const
STATIC_VISITOR
STATIC_VISITOR_CLASS
#undef CONST
#define CONST
STATIC_VISITOR
STATIC_VISITOR_CLASS
#undef CONST
#undef STATIC_VISITOR
#undef STATIC_VISITOR_CLASS
#undef DEBUG_BUF_PRINT

#undef TAG
#undef PROXY_TYPE
#undef TAG2CONF
};

template< typename _type >
struct GetAbstractType {
    typedef _type type;
};
template< typename storage_type, typename inner_type >
struct GetAbstractType <
    Container< storage_type, inner_type >
> {
    typedef typename GetAbstractType<
        inner_type
    >::type type;
};
template< typename element_descriptor >
struct GetAbstractType <
    Element< element_descriptor >
> {
    typedef Element< element_descriptor > type;
};

template< typename inner_type  >
struct GetAbstractType <
    Element< Element< inner_type > >
> {
    typedef typename GetAbstractType<
        Element< inner_type >
    >::type type;
};
template< typename _tag,
          typename _inner_type >
struct GetAbstractType < Tag< _tag, _inner_type > > {
    typedef Tag<
        _tag,
        typename GetAbstractType< _inner_type >::type
    > type;
};
template< typename _tag,
          typename _inner_type >
struct GetAbstractType < Tag< _tag, Element<_inner_type> > > {
    typedef Tag<
        _tag,
        typename GetAbstractType< _inner_type >::type
    > type;
};
template< typename _tag,
          typename storage_type,
          typename inner_type >
struct GetAbstractType <
    Tag< _tag, Container< storage_type, inner_type > >
> {
    typedef typename GetAbstractType<
        Tag<
            _tag,
            typename GetAbstractType<
                inner_type
            >::type
        >
    >::type type;
};
template< typename... args >
struct GetAbstractType <
    EnumStruct< args... >
> {
    typedef EnumStruct<
        typename GetAbstractType<
            args
        >::type...
    > type;
};
template< size_t length,
          typename element_descriptor >
struct GetAbstractType <
    Array< length, element_descriptor >
> {
    typedef Array<
        length,
        typename GetAbstractType<
            element_descriptor
        >::type
    > type;
};
template< typename... args >
struct GetAbstractType <
    Union< args... >
> {
    typedef Union<
        typename GetAbstractType<
            args
        >::type...
    > type;
};

// Defines implementation for anonymous element
template< typename super_t,
          typename base_config,
          typename element_descriptor >
struct GetImplementation<
    super_t,
    base_config,
    Element<element_descriptor>
> {
    typedef ElementImpl<
        super_t,
        base_config,
        typename GetAbstractType<
            element_descriptor
        >::type
    > implementation_type;
};

// Defines implementation for anonymous EnumStruct
template< typename super_t,
          typename base_config,
          typename... args>
struct GetImplementation<
    super_t,
    base_config,
    EnumStruct<args...>
> {
    typedef EnumStructImpl<
        super_t,
        base_config,
        typename GetAbstractType<
            args
        >::type...
    > implementation_type;
};
// Defines implementation for anonymous Array
template< typename super_t,
          typename base_config,
          size_t   length,
          typename element_descriptor>
struct GetImplementation <
    super_t,
    base_config,
    Array<length, element_descriptor>
> {
    typedef ArrayImpl <
        super_t,
        base_config,
        length,
        typename GetAbstractType<
            element_descriptor
        >::type
    > implementation_type;
};
// Defines implementation for anonymous Union
template< typename super_t,
          typename base_config,
          typename... args>
struct GetImplementation<
    super_t,
    base_config,
    Union< args... >
> {
    typedef EnumUnionImpl<
        super_t,
        base_config,
        typename GetAbstractType<
            args
        >::type...
    > implementation_type;
};


/////////////////////////////////////////////////////////////////////
#define DERIVED_CLASS                                               \
GetImplementation<                                                  \
    Proxy< _storage_type,                                           \
           typename GetAbstractType<_inner_type>::type,             \
           _nested_configuration >,                                 \
    _nested_configuration,                                          \
    typename GetAbstractType<_inner_type>::type                     \
>::implementation_type
template< typename _storage_type, // st
          typename _inner_type, //  Container<st, inner>
          typename _nested_configuration > // Terminal
struct Proxy : public DERIVED_CLASS
{
    typedef _storage_type storage_type;
    typedef typename GetAbstractType<_inner_type>::type inner_type;
    typedef Proxy< storage_type,
                   inner_type,
                   _nested_configuration > proxy_t;
    typedef Proxy< const storage_type,
                   inner_type,
                   _nested_configuration > const_proxy_t;
    typedef typename DERIVED_CLASS Base;
    typedef typename Base::standalone_type inner_standalone_type;
#undef DERIVED_CLASS

    Proxy( storage_type& _data,
           _nested_configuration _conf ) :
        data(_data),
        conf(_conf) { }

    storage_type& data;
    _nested_configuration conf;

    const proxy_t& get_proxy() const {
        return *this;
    }

    proxy_t& get_proxy() {
        return *this;
    }

    operator const inner_standalone_type () const {
#ifdef ENABLE_DEBUG_BUFFER
        ss << "Converted: " << conf << " " << data << std::endl;
#endif
        return this->to_standalone_type();
    }

    template<typename T>
    proxy_t& operator=( const T& rhs ){
        __set(rhs);
        return *this;
    }

protected:
    template<typename T>
    void __set( const T& rhs ) {
        Base::set(rhs);
    }
};

#define DERIVED_CLASS                             \
Proxy <                                           \
    _storage_type,                                \
    typename GetAbstractType<_inner_type>::type,  \
    Terminal                                      \
>
template< typename _storage_type, typename _inner_type >
struct Container : public DERIVED_CLASS
{
    typedef DERIVED_CLASS Base;
#undef DERIVED_CLASS

    typedef Container<
        _storage_type,
        typename GetAbstractType<
            _inner_type
        >::type
    > type;

    Container() : Base(data, Terminal() ), data(0) { }

    Container( const Container& rhs ) :
        Base(data, Terminal() ), data( rhs.data ) { }

    template<typename Rhs>
    Container( const Rhs& rhs ) : Base(data, Terminal() ), data(0) {
        Base::__set(rhs); // not optimal!
    }

    explicit Container( _storage_type value ) :
        Base(data, Terminal() ), data( value ) { }

    Container& operator=( const Container& rhs ){
        data = rhs.data;
        return *this;
    }

    template<typename Rhs>
    Container& operator=( const Rhs& rhs ){
        Base::__set(rhs);
        return *this;
    }

    struct const_iterator {
        typedef Container<_storage_type, _inner_type> Type;

        Type e;
        const_iterator ( const Type& _e ) : e(_e) { }

        const Type& operator*() const { return e; }
        const Type* operator->() const { return &(operator*()); }
        const_iterator& operator++ () { e.data++; return *this; }
        const_iterator operator+( ptrdiff_t rhs ) const {
            return const_iterator(
                Type( e.data + (_storage_type)rhs ));
        }
        const_iterator& operator+=( ptrdiff_t rhs ) {
            e.data += rhs;
            return *this;
        }

        bool operator<( const const_iterator& rhs) const {
            return e.data < rhs.e.data;
        }

        bool operator==(const const_iterator& rhs) const {
            return e.data == rhs.e.data;
        }

        bool operator!=(const const_iterator& rhs) const {
            return e.data != rhs.e.data;
        }
    };
    static const_iterator begin() {
        return const_iterator(
            Container<_storage_type, _inner_type>() );
    }
    static const_iterator end() {
        return const_iterator(
            Container<_storage_type, _inner_type>(
                (_storage_type) Container<
                    _storage_type, _inner_type
                >::size )
        );
    }

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
        ar & data;
    }

    _storage_type data;
};

template< typename _storage_type,
          typename _inner_type >
bool operator==(
    const Container<_storage_type, _inner_type>& lhs,
    const Container<_storage_type, _inner_type>& rhs
) {
    return lhs.data == rhs.data;
}

template< typename _storage_type,
          typename _inner_type >
bool operator!=(
    const Container<_storage_type, _inner_type>& lhs,
    const Container<_storage_type, _inner_type>& rhs
) {
    return lhs.data != rhs.data;
}

template< typename _storage_type,
          typename _inner_type >
bool operator<(
    const Container<_storage_type, _inner_type>& lhs,
    const Container<_storage_type, _inner_type>& rhs
) {
    return lhs.data < rhs.data;
}

// ARRAY PROXY PRINTER //////////////////////////////////////////////
template< typename StructType,
          size_t   length,
          typename element_descriptor,
          size_t   reverse_index >
struct ArrayPrinter;

template< typename ArrayType,
          size_t   length,
          typename element_descriptor >
struct ArrayPrinter< ArrayType, length, element_descriptor, 1 > {
    void operator()(std::ostream& o, const ArrayType& v) {
        o << v.template get< length - 1 >();
    }
};

template< typename ArrayType,
          size_t   length,
          typename element_descriptor,
          size_t   reverse_index >
struct ArrayPrinter : private ArrayPrinter<
    ArrayType, length, element_descriptor, reverse_index - 1
> {
    typedef ArrayPrinter<
        ArrayType, length, element_descriptor, reverse_index - 1
    > Base;

    void operator()(std::ostream& o, const ArrayType& v) {
        o << v.template get< length - reverse_index >() << ", ";
        Base::operator()(o, v);
    }
};

#define PROXY_ARRAY_TYPE      \
Proxy<                        \
    _storage_type,            \
    Array<                    \
        _length,              \
        _element_descriptor   \
    >,                        \
    _nested_configuration     \
>
template< typename _storage_type,
          typename _nested_configuration,
          size_t   _length,
          typename _element_descriptor >
std::ostream& operator<<( std::ostream& o,
                          const PROXY_ARRAY_TYPE& v ) {
#ifdef ENABLE_DEBUG_BUFFER
    SIndent i("ARRAY-PRINT");
#endif
    o << "[";
    //o << "--" << v.conf << "@" << v.data << "--";
    ArrayPrinter<
        PROXY_ARRAY_TYPE, _length, _element_descriptor, _length
    > array_printer;
    array_printer( o, v );
    o << "]";
    return o;
}
#undef PROXY_Array_TYPE


// STRUCT PROXY PRINTER /////////////////////////////////////////////
template< typename StructType,
          typename... _tags>
struct StructPrinter;

template< typename StructType,
          typename _tag>
struct StructPrinter< StructType, _tag> {
    void operator()(std::ostream& o, const StructType& v) {
        o << typename _tag::tag() << ":" <<
            v.template get<typename _tag::tag>();
    }
};

template< typename StructType,
          typename _tag,
          typename... _tags>
struct StructPrinter<StructType, _tag, _tags...> :
private StructPrinter<StructType, _tags... > {
    typedef StructPrinter<StructType, _tags... > Base;
    void operator()(std::ostream& o, const StructType& v) {
        o << typename _tag::tag() << ":" <<
            v.template get<typename _tag::tag>() << ", ";
        Base::operator()(o, v);
    }
};

#define PROXY_STRUCT_TYPE \
Proxy<                    \
    _storage_type,        \
    EnumStruct<           \
        _tags...          \
    >,                    \
    _nested_configuration \
>
template< typename _storage_type,
          typename _nested_configuration,
          typename... _tags >
std::ostream& operator<<( std::ostream& o,
                          const PROXY_STRUCT_TYPE& v ) {
#ifdef ENABLE_DEBUG_BUFFER
    SIndent i("STRUCT-PRINT");
#endif
    o << "{";
    StructPrinter<PROXY_STRUCT_TYPE, _tags...> struct_printer;
    struct_printer( o, v );
    o << "}";
    return o;
}
#undef PROXY_STRUCT_TYPE

template<typename keyset>
struct UnionCategory {
    template<typename tag, typename Arg>
    typename keyset::inner_type
    operator()( const Arg& ) {
        return keyset::template to_enum<tag>();
    }
};

template<typename keyset, typename Union>
typename keyset::inner_type category( const Union& u ) {
    UnionCategory<keyset> handler;
    return u.static_visit(handler);
}

// UNION PROXY PRINTER /////////////////////////////////////////////
struct UnionPrinter{
    template<typename tag, typename Arg>
    std::ostream&
    operator()(Arg arg, std::ostream& o) {
        o << tag() /*<< "--" << arg.conf << "@" << arg.data << "--"*/
            << ":" << arg.to_standalone_type();
        return o;
    }
};


#define PROXY_UNION_TYPE  \
Proxy<                    \
    _storage_type,        \
    Union<                \
        _tags...          \
    >,                    \
    _nested_configuration \
>
template< typename _storage_type,
          typename _nested_configuration,
          typename... _tags >
std::ostream& operator<<( std::ostream& o,
                          const PROXY_UNION_TYPE& v ) {
#ifdef ENABLE_DEBUG_BUFFER
    SIndent i("UNION-PRINT");
#endif

    o << "<";
    UnionPrinter p;
    v.static_visit(p, o);
    o << ">";
    return o;
}
#undef PROXY_STRUCT_TYPE


static const std::string __indent("    ");

#define PROXY                \
Proxy< _storage_type,        \
       Element<_inner_type>, \
       _base_config >
template< typename _storage_type,
          typename _inner_type,
          typename _base_config >
void __print_details( std::ostream& o,
                      std::string indent,
                      const PROXY& v ) {
    typedef typename GetImplementation<
        PROXY, _base_config, Element<_inner_type>
    >::implementation_type impl_type;

    typedef typename PROXY ::nested_configuration
        _nested_configuration;
#undef PROXY

    o << "Element proxy: Size: " <<
        impl_type::size << ", " <<
        v.get_conf();
}

template< typename _proxy_type,
          typename... args >
struct ____print_details_union_childs;

template< typename _proxy_type,
          typename head,
          typename... args >
struct ____print_details_union_childs<
    _proxy_type, head, args...
> : private ____print_details_union_childs<
    _proxy_type, args...
> {
    void operator( )( std::ostream& o,
                      std::string indent,
                      const _proxy_type& p ) {
        o << indent << "\"" << typename head::tag() << "\" ";
        __print_details(o, indent + __indent,
                        p.template union_get<typename head::tag>());
        o << std::endl;
        ____print_details_union_childs<
            _proxy_type,
            args...
        >::operator()(o, indent, p);
    }
};

template< typename _proxy_type,
          typename head >
struct ____print_details_union_childs<_proxy_type, head> {
    void operator( )( std::ostream& o,
                      std::string indent,
                      const _proxy_type& p ) {
        o << indent << "\"" << typename head::tag() << "\" ";
        __print_details(o, indent + __indent,
                        p.template union_get<typename head::tag>());
        o << std::endl;
    }
};

#define PROXY            \
Proxy< _storage_type,    \
       Union< args... >, \
       _base_config >
template< typename _storage_type,
          typename _base_config,
          typename... args >
void __print_details( std::ostream& o,
                      std::string indent,
                      const PROXY& v ) {
    typedef typename GetImplementation<
        PROXY, _base_config, Union< args... >
    >::implementation_type impl_type;

    typedef typename PROXY ::nested_configuration
        _nested_configuration;

    o << "Union proxy: Size: " <<
        impl_type::size << ", " <<
        v.get_conf() << std::endl;

    ____print_details_union_childs<PROXY, args...>()(
        o, indent + __indent, v );

#undef PROXY
}

template< typename _proxy_type,
          typename... args >
struct ____print_details_struct_childs;

template< typename _proxy_type,
          typename head,
          typename... args >
struct ____print_details_struct_childs<
    _proxy_type, head, args...
> : private ____print_details_struct_childs<
    _proxy_type, args...
> {
    void operator( )( std::ostream& o,
                      std::string indent,
                      const _proxy_type& p ) {
        o << indent << "\"" << typename head::tag() << "\" ";
        __print_details(o, indent ,
                        p.template get<typename head::tag>());
        o << std::endl;
        ____print_details_struct_childs<
            _proxy_type,
            args...
        >::operator()(o, indent, p);
    }
};

template< typename _proxy_type,
          typename head >
struct ____print_details_struct_childs<_proxy_type, head> {
    void operator( )( std::ostream& o,
                      std::string indent,
                      const _proxy_type& p ) {
        o << indent << "\"" << typename head::tag() << "\" ";
        __print_details(o, indent ,
                        p.template get<typename head::tag>());
        o << std::endl;
    }
};

#define PROXY               \
Proxy< _storage_type,       \
       EnumStruct<args...>, \
       _base_config>
template< typename _storage_type,
          typename _base_config,
          typename... args >
void __print_details( std::ostream& o,
                      std::string indent,
                      const PROXY& v ) {
    typedef typename GetImplementation<
        PROXY, _base_config, EnumStruct<args...>
    >::implementation_type impl_type;

    typedef typename PROXY ::nested_configuration
        _nested_configuration;

    o << "EnumStruct proxy: Size: " <<
        impl_type::size << ", " <<
        v.get_conf() << std::endl;

    ____print_details_struct_childs<PROXY, args...>()(
        o, indent + __indent, v );

#undef PROXY
}

template< typename _proxy_type,
          size_t length,
          size_t reverse_index >
struct __print_details_array_childs;

template< typename _proxy_type,
          size_t length,
          size_t reverse_index >
struct __print_details_array_childs {
    void operator( )( std::ostream& o,
                      std::string indent,
                      const _proxy_type& p ) {
        o << indent << "[" << length - reverse_index << "] ";
        __print_details(o, indent,
                        p.template get< length - reverse_index >());
        o << std::endl;
        __print_details_array_childs<
            _proxy_type,
            length,
            reverse_index - 1
        >next;
        next(o, indent, p);
    }
};

template< typename _proxy_type,
          size_t length >
struct __print_details_array_childs< _proxy_type, length, 1 > {
    void operator( )( std::ostream& o,
                      std::string indent,
                      const _proxy_type& p ) {
        o << indent << "[" << length - 1 << "] ";
        __print_details(o, indent ,
                        p.template get< length - 1 >());
        o << std::endl;
    }
};

#define PROXY                               \
Proxy< _storage_type,                       \
       Array<_length, _element_descriptor>, \
       _base_config>
template< typename _storage_type,
          typename _base_config,
          size_t   _length,
          typename _element_descriptor >
void __print_details( std::ostream& o,
                      std::string indent,
                      const PROXY& v ) {
    typedef typename GetImplementation<
        PROXY, _base_config, Array<_length, _element_descriptor>
    >::implementation_type impl_type;

    typedef typename PROXY ::nested_configuration
        _nested_configuration;

    o << "Array proxy: length: " << _length << " Size: " <<
        impl_type::size << ", " << v.get_conf() <<
        std::endl;

    __print_details_array_childs<
        PROXY, _length, _length
    >()( o, indent + __indent, v );
#undef PROXY
}

#define CONTAINER Container<_storage_type, _inner_type>
template< typename _storage_type, typename _inner_type >
void __print_details( std::ostream& o,
                      std::string indent,
                      const CONTAINER& v ) {
    typedef CONTAINER Container;
#undef CONTAINER

    typedef typename Container::Base ContainerProxy;
    typedef const ContainerProxy& ProxyRef;
    ProxyRef p = static_cast<ProxyRef>(v);

    o << "@CONTAINER@" << std::endl;
    o << (indent + __indent);
    __print_details(o, indent + __indent, p);
    o << std::endl;
}

#define CONTAINER Container<_storage_type, _inner_type>
template< typename _storage_type, typename _inner_type >
void print_details( const CONTAINER& v ) {
    typedef CONTAINER Container;
#undef CONTAINER
    __print_details(std::cout, std::string(""), v);
}

// Default tag's are elements other cases must be specialized
template< typename _tag,
          typename _inner_type >
struct Tag :
public Element< _inner_type >  {
    typedef Element< _inner_type > tag_inner_type;
    typedef _tag tag;
};
// Specialization of tag with inner element
template< typename _tag,
          typename _inner_type >
struct Tag< _tag, Element< _inner_type > > :
public Element< _inner_type > {
    typedef Element< _inner_type > tag_inner_type;
    typedef _tag tag;
};
// Specialization of tag with inner EnumStruct
template< typename _tag,
          typename... args >
struct Tag< _tag, EnumStruct< args... > > :
public EnumStruct< typename GetAbstractType<args>::type... > {
    typedef EnumStruct<
        typename GetAbstractType<
            args
        >::type...
    > tag_inner_type;
    typedef _tag tag;
};
// Specialization of tag with inner Array
template< typename _tag,
          size_t   length,
          typename element_descriptor >
struct Tag< _tag, Array< length, element_descriptor > > :
public Array<
    length,
    typename GetAbstractType<element_descriptor>::type
> {
    typedef Array<
        length,
        typename GetAbstractType<element_descriptor>::type
    > tag_inner_type;
    typedef _tag tag;
};
// Specialization of tag with inner Union
template< typename _tag,
          typename... args >
struct Tag< _tag, Union< args... > > :
public Union< typename GetAbstractType<args>::type... > {
    typedef Union<
        typename GetAbstractType<args>::type...
    > tag_inner_type;
    typedef _tag tag;
};
// Specialization of tag with inner Container
template< typename _tag,
          typename _storage_type,
          typename _inner_type >
struct Tag< _tag, Container< _storage_type, _inner_type > > :
public Tag< _tag, _inner_type > { };

#endif


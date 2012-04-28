#include <assert.h>
#include <string>
#include <ostream>
#include <sstream>
#include <iostream>
#include <iterator>
#include <type_traits>
#include <boost/mpl/vector.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/seq/reverse.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>

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
        struct Union { };                                           \
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

template< typename super_t,
          typename base_config,
          typename type >
struct GetImplementation;

template< typename type >
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

template< typename _storage_type,
          typename _inner_type >
struct Container;

template< typename storage_type, typename inner_type >
struct GetAbstractType <
    Container< storage_type, inner_type >
> {
    typedef inner_type type;
};

struct T{}; // T as in terminal

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
template< size_t size, typename tail = T >
struct ElementConfig_ : public tail {
    ElementConfig_( const tail& tail_value ) :
        tail( tail_value ) { }
};

template< size_t size >
struct NodeElementConfig { };
template< size_t size, typename tail = T >
struct NodeElementConfig_: public tail {
    NodeElementConfig_( const tail& tail_value ) :
        tail( tail_value ) { }
};

template< size_t _base >
struct StructConfig {
    static const size_t base = _base;
};

template< size_t _base,
          typename _tail = T >
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

template< size_t _length, size_t _base, typename _tail = T >
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
          typename _tail = T>
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

#define OPTIMIZE
#ifdef OPTIMIZE
template< size_t size >
struct CalculateConfiguration<
    T, NodeElementConfig<size>
> {
    typedef T type;
};
#endif

#ifdef OPTIMIZE
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

#ifdef OPTIMIZE
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

#ifdef OPTIMIZE
// S(base_1)N(size_1)S(base_2)(History) ->
// S(base_1 * base_2)N(size_1)(History)
template< size_t base_1,
          size_t size_1,
          size_t base_2,
          typename History >
struct CalculateConfiguration<
    NodeElementConfig_<
        size_1,
        StructConfig_<
            base_2,
            History
        >
    >,
    StructConfig<base_1>
> {
    typedef typename CalculateConfiguration<
        History,
        NodeElementConfig<size_1>
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

#ifdef OPTIMIZE
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

std::ostream& operator<<( std::ostream& o, const T& v ) {
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

std::stringstream ss;

template< typename storage_type,
          typename config >
struct BasicGet;

#define CONF StructConfig_< size, tail>
template< typename storage_type, size_t size, typename tail >
struct BasicGet< storage_type, CONF > {
    storage_type operator()(storage_type data, const CONF& conf ) {
        storage_type new_data = BasicGet<storage_type, tail>()(
            data, static_cast<const tail&>(conf));
        ss << new_data << " / " << size << " = " <<
            new_data / size << std::endl;
        return new_data / size;
    }
};
#undef CONF

#define CONF StructConfig_< size, T>
template< typename storage_type, size_t size >
struct BasicGet< storage_type, CONF > {
    storage_type operator()(storage_type data, const CONF& conf ) {
        ss << data  << " / " << size << " = " <<
            data / size << std::endl;
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
        ss << new_data << " / (" << size << " ^ " << conf.index <<
            ") = " << new_data / rt_base << std::endl;
        return new_data / rt_base;
    }
};
#undef CONF

#define CONF ArrayConfig_< length, size, T>
template< typename storage_type,
          size_t length,
          size_t size >
struct BasicGet< storage_type, CONF > {
    storage_type operator()(storage_type data, const CONF& conf ) {
        size_t rt_base = 1;
        for(size_t i = 0; i < conf.index; i ++ ) rt_base *= size;
        ss << data << " / (" << size << " ^ " << conf.index <<
            ") = " << data / rt_base << std::endl;
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
        ss << new_data << " - " << offset << " = " <<
            new_data - offset << std::endl;
        assert( new_data >= offset );
        assert( new_data - offset < size );
        return new_data - offset;
    }
};
#undef CONF

#define CONF UnionConfig_< size, offset, T >
template< typename storage_type,
          size_t size,
          size_t offset>
struct BasicGet< storage_type, CONF > {
    storage_type operator()(storage_type data, const CONF& conf ) {
        ss << data  << " - " << offset << " = " <<
            data - offset << std::endl;
        assert( data >= offset );
        assert( data - offset < size );
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
        ss << new_data << " % " << size << " = " <<
            new_data % size << std::endl;
        return new_data % size;
    }
};
#undef CONF

#define CONF ElementConfig_< size, T >
template< typename storage_type, size_t size >
struct BasicGet< storage_type, CONF > {
    storage_type operator()(storage_type data, const CONF& conf ) {
        ss << data  << " % " << size << " = " <<
            data % size << std::endl;
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
        ss << new_data << " % " << size << " = " <<
            new_data % size << std::endl;
        return new_data % size;
    }
};
#undef CONF

#define CONF NodeElementConfig_< size, T >
template< typename storage_type, size_t size >
struct BasicGet< storage_type, CONF > {
    storage_type operator()(storage_type data, const CONF& conf ) {
        ss << data  << " % " << size << " = " <<
            data % size << std::endl;
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
        return BasicSet<storage_type, tail>()(
            value * size, static_cast<const tail&>(conf) );
    }
};
#undef CONF

#define CONF StructConfig_< size, T >
template< typename storage_type, size_t size >
struct BasicSet< storage_type, CONF >{
    storage_type operator()(storage_type value, const CONF& conf ) {
        return value * size;
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
        return BasicSet<storage_type, tail>()(
            value + offset, static_cast<const tail&>(conf) );
    }
};
#undef CONF

#define CONF UnionConfig_< size, offset, T >
template< typename storage_type,
          size_t size,
          size_t offset>
struct BasicSet< storage_type, CONF >{
    storage_type operator()(storage_type value, const CONF& conf ) {
        return value + offset;
    }
};
#undef CONF

#define CONF ElementConfig_< size, tail>
template< typename storage_type, size_t size, typename tail >
struct BasicSet< storage_type, CONF >{
    storage_type operator()(storage_type value, const CONF& conf ) {
        return BasicSet<storage_type, tail>()(
            value, static_cast<const tail&>(conf) );
    }
};
#undef CONF

#define CONF ElementConfig_< size, T >
template< typename storage_type, size_t size >
struct BasicSet< storage_type, CONF >{
    storage_type operator()(storage_type value, const CONF& conf ) {
        return value;
    }
};
#undef CONF

#define CONF NodeElementConfig_<size, tail>
template< typename storage_type, size_t size, typename tail >
struct BasicSet< storage_type, CONF >{
    storage_type operator()(storage_type value, const CONF& conf ) {
        return BasicSet<storage_type, tail>()(
            value, static_cast<const tail&>(conf) );
    }
};
#undef CONF

#define CONF NodeElementConfig_< size, T>
template< typename storage_type, size_t size >
struct BasicSet< storage_type, CONF >{
    storage_type operator()(storage_type value, const CONF& conf ) {
        return value;
    }
};
#undef CONF

template<typename storage_type,
         typename nested_configuration>
void basic_clear( storage_type& data,
                  const nested_configuration& conf ){
    storage_type base_clear_value =
        BasicGet<storage_type, nested_configuration>()(
            data, conf);

    storage_type clear_value =
        BasicSet<storage_type, nested_configuration>()(
            base_clear_value, conf);

    data -= clear_value;
}

/////////////////////////////////////////////////////////////////////
template < typename element_descriptor >
struct Element {
    static const size_t size = element_descriptor::size;
};

template < typename super_t,
           typename base_config,
           typename element_descriptor >
struct ElementImpl {
    static const size_t size = element_descriptor::size;
    typedef typename element_descriptor::inner_type standalone_type;
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
        return (standalone_type)
            BasicGet<storage_type, nested_configuration>()(
                self.data, get_conf() );
    }

    void clear( ) {
        super_t& self = static_cast<super_t&>(*this);
        basic_clear<storage_type, nested_configuration>(
            self.data, get_conf() );
    }

    void set_no_clear(standalone_type new_value) {
        super_t& self = static_cast<super_t&>(*this);
        self.data += BasicSet<storage_type, nested_configuration>()(
            (storage_type)new_value, get_conf() );
    }

    void set(standalone_type new_value) {
        clear();
        set_no_clear(new_value);
    }

    void next_no_wrap() {
        super_t& self = static_cast<super_t&>(*this);
        storage_type current_value = (storage_type)
            BasicGet<storage_type, nested_configuration>()(
                self.data, get_conf() );
        if( current_value < ( size - 1 ) )
            set( (standalone_type) (current_value + 1) );
    }

    void next_wrap() {
        super_t& self = static_cast<super_t&>(*this);
        storage_type current_value = (storage_type)
            BasicGet<storage_type, nested_configuration>()(
                self.data, get_conf() );
        if( current_value < ( size - 1 ) )
            set( (standalone_type) (current_value + 1) );
        else
            set( (standalone_type) (0) );
    }

    //void prev_no_wrap() {
    //    super_t& self = static_cast<super_t&>(*this);
    //    if( basic_get<size, nested_configuration>(self.data) > 0 )
    //        self.data -= nested_configuration::base;
    //}

    //void prev_wrap() {
    //    super_t& self = static_cast<super_t&>(*this);
    //    if( basic_get<size, nested_configuration>(self.data) == 0 )
    //        basic_set_no_clear<size, nested_configuration>(
    //            self.data, size - 1 );
    //    else
    //        self.data -= nested_configuration::base;
    //}
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
        element_descriptor
    > implementation_type;
};

// Default tag's are elements other cases must be specialized
template< typename _tag,
          typename _inner_type = _tag >
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
};

template < typename super_t,
           typename base_config,
           typename... args >
struct EnumStructImpl {
    typedef typename GetStorageType<super_t>::type storage_type;
    static const size_t size = EnumStruct<args...>::size;

    typedef EnumStruct<args...> inner_type;
    typedef Container<
        storage_type,
        inner_type
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

    //standalone_type to_standalone_type() const {
    //    super_t const& self = static_cast<super_t const&>(*this);
    //    return standalone_type (
    //        basic_get<size, nested_configuration>(self.data));
    //}

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

    //void clear(standalone_type new_value) {
    //    super_t& self = static_cast<super_t&>(*this);
    //    basic_clear<size, nested_configuration>( self.data );
    //}

    //void set_no_clear(standalone_type new_value) {
    //    super_t& self = static_cast<super_t&>(*this);
    //    basic_set_no_clear<size, nested_configuration>(
    //        self.data, new_value.data );
    //}

    //void set(standalone_type new_value) {
    //    super_t& self = static_cast<super_t&>(*this);
    //    basic_clear<size, nested_configuration>( self.data );
    //    basic_set_no_clear<size, nested_configuration>(
    //        self.data, new_value.data );
    //}
};

template< typename... args >
struct GetAbstractType <
    EnumStruct< args... >
> {
    typedef EnumStruct< args... > type;
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
        args...
    > implementation_type;
};

// Specialization of tag with inner EnumStruct
template< typename _tag,
          typename... args >
struct Tag< _tag, EnumStruct< args... > > :
public EnumStruct< args... > {
    typedef EnumStruct< args... > tag_inner_type;
    typedef _tag tag;
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
};

template < typename super_t,
           typename base_config,
           size_t   length,
           typename element_descriptor >
struct ArrayImpl {
    typedef typename GetStorageType<super_t>::type storage_type;
    static const size_t size =
        Array<length, element_descriptor>::size;

    typedef Array<length, element_descriptor> inner_type;
    typedef Container<
        storage_type,
        inner_type
    > standalone_type;

    typedef CalculateConfiguration<
        base_config, ElementConfig<size>
    > calc_conf;

    typedef typename calc_conf::type nested_configuration;

    nested_configuration get_conf() const {
        super_t const& self = static_cast<super_t const&>(*this);
        return calc_conf()(ElementConfig<size>(), self.conf);
    }

/*
    standalone_type to_standalone_type() const {
        super_t const& self = static_cast<super_t const&>(*this);
        return standalone_type (
            basic_get<size, base_config>(self.data));
    }
*/


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
#define CONF typename CONF_CALC ::type
#define PROXY_TYPE                   \
Proxy< storage_type,                 \
       element_descriptor,           \
       CONF >
    PROXY_TYPE
    get( size_t idx ) const {
        const super_t& self = static_cast<const super_t&>(*this);
        return PROXY_TYPE( self.data,
                           CONF_CALC()(ARRAY_CONF(idx), self.conf) );
    }
#undef ARRAY_CONF
#undef PROXY_TYPE
#undef CONF_CALC
#undef CONF

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

template< size_t length,
          typename element_descriptor >
struct GetAbstractType <
    Array< length, element_descriptor >
> {
    typedef Array< length, element_descriptor > type;
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
    typedef typename GetAbstractType<
        element_descriptor
    >::type abstract_type;

    typedef ArrayImpl <
        super_t,
        base_config,
        length,
        abstract_type
    > implementation_type;
};

// Specialization of tag with inner Array
template< typename _tag,
          size_t   length,
          typename element_descriptor >
struct Tag< _tag, Array< length, element_descriptor > > :
public Array< length, element_descriptor > {
    typedef Array< length, element_descriptor > tag_inner_type;
    typedef _tag tag;
};

/////////////////////////////////////////////////////////////////////
template < typename keyset, typename... args>
struct EnumUnion {
    template< size_t prev_size, class type >
    struct SizeFunction {
        static const size_t new_size = type::size + prev_size;
    };

    static const size_t size = CalculateSize<
        0, SizeFunction, args...
    >::size;
};

template < typename... args >
struct UnionVisitor;

template < typename base,
           typename head,
           typename... tail >
struct UnionVisitor < base, head, tail... > :
public UnionVisitor< base, tail... > {
    template< size_t offset, typename storage_type, typename Visitor >
    void __visit(const storage_type& value, Visitor& visitor ) const {
        if( value >= ( head::size + offset ) ) {
            UnionVisitor< base, tail... >::template __visit<
                offset + head::size
            >( value, visitor );
        } else {
            const base& self = static_cast<const base&>(*this);
            visitor.template operator()<typename head::tag>(
                self.template union_get<typename head::tag>() );
        }
    }
};

template < typename base,
           typename head >
struct UnionVisitor < base, head > {
    template< size_t offset, typename storage_type, typename Visitor >
    void __visit(const storage_type& value, Visitor& visitor ) const {
        if( value >= ( head::size + offset ) ) {
            std::cerr << "union error" << std::endl;
            abort();
        } else {
            const base& self = static_cast<const base&>(*this);
            visitor.template operator()<typename head::tag>(
                self.template union_get<typename head::tag>() );
        }
    }
};

template < typename super_t,
           typename base_config,
           typename keyset,
           typename... args >
struct EnumUnionImpl :
public UnionVisitor <
    EnumUnionImpl<super_t, base_config, keyset, args...>, args...
> {
    typedef typename GetStorageType<super_t>::type storage_type;
    typedef EnumUnion<keyset, args...> inner_type;
    static const size_t size = inner_type::size;
    typedef Container<
        storage_type,
        inner_type
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

    //standalone_type to_standalone_type() const {
    //    super_t const& self = static_cast<super_t const&>(*this);
    //    return standalone_type (
    //        basic_get<size, base_config>(self.data));
    //}

    void clear() {
        super_t& self = static_cast<super_t&>(*this);
        basic_clear<storage_type, nested_configuration>(
            self.data, get_conf() );
    }

#define TAG2CONF           \
typename Tag2Conf<         \
    tag,                   \
    UnionConfig<0, 0>,     \
    childs_config,         \
    NextConf,              \
    ThisConf,              \
    args...  >
#define PROXY_TYPE         \
Proxy< storage_type,       \
       TAG2CONF::type,     \
       TAG2CONF::conf >
    template<typename tag>
    PROXY_TYPE
    union_get() {
        super_t& self = static_cast<super_t&>(*this);
        return PROXY_TYPE( self.data, TAG2CONF::conf(self.conf) );
    }

    template<typename tag>
    PROXY_TYPE
    union_set(typename PROXY_TYPE :: inner_standalone_type value ) {
        clear();
        super_t& self = static_cast<super_t&>(*this);
        self.data +=
            BasicSet< storage_type, TAG2CONF::conf >()(
                (storage_type)value, TAG2CONF::conf(self.conf) );
        return union_get<tag>();
    }

    template<typename tag>
    PROXY_TYPE
    union_clear() {
        clear();
        super_t& self = static_cast<super_t&>(*this);
        self.data +=
            BasicSet< storage_type, TAG2CONF::conf >()(
                (storage_type)0, TAG2CONF::conf(self.conf) );
        return union_get<tag>();
    }
#undef PROXY_TYPE

#define PROXY_TYPE         \
Proxy< const storage_type, \
       TAG2CONF::type,     \
       TAG2CONF::conf >
    template<typename tag>
    PROXY_TYPE
    union_get() const {
        const super_t& self = static_cast<const super_t&>(*this);
        return PROXY_TYPE( self.data, TAG2CONF::conf(self.conf) );
    }
#undef PROXY_TYPE
#undef TAG2CONF

    //void set(standalone_type new_value) {
    //    super_t& self = static_cast<super_t&>(*this);
    //    basic_clear<size, base_config>( self.data );
    //    basic_set_no_clear<size, base_config>(
    //        self.data, new_value.data );
    //}

    template< typename Visitor >
    void static_visit( Visitor& visitor ) const {
        super_t const& self = static_cast<super_t const&>(*this);
        storage_type value = BasicGet<
            storage_type,
            nested_configuration
        >()(self.data, get_conf() );
        this->template __visit<0>(value, visitor);
    }
};

template< typename keyset,
          typename... args >
struct GetAbstractType <
    EnumUnion< keyset, args... >
> {
    typedef EnumUnion< keyset, args... > type;
};

// Defines implementation for anonymous EnumUnion
template< typename super_t,
          typename base_config,
          typename keyset,
          typename... args>
struct GetImplementation<
    super_t,
    base_config,
    EnumUnion<keyset, args...>
> {
    typedef EnumUnionImpl<
        super_t,
        base_config,
        keyset,
        args...
    > implementation_type;
};

// Specialization of tag with inner EnumUnion
template< typename _tag,
          typename keyset,
          typename... args >
struct Tag< _tag, EnumUnion< keyset, args... > > :
public EnumUnion< keyset, args... > {
    typedef EnumUnion< keyset, args... > tag_inner_type;
    typedef _tag tag;
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
          typename _nested_configuration > // T
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

    operator inner_standalone_type () const {
        //std::cout << "Converted" << std::endl;
        return this->to_standalone_type();
    }

    proxy_t& operator=(const inner_standalone_type rhs) {
        this->set(rhs);
        return *this;
    }
};

#define DERIVED_CLASS                             \
Proxy <                                           \
    _storage_type,                                \
    typename GetAbstractType<_inner_type>::type,  \
    T                                             \
>
template< typename _storage_type, typename _inner_type >
struct Container : public DERIVED_CLASS
{
    typedef DERIVED_CLASS Base;
#undef DERIVED_CLASS

    Container() : Base(data, T() ), data(0) { }
    Container( const Container& rhs ) :
        Base(data), data( rhs.data ) { }
    explicit Container( _storage_type value ) :
        Base(data), data( value ) { }

    Container& operator=(const typename Base::standalone_type rhs) {
        this->set(rhs);
        return *this;
    }

    _storage_type data;
};

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
    o << "{";
    StructPrinter<PROXY_STRUCT_TYPE, _tags...> struct_printer;
    struct_printer( o, v );
    o << "}";
    return o;
}
#undef PROXY_STRUCT_TYPE

// UNION PROXY PRINTER /////////////////////////////////////////////
struct UnionPrinter{
    std::ostream& o;
    UnionPrinter(std::ostream& _o) : o(_o) { }
    template<typename tag, typename Arg>
    void operator()(Arg arg) {
        o << tag() << ":" << arg;
    }
};

#define PROXY_UNION_TYPE  \
Proxy<                    \
    _storage_type,        \
    EnumUnion<            \
        _keyset,          \
        _tags...          \
    >,                    \
    _nested_configuration \
>
template< typename _storage_type,
          typename _nested_configuration,
          typename _keyset,
          typename... _tags >
std::ostream& operator<<( std::ostream& o,
                          const PROXY_UNION_TYPE& v ) {
    o << "<";
    UnionPrinter p(o);
    v.static_visit(p);
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
          typename _keyset,
          typename... args >
struct ____print_details_union_childs;

template< typename _proxy_type,
          typename _keyset,
          typename head,
          typename... args >
struct ____print_details_union_childs<
    _proxy_type, _keyset, head, args...
> : private ____print_details_union_childs<
    _proxy_type, _keyset, args...
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
            _keyset,
            args...
        >::operator()(o, indent, p);
    }
};

template< typename _proxy_type,
          typename _keyset,
          typename head >
struct ____print_details_union_childs<_proxy_type, _keyset, head> {
    void operator( )( std::ostream& o,
                      std::string indent,
                      const _proxy_type& p ) {
        o << indent << "\"" << typename head::tag() << "\" ";
        __print_details(o, indent + __indent,
                        p.template union_get<typename head::tag>());
        o << std::endl;
    }
};

#define PROXY                       \
Proxy< _storage_type,               \
       EnumUnion<_keyset, args...>, \
       _base_config >
template< typename _storage_type,
          typename _base_config,
          typename _keyset,
          typename... args >
void __print_details( std::ostream& o,
                      std::string indent,
                      const PROXY& v ) {
    typedef typename GetImplementation<
        PROXY, _base_config, EnumUnion<_keyset, args...>
    >::implementation_type impl_type;

    typedef typename PROXY ::nested_configuration
        _nested_configuration;

    o << "EnumUnion proxy: Size: " <<
        impl_type::size << ", " <<
        v.get_conf() << std::endl;

    ____print_details_union_childs<PROXY, _keyset, args...>()(
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

// Specialization of tag with inner Container
template< typename _tag,
          typename _storage_type,
          typename _inner_type >
struct Tag< _tag, Container< _storage_type, _inner_type > > :
public Tag< _tag, _inner_type > { };

/////////////////////////////////////////////////////////////////////
GMB_ENUM_MPL(As,
    ((a1, A1))
    ((a2, A2))
);

GMB_ENUM_MPL(Bs,
    ((b1, B1))
    ((b2, B2))
    ((b3, B3))
    ((b4, B4))
    ((b5, B5))
    ((b6, B6))
    ((b7, B7))
);

GMB_ENUM_MPL(Cs,
    ((c1, C1))
    ((c2, C2))
    ((c3, C3))
);

GMB_ENUM_MPL(Tags,
    ((No1, NO1))
    ((No2, NO2))
    ((No3, NO3))
);

typedef Container< unsigned, Element<As> > As_1;
typedef Container< unsigned, Element<Bs> > Bs_1;
typedef Container< unsigned, Element<Cs> > Cs_1;

As_1 as_1;
Bs_1 bs_1;
Cs_1 cs_1;


typedef Container< size_t,
    EnumStruct<
        Tag<Tags::No1, Cs>,
        Tag<Tags::No2, Cs>,
        Tag<Tags::No3, Cs>
    >
> E;

typedef Container< size_t,
    Array < 5, Cs_1 >
> A;

typedef Container< size_t,
    EnumUnion< Tags,
        Tag<Tags::No1, Cs>,
        Tag<Tags::No2, Cs>,
        Tag<Tags::No3, Cs>
    >
> U;

typedef Container< size_t,
    Array < 5, E >
> AE;

typedef Container< size_t,
    Array < 5, U >
> AU;

typedef Container< size_t,
    Array < 5, AE >
> AAE;

typedef Container< size_t,
    Array < 5, AU >
> AAU;

typedef Container< size_t,
    EnumStruct<
        Tag<Tags::No1, E>,
        Tag<Tags::No2, E>,
        Tag<Tags::No3, E>
    >
> EE;

typedef Container< size_t,
    EnumUnion< Tags,
        Tag<Tags::No1, U>,
        Tag<Tags::No2, U>,
        Tag<Tags::No3, U>
    >
> UU;

typedef Container< size_t,
    EnumStruct<
        Tag<Tags::No1, EE>,
        Tag<Tags::No2, EE>,
        Tag<Tags::No3, EE>
    >
> EEE;

typedef Container< size_t,
    EnumUnion< Tags,
        Tag<Tags::No1, UU>,
        Tag<Tags::No2, UU>,
        Tag<Tags::No3, UU>
    >
> UUU;

typedef Container< size_t,
    EnumStruct<
        Tag<Tags::No1, U>,
        Tag<Tags::No2, U>,
        Tag<Tags::No3, U>
    >
> EU;

typedef Container< size_t,
    EnumUnion< Tags,
        Tag<Tags::No1, E>,
        Tag<Tags::No2, E>,
        Tag<Tags::No3, E>
    >
> UE;

typedef Container< size_t,
    EnumStruct<
        Tag<Tags::No1, UE>,
        Tag<Tags::No2, UE>,
        Tag<Tags::No3, UE>
    >
> EUE;

typedef Container< size_t,
    EnumUnion< Tags,
        Tag<Tags::No1, EU>,
        Tag<Tags::No2, EU>,
        Tag<Tags::No3, EU>
    >
> UEU;


typedef Container< unsigned,
    EnumStruct<
        Tag<As>,
        Tag<Bs>,
        Tag<Cs>
    >
> OneLevelStruct_1;

typedef Container< unsigned,
    EnumUnion<Tags,
        Tag<Tags::No1, As>,
        Tag<Tags::No2, Bs>,
        Tag<Tags::No3, Cs>
    >
> OneLevelUnion_1;

typedef Container< unsigned,
    EnumStruct<
        Tag<As, As_1>,
        Tag<Tags::No2,
            EnumStruct<
                Tag<As>,
                Tag<Bs>,
                Tag<Cs>
            >
        >,
        Tag<Cs>
    >
> TwoLevelStruct_1;

typedef Container< unsigned,
    EnumStruct<
        Tag<As, As_1>,
        Tag<Tags::No2, OneLevelStruct_1>,
        Tag<Cs>
    >
> Combining_1;

void clear_debug() {
    ss.clear();
    ss.str(std::string());
}

void sink_debug() {
    std::copy( std::istreambuf_iterator<char>(ss),
               std::istreambuf_iterator<char>(),
               std::ostreambuf_iterator<char>(std::cout) );
    std::cout << std::endl;
}


int main(int argc, const char *argv[])
{
    //OneLevelStruct_1 one_level_struct_1;
    //OneLevelUnion_1 one_level_union_1;
    //TwoLevelStruct_1 two_level_struct_1;
    //Combining_1 combining_1;
    //Combining_1 combining_2;

    print_details( as_1 );

    E e;
    U u;
    A a;
    std::cout << "E "; print_details( e );
    std::cout << "U "; print_details( u );
    std::cout << "A "; print_details( a );
    std::cout << a.get(0) << std::endl;

    EE ee;
    UU uu;
    std::cout << "EE "; print_details( ee );
    std::cout << "UU "; print_details( uu );

    EEE eee;
    UUU uuu;
    std::cout << "EEE "; print_details( eee );
    std::cout << "UUU "; print_details( uuu );

    EU eu;
    UE ue;
    AE ae;
    AU au;
    AAE aae;
    AAU aau;
    std::cout << "EU "; print_details( eu );
    std::cout << "UE "; print_details( ue );
    std::cout << "AE "; print_details( ae );
    std::cout << "AU "; print_details( au );
    std::cout << "AAE "; print_details( aae );
    std::cout << "AAU "; print_details( aau );

    EUE eue;
    UEU ueu;
    std::cout << "EUE "; print_details( eue );
    std::cout << "UEU "; print_details( ueu );

    std::cout << as_1 << std::endl;

    std::cout << e.get<Tags::No1>() << std::endl;

    std::cout << ee.get<Tags::No2>().get<Tags::No3>() << std::endl;

    std::cout << eee.get<Tags::No2>().
                     get<Tags::No3>().
                     get<Tags::No1>() << std::endl;

    //std::cout << u.get<Tags::No1>() << std::endl;

    //std::cout << uu.get<Tags::No2>().
    //                get<Tags::No2>() << std::endl;

    //std::cout << eu.get<Tags::No2>().
    //                get<Tags::No1>() << std::endl;

    std::cout << std::endl;
    std::cout << std::endl;

    print_details(eu);
    std::cout << eu << std::endl;
    clear_debug();
    std::cout << eu.get<Tags::No2>().
                    union_get<Tags::No1>() << std::endl;
    eu.get<Tags::No2>().
       union_get<Tags::No1>().set(Cs::C2);
    sink_debug();
    std::cout << eu.get<Tags::No2>().
                    union_get<Tags::No1>() << std::endl;
    sink_debug();
    eu.get<Tags::No1>().union_clear<Tags::No2>().set(Cs::C2);
    eu.get<Tags::No3>().union_clear<Tags::No3>().set(Cs::C3);
    std::cout << "Start:              " << eu << std::endl;
    eu.get<Tags::No2>().clear();
    std::cout << "After clear:        " << eu << std::endl;

    eu.get<Tags::No2>().union_set<Tags::No2>(Cs::C2);
    std::cout << "After set No2/C2:   " << eu << std::endl;
    eu.get<Tags::No2>().union_clear<Tags::No3>().set(Cs::C3);
    std::cout << "After clear to No3: " << eu << std::endl;

    std::cout << uuu << std::endl;
    uuu.union_clear<Tags::No2>().
        union_clear<Tags::No3>().
        union_clear<Tags::No2>().
        set(Cs::C1);
    std::cout << uuu << std::endl;
    uuu.union_get<Tags::No2>().
        union_get<Tags::No3>().
        union_get<Tags::No2>().
        next_no_wrap();
    std::cout << uuu << std::endl;
    uuu.union_get<Tags::No2>().
        union_get<Tags::No3>().
        union_get<Tags::No2>().
        next_no_wrap();
    std::cout << uuu << std::endl;
    uuu.union_get<Tags::No2>().
        union_get<Tags::No3>().
        union_get<Tags::No2>().
        next_no_wrap();
    std::cout << uuu << std::endl;
    uuu.union_get<Tags::No2>().
        union_get<Tags::No3>().
        union_get<Tags::No2>().
        next_wrap();
    std::cout << uuu << std::endl;
    uuu.union_get<Tags::No2>().
        union_get<Tags::No3>().
        union_get<Tags::No2>().
        next_wrap();
    std::cout << uuu << std::endl;


    //std::cout << eu << std::endl;
    //eu.get<Tags::No2>().get<Tags::No2>().set(Cs::C2);
    //std::cout << eu << std::endl;
    //eu.get<Tags::No2>().get<Tags::No2>().set(Cs::C3);
    //std::cout << eu << std::endl;
    //eu.get<Tags::No2>().get<Tags::No2>().set(Cs::C1);
    //std::cout << eu << std::endl;
    //eu.get<Tags::No2>().get<Tags::No2>().set(Cs::C2);
    //std::cout << eu << std::endl;
    //eu.get<Tags::No2>().get<Tags::No2>().set(Cs::C3);
    //std::cout << eu << std::endl;


    //std::cout << u << std::endl;
    //std::cout << eue << std::endl;
    //std::cout << ueu << std::endl;

    //for( ; eue.data < 1000; eue.data++ ) {
    //    std::cout << eue.data << ": " << eue << std::endl;
    //}

    /*
    std::cout << bs_1 << std::endl;
    std::cout << cs_1 << std::endl;

    std::cout << one_level_struct_1 << std::endl;
    std::cout << two_level_struct_1 << std::endl;

    combining_1.get<As>().set( As::A2 );
    combining_1.get<Tags::No2>().get<Bs>().set( Bs::B4 );

    cs_1 = Cs::C2;
    std::cout << combining_1.get<Cs>() << std::endl;
    combining_1.get<Cs>() = Cs::C3;
    std::cout << combining_1.get<Cs>() << std::endl;
    combining_1.get<Cs>() = cs_1;
    std::cout << combining_1.get<Cs>() << std::endl;

    combining_2 = combining_1;
    std::cout << combining_2 << std::endl;

    combining_2.get<Tags::No2>() = one_level_struct_1;
    one_level_struct_1 = combining_2.get<Tags::No2>();

    std::cout << std::endl;

    std::cout << OneLevelUnion_1::size << std::endl;

    print_details( one_level_struct_1 );
    print_details( two_level_struct_1 );
    print_details( one_level_union_1 );

    */

    return 0;
}


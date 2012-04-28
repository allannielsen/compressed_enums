#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE UnionVisitor
#include <boost/test/unit_test.hpp>

#include <compressed_enums.hxx>

GMB_ENUM_MPL(As,
    ((a1, A1))
    ((a2, A2))
    ((a3, A3))
    ((a4, A4))
    ((a5, A5))
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

GMB_ENUM_MPL(Ds,
    ((d1, D1))
    ((d2, D2))
);

GMB_ENUM_MPL(Tags,
    ((No1, NO1))
    ((No2, NO2))
    ((No3, NO3))
    ((No4, NO4))
);

typedef Container< unsigned,
    Union<
        Tag<Tags::No1, As>,
        Tag<Tags::No2, Bs>,
        Tag<Tags::No3, Cs>,
        Tag<Tags::No4, Ds>
    >
> E4;
E4 e4_1;
E4 e4_2;
E4 e4_3;
E4 e4_4;

template<typename tag> struct Handler;
template<> struct Handler<Tags::No1> {
    template<typename Arg> void operator()( Arg& a ) {
        a.next_no_wrap();
    }
};
template<> struct Handler<Tags::No2> {
    template<typename Arg> void operator()( Arg& a ) {
        a.next_wrap();
    }
};
template<> struct Handler<Tags::No3> {
    template<typename Arg> void operator()( Arg& a ) {
        a.prev_no_wrap();
    }
};
template<> struct Handler<Tags::No4> {
    template<typename Arg> void operator()( Arg& a ) {
        a.prev_wrap();
    }
};

BOOST_AUTO_TEST_CASE(print) {
    UnionCategory<Tags> u;
    e4_1.union_clear<Tags::No1>().set(As::A3);
    e4_2.union_clear<Tags::No2>().set(Bs::B3);
    e4_3.union_clear<Tags::No3>().set(Cs::C2);
    e4_4.union_clear<Tags::No4>().set(Ds::D2);

    BOOST_CHECK( e4_1.static_visit(u) == Tags::NO1 );
    BOOST_CHECK( e4_2.static_visit(u) == Tags::NO2 );
    BOOST_CHECK( e4_3.static_visit(u) == Tags::NO3 );
    BOOST_CHECK( e4_4.static_visit(u) == Tags::NO4 );

    BOOST_CHECK( e4_1.union_get<Tags::No1>() == As::A3 );
    BOOST_CHECK( e4_2.union_get<Tags::No2>() == Bs::B3 );
    BOOST_CHECK( e4_3.union_get<Tags::No3>() == Cs::C2 );
    BOOST_CHECK( e4_4.union_get<Tags::No4>() == Ds::D2 );

    e4_1.static_visit_class<Handler>();
    e4_2.static_visit_class<Handler>();
    e4_3.static_visit_class<Handler>();
    e4_4.static_visit_class<Handler>();

    BOOST_CHECK( e4_1.union_get<Tags::No1>() == As::A4 );
    BOOST_CHECK( e4_2.union_get<Tags::No2>() == Bs::B4 );
    BOOST_CHECK( e4_3.union_get<Tags::No3>() == Cs::C1 );
    BOOST_CHECK( e4_4.union_get<Tags::No4>() == Ds::D1 );

    e4_1.static_visit_class<Handler>();
    e4_2.static_visit_class<Handler>();
    e4_3.static_visit_class<Handler>();
    e4_4.static_visit_class<Handler>();

    BOOST_CHECK( e4_1.union_get<Tags::No1>() == As::A5 );
    BOOST_CHECK( e4_2.union_get<Tags::No2>() == Bs::B5 );
    BOOST_CHECK( e4_3.union_get<Tags::No3>() == Cs::C1 );
    BOOST_CHECK( e4_4.union_get<Tags::No4>() == Ds::D2 );

    e4_1.static_visit_class<Handler>();
    e4_2.static_visit_class<Handler>();
    e4_3.static_visit_class<Handler>();
    e4_4.static_visit_class<Handler>();

    BOOST_CHECK( e4_1.union_get<Tags::No1>() == As::A5 );
    BOOST_CHECK( e4_2.union_get<Tags::No2>() == Bs::B6 );
    BOOST_CHECK( e4_3.union_get<Tags::No3>() == Cs::C1 );
    BOOST_CHECK( e4_4.union_get<Tags::No4>() == Ds::D1 );

    e4_1.static_visit_class<Handler>();
    e4_2.static_visit_class<Handler>();
    e4_3.static_visit_class<Handler>();
    e4_4.static_visit_class<Handler>();

    BOOST_CHECK( e4_1.union_get<Tags::No1>() == As::A5 );
    BOOST_CHECK( e4_2.union_get<Tags::No2>() == Bs::B7 );
    BOOST_CHECK( e4_3.union_get<Tags::No3>() == Cs::C1 );
    BOOST_CHECK( e4_4.union_get<Tags::No4>() == Ds::D2 );

    e4_1.static_visit_class<Handler>();
    e4_2.static_visit_class<Handler>();
    e4_3.static_visit_class<Handler>();
    e4_4.static_visit_class<Handler>();

    BOOST_CHECK( e4_1.union_get<Tags::No1>() == As::A5 );
    BOOST_CHECK( e4_2.union_get<Tags::No2>() == Bs::B1 );
    BOOST_CHECK( e4_3.union_get<Tags::No3>() == Cs::C1 );
    BOOST_CHECK( e4_4.union_get<Tags::No4>() == Ds::D1 );

}


#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE BasicUnion
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

typedef Container< unsigned char, Element<As> > As1;
typedef Container< unsigned char, Element<Bs> > Bs1;
typedef Container< unsigned char, Element<Cs> > Cs1;
typedef Container< unsigned char, Element<Ds> > Ds1;

typedef Container< unsigned, Element<As> > As4;
typedef Container< unsigned, Element<Bs> > Bs4;
typedef Container< unsigned, Element<Cs> > Cs4;
typedef Container< unsigned, Element<Ds> > Ds4;

As1 as1_1; As1 as1_2;
Bs1 bs1_1; Bs1 bs1_2;
Cs1 cs1_1; Cs1 cs1_2;
Ds1 ds1_1; Ds1 ds1_2;

As4 as4_1; As4 as4_2;
Bs4 bs4_1; Bs4 bs4_2;
Cs4 cs4_1; Cs4 cs4_2;
Ds4 ds4_1; Ds4 ds4_2;

typedef Container< unsigned char,
    Union<
        Tag<Tags::No1, As1>,
        Tag<Tags::No2, Bs1>,
        Tag<Tags::No3, Cs1>,
        Tag<Tags::No4, Ds1>
    >
> E1;
E1 e1_1;
E1 e1_2;

typedef Container< unsigned short,
    Union<
        Tag<Tags::No1, As4>,
        Tag<Tags::No2, Bs4>,
        Tag<Tags::No3, Cs4>,
        Tag<Tags::No4, Ds4>
    >
> E2;
E2 e2_1;
E2 e2_2;

typedef Container< unsigned,
    Union<
        Tag<Tags::No1, Element<As>>,
        Tag<Tags::No2, Element<Bs>>,
        Tag<Tags::No3, Element<Cs>>,
        Tag<Tags::No4, Element<Ds>>
    >
> E3;
E3 e3_1;
E3 e3_2;

typedef Container< size_t,
    Union<
        Tag<Tags::No1, As>,
        Tag<Tags::No2, Bs>,
        Tag<Tags::No3, Cs>,
        Tag<Tags::No4, Ds>
    >
> E4;
E4 e4_1;
E4 e4_2;

typedef Container< unsigned,
    Union<
        Tag<Tags::No1, E1>,
        Tag<Tags::No2, E2>,
        Tag<Tags::No3, E3>,
        Tag<Tags::No4, E4>
    >
> EE1;
EE1 ee1_1;
EE1 ee1_2;


GMB_ENUM_CNT( TreasureMapPosition, 50 );

GMB_ENUM_MPL( ActionType,
    ((Play, PLAY))
);

GMB_ENUM_MPL( OutcomeCategory,
    ((ActionType, ACTION_TYPE))
	((CompassValue, COMPASS_VALUE))
	((MapPosition,  MAP_POSITION))
);


typedef Container<
    unsigned,
    Union<
        Tag<OutcomeCategory::ActionType, ActionType>,
        Tag<OutcomeCategory::MapPosition, TreasureMapPosition>
    >
> EventElementData;
EventElementData event_element_data;

BOOST_AUTO_TEST_CASE(print) {
    std::stringstream ss;
    ss << as1_1 << std::endl;
    ss << bs1_1 << std::endl;
    ss << cs1_1 << std::endl;
    ss << ds1_1 << std::endl;

    ss << as4_1 << std::endl;
    ss << bs4_1 << std::endl;
    ss << cs4_1 << std::endl;
    ss << ds4_1 << std::endl;

    ss << e1_1 << std::endl;
    ss << e2_1 << std::endl;
    ss << e3_1 << std::endl;
}

BOOST_AUTO_TEST_CASE(conversion) {
    e1_1 = e1_1;
    e1_1 = e2_1;
    e1_1 = e3_1;
    e1_1 = e4_1;

    e2_1 = e1_1;
    e2_1 = e2_1;
    e2_1 = e3_1;
    e2_1 = e4_1;

    e3_1 = e1_1;
    e3_1 = e2_1;
    e3_1 = e3_1;
    e3_1 = e4_1;

    e4_1 = e1_1;
    e4_1 = e2_1;
    e4_1 = e3_1;
    e4_1 = e4_1;
}
BOOST_AUTO_TEST_CASE(assign) {
    e1_1.clear();
    BOOST_CHECK( e1_1.union_get<Tags::No1>() == As::A1 );

    e1_2.clear();
    BOOST_CHECK( e1_2.union_get<Tags::No1>() == As::A1 );

    e1_1.union_clear<Tags::No1>().set( As::A2 );
    BOOST_CHECK( e1_1.union_get<Tags::No1>() == As::A2 );

    e1_1.union_clear<Tags::No2>().set( Bs::B5 );
    BOOST_CHECK( e1_1.union_get<Tags::No2>() == Bs::B5 );

    e1_1.union_clear<Tags::No3>().set( Cs::C3 );
    BOOST_CHECK( e1_1.union_get<Tags::No3>() == Cs::C3 );

    e1_1.union_clear<Tags::No4>().set( Ds::D2 );
    BOOST_CHECK( e1_1.union_get<Tags::No4>() == Ds::D2 );

    BOOST_CHECK( e1_1 != e1_2 );
    BOOST_CHECK( !( e1_1 == e1_2 ) );
    e1_2 = e1_1;
    BOOST_CHECK( ! ( e1_1 != e1_2 ) );
    BOOST_CHECK( e1_1 == e1_2 );

    e1_1.clear();
    BOOST_CHECK( e1_1.union_get<Tags::No1>() == As::A1 );

    e1_2.clear();
    BOOST_CHECK( e1_2.union_get<Tags::No1>() == As::A1 );
}

BOOST_AUTO_TEST_CASE(nested_assign) {
    ee1_1.union_clear<
        Tags::No3
    >().set( e3_1 );

    ee1_1.union_clear<
        Tags::No3
    >().union_clear<
        Tags::No2
    >().set( Bs::B5 );

    std::cout << ee1_1 << std::endl;
}

/*
BOOST_AUTO_TEST_CASE(static_size) {
    BOOST_CHECK( E1::size == 210 );
}
*/

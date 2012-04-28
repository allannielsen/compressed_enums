#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE BasicStruct
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

GMB_ENUM_MPL(Tags2,
    ((No1, NO1))
    ((No2, NO2))
);

GMB_ENUM_MPL(Tags4,
    ((No1, NO1))
    ((No2, NO2))
    ((No3, NO3))
    ((No4, NO4))
);

typedef Container< unsigned,
    EnumStruct<
        Tag<Tags2::No1, Element<Cs> >,
        Tag<Tags2::No2, Element<Ds> >
    >
>::type E1;
E1 e1_1;
E1 e1_2;

typedef Container< unsigned,
    EnumStruct<
        Tag<Tags4::No1, Element<As> >,
        Tag<Tags4::No2, E1>,
        Tag<Tags4::No3, Element<Bs>>,
        Tag<Tags4::No4, E1>
    >
>::type E2;
E2 e2_1;
E2 e2_2;

typedef Container< unsigned,
    EnumStruct<
        Tag<Tags4::No1, E1 >,
        Tag<Tags4::No2, Element<As> >,
        Tag<Tags4::No3, E1 >,
        Tag<Tags4::No4, Element<Bs> >
    >
>::type E3;
E3 e3_1;
E3 e3_2;


typedef Container< size_t,
    EnumStruct<
        Tag<Tags4::No1, Element<As> >,
        Tag<Tags4::No2, E1>,
        Tag<Tags4::No3, Element<Bs>>,
        Tag<Tags4::No4, E1>
    >
>::type E4;
E4 e4_1;
E4 e4_2;

typedef Container< size_t,
    EnumStruct<
        Tag<Tags4::No1, E1 >,
        Tag<Tags4::No2, Element<As> >,
        Tag<Tags4::No3, E1 >,
        Tag<Tags4::No4, Element<Bs> >
    >
>::type E5;
E5 e5_1;
E5 e5_2;

typedef Container< size_t,
    EnumStruct<
        Tag<Tags2::No1, Element<Cs> >,
        Tag<Tags2::No2, Element<Ds> >
    >
>::type E6;
E6 e6_1;
E6 e6_2;

typedef Container< size_t,
    EnumStruct<
        Tag<Tags2::No1, E4 >,
        Tag<Tags2::No2, E5 >
    >
>::type E7;
E7 e7_1;
E7 e7_2;


BOOST_AUTO_TEST_CASE(print) {
    std::stringstream ss;
    ss << e1_1 << e2_1;
}
BOOST_AUTO_TEST_CASE(conversion) {
    const E2& c2_1 = e2_1;
    const E4& c4_1 = e4_1;

    e1_1 = e2_1.get<Tags4::No2>();
    e1_1 = c2_1.get<Tags4::No2>();
    e6_1 = e2_1.get<Tags4::No2>();
    e6_1 = c2_1.get<Tags4::No2>();

    e1_1 = e4_1.get<Tags4::No2>();
    e1_1 = c4_1.get<Tags4::No2>();
    e6_1 = e4_1.get<Tags4::No2>();
    e6_1 = c4_1.get<Tags4::No2>();

    e1_1.set(e2_1.get<Tags4::No2>());
    e1_1.set(c2_1.get<Tags4::No2>());
    e6_1.set(e2_1.get<Tags4::No2>());
    e6_1.set(c2_1.get<Tags4::No2>());

    e1_1.set(e4_1.get<Tags4::No2>());
    e1_1.set(c4_1.get<Tags4::No2>());
    e6_1.set(e4_1.get<Tags4::No2>());
    e6_1.set(c4_1.get<Tags4::No2>());

    { E1 _e1_1 = e2_1.get<Tags4::No2>(); }
    { E1 _e1_1 = c2_1.get<Tags4::No2>(); }
    { E6 _e6_1 = e2_1.get<Tags4::No2>(); }
    { E6 _e6_1 = c2_1.get<Tags4::No2>(); }
    { E1 _e1_1 = e4_1.get<Tags4::No2>(); }
    { E1 _e1_1 = c4_1.get<Tags4::No2>(); }
    { E6 _e6_1 = e4_1.get<Tags4::No2>(); }
    { E6 _e6_1 = c4_1.get<Tags4::No2>(); }

    /*
    e2_1.get<Tags4::No2>().set(e1_1);
    e1_1.set(e2_1.get<Tags4::No2>());

    const E3& cref = e3_1;
    e2_1.get<Tags4::No2>().set(cref.get<Tags4::No1>());

    E3& ref = e3_1;
    e2_1.get<Tags4::No2>().set(ref.get<Tags4::No1>());

    const E2& ref2 = e2_1;
    e1_1 = e2_1.get<Tags4::No2>();
    e1_1 = ref2.get<Tags4::No2>();
    */


    print_details( e7_1 );

    //e2_1.get<Tags4::No2>().set(e1_1.to_standalone_type());
    //e2_1.get<Tags4::No2>().set(e1_1);
}



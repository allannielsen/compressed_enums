#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE BasicUsage
#include <boost/test/unit_test.hpp>

#include <compressed_enums.hxx>

GMB_ENUM_MPL(Cs,
    ((c1, C1))
    ((c2, C2))
    ((c3, C3))
);

typedef Container< unsigned char, Element<Cs> > Cs1_1;
Cs1_1 cs1_1;
Cs1_1 cs1_2;

typedef Container< unsigned, Element<Cs> > Cs4_2;
Cs4_2 cs4_2;

typedef Container< size_t, Element<Cs> > Cs8_3;
Cs8_3 cs8_3;

BOOST_AUTO_TEST_CASE(print) {
    std::stringstream ss;
    ss << cs1_1 << std::endl;
    ss << cs4_2 << std::endl;
    ss << cs8_3 << std::endl;
}


void xx(Cs1_1::inner_standalone_type) {}

BOOST_AUTO_TEST_CASE(conversions) {
    cs1_1 = Cs::C2;

    cs1_1 = cs1_2;

    cs1_1 = cs4_2;
    cs1_1 = cs8_3;

    cs4_2 = cs1_1;
    cs4_2 = cs8_3;

    cs8_3 = cs1_1;
    cs8_3 = cs4_2;
}
BOOST_AUTO_TEST_CASE(assign) {
    cs1_1 = Cs::C1;
    cs4_2 = Cs::C2;
    cs8_3 = Cs::C3;

    BOOST_CHECK( cs1_1 == Cs::C1 );
    BOOST_CHECK( cs4_2 == Cs::C2 );
    BOOST_CHECK( cs8_3 == Cs::C3 );
    BOOST_CHECK( cs1_1.get() == Cs::C1 );
    BOOST_CHECK( cs4_2.get() == Cs::C2 );
    BOOST_CHECK( cs8_3.get() == Cs::C3 );

    cs1_1 = cs4_2;
    cs8_3 = cs4_2;

    BOOST_CHECK( cs1_1 == Cs::C2 );
    BOOST_CHECK( cs4_2 == Cs::C2 );
    BOOST_CHECK( cs8_3 == Cs::C2 );
    BOOST_CHECK( cs1_1.get() == Cs::C2 );
    BOOST_CHECK( cs4_2.get() == Cs::C2 );
    BOOST_CHECK( cs8_3.get() == Cs::C2 );

}
BOOST_AUTO_TEST_CASE(static_size) {
    BOOST_CHECK( Cs1_1::size == 3 );
    BOOST_CHECK( Cs4_2::size == 3 );
    BOOST_CHECK( Cs8_3::size == 3 );
}
BOOST_AUTO_TEST_CASE(next) {
    cs1_1 = Cs::C1;
    cs4_2 = Cs::C1;
    cs8_3 = Cs::C1;

    BOOST_CHECK( cs1_1.get() == Cs::C1 );
    BOOST_CHECK( cs4_2.get() == Cs::C1 );
    BOOST_CHECK( cs8_3.get() == Cs::C1 );

    cs1_1.next_no_wrap().next_no_wrap();
    cs4_2.next_no_wrap();
    cs8_3.next_wrap();

    BOOST_CHECK( cs1_1.get() == Cs::C3 );
    BOOST_CHECK( cs4_2.get() == Cs::C2 );
    BOOST_CHECK( cs8_3.get() == Cs::C2 );

    cs1_1.next_no_wrap().next_no_wrap();
    cs4_2.next_no_wrap();
    cs8_3.next_wrap();

    BOOST_CHECK( cs1_1.get() == Cs::C3 );
    BOOST_CHECK( cs4_2.get() == Cs::C3 );
    BOOST_CHECK( cs8_3.get() == Cs::C3 );

    cs1_1.next_no_wrap().next_no_wrap();
    cs4_2.next_no_wrap();
    cs8_3.next_wrap();

    BOOST_CHECK( cs1_1.get() == Cs::C3 );
    BOOST_CHECK( cs4_2.get() == Cs::C3 );
    BOOST_CHECK( cs8_3.get() == Cs::C1 );

    cs8_3.next_wrap().next_wrap().next_wrap();
    BOOST_CHECK( cs8_3.get() == Cs::C1 );
}
BOOST_AUTO_TEST_CASE(prev) {
    cs1_1 = Cs::C3;
    cs4_2 = Cs::C3;
    cs8_3 = Cs::C3;

    BOOST_CHECK( cs1_1.get() == Cs::C3 );
    BOOST_CHECK( cs4_2.get() == Cs::C3 );
    BOOST_CHECK( cs8_3.get() == Cs::C3 );

    cs1_1.prev_no_wrap().prev_no_wrap();
    cs4_2.prev_no_wrap();
    cs8_3.prev_wrap();

    BOOST_CHECK( cs1_1.get() == Cs::C1 );
    BOOST_CHECK( cs4_2.get() == Cs::C2 );
    BOOST_CHECK( cs8_3.get() == Cs::C2 );

    cs1_1.prev_no_wrap().prev_no_wrap();
    cs4_2.prev_no_wrap();
    cs8_3.prev_wrap();

    BOOST_CHECK( cs1_1.get() == Cs::C1 );
    BOOST_CHECK( cs4_2.get() == Cs::C1 );
    BOOST_CHECK( cs8_3.get() == Cs::C1 );

    cs1_1.prev_no_wrap().prev_no_wrap();
    cs4_2.prev_no_wrap();
    cs8_3.prev_wrap();

    BOOST_CHECK( cs1_1.get() == Cs::C1 );
    BOOST_CHECK( cs4_2.get() == Cs::C1 );
    BOOST_CHECK( cs8_3.get() == Cs::C3 );

    cs8_3.prev_wrap().prev_wrap().prev_wrap();
    BOOST_CHECK( cs8_3.get() == Cs::C3 );
}
BOOST_AUTO_TEST_CASE(get_n_set) {
    cs1_1 = Cs::C1;
    cs4_2 = Cs::C1;
    cs8_3 = Cs::C1;

    const Cs1_1& const_cs_1 = cs1_1;
    const Cs4_2& const_cs_2 = cs4_2;
    const Cs8_3& const_cs_3 = cs8_3;

    BOOST_CHECK( cs1_1.get() == Cs::C1 );
    BOOST_CHECK( cs1_1.get().get() == Cs::C1 );
    BOOST_CHECK( const_cs_1.get() == Cs::C1 );
    BOOST_CHECK( const_cs_1.get().get() == Cs::C1 );

    BOOST_CHECK( cs4_2.get() == Cs::C1 );
    BOOST_CHECK( cs4_2.get().get() == Cs::C1 );
    BOOST_CHECK( const_cs_2.get() == Cs::C1 );
    BOOST_CHECK( const_cs_2.get().get() == Cs::C1 );

    BOOST_CHECK( cs8_3.get() == Cs::C1 );
    BOOST_CHECK( cs8_3.get().get() == Cs::C1 );
    BOOST_CHECK( const_cs_3.get() == Cs::C1 );
    BOOST_CHECK( const_cs_3.get().get() == Cs::C1 );

    cs1_1.set( Cs::C1 );
    cs4_2.set( Cs::C2 );
    cs8_3.set( Cs::C3 );

    BOOST_CHECK( cs1_1.get() == Cs::C1 );
    BOOST_CHECK( cs1_1.get().get() == Cs::C1 );
    BOOST_CHECK( const_cs_1.get() == Cs::C1 );
    BOOST_CHECK( const_cs_1.get().get() == Cs::C1 );

    BOOST_CHECK( cs4_2.get() == Cs::C2 );
    BOOST_CHECK( cs4_2.get().get() == Cs::C2 );
    BOOST_CHECK( const_cs_2.get() == Cs::C2 );
    BOOST_CHECK( const_cs_2.get().get() == Cs::C2 );

    BOOST_CHECK( cs8_3.get() == Cs::C3 );
    BOOST_CHECK( cs8_3.get().get() == Cs::C3 );
    BOOST_CHECK( const_cs_3.get() == Cs::C3 );
    BOOST_CHECK( const_cs_3.get().get() == Cs::C3 );

    auto xx = cs1_1;
    cs1_1 = cs4_2;
    cs4_2 = cs8_3;
    cs8_3 = xx;

    BOOST_CHECK( cs1_1.get() == Cs::C2 );
    BOOST_CHECK( cs4_2.get() == Cs::C3 );
    BOOST_CHECK( cs8_3.get() == Cs::C1 );

    cs1_1.set(cs4_2);
    cs4_2.get().set(cs8_3.get());
    cs8_3.set(Cs::C2);

    BOOST_CHECK( cs1_1.get() == Cs::C3 );
    BOOST_CHECK( cs4_2.get() == Cs::C1 );
    BOOST_CHECK( cs8_3.get() == Cs::C2 );
}

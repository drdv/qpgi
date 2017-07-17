#include "common.h"
#include "common_boost_utf.h"
#include "constraints_register.h"
#include "ostream_enum_map.h"

class ConstraintsRegisterFixture
{
public:
    qpgi::ConstraintsRegister constraints_register;

public:
    ConstraintsRegisterFixture()
    {
        constraints_register.initialize(50,50);

        constraints_register.add(6,qpgi::ConstraintActivationStatus::ACTIVE_UPPER_BOUND);
        constraints_register.add(2,qpgi::ConstraintActivationStatus::ACTIVE_LOWER_BOUND);
        constraints_register.add(7,qpgi::ConstraintActivationStatus::ACTIVE_EQUALITY);
        constraints_register.add(9,qpgi::ConstraintActivationStatus::ACTIVE_EQUALITY);
        constraints_register.add(8,qpgi::ConstraintActivationStatus::ACTIVE_LOWER_BOUND);
        constraints_register.add(1,qpgi::ConstraintActivationStatus::ACTIVE_EQUALITY);
    }

    void test_index()
    {
        BOOST_CHECK_EQUAL( constraints_register.get_numb_active_ctr(), 6 );

        BOOST_CHECK_EQUAL( constraints_register.get_index_active(0), 6 );
        BOOST_CHECK_EQUAL( constraints_register.get_index_active(1), 2 );
        BOOST_CHECK_EQUAL( constraints_register.get_index_active(2), 7 );
        BOOST_CHECK_EQUAL( constraints_register.get_index_active(3), 9 );
        BOOST_CHECK_EQUAL( constraints_register.get_index_active(4), 8 );
        BOOST_CHECK_EQUAL( constraints_register.get_index_active(5), 1 );
    }

    void test_status()
    {
        BOOST_CHECK_EQUAL( constraints_register.get_numb_active_ctr(), 6 );

        BOOST_CHECK_EQUAL( constraints_register.get_status(6),
                           qpgi::ConstraintActivationStatus::ACTIVE_UPPER_BOUND );

        // using BOOST_CHECK_EQUAL is possible because I have overloaded
        // std::ostream& operator<<(std::ostream& out, const ConstraintActivationStatus value)
        // see ostream_enum_map.h, otherwise I would have to use
        // BOOST_CHECK( constraints_register.get_status(6) == qpgi::ConstraintActivationStatus::ACTIVE_UPPER_BOUND );
        BOOST_CHECK_EQUAL( constraints_register.get_status(6),
                           qpgi::ConstraintActivationStatus::ACTIVE_UPPER_BOUND );
        BOOST_CHECK_EQUAL( constraints_register.get_status(2),
                           qpgi::ConstraintActivationStatus::ACTIVE_LOWER_BOUND );
        BOOST_CHECK_EQUAL( constraints_register.get_status(7),
                           qpgi::ConstraintActivationStatus::ACTIVE_EQUALITY );
        BOOST_CHECK_EQUAL( constraints_register.get_status(9),
                           qpgi::ConstraintActivationStatus::ACTIVE_EQUALITY );
        BOOST_CHECK_EQUAL( constraints_register.get_status(8),
                           qpgi::ConstraintActivationStatus::ACTIVE_LOWER_BOUND );
        BOOST_CHECK_EQUAL( constraints_register.get_status(1),
                           qpgi::ConstraintActivationStatus::ACTIVE_EQUALITY );
    }

    void test_remove()
    {
        constraints_register.remove_inequality(4);
        constraints_register.remove_inequality(1);

        BOOST_CHECK_EQUAL( constraints_register.get_numb_active_ctr(), 4 );

        BOOST_CHECK_EQUAL( constraints_register.get_index_active(0), 6 );
        BOOST_CHECK_EQUAL( constraints_register.get_index_active(1), 7 );
        BOOST_CHECK_EQUAL( constraints_register.get_index_active(2), 9 );
        BOOST_CHECK_EQUAL( constraints_register.get_index_active(3), 1 );

        BOOST_CHECK_EQUAL( constraints_register.get_status(6),
                           qpgi::ConstraintActivationStatus::ACTIVE_UPPER_BOUND );
        BOOST_CHECK_EQUAL( constraints_register.get_status(2),
                           qpgi::ConstraintActivationStatus::INACTIVE );
        BOOST_CHECK_EQUAL( constraints_register.get_status(7),
                           qpgi::ConstraintActivationStatus::ACTIVE_EQUALITY );
        BOOST_CHECK_EQUAL( constraints_register.get_status(9),
                           qpgi::ConstraintActivationStatus::ACTIVE_EQUALITY );
        BOOST_CHECK_EQUAL( constraints_register.get_status(8),
                           qpgi::ConstraintActivationStatus::INACTIVE );
        BOOST_CHECK_EQUAL( constraints_register.get_status(1),
                           qpgi::ConstraintActivationStatus::ACTIVE_EQUALITY );
    }

};

BOOST_FIXTURE_TEST_CASE( test_ActiveConstraintsRegister_index, ConstraintsRegisterFixture )
{
    test_index();
}

BOOST_FIXTURE_TEST_CASE( test_ActiveConstraintsRegister_status, ConstraintsRegisterFixture )
{
    test_status();
}

BOOST_FIXTURE_TEST_CASE( test_ActiveConstraintsRegister_remove, ConstraintsRegisterFixture )
{
    test_remove();
}

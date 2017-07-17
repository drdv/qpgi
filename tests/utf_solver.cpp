#include "common_boost_utf.h"
#include "solver.h"
#include "ostream_enum_map.h"

class SolverFixture
{
public:
    qpgi::Solver solver;
    qpgi::TerminationReason exitflag;
    qpgi::RealScalar tolerance = 1e-12;

    qpgi::Matrix H, C;
    qpgi::Vector h, c, x, x_ref;

public:

    void solve_and_verify()
    {
        exitflag = solver.solve(x, H, h, C, c);

        BOOST_CHECK_EQUAL(exitflag, qpgi::TerminationReason::SOLVED);
        BOOST_CHECK(x.isApprox(x_ref, tolerance));
    }
};

// problem from
// https://fr.mathworks.com/help/optim/ug/quadprog.html
BOOST_FIXTURE_TEST_CASE(test_problem01, SolverFixture)
{
    qpgi::Index numb_var = 2;
    qpgi::Index numb_ctr = 5;

    x.resize(numb_var);
    x_ref.resize(numb_var);

    H.resize(numb_var,numb_var);
    h.resize(numb_var);
    C.resize(numb_ctr,numb_var);
    c.resize(numb_ctr);

    H << 1, -1, -1, 2;
    C << 1, 1, -1, 2, 2, 1, -1, 0, 0, -1;
    c << 2, 2, 3, 0, 0;
    h << -2, -6;

    x_ref << 0.666666666666667, 1.333333333333333;

    solve_and_verify();
}

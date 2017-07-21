#include "common_boost_utf.h"
#include "solver.h"
#include "ostream_enum_map.h"

class SolverFixture
{
public:
    qpgi::Solver solver;
    qpgi::TerminationReason exitflag;
    qpgi::RealScalar tolerance;

    qpgi::Matrix H, C;
    qpgi::Vector h, c_lb, c_ub, x, x_ref;

    qpgi::RealScalar Inf;

public:
    SolverFixture()
    {
        Inf = 1e10;
        tolerance = 1e-12;
    }

    void solve_and_verify()
    {
        exitflag = solver.solve(x, H, h, C, c_lb, c_ub);

        std::cout << "x = \n" << x << "\n\n";
        std::cout << "iter = \n" << solver.numb_iterations() << "\n\n";

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
    c_lb.resize(numb_ctr);
    c_ub.resize(numb_ctr);

    H << 1, -1, -1, 2;
    C << 1, 1,
        -1, 2,
        2, 1,
        1, 0,
        0, 1;
    c_ub << 2, 2, 3, Inf, Inf;
    c_lb << -Inf, -Inf, -Inf, 0, 0;
    h << -2, -6;

    x_ref << 0.666666666666667, 1.333333333333333;

    solve_and_verify();
}

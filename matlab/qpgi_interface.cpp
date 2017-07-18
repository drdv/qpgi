#include <iostream>
#include "mex.h"

#include "solver.h"
#include "ostream_enum_map.h"

void mexFunction( int numb_output, mxArray *output[], int numb_input, const mxArray *input[] )
{
    // ----------------------------------------------------------------
    // handle mex input:
    // ----------------------------------------------------------------

    const mxArray *H_mex = input[0];
    const mxArray *h_mex = input[1];
    const mxArray *C_mex = input[2];
    const mxArray *c_mex = input[3];
    const mxArray *tol_mex = input[4];

    int numb_var = mxGetM(H_mex);
    int numb_ctr = mxGetM(C_mex);

    mxArray *x_mex = mxCreateDoubleMatrix(numb_var, 1, mxREAL);
    mxArray *u_mex = mxCreateDoubleMatrix(numb_ctr, 1, mxREAL);
    mxArray *numb_iterations = mxCreateDoubleMatrix(1, 1, mxREAL);

    // ----------------------------------------------------------------
    // handle QPGI input:
    // ----------------------------------------------------------------

    // deep copy mex_H because otherwise it would be modified (and in Matlab this would be strange)
    qpgi::Matrix H = Eigen::Map<qpgi::Matrix> (mxGetPr(H_mex), numb_var, numb_var);

    // no need to make deep copies for the other parameters
    Eigen::Map<qpgi::Vector> h(mxGetPr(h_mex), numb_var);
    Eigen::Map<qpgi::Matrix> C(mxGetPr(C_mex), numb_ctr, numb_var);
    Eigen::Map<qpgi::Vector> c(mxGetPr(c_mex), numb_ctr);
    Eigen::Map<qpgi::Vector> x(mxGetPr(x_mex), numb_var);

    double tol = *mxGetPr(tol_mex);

    // ----------------------------------------------------------------
    // solve problem:
    // ----------------------------------------------------------------

    qpgi::Solver solver;
    qpgi::TerminationReason termination_reason;
    try
    {
        termination_reason = solver.solve(x, H, h, C, c, tol);
        Eigen::Map<qpgi::Vector>((double *)mxGetPr(u_mex), numb_ctr, 1) = solver.dual_variable();
        *mxGetPr(numb_iterations) = solver.numb_iterations();
    }
    catch(std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }

    // ----------------------------------------------------------------
    // handle mex output:
    // ----------------------------------------------------------------

    // format termination_reason
    std::ostringstream oss;
    oss << termination_reason;     // termination_reason -> stream
    std::string s = oss.str();     // stream -> string
    s = s.substr(s.find(":") + 2); // strip the 'TerminationReason::' part

    output[0] = x_mex;                     // primal solution
    output[1] = mxCreateString(s.c_str()); // termination reason
    output[2] = u_mex;                     // dual solution
    output[3] = numb_iterations;           // number of iterations

    return;
}

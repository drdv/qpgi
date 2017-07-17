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

    int numb_var = mxGetM(H_mex);
    int numb_ctr = mxGetM(C_mex);

    mxArray *x_mex = mxCreateDoubleMatrix(numb_var, 1, mxREAL);

    // ----------------------------------------------------------------
    // handle QPGI input:
    // ----------------------------------------------------------------

    // deep copy mex_H because otherwise it would be modified (and in Matlab this would be strange)
    qpgi::Matrix H = Eigen::Map<qpgi::Matrix> ((double*) mxGetPr(H_mex), numb_var, numb_var);

    // no need to make deep copies for the other parameters
    Eigen::Map<qpgi::Vector> h((double*) mxGetPr(h_mex), numb_var);
    Eigen::Map<qpgi::Matrix> C((double*) mxGetPr(C_mex), numb_ctr, numb_var);
    Eigen::Map<qpgi::Vector> c((double*) mxGetPr(c_mex), numb_ctr);
    Eigen::Map<qpgi::Vector> x((double*) mxGetPr(x_mex), numb_var);

    // ----------------------------------------------------------------
    // solve problem:
    // ----------------------------------------------------------------

    qpgi::Solver solver;
    qpgi::TerminationReason termination_reason;
    try
    {
        termination_reason = solver.solve(x, H, h, C, c);
    }
    catch(std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }

    // ----------------------------------------------------------------
    // handle mex output:
    // ----------------------------------------------------------------

    output[0] = x_mex;

    // termination_reason -> stream
    std::ostringstream oss;
    oss << termination_reason;

    // stream -> string
    std::string s = oss.str();

    //strip the 'TerminationReason::' part
    s = s.substr(s.find(":") + 2);
    output[1] = mxCreateString(s.c_str());

    return;
}

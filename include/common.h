#pragma once

#include <iostream>
#include <iomanip>
#include <vector>
#include <stdexcept>
#include <Eigen/Dense>
#include <cassert>

namespace qpgi
{
    typedef int    Index;
    typedef double RealScalar;

    typedef Eigen::Matrix<RealScalar, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor> Matrix;
    typedef Eigen::Matrix<RealScalar, Eigen::Dynamic, 1>                               Vector;
    typedef Eigen::Block<Matrix, Eigen::Dynamic, Eigen::Dynamic>                       MatrixBlock;
    typedef Eigen::Block<Matrix, Eigen::Dynamic, 1>                                    VectorBlockFromMatrix;

    enum class ConstraintActivationStatus
    {
        INACTIVE,
        ACTIVE_EQUALITY,
        ACTIVE_LOWER_BOUND,
        ACTIVE_UPPER_BOUND
    };

    enum class TerminationReason
    {
        SOLVED,
        INFEASIBLE,
        HESSIAN_FACTORIZATION_PROBLEMS,
        INCONSISTENT_BOUNDS,
        ITERATIONS_LIMIT
    };

    enum class StepType
    {
        UNDEFINED,
        STOP,
        DUAL_STEP,
        PARTIAL_STEP,
        FULL_STEP
    };

    struct StepLength
    {
        RealScalar _primal;
        RealScalar _dual;
        RealScalar _actual;
        Index _active_ctr_index_with_blocking_dual;
        StepType _step_type;

        void reset(StepType step_type = StepType::UNDEFINED)
        {
            _primal = std::numeric_limits<double>::infinity();
            _dual = std::numeric_limits<double>::infinity();
            _actual = 0;
            // index of active (primal) constraint, whose assocated
            // dual constraint prevents making a full primal step
            _active_ctr_index_with_blocking_dual = -1;
            _step_type = step_type;
        }

        void determine_actual_step()
        {
            if ((_primal == std::numeric_limits<double>::infinity()) &&
                (_dual == std::numeric_limits<double>::infinity()))
            {
                _step_type = StepType::STOP;
            }
            else if (_primal == std::numeric_limits<double>::infinity())
            {
                _step_type = StepType::DUAL_STEP;
                _actual = _dual;
            }
            else
            {
                _step_type = (_primal <= _dual) ? StepType::FULL_STEP: StepType::PARTIAL_STEP;
                if (_step_type == StepType::FULL_STEP)
                {
                    _actual = _primal;
                }
                else
                {
                    _actual = _dual;
                }
            }
        }
    };

    struct CandidateConstraint
    {
        Index _index;
        RealScalar _dual_variable;
        RealScalar _ctr_violation;
        RealScalar _step_along_the_ctr_normal;
        ConstraintActivationStatus _status;

        void reset(RealScalar tolerance)
        {
            _index = -1;
            _dual_variable = 0.0;
            _ctr_violation = tolerance;
            _step_along_the_ctr_normal = 0;
            _status = ConstraintActivationStatus::INACTIVE;
        }

        void update_ctr_violation(RealScalar step_length)
        {
            _ctr_violation += step_length * _step_along_the_ctr_normal;
        }

        void update_step_along_the_ctr_normal(RealScalar step_along_the_ctr_normal)
        {
            _step_along_the_ctr_normal = step_along_the_ctr_normal;
        }
    };

}

#pragma once

#include "solver_eqp.h"
#include "constraints_register.h"

namespace qpgi
{
    class Solver
    {
    private:
        SolverEQP           _eqp;
        CandidateConstraint _candidate_constraint;
        ConstraintsRegister _constraints_register;
        StepLength          _step_length;

        Vector _dual_variable;                  // same order as the constraints
        Vector _dual_step_direction_active_ctr; // same order as the active constraints
        Vector _primal_step_direction;

        Vector _ctr_normals_dot_x;
        Index  _iteration_number;

        // FIXME: put parameters in a separate class
        RealScalar _tolerance;
        //Index _iteration_limit;

    public:

        /**
           \brief minimize 0.5*x'*H*x + x'*h, subject to C*x <= c
        */
        TerminationReason solve(Eigen::Ref<Vector> x,
                                Eigen::Ref<Matrix> H,
                                const Eigen::Ref<Vector> h,
                                const Eigen::Ref<Matrix> C,
                                const Eigen::Ref<Vector> c,
                                RealScalar tolerance=1e-12)
        {
            _tolerance = tolerance;

            // -----------------------------------------------------------------------------
            // FIXME: delegate the parsing of the problem data
            // -----------------------------------------------------------------------------
            Index primal_variable_size = h.size();
            Index dual_variable_size = c.size(); // number of constraints
            // -----------------------------------------------------------------------------

            // Initialize these here because even if the unconstrained minimizer
            // solves the problem, the user still might want to inspect them.
            _constraints_register.initialize(primal_variable_size, dual_variable_size);
            _dual_variable.setZero(dual_variable_size);

            // inplace Cholesky decomposition (stored in the lower triangular part of H)
            Eigen::LLT<Eigen::Ref<Matrix>, Eigen::Lower> chol(H);

            // Note that Cholesky decomposition is not rank-revealing, e.g.,
            // H = [2 -2;-2 2]; has Cholesky decomposition L = chol(H) = [x x;0 2.107342e-08];
            // On the other hand Matlab/Octave/Eigen cannot Cholesky decompose H = [1 -1;-1 1];
            // So, here we terminate when Eigen reports problems during the factorization process.
            // Otherwise we proceed with solving the problem even though the Hessian might be singular.
            if(chol.info() == Eigen::NumericalIssue)
            {
                return TerminationReason::HESSIAN_FACTORIZATION_PROBLEMS;
            }

            // compute unconstrained minimizer
            x.noalias() = -chol.solve(h);

            _iteration_number = 0;
            _step_length.initialize(StepType::FULL_STEP);
            while (true)
            {
                // determine a new candidate constraint
                if (_step_length._step_type == StepType::FULL_STEP)
                {
                    _ctr_normals_dot_x.noalias() = C*x;
                    find_violated_constraint(c, _candidate_constraint);

                    // no candidate constraints for activation
                    if (_candidate_constraint._status == ConstraintActivationStatus::INACTIVE)
                    {
                        return TerminationReason::SOLVED;
                    }
                }

                // enter here only during the first iteration
                // if there is a need for a constraint to be activated
                if (_iteration_number == 0)
                {
                    _primal_step_direction.resize(primal_variable_size);
                    _dual_step_direction_active_ctr.resize(primal_variable_size);
                    _eqp.prepare_factorization_storage(H);
                }

                _step_length.reset();

                // compute primal & dual step directions as well as the primal step length
                _eqp.solve(_primal_step_direction,
                           _dual_step_direction_active_ctr,
                           _step_length,
                           _candidate_constraint,
                           C,
                           _tolerance);

                // compute dual step length
                RealScalar a_dual_step_length;
                for (Index i=0; i<_constraints_register.get_numb_active_ctr(); ++i)
                {
                    if (_dual_step_direction_active_ctr[i] < -_tolerance)
                    {
                        a_dual_step_length =
                            -_dual_variable[_constraints_register.get_index_active(i)] /
                            _dual_step_direction_active_ctr[i];
                        if (a_dual_step_length < _step_length._dual)
                        {
                            _step_length._dual = a_dual_step_length;
                            _step_length._active_ctr_index_with_blocking_dual = i;
                        }
                    }
                }

                _step_length.determine_actual_step();

                if (_step_length._step_type == StepType::STOP)
                {
                    return TerminationReason::INFEASIBLE;
                }
                else if (_step_length._step_type == StepType::DUAL_STEP)
                {
                    perform_dual_step();
                    _constraints_register.remove_inequality(_step_length._active_ctr_index_with_blocking_dual);
                    _eqp.remove_constraint_from_basis(_step_length._active_ctr_index_with_blocking_dual);
                }
                else
                {
                    x += _step_length._actual * _primal_step_direction;
                    _candidate_constraint.update_ctr_violation(_step_length._actual);
                    perform_dual_step();
                    if (_step_length._step_type == StepType::FULL_STEP)
                    {
                        _constraints_register.add(_candidate_constraint._index,
                                                  ConstraintActivationStatus::ACTIVE_UPPER_BOUND);
                        _dual_variable(_candidate_constraint._index) = _candidate_constraint._dual_variable;
                        _eqp.add_candidate_constraint_to_basis();
                    }
                    else // _step_lengt._step_type == StepType::PARTIAL_STEP
                    {
                        _constraints_register.remove_inequality(_step_length._active_ctr_index_with_blocking_dual);
                        _eqp.remove_constraint_from_basis(_step_length._active_ctr_index_with_blocking_dual);
                    }
                }

                _iteration_number++;
            }
        }

        void perform_dual_step()
        {
            for (Index i=0; i<_constraints_register.get_numb_active_ctr(); ++i)
            {
                _dual_variable[_constraints_register.get_index_active(i)] +=
                    _step_length._actual *
                    _dual_step_direction_active_ctr[i];
            }
            _candidate_constraint._dual_variable += _step_length._actual;
        }

        void find_violated_constraint(const Eigen::Ref<Vector> c,
                                      CandidateConstraint & candidate_constraint)
        {
            candidate_constraint.reset(_tolerance);
            RealScalar ctr_violation;
            for (Index i=0; i<_constraints_register.get_numb_ctr(); ++i)
            {
                if (!_constraints_register.is_active(i))
                {
                    ctr_violation = _ctr_normals_dot_x(i) - c(i);
                    if (ctr_violation > candidate_constraint._ctr_violation)
                    {
                        candidate_constraint._index = i;
                        candidate_constraint._ctr_violation = ctr_violation;
                        candidate_constraint._status = ConstraintActivationStatus::ACTIVE_UPPER_BOUND;
                    }
                }
            }
        }

        // -----------------------------------------------------------------------
        // getters
        // -----------------------------------------------------------------------

        Vector dual_variable()
        {
            return _dual_variable;
        }

        Index numb_iterations()
        {
            return _iteration_number+1;
        }
    };
}

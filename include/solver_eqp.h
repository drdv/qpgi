#pragma once

#include "common.h"

namespace qpgi
{
    // This class:
    // 1. should be agnostic about which bound is active, or whether it handles equality or inequality constraints
    // 2. TODO: should be aware of structure in order to exploit it (e.g., simple bounds)
    class SolverEQP
    {
    private:
        Matrix _R;
        Matrix _J;
        Index _numb_var;
        Index _numb_ctr;
        Eigen::JacobiRotation<RealScalar> _gr;

    public:
        void prepare_factorization_storage(Eigen::Ref<Matrix> chol_stored_in_upper_H)
        {
            _numb_ctr = 0;
            _numb_var = chol_stored_in_upper_H.cols();

            // form J = inv(L)'
            _J.setIdentity(_numb_var,_numb_var);

            // --------------------------------------------------------------------------------------------
            // FIXME: to do something about this
            // --------------------------------------------------------------------------------------------
            // When in solver.h I use: Eigen::LLT<Eigen::Ref<Matrix>, Eigen::Upper> chol(H);
            // The code below has problems when solving with the Cholesky factor of the regularized Hessian in
            // ../matlab/other_test_problems/test_problem_5.m: 2*10^-5*[1 -1;-1 1] + 1e-14*eye(2)
            // chol_stored_in_upper_H.triangularView<Eigen::Upper>().solveInPlace(_J);

            // The above problem doesn't appear when I do the transposition trick below.
            // In solver.h I use: Eigen::LLT<Eigen::Ref<Matrix>, Eigen::Lower> chol(H);
            // but I require the extra transposeInPlace()
            chol_stored_in_upper_H.triangularView<Eigen::Lower>().solveInPlace(_J);
            _J.transposeInPlace();
            // --------------------------------------------------------------------------------------------

            // I allocate one extra column which would store the vector 'd' when _numb_ctr == _numb_var
            // In this way I can update 'd' in remove_constraint_from_basis(...)
            _R.resize(_numb_var,_numb_var+1);
        }

        void solve(Eigen::Ref<Vector> primal_step_direction,
                   Eigen::Ref<Vector> dual_step_direction,
                         StepLength & step_length,
                         CandidateConstraint & candidate_constraint,
                         const Eigen::Ref<Matrix> C,
                         const RealScalar tolerance)
        {
            // form blocks for convenience
            VectorBlockFromMatrix d (_R,         0, _numb_ctr,             _numb_var, 1                    );
            VectorBlockFromMatrix d1(_R,         0, _numb_ctr, _numb_ctr            , 1                    );
            VectorBlockFromMatrix d2(_R, _numb_ctr, _numb_ctr, _numb_var - _numb_ctr, 1                    );
            MatrixBlock           R (_R,         0,         0, _numb_ctr            , _numb_ctr            );
            MatrixBlock           J2(_J,         0, _numb_ctr, _numb_var            , _numb_var - _numb_ctr);

            // if during the previous iteration a StepType::FULL_STEP was performed
            if (step_length._step_type == StepType::FULL_STEP)
            {
                // form 'd' from scratch (we have a new candidate constraint to deal with)
                d.noalias() = _J.transpose() * C.row(candidate_constraint._index).transpose();
            }
            else // if StepType::DUAL_STEP or StepType::PARTIAL_STEP
            {
                // copy 'd' from the previous step
                d = _R.col(_numb_ctr+1);
            }

            // form dual step direction
            if (_numb_ctr > 0)
            {
                // FIXME: if the inequality constraints come after the equality constraints
                //        we can backsolve only for the former ones if desired
                dual_step_direction.head(_numb_ctr).noalias() = -R.triangularView<Eigen::Upper>().solve(d1);
            }

            // form primal step direction
            if (_numb_ctr < _numb_var)
            {
                primal_step_direction.noalias() = -J2 * d2;
            }
            else
            {
                // when _numb_ctr == _numb_var, primal_step_direction = 0,
                // but there is no need to set it because it would not be used
                // no need to set candidate_constraint.update_step_along_the_ctr_normal either
                // so I simply return
                return;
            }

            RealScalar denominator = C.row(candidate_constraint._index) * primal_step_direction;
            // idea from qpmad (however, it seems to be more reasonable to square the tolerance)
            // (this hasn't been tested properly)
            if (std::abs(denominator) >= tolerance*tolerance)
            {
                step_length._primal = -candidate_constraint._ctr_violation / denominator;
                candidate_constraint.update_step_along_the_ctr_normal(denominator);
            }
        }

        // assumes that 'd' has been formed appropriately
        double add_candidate_constraint_to_basis()
        {
            // form blocks for convenience
            VectorBlockFromMatrix d2(_R, _numb_ctr, _numb_ctr, _numb_var - _numb_ctr, 1                    );
            MatrixBlock           J2(_J,         0, _numb_ctr, _numb_var            , _numb_var - _numb_ctr);

            // update factorization
            RealScalar givens_norm;
            for (Index i=1; i<d2.size(); ++i)
            {
                _gr.makeGivens(d2.coeffRef(0),
                               d2.coeffRef(i),
                               &givens_norm);

                // apply _gr.adjoint() to the rows of d2
                d2.coeffRef(0) = givens_norm;
                d2.coeffRef(i) = 0;

                // apply _gr to the columns of J2
                J2.applyOnTheRight(0,i,_gr);
            }

            _numb_ctr++;

            // Return an indication whether the candidat constraint is linearly dependent.
            // Note that this is guaranteed to be the case by the inned GI loop and thus
            // this return value is never checked. But one might want to use this class
            // on its own.
            return givens_norm;
        }

        void remove_constraint_from_basis(Index ctr_index)
        {
            // because the algorithm cannot include more than _numb_var constraints
            // in the working set we cannot go out of range for _R due to the "+1"
            MatrixBlock R (_R, 0, 0, _numb_ctr, _numb_ctr+1);

            // downdate factorization
            // (no work is required when removing the last constraint)
            RealScalar givens_norm;
            while (ctr_index < _numb_ctr-1)
            {
                // FIXME: this should be written in a more descriptive way
                Index trailing_columns = _numb_ctr - ctr_index - 1;

                _gr.makeGivens(R.coeffRef(ctr_index  ,ctr_index+1),
                               R.coeffRef(ctr_index+1,ctr_index+1),
                               &givens_norm);

                // apply _gr.adjoint() to the rows of R
                R.coeffRef(ctr_index  ,ctr_index+1) = givens_norm;
                R.coeffRef(ctr_index+1,ctr_index+1) = 0;
                // here we apply the transformation to the 'd' as well
                // (it is stored in the last column)
                R.rightCols(trailing_columns).applyOnTheLeft(ctr_index,
                                                             ctr_index+1,
                                                             _gr.adjoint());

                // shift garbage to the right
                R.col(ctr_index).head(ctr_index+1) = R.col(ctr_index+1).head(ctr_index+1);

                // apply _gr to the columns of J
                _J.applyOnTheRight(ctr_index,ctr_index+1,_gr);

                ctr_index++;
            }

            _numb_ctr--;
        }
    };
}

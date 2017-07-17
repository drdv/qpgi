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
            chol_stored_in_upper_H.triangularView<Eigen::Upper>().solveInPlace(_J);

            // From this point on H is never used and one is tempted to store _R in it.
            // Doing this using a dependency injection with a reference seems to be the way
            // to go even though it would restrict the use of the class. Anyway, for the moment
            // I simply allocate memory.
            _R.resize(_numb_var,_numb_var);
        }

        void solve(Eigen::Ref<Vector> primal_step_direction,
                   Eigen::Ref<Vector> dual_step_direction,
                         RealScalar & primal_step_length,
                         CandidateConstraint & candidate_constraint,
                         const Eigen::Ref<Matrix> C,
                         const RealScalar tolerance)
        {
            // Protection against accessing memory outside of what is allocated for _R.
            // Since _R is full-rank, when _numb_ctr == _numb_var, the primal step is zero.
            // FIXME: to organize differently
            if (_numb_ctr == _numb_var)
            {
                primal_step_direction.setZero();
                dual_step_direction.noalias() =
                    -_R.triangularView<Eigen::Upper>()
                    .solve(_J.transpose() * C.row(candidate_constraint._index).transpose());
                return;
            }

            // form blocks for convenience
            VectorBlockFromMatrix d (_R,         0, _numb_ctr,             _numb_var, 1                    );
            VectorBlockFromMatrix d1(_R,         0, _numb_ctr, _numb_ctr            , 1                    );
            VectorBlockFromMatrix d2(_R, _numb_ctr, _numb_ctr, _numb_var - _numb_ctr, 1                    );
            MatrixBlock           R (_R,         0,         0, _numb_ctr            , _numb_ctr            );
            MatrixBlock           J2(_J,         0, _numb_ctr, _numb_var            , _numb_var - _numb_ctr);

            // FIXME: I do the product explicitly for the moment
            d.noalias() = _J.transpose() * C.row(candidate_constraint._index).transpose();

            // form step directions in primal and dual space
            primal_step_direction.noalias() = -J2 * d2;
            if (_numb_ctr > 0)
            {
                // FIXME: if the inequality constraints come after the equality constraints
                //        we can backsolve only for the former ones if desired
                dual_step_direction.head(_numb_ctr).noalias() = -R.triangularView<Eigen::Upper>().solve(d1);
            }

            RealScalar denominator = C.row(candidate_constraint._index) * primal_step_direction;
            if (std::abs(denominator) >= tolerance) // idea from qpmad (I am not sure about the implications)
            {
                primal_step_length = -candidate_constraint._ctr_violation / denominator;
            }
            candidate_constraint.update_step_along_the_ctr_normal(denominator);
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
            MatrixBlock R (_R, 0, 0, _numb_ctr, _numb_ctr);

            // downdate factorization
            // (no work is required when removing the last constraint)
            RealScalar givens_norm;
            while (ctr_index < _numb_ctr-1)
            {
                Index trailing_columns = _numb_ctr - ctr_index - 2;

                _gr.makeGivens(R.coeffRef(ctr_index  ,ctr_index+1),
                               R.coeffRef(ctr_index+1,ctr_index+1),
                               &givens_norm);

                // apply _gr.adjoint() to the rows of R
                R.coeffRef(ctr_index  ,ctr_index+1) = givens_norm;
                R.coeffRef(ctr_index+1,ctr_index+1) = 0;
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

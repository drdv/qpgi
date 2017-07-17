#pragma once

namespace qpgi
{
    class ConstraintsRegister
    {
    private:
        std::vector<Index> _indexes_active_ctr;
        std::vector<ConstraintActivationStatus> _status_all_ctr;
        Index _numb_active_eq;
        Index _numb_active_ineq;

    public:
        void initialize(const Index max_number_of_active_constraints,
                        const Index number_of_all_constraints)
        {
            _indexes_active_ctr.reserve(max_number_of_active_constraints);
            _status_all_ctr.resize(number_of_all_constraints, ConstraintActivationStatus::INACTIVE);
            _numb_active_eq   = 0;
            _numb_active_ineq = 0;
        }

        void add(const Index ctr_index,
                 const ConstraintActivationStatus ctr_status)
        {
            // FIXME: to remove
            assert(ctr_status != ConstraintActivationStatus::INACTIVE &&
                   "Active constraints can be: ACTIVE_EQUALITY, ACTIVE_LOWER_BOUND, ACTIVE_UPPER_BOUND");

            if (ctr_status == ConstraintActivationStatus::ACTIVE_EQUALITY)
                ++_numb_active_eq;
            else
                ++_numb_active_ineq;

            _indexes_active_ctr.push_back(ctr_index);
            _status_all_ctr[ctr_index] = ctr_status;
        }

        void remove_inequality(const Index active_ctr_index)
        {
            // FIXME: to remove
            assert(_status_all_ctr[get_index_active(active_ctr_index)] != ConstraintActivationStatus::ACTIVE_EQUALITY &&
                   "deactivating an equality constraint");

            _status_all_ctr[get_index_active(active_ctr_index)] = ConstraintActivationStatus::INACTIVE;

            // preserve the order of remaining constraints
            _indexes_active_ctr.erase(_indexes_active_ctr.begin() + active_ctr_index);
            --_numb_active_ineq;
        }

        inline Index get_index_active(const Index active_ctr_index)
        {
            return _indexes_active_ctr[active_ctr_index];
        }

        inline ConstraintActivationStatus get_status(const Index ctr_index)
        {
            return _status_all_ctr[ctr_index];
        }

        inline Index get_numb_ctr()
        {
            return _status_all_ctr.size();
        }

        inline Index get_numb_active_ctr()
        {
            return _indexes_active_ctr.size();
        }

        inline Index get_numb_active_eq()
        {
            return _numb_active_eq;
        }

        inline Index get_numb_active_ineq()
        {
            return _numb_active_ineq;
        }

        inline Index is_active(const Index ctr_index)
        {
            if (_status_all_ctr[ctr_index] == ConstraintActivationStatus::INACTIVE)
                return false;
            else
                return true;
        }
    };
}

#pragma once

#include <map>

namespace qpgi
{
#define INSERT_ELEMENT(p) map[p] = #p

    // required in tests/constraints_register.cpp:test_status()
    std::ostream& operator<<(std::ostream& os, const ConstraintActivationStatus value)
    {
        static std::map<ConstraintActivationStatus, std::string> map;
        if (map.size() == 0)
        {
            INSERT_ELEMENT(ConstraintActivationStatus::INACTIVE);
            INSERT_ELEMENT(ConstraintActivationStatus::ACTIVE_EQUALITY);
            INSERT_ELEMENT(ConstraintActivationStatus::ACTIVE_LOWER_BOUND);
            INSERT_ELEMENT(ConstraintActivationStatus::ACTIVE_UPPER_BOUND);
        }

        return os << map[value];
    }

    std::ostream& operator<<(std::ostream& os, const TerminationReason value)
    {
        static std::map<TerminationReason, std::string> map;
        if (map.size() == 0)
        {
            INSERT_ELEMENT(TerminationReason::SOLVED);
            INSERT_ELEMENT(TerminationReason::INFEASIBLE);
            INSERT_ELEMENT(TerminationReason::HESSIAN_FACTORIZATION_PROBLEMS);
            INSERT_ELEMENT(TerminationReason::INCONSISTENT_BOUNDS);
            INSERT_ELEMENT(TerminationReason::ITERATIONS_LIMIT);
        }

        return os << map[value];
    }

    std::ostream& operator<<(std::ostream& os, const StepType value)
    {
        static std::map<StepType, std::string> map;
        if (map.size() == 0)
        {
            INSERT_ELEMENT(StepType::UNDEFINED);
            INSERT_ELEMENT(StepType::STOP);
            INSERT_ELEMENT(StepType::DUAL_STEP);
            INSERT_ELEMENT(StepType::PARTIAL_STEP);
            INSERT_ELEMENT(StepType::FULL_STEP);
        }

        return os << map[value];
    }

#undef INSERT_ELEMENT
}

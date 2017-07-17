#pragma once

#include <fstream>
#include "ostream_enum_map.h"

namespace qpgi
{
    struct EigenOutputFormat
    {
        static Eigen::IOFormat matlab()
        {
            return Eigen::IOFormat(Eigen::FullPrecision, 0, ", ", ";\n", "", "", "[", "];\n");
        }
    };

    class Logger
    {
    public:
        template<typename Derived>
        static void log_eigen_data(std::ostream &os,
                                   const Eigen::MatrixBase<Derived> &data,
                                   const std::string &name,
                                   const Index index=0)
        {
            if (index <= 0)
            {
                os << name << " = ...\n"
                   << data.format(EigenOutputFormat::matlab()) << "\n";
            }
            else
            {
                os << name << "{" << index << "} = ...\n"
                   << data.format(EigenOutputFormat::matlab()) << "\n";
            }
        }

        template<typename T>
        static void log_data(std::ostream &os,
                             const T &data,
                             const std::string &name,
                             const Index index=0)
        {
            if (index <= 0)
            {
                os << name << " = "
                   << data << ";\n";
            }
            else
            {
                os << name << "{" << index << "} = "
                   << data << ";\n";
            }
        }

        static void log_text(std::ostream &os,
                             const std::string &text)
        {
            os << text << "\n\n";
        }

        static void log_step_length(std::ostream &os,
                                    StepLength &step_length)
        {
            os << "step_length:\n"
               << "  primal        : " << step_length._primal << "\n"
               << "  dual          : " << step_length._dual << "\n"
               << "  actual        : " << step_length._actual << "\n"
               << "  blocking_index: " << step_length._active_ctr_index_with_blocking_dual << "\n"
               << "  step_type     : " << step_length._step_type << "\n\n";
        }

        static void log_candidate_constraint(std::ostream &os,
                                             CandidateConstraint &candidate_constraint)
        {
            os << "candidate_constraint:\n"
               << "  index    : " << candidate_constraint._index << "\n"
               << "  violation: " << candidate_constraint._ctr_violation << "\n"
               << "  status   : " << candidate_constraint._status << "\n"
               << "  dual     : " << candidate_constraint._dual_variable << "\n\n";
        }

        static void log_constraints_register(std::ostream &os,
                                             ConstraintsRegister &constraints_register,
                                             bool show_status = false)
        {
            os << "constraints_register:\n"
               << "  numb constraints      : " << constraints_register.get_numb_ctr() << "\n"
               << "  numb active inequality: " << constraints_register.get_numb_active_ineq() << "\n"
               << "  numb active equality  : " << constraints_register.get_numb_active_eq() << "\n"
               << "  active: { ";
            for (Index i=0; i<constraints_register.get_numb_active_ctr(); ++i)
            {
                os << constraints_register.get_index_active(i) << " ";
            }
            os << "}\n";

            if (show_status)
            {
                for (Index i=0; i<constraints_register.get_numb_ctr(); ++i)
                {
                    os << "  " << i << ": "
                       << constraints_register.get_status(i)
                       << "\n";
                }
            }

            os << "\n";
        }

        static void insert_separator(std::ostream &os)
        {
            os << "%% " + std::string(80, '=') << "\n\n";
        }

        static void insert_empty_line(std::ostream &os)
        {
            os << "\n";
        }

        static std::ofstream get_ofstream(const std::string &file_name,
                                          bool clear = false)
        {
            if (clear)
            {
                std::ofstream tmp_ofs(file_name, std::ios::out | std::ofstream::trunc);
                tmp_ofs.close();
            }
            return std::ofstream(file_name, std::ios::out | std::ios::app);
        }

    };
}

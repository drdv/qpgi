find_package(Boost REQUIRED unit_test_framework)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})

qpgi_add_test("utf_constraints_register" "utf_constraints_register.cpp")
qpgi_add_test("utf_solver" "utf_solver.cpp")

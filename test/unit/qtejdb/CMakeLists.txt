find_package(Qt5Core)
find_package(Qt5Test)

add_executable ( tst_qejdb main.cpp tst_qejdbdatabase.cpp tst_qejdbcollection.cpp tst_qejdbperformance.cpp tst_qbsonmodel.cpp tst_base.cpp)
target_link_libraries ( tst_qejdb Qt5::Core Qt5::Test qtrpc2 qejdb qbson pthread z)
target_include_directories (tst_qejdb PUBLIC ../../include)

add_test(tst_qejdb tst_qejdb)
SETUP_TARGET_FOR_COVERAGE(tst_qejdb_coverage  tst_qejdb coverage)

enable_testing()

file(GLOB QML_JS_FILES *.qml *.js)

add_custom_target(tst_qejdbmodel_QmlFiles ALL SOURCES ${QML_JS_FILES})
set(tst_qejdbmodel_SRCS qtquicktest.cpp)

qt5_add_resources(tst_qejdbmodel_QRC qtquicktest.qrc)

add_executable(qtquicktest ${tst_qejdbmodel_SRCS} ${tst_qejdbmodel_QRC})

target_link_libraries(
    qtquicktest
    Qt5::Qml
    Qt5::Quick
    Qt5::QuickTest
)

add_custom_command(TARGET qtquicktest POST_BUILD
  COMMAND cp -uv ${CMAKE_CURRENT_SOURCE_DIR}/*.qml ${CMAKE_CURRENT_BINARY_DIR}
)

add_test(QQmlPluginUnitTest qtquicktest -txt -import ${CMAKE_BINARY_DIR}/plugins)

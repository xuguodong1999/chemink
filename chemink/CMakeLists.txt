project(chemink)

add_compile_definitions(QT_DISABLE_DEPRECATED_BEFORE=ON)

qt5_add_resources(RES_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/res/chemink.qrc
        ${CMAKE_CURRENT_SOURCE_DIR}/res/obabel.qrc)

qt5_add_big_resources(BIG_RES_FILES ${CMAKE_CURRENT_SOURCE_DIR}/res/big_res.qrc)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SRC_FILES)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/balance SRC_BALANCE)
add_executable(${PROJECT_NAME}
        # WIN32
        ${SRC_FILES}
        ${SRC_BALANCE}
        ${RES_FILES}
        ${BIG_RES_FILES}
        ${SRC_DIR}/cocr_types.cpp
        ${SRC_DIR}/std_util.cpp
        ${SRC_DIR}/isomer.cpp
        ${SRC_DIR}/timer.cpp
        "chemink.rc")

target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:/MTd>
        $<$<OR:$<CONFIG:Release>>:/MT>)

target_compile_definitions(${PROJECT_NAME} PRIVATE
        $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

find_package(OpenGL REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt5::Widgets Qt5::OpenGL
        ${OPENBABEL2_LIBRARIES}
        ${OpenCV_LIBS}
        OpenGL::GL
        OpenGL::GLU)

        

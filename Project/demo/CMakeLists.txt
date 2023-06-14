cmake_minimum_required(VERSION 3.8)

set(target_name "demo")

set(INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/src/MLIB)

# 设置可执行文件的输出路径
set(OUTPATH ${CMAKE_BINARY_DIR}/Project/${target_name})

file(GLOB SRCS ${CMAKE_SOURCE_DIR}/src/MLIB/*.cpp)
file(GLOB HEADERS ${CMAKE_SOURCE_DIR}/src/MLIB/*.h)
file(GLOB LIBRARIES ${CMAKE_SOURCE_DIR}/lib/*.[al]ib)  # 使用 *.a 文件替代 *.lib 文件
file(GLOB DYNAMICLIB ${CMAKE_SOURCE_DIR}/dll/*.dll)

# 若要添加自己的库，可以将注释取消
add_library(m_library
	${SRCS}
	${HEADERS}
)

target_link_libraries(m_library PRIVATE ${LIBRARIES})


add_executable(${target_name} "main.cpp")

# 将动态库拷贝到可执行文件目录下
add_custom_command(TARGET ${target_name} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${DYNAMICLIB}
    ${OUTPATH}/
)

include_directories(${INCLUDE_DIRS} ${HEADERS})

target_include_directories(${target_name} PRIVATE ${INCLUDE_DIRS} ${HEADERS})
target_link_libraries(${target_name} PRIVATE ${LIBRARIES} m_library)

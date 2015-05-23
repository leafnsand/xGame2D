include_directories(../src)
aux_source_directory(. mingw_sample_src)
add_executable(mingw_sample ${mingw_sample_src})
target_link_libraries(mingw_sample xGame2D glfw3 glew32 png opengl32)
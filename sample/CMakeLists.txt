include_directories(../xGame2D)
aux_source_directory(. sample_src)
add_executable(sample ${sample_src})
target_link_libraries(sample xGame2D glfw3 glew32 png opengl32)
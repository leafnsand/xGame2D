include_directories(.)
if(WIN32)
    set(xGame2D_platform_src
        platform/windows/ApplicationWindows.cpp
        platform/windows/FileUtilsWindows.cpp
        platform/windows/OpenGLWindows.cpp
    )
endif()
set(xGame2D_platform_src
    ${xGame2D_platform_src}
    platform/Application.cpp
    platform/FileUtils.cpp
    platform/OpenGL.cpp
)
aux_source_directory(base xGame2D_base_src)
aux_source_directory(display xGame2D_display_src)
aux_source_directory(geom xGame2D_geom_src)
aux_source_directory(render xGame2D_render_src)
aux_source_directory(textures xGame2D_textures_src)
aux_source_directory(timer xGame2D_timer_src)
aux_source_directory(util xGame2D_util_src)
set(xGame2D_src
    ${xGame2D_base_src}
    ${xGame2D_display_src}
    ${xGame2D_geom_src}
    ${xGame2D_platform_src}
    ${xGame2D_render_src}
    ${xGame2D_textures_src}
    ${xGame2D_timer_src}
    ${xGame2D_util_src}
)
add_library(xGame2D ${xGame2D_src})
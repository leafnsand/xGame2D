include_directories(. ../third-party/win32/include)
aux_source_directory(base xGame2D_base_src)
aux_source_directory(display xGame2D_display_src)
aux_source_directory(geom xGame2D_geom_src)
aux_source_directory(render xGame2D_render_src)
aux_source_directory(textures xGame2D_textures_src)
aux_source_directory(timer xGame2D_timer_src)
aux_source_directory(util xGame2D_util_src)
set(xGame2D_src Game.cpp
    ${xGame2D_base_src}
    ${xGame2D_display_src}
    ${xGame2D_geom_src}
    ${xGame2D_render_src}
    ${xGame2D_textures_src}
    ${xGame2D_timer_src}
    ${xGame2D_util_src}
)
add_library(xGame2D ${xGame2D_src} timer/Timer.cpp timer/Timer.h timer/TimerHandler.cpp timer/UpdateTimer.cpp timer/UpdateTimer.h timer/CallbackTimer.cpp timer/CallbackTimer.h)
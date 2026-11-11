add_executable(skeleton_app main.cpp)
target_link_libraries(skeleton_app PRIVATE ews_core)
target_compile_options(skeleton_app PRIVATE -Wall -Wextra -Wpedantic)

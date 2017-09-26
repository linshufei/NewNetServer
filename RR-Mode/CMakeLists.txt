cmake_minimum_required(VERSION 3.8)
project(RR-Mode)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES config_file.h defines.h global_settings.h
        init_configure.h init_configure.cc
        master_thread.h master_thread.cc
        RRMode.cpp socket_wrapper.h threadSafe_container.h
        utils.h worker_threads.h worker_threads.cc)
add_executable(RR-Mode ${SOURCE_FILES})
target_link_libraries(RR-Mode pthread log4cxx boost_system boost_filesystem event pthread)
set_target_properties(RR-Mode PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../)
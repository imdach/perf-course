cmake_minimum_required(VERSION 3.19)
project(perf_course)

set(CMAKE_CXX_STANDARD 11)

add_executable(perf_course cpu/practice/cpu_01.c cpu/practice/d_state/my_char_dev.c cpu/practice/cpu_04.c cpu/practice/cpu_05.c cpu/practice/cpu_06.c cpu/practice/cpu_orphan.c cpu/practice/cpu_zombie.c cpu/day01/clone_test.c cpu/day01/fork_test.c cpu/day01/fork_cow_test.c tmp/tmp.c cpu/day01/pid_alloc_test.c cpu/day01/fd_test.c cpu/day02/busy.c cpu/dayxx/cache_line_test.c cpu/day03/busy.c cpu/day03/docker_main.c toy_docker/namespace_test.cpp toy_docker/step_chroot/toy_docker.c toy_docker/step_namespace/toy_docker.c
        toy_docker/step_cgroup/toy_docker.c toy_docker/step_network/toy_docker.c
        toy_docker/step_network/lxc_network/network.c
        toy_docker/step_network/lxc_network/network.h
        toy_docker/step_network/lxc_network/nl.c
        toy_docker/step_network/lxc_network/nl.h
        )
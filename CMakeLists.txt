cmake_minimum_required(VERSION 3.6)
project(Delegator)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        delegator/delegator.hpp
        catch/catch.hpp
        catch/catch_main.cpp tests/complex_test.cpp)

add_executable(utests ${SOURCE_FILES})
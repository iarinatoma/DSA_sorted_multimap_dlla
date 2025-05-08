cmake_minimum_required(VERSION 3.30)
project(SortedMultiMap)

set(CMAKE_CXX_STANDARD 20)

add_executable(SortedMultiMap SortedMultiMap.cpp SMMIterator.cpp App.cpp ShortTest.cpp ExtendedTest.cpp)

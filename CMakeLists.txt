cmake_minimum_required(VERSION 3.9)
project(Regex C)

set(CMAKE_C_STANDARD 11)

add_executable(Regex main.c statemach.h statemach.c regex.h regex.c pattern.h pattern.c token.h token.c quantifier.h quantifier.c util/stack.h util/stack.c util/list.h util/list.c test/quantifier_test.c test/regex_test.c test/tests.h test/tests.c test/myassert.h)
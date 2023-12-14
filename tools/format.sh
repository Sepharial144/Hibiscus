#!/bin/sh

find source/ -iname *.hpp -o -iname *.cpp | xargs clang-format -style=file -i --verbose
find tests/ -iname *.hpp -o -iname *.cpp | xargs clang-format -style=file -i --verbose 
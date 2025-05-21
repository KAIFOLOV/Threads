# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\lab1_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\lab1_autogen.dir\\ParseCache.txt"
  "CMakeFiles\\lab2_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\lab2_autogen.dir\\ParseCache.txt"
  "CMakeFiles\\lab3_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\lab3_autogen.dir\\ParseCache.txt"
  "CMakeFiles\\lab4_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\lab4_autogen.dir\\ParseCache.txt"
  "CMakeFiles\\lab5_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\lab5_autogen.dir\\ParseCache.txt"
  "CMakeFiles\\lab6_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\lab6_autogen.dir\\ParseCache.txt"
  "lab1_autogen"
  "lab2_autogen"
  "lab3_autogen"
  "lab4_autogen"
  "lab5_autogen"
  "lab6_autogen"
  )
endif()

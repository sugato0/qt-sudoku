# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "RelWithDebInfo")
  file(REMOVE_RECURSE
  "CMakeFiles\\sudoku1_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\sudoku1_autogen.dir\\ParseCache.txt"
  "sudoku1_autogen"
  )
endif()

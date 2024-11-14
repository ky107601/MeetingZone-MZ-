# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/mz_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/mz_autogen.dir/ParseCache.txt"
  "mz_autogen"
  )
endif()

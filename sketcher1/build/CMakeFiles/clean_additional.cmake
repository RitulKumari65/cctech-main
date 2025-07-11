# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\ProjectQT_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\ProjectQT_autogen.dir\\ParseCache.txt"
  "ProjectQT_autogen"
  )
endif()

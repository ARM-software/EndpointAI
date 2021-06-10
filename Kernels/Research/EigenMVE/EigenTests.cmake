# Taken from the Eigen project so that we use the same cmake logic to build the tests.

add_custom_target(buildtests)

macro(ei_add_property prop value)
  get_property(previous GLOBAL PROPERTY ${prop})
  if ((NOT previous) OR (previous STREQUAL ""))
    set_property(GLOBAL PROPERTY ${prop} "${value}")
  else()
    set_property(GLOBAL PROPERTY ${prop} "${previous} ${value}")
  endif()
endmacro()

macro(ei_add_target_property target prop value)

  get_target_property(previous ${target} ${prop})
  # if the property wasn't previously set, ${previous} is now "previous-NOTFOUND" which cmake allows catching with plain if()
  if(NOT previous)
    set(previous "")
  endif()
  set_target_properties(${target} PROPERTIES ${prop} "${previous} ${value}")
endmacro()

#internal. See documentation of ei_add_test for details.
macro(ei_add_test_internal testname testname_with_suffix)
  set(targetname ${testname_with_suffix})

  if(EIGEN_ADD_TEST_FILENAME_EXTENSION)
    set(filename ${EIGEN}/test/${testname}.${EIGEN_ADD_TEST_FILENAME_EXTENSION})
  else()
    set(filename ${EIGEN}/test/${testname}.cpp)
  endif()

  # Add the current target to the list of subtest targets
  get_property(EIGEN_SUBTESTS_LIST GLOBAL PROPERTY EIGEN_SUBTESTS_LIST)
  set(EIGEN_SUBTESTS_LIST "${EIGEN_SUBTESTS_LIST}${targetname}\n")
  set_property(GLOBAL PROPERTY EIGEN_SUBTESTS_LIST "${EIGEN_SUBTESTS_LIST}")

  if(EIGEN_ADD_TEST_FILENAME_EXTENSION STREQUAL cu)
    if(EIGEN_TEST_HIP)
      hip_reset_flags()
      hip_add_executable(${targetname} ${filename} HIPCC_OPTIONS "-DEIGEN_USE_HIP ${ARGV2}")
    elseif(EIGEN_TEST_CUDA_CLANG)
      set_source_files_properties(${filename} PROPERTIES LANGUAGE CXX)
      
      if(CUDA_64_BIT_DEVICE_CODE AND (EXISTS "${CUDA_TOOLKIT_ROOT_DIR}/lib64"))
        link_directories("${CUDA_TOOLKIT_ROOT_DIR}/lib64")
      else()
        link_directories("${CUDA_TOOLKIT_ROOT_DIR}/lib")
      endif()

      if (${ARGC} GREATER 2)
        add_executable(${targetname} ${filename})
      else()
        add_executable(${targetname} ${filename} OPTIONS ${ARGV2})
      endif()
      set(CUDA_CLANG_LINK_LIBRARIES "cudart_static" "cuda" "dl" "pthread")
      if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
      set(CUDA_CLANG_LINK_LIBRARIES ${CUDA_CLANG_LINK_LIBRARIES} "rt")
      endif()
      target_link_libraries(${targetname} ${CUDA_CLANG_LINK_LIBRARIES})

    else()
      if (${ARGC} GREATER 2)
        cuda_add_executable(${targetname} ${filename} OPTIONS ${ARGV2})
      else()
        cuda_add_executable(${targetname} ${filename})
      endif()
    endif()
  else()
    add_executable(${targetname} ${filename})
  endif()

  if (targetname MATCHES "^eigen2_")
    add_dependencies(eigen2_buildtests ${targetname})
  else()
    add_dependencies(buildtests ${targetname})
  endif()

  if(EIGEN_NO_ASSERTION_CHECKING)
    ei_add_target_property(${targetname} COMPILE_FLAGS "-DEIGEN_NO_ASSERTION_CHECKING=1")
  else()
    if(EIGEN_DEBUG_ASSERTS)
      ei_add_target_property(${targetname} COMPILE_FLAGS "-DEIGEN_DEBUG_ASSERTS=1")
    endif()
  endif()

  ei_add_target_property(${targetname} COMPILE_FLAGS "-DEIGEN_TEST_MAX_SIZE=${EIGEN_TEST_MAX_SIZE}")

  if(MSVC)
    ei_add_target_property(${targetname} COMPILE_FLAGS "/bigobj")
  endif()

  # let the user pass flags.
  if(${ARGC} GREATER 2)
    ei_add_target_property(${targetname} COMPILE_FLAGS "${ARGV2}")
  endif()

  if(EIGEN_TEST_CUSTOM_CXX_FLAGS)
    ei_add_target_property(${targetname} COMPILE_FLAGS "${EIGEN_TEST_CUSTOM_CXX_FLAGS}")
  endif()

  if(EIGEN_STANDARD_LIBRARIES_TO_LINK_TO)
    target_link_libraries(${targetname} ${EIGEN_STANDARD_LIBRARIES_TO_LINK_TO})
  endif()
  if(EXTERNAL_LIBS)
    target_link_libraries(${targetname} ${EXTERNAL_LIBS})
  endif()
  if(EIGEN_TEST_CUSTOM_LINKER_FLAGS)
    target_link_libraries(${targetname} ${EIGEN_TEST_CUSTOM_LINKER_FLAGS})
  endif()

  target_include_directories(${targetname} PUBLIC ${EIGEN})
  configApp(${targetname} ${ROOT})

  if (EXTERNALBUFFER)
  target_compile_definitions(${targetname} PRIVATE EIGEN_HELIUM_BUFFER_EXTERNAL) 
  target_sources(${targetname} PRIVATE buffers.cpp)
  endif()

  if(${ARGC} GREATER 3)
    set(libs_to_link ${ARGV3})
    # it could be that some cmake module provides a bad library string " "  (just spaces),
    # and that severely breaks target_link_libraries ("can't link to -l-lstdc++" errors).
    # so we check for strings containing only spaces.
    string(STRIP "${libs_to_link}" libs_to_link_stripped)
    string(LENGTH "${libs_to_link_stripped}" libs_to_link_stripped_length)
    if(${libs_to_link_stripped_length} GREATER 0)
      # notice: no double quotes around ${libs_to_link} here. It may be a list.
      target_link_libraries(${targetname} ${libs_to_link})
    endif()
  endif()

  add_test(${testname_with_suffix} "${targetname}")

  # Specify target and test labels according to EIGEN_CURRENT_SUBPROJECT
  get_property(current_subproject GLOBAL PROPERTY EIGEN_CURRENT_SUBPROJECT)
  if ((current_subproject) AND (NOT (current_subproject STREQUAL "")))
    set_property(TARGET ${targetname} PROPERTY LABELS "Build${current_subproject}")
    add_dependencies("Build${current_subproject}" ${targetname})
    set_property(TEST ${testname_with_suffix} PROPERTY LABELS "${current_subproject}")
  endif()
  if(EIGEN_SYCL)
    # Force include of the SYCL file at the end to avoid errors.
    set_property(TARGET ${targetname} PROPERTY COMPUTECPP_INCLUDE_AFTER 1)
    # Set COMPILE_FLAGS to COMPILE_DEFINITIONS instead to avoid having to duplicate the flags
    # to the device compiler.
    get_target_property(target_compile_flags ${targetname} COMPILE_FLAGS)
    separate_arguments(target_compile_flags)
    foreach(flag ${target_compile_flags})
      if(${flag} MATCHES "^-D.*")
        string(REPLACE "-D" "" definition_flag ${flag})
        set_property(TARGET ${targetname} APPEND PROPERTY COMPILE_DEFINITIONS ${definition_flag})
        list(REMOVE_ITEM target_compile_flags ${flag})
      endif()
    endforeach()
    set_property(TARGET ${targetname} PROPERTY COMPILE_FLAGS ${target_compile_flags})
    # Link against pthread and add sycl to target
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    target_link_libraries(${targetname} Threads::Threads)
    add_sycl_to_target(TARGET ${targetname} SOURCES ${filename})
  endif(EIGEN_SYCL)
endmacro(ei_add_test_internal)

macro(ei_add_test testname)
  get_property(EIGEN_TESTS_LIST GLOBAL PROPERTY EIGEN_TESTS_LIST)
  set(EIGEN_TESTS_LIST "${EIGEN_TESTS_LIST}${testname}\n")
  set_property(GLOBAL PROPERTY EIGEN_TESTS_LIST "${EIGEN_TESTS_LIST}")

  if(EIGEN_ADD_TEST_FILENAME_EXTENSION)
    set(filename ${EIGEN}/test/${testname}.${EIGEN_ADD_TEST_FILENAME_EXTENSION})
  else()
    set(filename ${EIGEN}/test/${testname}.cpp)
  endif()

  file(READ "${filename}" test_source)
  string(REGEX MATCHALL "CALL_SUBTEST_[0-9]+|EIGEN_TEST_PART_[0-9]+|EIGEN_SUFFIXES(;[0-9]+)+"
         occurrences "${test_source}")
  string(REGEX REPLACE "CALL_SUBTEST_|EIGEN_TEST_PART_|EIGEN_SUFFIXES" "" suffixes "${occurrences}")
  list(REMOVE_DUPLICATES suffixes)
  set(explicit_suffixes "")
  if( (NOT EIGEN_SPLIT_LARGE_TESTS) AND suffixes)
    # Check whether we have EIGEN_TEST_PART_* statements, in which case we likely must enforce splitting.
    # For instance, indexed_view activate a different c++ version for each part.
    string(REGEX MATCHALL "EIGEN_TEST_PART_[0-9]+" occurrences "${test_source}")
    string(REGEX REPLACE "EIGEN_TEST_PART_" "" explicit_suffixes "${occurrences}")
    list(REMOVE_DUPLICATES explicit_suffixes)
  endif()
  if( (EIGEN_SPLIT_LARGE_TESTS AND suffixes) OR explicit_suffixes)
    add_custom_target(${testname})
    foreach(suffix ${suffixes})
      ei_add_test_internal(${testname} ${testname}_${suffix}
        "${ARGV1} -DEIGEN_TEST_PART_${suffix}=1" "${ARGV2}")
      add_dependencies(${testname} ${testname}_${suffix})
    endforeach()
  else()
    ei_add_test_internal(${testname} ${testname} "${ARGV1} -DEIGEN_TEST_PART_ALL=1" "${ARGV2}")
  endif()
endmacro()

# adds a failtest, i.e. a test that succeed if the program fails to compile
# note that the test runner for these is CMake itself, when passed -DEIGEN_FAILTEST=ON
# so here we're just running CMake commands immediately, we're not adding any targets.
macro(ei_add_failtest testname)

  set(test_target_ok ${testname}_ok)
  set(test_target_ko ${testname}_ko)

  # Add executables
  add_executable(${test_target_ok} ${testname}.cpp)
  add_executable(${test_target_ko} ${testname}.cpp)

  # Remove them from the normal build process
  set_target_properties(${test_target_ok} ${test_target_ko} PROPERTIES
                        EXCLUDE_FROM_ALL TRUE
                        EXCLUDE_FROM_DEFAULT_BUILD TRUE)

  # Configure the failing test
  target_compile_definitions(${test_target_ko} PRIVATE EIGEN_SHOULD_FAIL_TO_BUILD)

  # Add the tests to ctest.
  add_test(NAME ${test_target_ok}
          COMMAND ${CMAKE_COMMAND} --build . --target ${test_target_ok} --config $<CONFIGURATION>
          WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
  add_test(NAME ${test_target_ko}
          COMMAND ${CMAKE_COMMAND} --build . --target ${test_target_ko} --config $<CONFIGURATION>
          WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

  # Expect the second test to fail
  set_tests_properties(${test_target_ko} PROPERTIES WILL_FAIL TRUE)
endmacro()
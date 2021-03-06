# - Find numpy
# Find the native numpy includes
# This module defines
#  PYTHON_NUMPY_INCLUDE_DIR, where to find numpy/arrayobject.h, etc.
#  PYTHON_NUMPY_FOUND, If false, do not try to use numpy headers.

if (PYTHON_NUMPY_INCLUDE_DIR)
    # in cache already
    set (PYTHON_NUMPY_FIND_QUIETLY TRUE)
endif()

find_package(PythonInterp)

if (PYTHON_EXECUTABLE)
    exec_program("${PYTHON_EXECUTABLE}"
                 ARGS "-c \"import numpy; print numpy.get_include()\""
                 OUTPUT_VARIABLE PYTHON_NUMPY_INCLUDE_DIR
                 RETURN_VALUE PYTHON_NUMPY_NOT_FOUND)

    if (PYTHON_NUMPY_INCLUDE_DIR)
        set(PYTHON_NUMPY_FOUND TRUE)
        set(PYTHON_NUMPY_INCLUDE_DIR ${PYTHON_NUMPY_INCLUDE_DIR} CACHE STRING "Numpy include path")
    else (PYTHON_NUMPY_INCLUDE_DIR)
        set(PYTHON_NUMPY_FOUND FALSE)
    endif()
endif()

if (PYTHON_NUMPY_FOUND)
    if (NOT PYTHON_NUMPY_FIND_QUIETLY)
        message(STATUS "Numpy headers found")
    endif()
else()
    if (PYTHON_NUMPY_FIND_REQUIRED)
        message(FATAL_ERROR "Numpy headers missing")
    endif()
endif()

mark_as_advanced(PYTHON_NUMPY_INCLUDE_DIR)

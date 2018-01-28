if (CMAKE_CXX_COMPILER_ID MATCHES "Clang") 
    option(WITH_ASAN "Using clang address santisizer" OFF)
    option(WITH_TSAN "Using clang thread santisizer" OFF)
    option(WITH_MSAN "Using clang memory santisizer" OFF)
    option(WITH_UBSAN "Using clang undefined-behaviour santisizer" OFF)

    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O1 -g")

    if (WITH_ASAN) 
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
    endif()

    if (WITH_TSAN) 
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread")
    endif()

    if (WITH_MSAN) 
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=memory")
    endif()

    if (WITH_UBSAN) 
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")
    endif()
endif()
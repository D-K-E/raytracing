cmake_minimum_required (VERSION 3.0.2)
project("Isin Izleme - Raytracing")

#find_package(OpenGL REQUIRED)

set (CMAKE_CXX_FLAGS "-std=c++17")
set (FLAGS "-ldl -ggdb -Wall -Wextra")

include_directories(
    # my headers
    "./include/"
    )

# ---------- Ortak -------------------

# ---------- Haftasonu ---------------
# ---------- Sonraki -----------------
# ---------- Nihai -------------------

#set( GLFW_SHARED_LIB
#    "${PROJECT_SOURCE_DIR}/libs/glfw/src/libglfw.so")


set ( ALL_LIBS
    #${OpenGL}
    #${GLFW_SHARED_LIB}
    #${ASSIMP_SHARED_LIB}
    "-ldl"
    )

# ---------- Ortak -------------------

# ---------- Haftasonu ---------------
add_executable(ppmresim.out "src/haftasonu/ppm.cpp")
target_link_libraries(ppmresim.out ${ALL_LIBS})

install(TARGETS ppmresim.out DESTINATION "${PROJECT_SOURCE_DIR}/bin/haftasonu/")
# ------------------------------------
add_executable(ppmgostergeli.out "src/haftasonu/ppmgostergeli.cpp")
target_link_libraries(ppmgostergeli.out ${ALL_LIBS})

install(TARGETS ppmgostergeli.out DESTINATION "${PROJECT_SOURCE_DIR}/bin/haftasonu/")

# -----------------------------------
add_executable(ppmsonhal.out "src/haftasonu/ppmsonhal.cpp")
target_link_libraries(ppmsonhal.out ${ALL_LIBS})

install(TARGETS ppmsonhal.out DESTINATION "${PROJECT_SOURCE_DIR}/bin/haftasonu/")
# ------------------------------------
add_executable(isin.out "src/haftasonu/isin.cpp")
target_link_libraries(isin.out ${ALL_LIBS})

install(TARGETS isin.out DESTINATION "${PROJECT_SOURCE_DIR}/bin/haftasonu/")


# ---------- Sonraki -----------------
# ---------- Nihai -------------------

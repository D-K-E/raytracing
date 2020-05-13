cmake_minimum_required (VERSION 3.0.2)
project("Isin Izleme - Raytracing")

#find_package(OpenGL REQUIRED)
find_package (Threads REQUIRED)

set (CMAKE_CXX_FLAGS "-std=c++17")
set (FLAGS "-ldl -ggdb -Wall -Wextra -pthread")

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
    ${Threads}
    ${FLAGS}
    )

# ---------- Ortak -------------------

# ---------- Haftasonu ---------------
#add_executable(ppmresim.out "src/haftasonu/ppm.cpp")
#target_link_libraries(ppmresim.out ${ALL_LIBS})

#install(TARGETS ppmresim.out DESTINATION "${PROJECT_SOURCE_DIR}/bin/haftasonu/")
# ------------------------------------
#add_executable(ppmgostergeli.out "src/haftasonu/ppmgostergeli.cpp")
#target_link_libraries(ppmgostergeli.out ${ALL_LIBS})

#install(TARGETS ppmgostergeli.out DESTINATION "${PROJECT_SOURCE_DIR}/bin/haftasonu/")

# -----------------------------------
#add_executable(ppmsonhal.out "src/haftasonu/ppmsonhal.cpp")
#target_link_libraries(ppmsonhal.out ${ALL_LIBS})
#install(TARGETS ppmsonhal.out DESTINATION "${PROJECT_SOURCE_DIR}/bin/haftasonu/")
# ------------------------------------
#add_executable(isin.out "src/haftasonu/isin.cpp")
#target_link_libraries(isin.out ${ALL_LIBS})

#install(TARGETS isin.out DESTINATION "${PROJECT_SOURCE_DIR}/bin/haftasonu/")
# ------------------------------------
#add_executable(cokyuzey.out "src/haftasonu/cokyuzey.cpp")
#target_link_libraries(cokyuzey.out ${ALL_LIBS})
#install(TARGETS cokyuzey.out DESTINATION "${PROJECT_SOURCE_DIR}/bin/haftasonu/")
# ------------------------------------
#add_executable(antialiasing.out "src/haftasonu/antialiasing.cpp")
#target_link_libraries(antialiasing.out ${ALL_LIBS})
#install(TARGETS antialiasing.out DESTINATION "${PROJECT_SOURCE_DIR}/bin/haftasonu/")
# ------------------------------------
#add_executable(diffuse.out "src/haftasonu/diffuse.cpp")
#target_link_libraries(diffuse.out ${ALL_LIBS})
#install(TARGETS diffuse.out DESTINATION "${PROJECT_SOURCE_DIR}/bin/haftasonu/")
# ------------------------------------
add_executable(metal.out "src/haftasonu/metal.cpp")
target_link_libraries(metal.out ${ALL_LIBS})
install(TARGETS metal.out DESTINATION "${PROJECT_SOURCE_DIR}/bin/haftasonu/")
# ------------------------------------

add_executable(bookcover.out "src/haftasonu/bookcover.cpp")
target_link_libraries(bookcover.out ${ALL_LIBS})
install(TARGETS bookcover.out DESTINATION "${PROJECT_SOURCE_DIR}/bin/haftasonu/")

# ---------- Sonraki -----------------

add_executable(motionblur.out "src/sonraki/motionblur.cpp")
target_link_libraries(motionblur.out ${ALL_LIBS})
install(TARGETS motionblur.out DESTINATION "${PROJECT_SOURCE_DIR}/bin/sonraki/")

# ---------- Nihai -------------------

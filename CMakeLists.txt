cmake_minimum_required (VERSION 3.0.2)
project("Isin Izleme - Raytracing")

#find_package(OpenGL REQUIRED)
find_package (Threads REQUIRED)

set (CMAKE_CXX_FLAGS "-std=c++17")
set (CMAKE_BUILD_TYPE "Debug")
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

#add_executable(motionblur.out "src/sonraki/motionblur.cpp")
#target_link_libraries(motionblur.out ${ALL_LIBS})
#install(TARGETS motionblur.out DESTINATION "${PROJECT_SOURCE_DIR}/bin/sonraki/")


#add_executable(texture.out "src/sonraki/texture.cpp")
#target_link_libraries(texture.out ${ALL_LIBS})
#install(TARGETS texture.out DESTINATION "${PROJECT_SOURCE_DIR}/bin/sonraki/")

#add_executable(twotexture.out "src/sonraki/twotexture.cpp")
#target_link_libraries(twotexture.out ${ALL_LIBS})
#install(TARGETS twotexture.out DESTINATION "${PROJECT_SOURCE_DIR}/bin/sonraki/")

#add_executable(twoperlin.out "src/sonraki/twoperlin.cpp")
#target_link_libraries(twoperlin.out ${ALL_LIBS})
#install(TARGETS twoperlin.out DESTINATION "${PROJECT_SOURCE_DIR}/bin/sonraki/")

#add_executable(imtexture.out "src/sonraki/imtexture.cpp")
#target_link_libraries(imtexture.out ${ALL_LIBS})
#install(TARGETS imtexture.out DESTINATION "${PROJECT_SOURCE_DIR}/bin/sonraki/")

#add_executable(imlighttexture.out "src/sonraki/imlighttexture.cpp")
#target_link_libraries(imlighttexture.out ${ALL_LIBS})
#install(TARGETS imlighttexture.out DESTINATION "${PROJECT_SOURCE_DIR}/bin/sonraki/")

#add_executable(cornell.out "src/sonraki/cornell.cpp")
#target_link_libraries(cornell.out ${ALL_LIBS})
#install(TARGETS cornell.out DESTINATION "${PROJECT_SOURCE_DIR}/bin/sonraki/")

#add_executable(fcornell.out "src/sonraki/flipedcornell.cpp")
#target_link_libraries(fcornell.out ${ALL_LIBS})
#install(TARGETS fcornell.out DESTINATION "${PROJECT_SOURCE_DIR}/bin/sonraki/")

add_executable(cornellbox.out "src/sonraki/cornellbox.cpp")
target_link_libraries(cornellbox.out ${ALL_LIBS})
install(TARGETS cornellbox.out DESTINATION "${PROJECT_SOURCE_DIR}/bin/sonraki/")




# ---------- Nihai -------------------

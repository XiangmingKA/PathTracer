cmake_minimum_required(VERSION 3.14)
project(Path_Tracer_2020)

set(CMAKE_CXX_STANDARD 14)

file(GLOB SOURCES
        boundingbox.h						//*号对应具体的文件名
        boundingbox.cpp
        cobject.cpp
        cobject.h
        cray.cpp
        cray.h
        csphere.cpp
        csphere.h
        ctriangle.cpp
        ctriangle.h
        display.h
        global.h
        gvector3.cpp
        gvector3.h
        lights.cpp
        lights.h
        material.cpp
        material.h
        myScene.cpp
        myScene.h
        perspectiveCamera.cpp
        perspectiveCamera.h
        render.cpp
        render.h
        scene_io.cpp
        scene_io.h

        )


add_executable(ChainList raytracer.cpp ${SOURCE_FILES} ${SOURCES})
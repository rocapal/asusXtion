
cmake_minimum_required(VERSION 2.8)

SET( SOURCE_FILES main.cpp)

project(XtionImage)
add_executable (XtionImage  ${SOURCE_FILES})

#automated opencv
include(FindPkgConfig)
FIND_PATH( OpenCV_INCLUDE_DIR NAMES cv.h  PATHS ENV C++LIB ENV PATH PATH_SUFFIXES include/opencv/)
IF( OpenCV_INCLUDE_DIR )
	#MESSAGE("OpenCV was found in: ${OpenCV_INCLUDE_DIR}")
	MESSAGE("*** OpenCV ok")
    FIND_LIBRARY( OpenCV_LIBRARY1 NAMES opencv_core PATHS ENV C++LIB ENV PATH PATH_SUFFIXES lib lib64)
    FIND_LIBRARY( OpenCV_LIBRARY2 NAMES opencv_highgui PATHS ENV C++LIB ENV PATH PATH_SUFFIXES lib lib64)
    FIND_LIBRARY( OpenCV_LIBRARY3 NAMES opencv_contrib PATHS ENV C++LIB ENV PATH PATH_SUFFIXES lib lib64)
    FIND_LIBRARY( OpenCV_LIBRARY4 NAMES opencv_calib3d PATHS ENV C++LIB ENV PATH PATH_SUFFIXES lib lib64)
    FIND_LIBRARY( OpenCV_LIBRARY5 NAMES opencv_flann PATHS ENV C++LIB ENV PATH PATH_SUFFIXES lib lib64)
    FIND_LIBRARY( OpenCV_LIBRARY6 NAMES opencv_gpu PATHS ENV C++LIB ENV PATH PATH_SUFFIXES lib lib64)
    FIND_LIBRARY( OpenCV_LIBRARY7 NAMES opencv_imgproc PATHS ENV C++LIB ENV PATH PATH_SUFFIXES lib lib64)
    FIND_LIBRARY( OpenCV_LIBRARY8 NAMES opencv_legacy PATHS ENV C++LIB ENV PATH PATH_SUFFIXES lib lib64)
    FIND_LIBRARY( OpenCV_LIBRARY9 NAMES opencv_ml PATHS ENV C++LIB ENV PATH PATH_SUFFIXES lib lib64)
    FIND_LIBRARY( OpenCV_LIBRARY11 NAMES opencv_objdetect PATHS ENV C++LIB ENV PATH PATH_SUFFIXES lib lib64)
    FIND_LIBRARY( OpenCV_LIBRARY14 NAMES opencv_video PATHS ENV C++LIB ENV PATH PATH_SUFFIXES lib lib64)
    SET (OpenCV_LIBRARIES ${OpenCV_LIBRARY1} ${OpenCV_LIBRARY2} ${OpenCV_LIBRARY3} ${OpenCV_LIBRARY4} ${OpenCV_LIBRARY5} ${OpenCV_LIBRARY6} ${OpenCV_LIBRARY7} ${OpenCV_LIBRARY8} ${OpenCV_LIBRARY9} ${OpenCV_LIBRARY11} ${OpenCV_LIBRARY14} )
    IF( OpenCV_LIBRARIES )
		#MESSAGE("OPENCV LIBRARY FOUND IN ${OpenCV_LIBRARIES}")
    ENDIF( OpenCV_LIBRARIES )
ENDIF(OpenCV_INCLUDE_DIR)

IF (NOT  OpenCV_INCLUDE_DIR)
	MESSAGE("*** OpenCV not found")
ENDIF()

link_directories(${OpenCV_LIBRARIES})
include_directories(${OpenCV_INCLUDE_DIR})

#manualalut
FIND_PATH( alut_INCLUDE_DIR NAMES alut.h  PATHS ENV C++LIB ENV PATH PATH_SUFFIXES AL)

IF( alut_INCLUDE_DIR )
    FIND_LIBRARY( alut_LIBRARIES NAMES alut PATHS ENV C++LIB ENV PATH PATH_SUFFIXES lib lib64 )
    IF( alut_LIBRARIES )
		MESSAGE ("-- ALUT found at ${alut_LIBRARIES}")
		include_directories(${alut_INCLUDE_DIR})
		link_directories(${alut_LIBRARIES})
    ENDIF( alut_LIBRARIES )
ENDIF(alut_INCLUDE_DIR)

IF(NOT alut_LIBRARIES)
		MESSAGE ("*** ALUT not found")
ENDIF()

#manual openni libraries
FIND_PATH( OPENNI2_INC NAMES OpenNI.h  PATHS ENV C++LIB ENV PATH PATH_SUFFIXES openni2)
IF( OPENNI2_INC )
	message(STATUS "OpenNI2 found at ${OPENNI2_LIB}")
	FIND_LIBRARY( OPENNI2_LIB NAMES OpenNI2 PATHS ENV C++LIB ENV PATH PATH_SUFFIXES lib/openni2 lib64/openni2)
	FIND_PATH( OPENNI2_LIB_PATH NAMES libOpenNI2.so PATHS ENV C++LIB ENV PATH PATH_SUFFIXES lib/openni2 lib64/openni2)
	FILE(GLOB openniFiles ${OPENNI2_LIB_PATH}/libDummyDevice.so  ${OPENNI2_LIB_PATH}/libOniFile.so  ${OPENNI2_LIB_PATH}/libPS1080.so  ${OPENNI2_LIB_PATH}/libPSLink.so)
	if (NOT IS_DIRECTORY ./OpenNI2)
		file (COPY ${OPENNI2_LIB} DESTINATION .)
		file (COPY ${openniFiles} DESTINATION OpenNI2/Drivers)
		message("-------------CMake will install local OpenNI2 lib and Driver")
	endif()
	include_directories(${OPENNI2_INC})
ELSE()
	message("OpenNI2 library not found")
ENDIF()


set( CMAKE_CXX_FLAGS "-Wno-deprecated" ) # Opciones para el compilador


TARGET_LINK_LIBRARIES(XtionImage
	${OpenCV_LIBRARIES}
	${GLUT_LIBRARIES}
	${OPENNI2_LIB}
)


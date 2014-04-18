if( WIN32 )
	find_path( KINECTSDK_INCLUDE_DIR
		NAMES NuiApi.h NuiImageCamera.h NuiSensor.h NuiSkeleton.h
		PATHS $ENV{KINECTSDK10_DIR}inc
	)
	find_library( KINECTSDK_LIBRARY
		NAMES Kinect10
		PATHS $ENV{KINECTSDK10_DIR}lib/x86
	)
endif()

include( FindPackageHandleStandardArgs )
FIND_PACKAGE_HANDLE_STANDARD_ARGS( KINECTSDK REQUIRED_VARS KINECTSDK_LIBRARY KINECTSDK_INCLUDE_DIR )

if( KINECTSDK_FOUND )
	set( KINECTSDK_LIBRARIES ${KINECTSDK_LIBRARY} )
	set( KINECTSDK_INCLUDE_DIRS ${KINECTSDK_INCLUDE_DIR} )
endif()

mark_as_advanced( KINECTSDK_INCLUDE_DIR KINECTSDK_LIBRARY )

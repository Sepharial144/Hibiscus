message(STATUS "Load ... package.cmake")

find_package(fmt CONFIG REQUIRED)

# link libraries
SET(PACKAGE_LIBRARIES
	fmt::fmt-header-only
)
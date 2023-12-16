message(STATUS "Load ... package.cmake")

find_package(fmt CONFIG REQUIRED)


message(STATUS "Fmt: ${fmt}")

# link libraries
SET(PACKAGE_LIBRARIES
	fmt::fmt
)
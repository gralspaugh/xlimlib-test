# add the library that runs
add_library(XlimlibUtils XlimlibUtils.cpp)

# state that anybody linking to us needs to include the current source dir
# to find XlimlibUtils.h, while we don't.
target_include_directories(XlimlibUtils
    INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

# Include GNU Scientific Library
include(FindGSL REQUIRED)
if(GSL_FOUND)
    target_link_libraries(XlimlibUtils PRIVATE gsl)
    # TODO: Different BLAS?
    target_link_libraries(XlimlibUtils PRIVATE gslcblas)
endif()

target_link_libraries(XlimlibUtils PUBLIC compiler_flags)

# define the symbol stating we are using the declspec(dllexport) when
# building on windows
target_compile_definitions(XlimlibUtils PRIVATE "EXPORTING_XLIMLIB_UTILS")

# setup the version numbering
set_property(TARGET XlimlibUtils PROPERTY VERSION "0.1.0")
set_property(TARGET XlimlibUtils PROPERTY SOVERSION "1")

# install rules
set(installable_libs XlimlibUtils compiler_flags)
install(TARGETS ${installable_libs}
        EXPORT XlimlibUtilsTargets
        DESTINATION lib)
install(FILES XlimlibUtils.h DESTINATION include)
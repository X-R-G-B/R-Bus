include(CMakeFindDependencyMacro)
find_dependency(libsbml-static CONFIG)
if(NOT TARGET libsbml)
    add_library(libsbml INTERFACE IMPORTED)
    target_link_libraries(libsbml INTERFACE libsbml-static)
endif()

file(GLOB SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cxx
)

set(MIDASIO_SOURCES
    ${SOURCE_FILES}
    PARENT_SCOPE
)

# add_library(midasio SHARED
#     ${MIDASIO_SOURCES}
# )

# target_include_directories(midasio
#     PUBLIC
#         ${PROJECT_SOURCE_DIR}/midasio
# )

# target_link_libraries(midasio
#     PRIVATE
#         ${ZLIB_LIBRARIES}
# )
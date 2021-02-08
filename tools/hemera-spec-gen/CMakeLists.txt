include_directories(${CMAKE_SOURCE_DIR}/plugins/qml/settings)

set(hemeraspecgen_SRCS
    main.cpp
    specgenerator.cpp
)

# final libraries
add_executable(hemera-spec-gen ${hemeraspecgen_SRCS})
target_link_libraries(hemera-spec-gen Qt5::Core Qt5::Network Qt5::DBus Qt5::Qml HemeraQt5SDK::Core HemeraQt5SDK::Qml HemeraQt5SDK::Generators)

# Install phase
install(TARGETS hemera-spec-gen
        RUNTIME DESTINATION "${HA_TOOLS_DIR}" COMPONENT bin
        LIBRARY DESTINATION "${HA_TOOLS_DIR}" COMPONENT shlib
        COMPONENT Tools)

install(FILES hemera_app.spec.in DESTINATION ${HA_SDK_DATA_DIR} COMPONENT data)

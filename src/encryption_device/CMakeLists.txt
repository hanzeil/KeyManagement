set(ENCRYPTION_DEVICE_SOURCE_FILES
        EncryptionDeviceFactoryInterface.h
        EncryptionDeviceFactoryInterface.cpp
        EncryptionDeviceProductInterface.h
        EncryptionDeviceProductInterface.cpp
        )

if (SJK_1238)
    add_subdirectory(sjk1238)
    set(ENCRYPTION_DEVICE_SOURCE_FILES
            ${ENCRYPTION_DEVICE_SOURCE_FILES}
            sjk1238/SJK1238Factory.h
            sjk1238/SJK1238Factory.cpp
            sjk1238/SJK1238.h
            sjk1238/SJK1238.cpp
            sjk1238/server_authen.cpp
            )
endif (SJK_1238)

if (SIMULATION)
    add_subdirectory(simulation)
    set(ENCRYPTION_DEVICE_SOURCE_FILES
            ${ENCRYPTION_DEVICE_SOURCE_FILES}
            simulation/SimulationFactory.cpp
            simulation/SimulationFactory.h
            simulation/Simulation.cpp
            simulation/Simulation.h)
endif (SIMULATION)

add_library(encryption_device STATIC ${ENCRYPTION_DEVICE_SOURCE_FILES})

# library
if (SJK_1238)
    find_library(SWSDS swsds)
    target_link_libraries(encryption_device
            ${SWSDS})
endif (SJK_1238)

if (SIMULATION)
    find_library(CRYPTO crypto)
    target_link_libraries(encryption_device
            ${CRYPTO})
endif (SIMULATION)

target_link_libraries(encryption_device
        global
        )

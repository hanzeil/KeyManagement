set(HARDWARE_SOURCE_FILES
        HardwareFactoryInterface.h
        HardwareFactoryInterface.cpp
        HardwareProductInterface.h
        HardwareProductInterface.cpp
        )

if (HARDWARE MATCHES SJK1238)
    add_subdirectory(sjk1238)
    set(HARDWARE_SOURCE_FILES ${HARDWARE_SOURCE_FILES}
            sjk1238/SJK1238Factory.h
            sjk1238/SJK1238Factory.cpp
            sjk1238/SJK1238.h
            sjk1238/SJK1238.cpp
            )
endif (HARDWARE MATCHES SJK1238)

if (HARDWARE MATCHES SIMULATION)
    add_subdirectory(simulation)
    set(HARDWARE_SOURCE_FILES ${HARDWARE_SOURCE_FILES}
            simulation/SimulationFactory.cpp
            simulation/SimulationFactory.h
            simulation/Simulation.cpp
            simulation/Simulation.h)
endif (HARDWARE MATCHES SIMULATION)

add_library(hardware STATIC ${HARDWARE_SOURCE_FILES})

# library
if (HARDWARE MATCHES SJK1238)
    find_library(SWSDS swsds)
    target_link_libraries(hardware
            ${SWSDS})
endif (HARDWARE MATCHES SJK1238)

if (HARDWARE MATCHES SIMULATION)
    find_library(CRYPTO crypto)
    target_link_libraries(hardware
            ${CRYPTO})
endif (HARDWARE MATCHES SIMULATION)


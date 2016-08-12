set(HARDWARE_LIB
        HardwareFactoryInterface.h
        HardwareFactoryInterface.cpp
        HardwareProductInterface.h
        HardwareProductInterface.cpp
        )

if (HARDWARE MATCHES SJK1238)
    add_subdirectory(SJK1238)
    set(HARDWARE_LIB ${HARDWARE_LIB}
            sjk1238/SJK1238Factory.h
            sjk1238/SJK1238Factory.cpp
            sjk1238/SJK1238.h
            sjk1238/SJK1238.cpp
            )
endif (HARDWARE MATCHES SJK1238)

if (HARDWARE MATCHES SIMULATION)
    add_subdirectory(simulation)
    set(HARDWARE_LIB ${HARDWARE_LIB}
            simulation/SimulationFactory.cpp
            simulation/SimulationFactory.h
            simulation/Simulation.cpp
            simulation/Simulation.h)
endif (HARDWARE MATCHES SIMULATION)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpthread -DBOOST_LOG_DYN_LINK")
add_library(hardware STATIC ${HARDWARE_LIB})

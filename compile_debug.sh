gcc -g -Iinclude -Isymphony -Itools\
    -D VRPDEBUG \
    src/mt19937ar.cpp \
    src/main.cpp \
    src/host_vrp.cpp src/mct_node.cpp src/mct_selector.cpp \
    src/solution.cpp src/vehicle.cpp \
    src/simulator.cpp \
    symphony/compute_cost.c symphony/vrp_io.c symphony/vrp_macros.c \
    -lstdc++

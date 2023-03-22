import ctypes
import os

################### Definition for C interfaces ################

cycle_module = ctypes.CDLL(os.path.join(os.path.split(__file__)[0], 
                                        'find_cycle.so'))

c_count_cycles = cycle_module.graph_count_cycles
c_count_cycles.argtypes = [ctypes.POINTER(ctypes.c_long),
                           ctypes.c_long,
                           ctypes.c_long,
                           ctypes.c_long, 
                           ctypes.POINTER(ctypes.c_long)]

c_count_paths = cycle_module.graph_count_paths
c_count_paths.argtypes = [ctypes.POINTER(ctypes.c_long),
                          ctypes.c_long,
                          ctypes.c_long,
                          ctypes.c_long, 
                          ctypes.POINTER(ctypes.c_long)]

c_count_substruct = cycle_module.graph_count_substruct
c_count_substruct.argtypes = [ctypes.POINTER(ctypes.c_long),
                              ctypes.c_long,
                              ctypes.c_long,
                              ctypes.POINTER(ctypes.c_long),
                              ctypes.c_char_p]

#################################################################
import torch
import numpy as np
import ctypes

################### Definition for C interfaces ################

cycle_module = ctypes.CDLL('./find_cycle.so')

c_count_cycles = cycle_module.graph_count_cycles
c_count_cycles.argtypes = [ctypes.POINTER(ctypes.c_long),
                           ctypes.c_long,
                           ctypes.c_long,
                           ctypes.c_long]
c_count_cycles.restype = ctypes.POINTER(ctypes.c_long)

c_free = cycle_module.free_array
c_free.argtypes = [ctypes.POINTER(ctypes.c_long)]

#################################################################

def count_cycles(edge_index: torch.LongTensor,
                 num_nodes: int, num_edges: int, k: int):
    c_array = c_count_cycles(np.ctypeslib.as_ctypes(edge_index.reshape(-1, 1).squeeze().numpy()), 
                             ctypes.c_long(num_nodes), 
                             ctypes.c_long(num_edges), 
                             ctypes.c_long(k))
    return torch.from_numpy(np.ctypeslib.as_array(c_array, (num_nodes,))), c_array

def free_array(c_array: ctypes.POINTER(ctypes.c_long)):
    c_free(c_array)

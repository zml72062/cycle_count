import torch
import numpy as np
import ctypes
from typing import Union

################### Definition for C interfaces ################

cycle_module = ctypes.CDLL('./find_cycle.so')

c_count_cycles = cycle_module.graph_count_cycles
c_count_cycles.argtypes = [ctypes.POINTER(ctypes.c_long),
                           ctypes.c_long,
                           ctypes.c_long,
                           ctypes.c_long, 
                           ctypes.POINTER(ctypes.c_long)]

#################################################################

def count_cycles(edge_index: Union[np.ndarray, torch.LongTensor],
                 num_nodes: int, num_edges: int, k: int):
    count = np.zeros((num_nodes, ), dtype=np.int64)
    c_count_cycles(np.ctypeslib.as_ctypes(
                        edge_index.reshape(-1, 1).squeeze().numpy()
                        if isinstance(edge_index, torch.Tensor) else
                        edge_index.reshape(-1, 1).squeeze()), 
                   ctypes.c_long(num_nodes), 
                   ctypes.c_long(num_edges), 
                   ctypes.c_long(k),
                   np.ctypeslib.as_ctypes(count))
    return count


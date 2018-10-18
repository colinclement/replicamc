"""
Tests to make sure things work
"""

import numpy as np
from itertools import repeat, product
from collections import Counter

import replicamc

rng = np.random.RandomState(14850)

def smallprobabilitytest():
    """ 
    for a 4x4 system exactly enumerate states and compare to rmc
    """
    L = 4
    J = rng.randn(2, L, L)
    tlist = np.array([.1, 1.])
    blist = 1./tlist
    #mc = replicamc.ReplicaMonteCarlo(L, J=J)

    allstates = product([1,-1], repeat=L*L)
    energies = list(map(hamiltonian, repeat(J), allstates))
    probs = np.array([np.exp(-b*np.array(energies)) for b in blist])
    probs /= probs.sum(1)[:,None]
    return probs



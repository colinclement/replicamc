"""
Tests to make sure things work
"""

import numpy as np
from itertools import repeat, product
import scipy.ndimage as ndi
import replicamc

rng = np.random.RandomState(14850)

def hamiltonian(J, s):
    s = np.array(list(s)).reshape(J.shape[1:])
    sh = s * np.roll(s, -1)
    sv = s * np.roll(s, -1, 0)
    return -np.sum(J[0]*sh + J[1]*sv)

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

def testcluster(p=0.3):
    img = 1*(np.random.rand(100,100)<p)
    img = ndi.binary_closing(img, border_value=1)
    lab, num = ndi.label(img)
    ind = np.arange(img.size+1)
    clusters = [set(ind[lab==i]) for i in range(num)]


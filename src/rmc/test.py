"""
Tests to make sure things work
"""

import numpy as np
from itertools import repeat, product
from collections import Counter
import matplotlib.pyplot as plt

import replicamc

rng = np.random.RandomState(14850)
L = 4
J = rng.randn(2, L, L)
blist=[1.5, 1.]

def allstates(L=4):
    return product([1,-1], repeat=L*L)

def exactprobs(J, blist):
    """ 
    for a 4x4 system exactly enumerate states and compare to rmc
    """
    L = J.shape[1]

    G = replicamc.SquareLattice(L, J)
    energies = list(map(G.hamiltonian, allstates()))
    probs = np.array([np.exp(-b*np.array(energies)) for b in blist])
    probs /= probs.sum(1)[:,None]
    return probs

def countstates(J, blist, n=200000):
    L = J.shape[1]
    mc = replicamc.ReplicaMonteCarlo(J.shape[2], J=J, tlist=1./np.array(blist))
    samples = np.array(mc.run(n, period=2, thin=20))
    counters = [Counter(map(tuple, samples[:,i])) for i in
                range(samples.shape[1])]
    N = len(samples)
    probs = [[cnt[tuple(s)]/N for s in allstates()] 
            for i, cnt in enumerate(counters)]
    return np.array(probs)

def compare(J=J, blist=blist, n=1000000):
    p = exactprobs(J, blist)
    pmc = countstates(J, blist, n)
    order = np.argsort(p[0])
    for i in range(len(blist)):
        plt.plot(p[i][order], label='Exact $\beta$={}'.format(blist[i]))
        plt.plot(pmc[i][order], '.', label='RMC$\beta$={}'.format(blist[i]))
    plt.yscale('log') 
    plt.legend()

    nonzero = np.nonzero(pmc[0,order])[0]
    plt.xlim([nonzero.min(), 2**16])
    plt.ylim([pmc[0,order][nonzero].min(), p.max()])
    return plt.gcf()

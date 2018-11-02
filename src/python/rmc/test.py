"""
Tests to make sure things work
"""

import numpy as np
from itertools import repeat, product
from collections import Counter
import matplotlib.pyplot as plt

import replicamc

def loadsamples(fname):
    ss = np.loadtxt(fname, dtype='str')
    return 2*np.array([list(map(int, s)) for s in ss])-1

rng = np.random.RandomState(14850)
L = 4
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

def mcsample(J, blist, n=200000):
    mc = replicamc.ReplicaMonteCarlo(J.shape[2], J=J, tlist=1./np.array(blist))
    return np.array(mc.run(n, period=2, thin=20))

def countstates(samples):
    counters = [Counter(map(tuple, samples[:,i])) for i in
                range(samples.shape[1])]
    N = len(samples)
    probs = [[cnt[tuple(s)]/N for s in allstates()] 
            for i, cnt in enumerate(counters)]
    return np.array(probs)

def compare(p, pmc, blist=blist):
    order = np.argsort(p[0])
    for i in range(len(blist)):
        plt.plot(p[i][order], label=r'Exact $\beta$={}'.format(blist[i]))
        plt.plot(pmc[i][order], '.', label=r'RMC $\beta$={}'.format(blist[i]))
    plt.yscale('log') 
    plt.legend()

    nonzero = np.nonzero(pmc[0,order])[0]
    plt.xlim([nonzero.min(), 2**16])
    plt.ylim([pmc[0,order][nonzero].min(), p.max()])
    return plt.gcf()


if __name__=="__main__":
    J = np.loadtxt("inp-bonds.txt").reshape(2, L, L)
    samples = loadsamples("inp-output.txt").reshape(-1, 2, L*L)
    pmc = countstates(samples)
    p = exactprobs(J, blist)
    compare(p, pmc)

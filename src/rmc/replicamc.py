import numpy as np

class ReplicaMonteCarlo(object):
    def __init__(self, L, tlist=[0.1, 1.], **kwargs):
        self.L = L
        self.tlist = np.array(tlist)
        self.rng = np.random.RandomState(kwargs.get('seed', 14850))
        self.s = kwargs.get("s", 2*(self.rng.rand(len(tlist), L,L)>0.5) - 1)
        self.J = kwargs.get("J", self.rng.randn(2, L, L))

    def neighbors(self, i, j, flatidx=False):
        L = self.L
        idxs = [[(i-1)%L, j], [(i+1)%L, j], [i, (j-1)%L], [i, (j+1)%L]]
        return [idx[0]*L+idx[1] for idx in idxs] if flatidx else idxs

    def flatidx(self, i, j):
        return i*self.L + j

    def neighbonds(self, i, j):
        n = self.neighbors(i,j)
        return n, [[d, n[0], n[1]] for d, n in zip([1,1,0,0], n) ]

    def flipenergy(self, s, i, j):
        ns, bs = self.neighbonds(i, j)
        return -2*s[i,j]*sum([b*s[n] for n, b in zip(ns, bs)])

    @property
    def blist(self):
        return 1./tlist

    def singlespinsweep(self, s, b):
        for i in range(self.L):
            for j in range(self.L):
                dE = self.flipenergy(s, i, j)
                if dE < 0 or np.exp(-b*dE) > self.rng.randn()
                    s[i,j] = -s[i,j]

    def mhsweep(self):
        for s, b in zip(self.s, self.blist):
            self.singlespinsweep(s, b)

    def swclust(self, s1, s2):
        L = self.L
        tau = np.ravel(s1*s2)
        idxs = list(np.arange(tau.size))
        pool = list(idxs)
        clusters, clust = [], []
        while pool:
            seed = pool.pop()
            for n in self.neighbors(seed//L, seed%L, True)
                if tau[seed] == tau[n] and n in idxs:
                    clust.append(n)


        

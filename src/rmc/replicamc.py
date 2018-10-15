import numpy as np
from graph import Graph


class SquareLattic(object):
    def __init__(self, L):
        self.L = L

    def neighbors(self, i):
        y, x = i//self.L, i%self.L
        return 

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

    def neighbonds(self, i, j, flatidx=False):
        n = self.neighbors(i,j, flatidx)
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
                if dE < 0 or np.exp(-b*dE) > self.rng.randn():
                    s[i,j] = -s[i,j]

    def mhsweep(self):
        for s, b in zip(self.s, self.blist):
            self.singlespinsweep(s, b)

    def swclust(self, s1, s2):
        L = self.L
        tau = np.ravel(s1*s2)
        spins = set(np.arange(tau.size))
        clusters, outbounds = [], []
        while spins:
            seed = spins.pop()
            clust = {seed}
            queue = [seed]
            outb ={} 
            while queue:
                nxt = queue.pop()
                neighs = self.neighbors(nxt//L, nxt%L, True)
                for n in neighs:
                    if not tau[nxt] == tau[n]: 
                        neighs.remove(n)
                        outb.update(n)
                clust.update(neighs)
                queue.extend(neighs)
                spins.difference_update(neighs)
            outbounds.append(outb)
            clusters.append(clust)
        return clusters, outbounds

    def clustergraph(self, s1, s2, b1, b2):
        clusters, outbounds = self.swclust(s1, s2)
        L = self.L
        G = Graph(len(clusters))
        for i, (c0, ob0) in enumerate(zip(clusters, outbounds)):
            for j, (c1, ob1) in enumerate(zip(clusters[i:], outbounds[1:])):
                bond = 0
                for idx in ob0:
                    idx_i, idx_j = idx//L, idx%L
                    for n in self.neighbors(idx_i, idx_j, True):
                        if n in ob1:
                            n_i, n_j = n//L, n%L
                            gauge = s1[idx_i, idx_j]*s1[n_i, n_j]
                            if n == idx+1:  # right
                                bond += self.J[0, idx_i, idx_j]*gauge
                            elif n == idx-1:  # left
                                bond += self.J[0, n_i, n_j]*gauge
                            elif n == idx+L:  # down
                                bond += self.J[1, idx_i, idx_j]*gauge
                            elif n == idx-L:  # up
                                bond += self.J[1, n_i, n_j]*gauge
                if not bond == 0:
                    G.insert(i, j, w=(b1 - b1)*bond)
        return G

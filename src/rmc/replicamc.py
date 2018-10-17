import numpy as np
from graph import Graph


class SquareLattice(object):
    def __init__(self, L, J):
        self.L = L
        self.nv = L*L
        self.ne = 2*L*L
        self.J = J
        assert J.shape == (2, L, L), "J is incorrect shape"
        self._1100 = [1,1,0,0]

    def flatidx(self, i, j):
        return i*self.L + j
    
    def neighbors(self, i, flatidx=True):
        assert 0<= i < self.nv, "index i out of range"
        L = self.L
        y, x = i//L, i%L
        idxs = [[(y-1)%L, x], [(y+1)%L, x], [y, (x-1)%L], [y, (x+1)%L]]
        return [self.flatidx(*idx) for idx in idxs] if flatidx else idxs

    def neighbonds(self, i, flatidx=True):
        L = self.L
        n = self.neighbors(i, flatidx)
        return n, [self.J[d, nn//L, nn%L] for d, nn in zip(self._1100, n)]

    def bond(self, i, j):
        for n, J in zip(*self.neighbonds(i)):
            if i == n:
                return J


class GeneralLattice(Graph):
    def __init__(self, nvertices):
        super().__init__(nvertices)

    def neighbors(self, i):
        edge = self.edges[i]
        idxs = []
        while edge:
            idxs.append(edge.y)
            edge = edge.nextedge
        return idxs

    def neighbonds(self, i):
        edge = self.edges[i]
        idxs, bonds = [], []
        while edge:
            idxs.append(edge.y)
            bonds.append(edge.w)
            edge = edge.nextedge
        return idxs, bonds

    def bond(self, i, j):
        edge = self.edges[i]
        while edge:
            if edge.y == j:
                return edge.w
            edge = edge.nextedge


class ReplicaMonteCarlo(object):
    def __init__(self, L, tlist=[0.1, 1.], **kwargs):
        self.L = L
        self.tlist = np.array(tlist)
        self.rng = np.random.RandomState(kwargs.get('seed', 14850))
        self.s = kwargs.get("s", 2*(self.rng.rand(len(tlist), L*L)>0.5) - 1)
        self.J = kwargs.get("J", self.rng.randn(2, L, L))
        self.G = SquareLattice(self.L, self.J)
        self.t = 0

    @property
    def blist(self):
        return 1./tlist

    def deltaE(self, i, s, G):
        ns, bs = self.G.neighbonds(i, j)
        return 2*s[i]*sum([b*s[n] for n, b in zip(ns, bs)])

    def singlespinsweep(self, s, G, b):
        for i in range(len(s)):
            if dE < 0 or np.exp(-b*self.deltaE(i, s, G)) > self.rng.randn():
                s[i] = -s[i]

    def mhsweep(self):
        for s, b in zip(self.s, self.blist):
            self.singlespinsweep(s, self.G, b)

    def swsweep(self):
        s, b = self.s, self.blist
        for i in range(len(b)-1):
            s1, s2 = s[i], s[i+1]
            clusterG = clustergraph(s1, s2, self.G)
            tau = s1 * s2
            self.singlespinsweep(tau, clusterG, b[i]-b[i+1])
            s1 *= tau
            s2 *= tau

    def step(self):
        self.mhsweep()
        self.swsweep()
        self.t += 1


def swendsenwangcluster(s1, s2, G):
    L = s1.shape[0]
    tau = s1*s2
    spins = set(np.arange(tau.size))
    clusters, outbounds = [], []
    while spins:
        seed = spins.pop()
        clust = {seed}
        queue = [seed]
        outb =set([])
        while queue:
            neighs = set(G.neighbors(queue.pop(), True))
            neighs = neighs.difference(clust)
            for n in list(neighs):
                if not tau[seed] == tau[n]: 
                    neighs.remove(n)
                    outb.add(n)
            clust.update(neighs)
            queue.extend(neighs)
            spins.difference_update(neighs)
        outbounds.append(outb)
        clusters.append(clust)
    return clusters, outbounds

def clustergraph(s1, s2, squareG):
    clusters, outbounds = swendsenwangcluster(s1, s2, squareG)
    L, J = squareG.L, squareG.J
    G = GeneralLattice(len(clusters))
    for i, (c0, ob0) in enumerate(zip(clusters, outbounds)):
        for j, (c1, ob1) in enumerate(zip(clusters[i+1:], outbounds[i+1:])):
            clusterbond = 0
            for idx in ob0.intersection(c1):
                for n, bond in zip(*squareG.neighbonds(idx)):
                    if n in c0:
                        clusterbond += bond * s1[idx] * s1[n]
            if not bond == 0:
                G.insert(i, j+i+1, w=clusterbond)
    return G

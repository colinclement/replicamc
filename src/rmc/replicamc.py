import numpy as np
from graph import Graph


class SquareLattice(object):
    def __init__(self, L, J):
        assert J.shape == (2, L, L), "J is incorrect shape"
        self.L = L
        self.nv = L*L
        self.ne = 2*L*L
        self.J = J

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
        y, x = i//L, i%L
        n = self.neighbors(i, flatidx)
        J = [self.J[1, n[0]//L, n[0]%L], self.J[1, y, x], 
             self.J[0, n[2]//L, n[2]%L], self.J[0, y, x]]
        return n, J

    def hamiltonian(self, s):
        s = s.reshape(self.J.shape[1:])
        sh = s * np.roll(s, -1, 1)
        sv = s * np.roll(s, -1, 0)
        return -np.sum(self.J[0]*sh + self.J[1]*sv)

    def graph_test_hamiltonian(self, s):
        s = s.ravel()
        energy = 0
        for i in range(self.nv):
            for n, j in zip(*self.neighbonds(i)):
                energy -= j * s[i] * s[n]
        return energy/2.  # double counted


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

    def hamiltonian(self, s):
        energy = 0
        for i in range(self.nv):
            for n, j in zip(*self.neighbonds(i)):
                energy -= j * s[i] * s[n]
        return energy/2.  # double counted


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
        return 1./self.tlist

    def hamiltonian(J, s):
        s = np.array(list(s)).reshape(J.shape[1:])
        sh = s * np.roll(s, -1)
        sv = s * np.roll(s, -1, 0)
        return -np.sum(J[0]*sh + J[1]*sv)

    def deltaE(self, i, s, G):
        ns, bs = G.neighbonds(i)
        return 2*s[i]*sum([b*s[n] for n, b in zip(ns, bs)])

    def singlespinsweep(self, s, G, b):
        for i in range(len(s)):
            dE = self.deltaE(i, s, G)
            if dE < 0 or np.exp(-b*dE) > self.rng.rand():
                s[i] = -s[i]

    def mhsweep(self):
        for s, b in zip(self.s, self.blist):
            self.singlespinsweep(s, self.G, b)

    def swsweep(self):
        s, b = self.s, self.blist
        for i in range(len(b)-1):
            s1, s2 = s[i], s[i+1]
            clusterG, clust = clustergraph(s1, s2, self.G)
            flip = np.ones(clusterG.nv)
            self.singlespinsweep(flip, clusterG, b[i]-b[i+1])
            for f, cl in zip(flip, clust):
                for c in cl:
                    s1[c] *= f
                    s2[c] *= f

    def step(self, period=5):
        self.mhsweep()
        if self.t % period == 0:
            self.swsweep()
        self.t += 1

    def run(self, n):
        samples = []
        for i in range(n):
            self.step()
            samples.append(self.s.copy())
        return samples


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
        for j, c1 in enumerate(clusters[i+1:]):
            clusterbond = 0
            for idx in ob0.intersection(c1):
                for n, bond in zip(*squareG.neighbonds(idx)):
                    if n in c0:
                        clusterbond += bond * s1[idx] * s1[n]
            if not clusterbond == 0:
                G.insert(i, j+i+1, w = - clusterbond)
    return G, clusters

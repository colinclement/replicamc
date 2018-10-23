#ifndef CLUSTER_H
#define CLUSTER_H

// given two spin configurations s1 and s2 of size L*L, this function finds the 
// clusters of same value of tau[i]=s1[i]*s2[i] by filling the list of lists
// clusters with linked lists of cluster indices. outbounds is filled with lists 
// containing the spins which are adjacent to each cluster but not contained in
// it. These indices will allow us to form the adjacency graph upon which
// Metropolis-Hastings steps can be taken.
void swendsenWangCluster(int *s1, int *s2, int **label, int *Nc, lol **outbounds, int L);

void clusterGraph(int *s1, int *s2, float *J, graph **G, int L);

#endif

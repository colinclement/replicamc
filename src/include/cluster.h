#ifndef CLUSTER_H
#define CLUSTER_H

#include <graph.h>

/* given two spin configurations s1 and s2 of size L*L, this function finds the 
 * clusters of same value of tau[i]=s1[i]*s2[i]
 */
void swendsenWangCluster(int *s1, int *s2, int L, int *label, int *Nc);

void clusterGraph(int *s1, int *s2, float *J, int L, int *label, graph **G);

void flipClusters(int *s1, int *s2, int L, int *flip, int *label);

#endif

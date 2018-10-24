#ifndef CLUSTER_H
#define CLUSTER_H

    /* given two spin configurations s1 and s2 of size L*L, this function finds the 
 * clusters of same value of tau[i]=s1[i]*s2[i]
 */
void swendsenWangCluster(int *s1, int *s2, int **label, int *Nc, int L);

void clusterGraph(int *s1, int *s2, float *J, graph **G, int L);

#endif

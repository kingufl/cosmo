

import Bio.Seq
import pickle
import sys
complement = { 'A' : 'T',
                 'T' :'A',
                 'G' :'C',
                 'C' :'G'}

print("loading cortex pickle from",sys.argv[1])
cortex_nodes = pickle.load( open(sys.argv[1], "rb" ) ) #  "cortex_pd_withdump.pickle"
print("loaded", len(cortex_nodes),"nodes")

print("loading cosmo pickle from", sys.argv[2])
cosmo_nodes = pickle.load( open(sys.argv[2] , "rb" ) ) # "cosmo_pd.out_pd_calls_graphdump.pickle"
print("loaded", len(cosmo_nodes),"nodes")


for entry_num, (cosmo_node, (cosmo_out_edges, cosmo_in_edges)) in enumerate(cosmo_nodes.items()):
    if entry_num % 100000 == 0: print("cosmo dict entry #", entry_num, "node:", cosmo_node)
    cosmo_node_revcomp = str(Bio.Seq.Seq(cosmo_node).reverse_complement())
    if cosmo_node in cortex_nodes:
        cortex_out_edges, cortex_in_edges = cortex_nodes[cosmo_node]
        # cortex's 'reverse' orientation arg for edge queries is actually the complement of the indegree; complement again for indegree 
        if (cosmo_out_edges, cosmo_in_edges) != (cortex_out_edges, cortex_in_edges):
            print("Mismatch: cosmo: ",  cosmo_node, (cosmo_out_edges, cosmo_in_edges), "dict entry", entry_num)
            print("         cortex: ", cosmo_node, (cortex_out_edges, cortex_in_edges))
    elif cosmo_node_revcomp in cortex_nodes:
        cortex_out_edges, cortex_in_edges = cortex_nodes[cosmo_node_revcomp]
        # cortex's forward orientation arg for edge queries where we want the revcomp of the node label needs edges compl'd
        cortex_out_edges = {complement[e] for e in cortex_out_edges}
        cortex_in_edges = {complement[e] for e in cortex_in_edges}        
        # we need to swap the in/out order b/c we're considering the cortex node in the non-canonical direction
        if (cosmo_out_edges, cosmo_in_edges) != (cortex_in_edges, cortex_out_edges):
            print("revcomp Mismatch: cosmo: ",  cosmo_node, (cosmo_out_edges, cosmo_in_edges),  "dict entry", entry_num)
            # again, we need to swap the in/out order b/c we're considering the cortex node in the non-canonical direction            
            print("                 cortex: ", cosmo_node_revcomp, (cortex_in_edges, cortex_out_edges))
    else:
        print("dict entry ", entry_num, ": Can't find cortex node for cosmo node:", cosmo_node)

# check for any nodes cosmo is missing that cortex has
for entry_num, (cortex_node, (cortex_out_edges, cortex_in_edges)) in enumerate(cortex_nodes.items()):
    if (cortex_out_edges or cortex_in_edges) and cortex_node not in cosmo_nodes:
        print("entry", entry_num, " cosmo is missing", (cortex_node, (cortex_out_edges, cortex_in_edges)))



        

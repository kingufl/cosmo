import sys
import re
import pickle
non_null_nodes = 0
total_nodes = 0
nodes = {}

node_re = re.compile(r"([AGCT]+) orientation 0 \{ ((?:[AGCT]\d+)*)\} orientation 1 \{ ((?:[AGCT]\d+)*)\}")
with open(sys.argv[1]) as f:
    for line in f:
        mobj = node_re.search(line)
        if mobj:
            total_nodes += 1
            group = mobj.groups()
            if len(group[1]) > 0 or  len(group[2]) > 0:
                non_null_nodes += 1
            print("out_deg:", len(group[1]), "in_deg:", len(group[2]), group)
            nodes[group[0]] = (group[1], group[2])
        

print("non_null_nodes =", non_null_nodes, "total_nodes =", total_nodes)            

pickle.dump( nodes, open( sys.argv[1] + ".pickle", "wb" ) )

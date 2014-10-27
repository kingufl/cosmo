#include <iostream>
#include <fstream>

#include <libgen.h> // basename

#include "tclap/CmdLine.h"

#include <sdsl/bit_vectors.hpp>
#include <sdsl/wavelet_trees.hpp>

#include "io.hpp"
#include "debruijn_graph.hpp"
#include "debruijn_hypergraph.hpp"
#include "algorithm.hpp"

using namespace std;
using namespace sdsl;

string graph_extension = ".dbg";
string contig_extension = ".fasta";

struct parameters_t {
  std::string input_filename = "";
  std::string output_prefix = "";
};

void parse_arguments(int argc, char **argv, parameters_t & params);
void parse_arguments(int argc, char **argv, parameters_t & params)
{
  TCLAP::CmdLine cmd("Cosmo Copyright (c) Alex Bowe (alexbowe.com) 2014", ' ', VERSION);
  TCLAP::UnlabeledValueArg<std::string> input_filename_arg("input",
            ".dbg file (output from cosmo-build).", true, "", "input_file", cmd);
  string output_short_form = "output_prefix";
  TCLAP::ValueArg<std::string> output_prefix_arg("o", "output_prefix",
            "Output prefix. Contigs will be written to [" + output_short_form + "]" + contig_extension + ". " +
            "Default prefix: basename(input_file).", false, "", output_short_form, cmd);
  cmd.parse( argc, argv );

  // -d flag for decompression to original kmer biz
  params.input_filename  = input_filename_arg.getValue();
  params.output_prefix   = output_prefix_arg.getValue();
}

int main(int argc, char* argv[]) {
  parameters_t p;
  parse_arguments(argc, argv, p);

  // The parameter should be const... On my computer the parameter
  // isn't const though, yet it doesn't modify the string...
  // This is still done AFTER loading the file just in case
  char * base_name = basename(const_cast<char*>(p.input_filename.c_str()));
  string outfilename = ((p.output_prefix == "")? base_name : p.output_prefix);

  // TO LOAD:
  debruijn_graph<> g;
  load_from_file(g, p.input_filename);

  cerr << "k             : " << g.k << endl;
  cerr << "num_nodes()   : " << g.num_nodes() << endl;
  cerr << "num_edges()   : " << g.num_edges() << endl;
  cerr << "W size        : " << size_in_mega_bytes(g.m_edges) << " MB" << endl;
  cerr << "L size        : " << size_in_mega_bytes(g.m_node_flags) << " MB" << endl;
  cerr << "Total size    : " << size_in_mega_bytes(g) << " MB" << endl;
  cerr << "Bits per edge : " << bits_per_element(g) << " Bits" << endl;

  #ifdef VAR_ORDER
  wt_huff<rrr_vector<63>> lcs;
  load_from_file(lcs, p.input_filename + ".lcs.wt");

  cerr << "LCS size      : " << size_in_mega_bytes(lcs) << " MB" << endl;
  cerr << "LCS bits/edge : " << bits_per_element(lcs) << " Bits" << endl;

  typedef debruijn_hypergraph<> dbh;
  dbh h(g, lcs);

  typedef dbh::node_type node_type;
  //node_type v(12,215855); // ...aaa*
  //node_type v(3422777,3422778); // ...ttattccgtagc->[t,g]. Other than that, totally different nodes.
  node_type v(3422774,3422778); // ....tattccgtagc->[t3,g2]
  //node_type v(3422771,3422796); // ......ttccgtagc->[acgt]
  node_type u = h.maxlen(v);
  cout << "maxlen: " << u.first << ", " << u.second << endl;
  auto y = h.maxlen(v,1);
  if (!y) cout << "NONE" << endl;
  else cout << "maxlen: " << y->first << ", " << y->second << endl;
  #endif

}


char complement(char c)
{
    switch (c) {
    case 'A': return 'T';
    case 'C': return 'G';
    case 'G': return 'C';
    case 'T': return 'A';
    }
}

int mdm_kmer_size;
char* dna_bases = "ACGT";
dump_element(Element* e)
{
    char tmp_zam2[mdm_kmer_size+1];
    binary_kmer_to_seq(element_get_kmer(e), mdm_kmer_size, tmp_zam2);
//    int color = 1;
//    Edges edges = get_edge_copy(e, color);
//    boolean db_node_edge_exist(dBNode *e, Nucleotide n, Orientation o, int colour);
    printf("%s ", tmp_zam2);
    for (int o = 0; o < 2; ++o) {
        printf("orientation %d { ", o);
        for (int nt=0; nt < 4; ++nt) {
            for (int color=0; color < 6; ++color) {
                if (db_node_edge_exist(e, nt, o, color)) {
                    printf("%c", o == 0 ? dna_bases[nt] : complement(dna_bases[nt]));
                    //printf("nt%c:%d ", nt, db_node_edge_exist(e, nt, o, color));
                }
            }
        }
        printf("} ");
    }
    printf("\n");


}

// MDM code
dump_graph(dBGraph* db_graph)
{
    mdm_kmer_size = db_graph->kmer_size;
    printf("BEGIN dump_graph()\n");
    printf("db_graph->kmer_size = %d\n", db_graph->kmer_size);
    hash_table_traverse(&dump_element, db_graph);    
    printf("END dump_graph()\n");
}

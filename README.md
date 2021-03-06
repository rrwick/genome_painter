# GenomePainter
Paint genomes with taxonomic probabilities using *k*-mers from curated datasets

## Installation
### Quick
Statically linked binaries for generic amd64 linux can be found under the GitHub releases page. Downloading and extracting these binaries will provide immediate access.

### Not so quick
If you're feeling brave, you can attempt to compile. Autotools has been used for the build system, so theoretically things Should Just Work<sup>tm</sup>. You'll need a compiler with C++11 support as well as the zlib and OpenMP libraries. Once these dependencies have been provisioned, compiling can be done by issuing:
```bash
./autogen.sh
./configure --prefix=$MY_INSTALL_PREFIX
make install -j
```

## Usage
### Data selection
The taxonomic groupings must be defined prior to using this tool. This typically involves acquisition of publicly available assemblies which are then curated. See [fetching genomes](https://github.com/scwatts/fetch_genomes) for a bare bones method of quickly collecting **all** members (specified rank and below) of a taxonomic group. Curation is done manually by examining high-level, low-resolution phylogenetic relationships using mashtree.

### *k*-mer counting
For each taxonomic group, the frequency of *k*-mers observed between assemblies is calculated. Currently only *32*-mers can be used. Here the input data is a set of FASTA files representing genomes assemblies of a single taxa and the output is a single count file for that rank. So for each rank you must run:
```bash
./count_kmers --genome_fps [taxa_A_fasta_1, taxa_A_fasta_2, ...] --output_fp counts/taxa_A.tsv
```
This is straightforward to parallelise if the FASTA files are separated in some way (e.g. on a filesystem or via a predefined list):
```bash
parallel './count_kmers --genome_fps {}/*fasta --output_fp counts/{/}.tsv' ::: assemblies/*
```
Optionally `count_kmers` allows you to utilise additional threads by specifying `--threads int` , but this is only beneficial with thousands of genomes within a single rank.

### Database generation
After *k*-mer counts have been done for all  taxonomic groups, the final database can be generated:
```bash
./generate_database --count_fps counts/*tsv --output_fp database/my_database.bin
```
During database generation, the probability that a *k*-mer occurs in a species given a specific *k*-mer sequence is calculated. Extreme differences in sample sizes can cause apparent skewing of these probabilities. To mitigate these effects, you can specify a smooth factor `α` ([see here for details](https://en.wikipedia.org/wiki/Additive_smoothing)) using `--alpha`.
There are naturally many *k*-mers which cannot be used discriminate between taxa. Typically these *k*-mers are unwanted and can be filtered by adjusting the `--threshold` parameter. The value of this parameter relates to the absolute difference between the highest and second highest taxa probabilities for a given *k*-mer. If the difference is less than `--threshold` value for these two probabilities, then the *k*-mer is excluded from the database.

### Genome painting
To perform genome painting, all that is required is a database and one or more input FASTA assemblies:
```bash
./paint_genome --genome_fp [genome_1, genome_2, ...] --kmer_db_fp database/my_database.bin --output_dir output
```
During painting both the forward and reverse strand are considered. The *k*-mer with the highest probability for each position is selected. Painting a genome produces a single tab-delimited file for each input genome. The output file contains a set of all taxa probabilities for each position which has a hit in the database. See below for visualising the probabilities.
The use of threads here can greatly reduce the runtime of painting (the process becomes disk I/O bound eventually).

## Visualising results
The data is straightforward  to plot using a variety of tools. Here is an example using `R` and `ggplot2` ([see here for script](https://github.com/scwatts/paint_plotter)):

![Example Plot](https://image.ibb.co/kvymqx/example_plot.png)

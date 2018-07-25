# De-MetaST-BLAST
Files included are for the program De-MetaST-BLAST. Additional screencast tutorial videos are provided to describe how to install the program as well as examples for executing De-MetaST-BLAST.

### Example install
```
git clone https://github.com/chrisgulvik/De-MetaST-BLAST.git ~/De-MetaST-BLAST
cd ~/De-MetaST-BLAST
make
echo 'export PATH="$PATH:$HOME/De-MetaST-BLAST:$HOME/De-MetaST-BLAST/Table"' >> ~/.bashrc
source ~/.bashrc
command -v blastx > /dev/null 2>&1 || {echo 'blast+ needs to be installed still';}
```

### Example test run (without prompts)
```
gunzip ~/De-MetaST-BLAST/test/AAFX01.1.fsa_nt.NO-Wraps.fasta.gz
mkdir ~/My_Test && cd ~/My_Test
echo "$HOME/De-MetaST-BLAST/Primers.txt" | De-MetaST-BLAST ~/De-MetaST-BLAST/AAFX01.1.fsa_nt.NO-Wraps.fa | tee log.txt
```

++++++++++++++++++++++++++++

De-MetaST-BLAST = Metagenome Search Tool + BLAST

- output:

 - FastA file(s) of retrieved amplicons. When >1 primer set is queried, each will have its own FastA output file in order of Primers.txt input.
 - TSV file(s) of the top 10 predicted functions for each amplicon
 - XML file(s) of all blastx hits


- test dataset:

 - CAMERA's FarmSoil metagenome is on [NCBI](https://www.ncbi.nlm.nih.gov/Traces/wgs/?val=AAFX01#contigs) and is also in the test directory.


- tutorial videos:

 - INSTALL_De-MetaST-BLAST.mp4
 - INSTALL_local_BLAST.mp4
 - INSTALL_local_BLAST_database.mp4

++++++++++++++++++++++++++++

Be sure to read the README-De-MetaST-BLAST.txt or README-De-MetaST-BLAST.pdf file!

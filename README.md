# Genome-Alignment

In bioinformatics, sequence alignment is used to arrange sequences of DNA, RNA, or protein to identify regions of similarity. This alignment may indicate the functional, structural, evolutionary significance of the sequence. In this project, I will analyze gene and protein sequences of three  different organisms from Primates family using global and local pair-wise sequence alignments. Within the Primates family, I will compare sequence alignments of Human and Chimpanzee with the alignments of Human and Baboon. I expect that Chimpanzees will have more similarities in genes and proteins, and that the sequence alignments of Human and Chimpanzee will have high score and consist of more matches on the sequence alignments. I derive datasets from PFAM, KEGG GENES database and NCBI database to query specific gene and protein families that I wish to compare.

Instruction:
1. The input for this program will be in the following form:
   ./<program> [Sequence1] [Sequence2] [Match Score] [Mismatch Score] [Gap Penalty] [NW(NeedlemanWunsch)/SW(SmithWaterman)]
   Ex) ./a.out sequence1.txt sequence2.txt 1 -1 1 NW
   The program will accept two different .txt files that contain gene/protein sequences along with scores for match, mismatch, and Gap Penalty, 
   and lastly the method to use ("NW" for NeedlemanWunsch Algorithm, "SW" for SmithWaterman Algorithm).
2. After the the commandline is correctly inputted, the program will execute the algorithm that the user had specified on two input sequences.
3. The program will output the Dynamic Programming Table, input sequences, algorithm used, match/mismatch score, gap penalty, score, and the 
   output sequence alignment.
4. The program will ask the name of result file to be saved, and the result will be saved in a <name>.txt file.

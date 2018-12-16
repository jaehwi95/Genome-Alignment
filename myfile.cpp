#include <libgen.h>
#include <linux/limits.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int NeedlemanWunsch(const string &seq1, const string &seq2, int match, int mismatch,
                    int gap, string &ret_seq1, string &ret_seq2);
int SmithWaterman(const string &seq1, const string &seq2, int match, int mismatch,
                    int gap, string &ret_seq1, string &ret_seq2);
int find_max_from_three(int x, int y, int z);
int find_max_from_four(int w, int x, int y, int z);

int find_max_from_four(int w, int x, int y, int z) {
    //this function finds the maximum from four inputs
    if (x >= y && x >= z && x >= w) {
        return x;
    }
    else if (y >= x && y >= z && y >= w) {
        return y;
    }
    else if (z >= x && z >= y && z >= w) {
        return z;
    }
    else {
        return w;
    }
}


int find_max_from_three(int x, int y, int z) {
    //this function finds the maximum from three inputs
    if (x >= y && x >= z) {
        return x;
    }
    else if (y >= x && y >= z) {
        return y;
    }
    else {
        return z;
    }
}

int NeedlemanWunsch(const string &seq1, const string &seq2, int match, int mismatch,
                    int gap, string &ret_seq1, string &ret_seq2)
{
    //setup matrix
    size_t seq1_size = seq1.size();
    size_t seq2_size = seq2.size();
    vector< vector<int> > s((seq1_size+1), (vector<int>(seq2_size)));
    s[0][0] = 0;
    for (size_t i = 1; i < seq1_size+1; i++) {
        s[i][0] = s[i-1][0] - gap;
    }
    for (size_t i = 1; i < seq2_size+1; i++) {
        s[0][i] = s[0][i-1] - gap;
    }

    //do NeedlemanWunsch algorithm to fill out the matrix
    int match_or_mismatch = 0;
    for (size_t i = 1; i < seq1_size+1; i++) {
        for (size_t j = 1; j < seq2_size+1; j++) {
            if (seq1[i-1] == seq2[j-1]) {
                match_or_mismatch = s[i-1][j-1] + match;
            }
            else {
                match_or_mismatch = s[i-1][j-1] + mismatch;
            }
            s[i][j] = find_max_from_three(s[i-1][j]-gap,
                                          s[i][j-1]-gap,
                                          match_or_mismatch);
        }
    }
    //now backtrace to align the sequences
    ret_seq1 = seq1;
    ret_seq2 = seq2;
    size_t i = seq1_size;
    size_t j = seq2_size;
    while(i >= 1 || j >=1) {
        if ((ret_seq1[i-1] == ret_seq2[j-1]) && (s[i][j] == (s[i-1][j-1]+match))) {
            i--;
            j--;
        }
        else if ((ret_seq1[i-1] != ret_seq2[j-1]) && (s[i][j] == (s[i][j-1]-gap))) {
            ret_seq1.insert(i, "-");
            j--;
        }
        else if ((ret_seq1[i-1] != ret_seq2[j-1]) && (s[i][j] == (s[i-1][j]-gap))) {
            ret_seq2.insert(j, "-");
            i--;
        }
        else {
            i--;
            j--;
        }

        if (i == 0 && j == 0) {
            break;
        }

        if (i == 0) {
            ret_seq1.insert(i, "-");
            j--;
        }
        else if (j == 0) {
            ret_seq2.insert(j, "-");
            i--;
        }
    }

    // printout the Dynamic programming table
    for (int i = 0; i < seq1_size+1; i++) {
        for (int j = 0; j < seq2_size+1; j++) {
            cout << s[i][j] << "\t";
        }
        cout << "" << endl;
    }

    //return last elment in table (i.e. score)
    return s[seq1_size][seq2_size];
}

int SmithWaterman(const string &seq1, const string &seq2, int match, int mismatch,
                    int gap, string &ret_seq1, string &ret_seq2)
{
    //setup matrix
    size_t seq1_size = seq1.size();
    size_t seq2_size = seq2.size();
    vector< vector<int> > s((seq1_size+1), (vector<int>(seq2_size)));
    s[0][0] = 0;
    for (size_t i = 1; i < seq1_size+1; i++) {
        s[i][0] = 0;
    }
    for (size_t i = 1; i < seq2_size+1; i++) {
        s[0][i] = 0;
    }

    //do SmithWaterman algorithm to fill out the matrix
    int match_or_mismatch = 0;
    for (size_t i = 1; i < seq1_size+1; i++) {
        for (size_t j = 1; j < seq2_size+1; j++) {
            if (seq1[i-1] == seq2[j-1]) {
                match_or_mismatch = s[i-1][j-1] + match;
            }
            else {
                match_or_mismatch = s[i-1][j-1] + mismatch;
            }
            s[i][j] = find_max_from_four(0, s[i-1][j]-gap,
                                          s[i][j-1]-gap,
                                          match_or_mismatch);
        }
    }

    //find the score, and index of the score elem
    int max_xpos = 0;
    int max_ypos = 0;
    int maxscore = 0;
    for (size_t i = 0; i <= seq1_size; i++) {
        for (size_t j = 0; j <= seq2_size; j++) {
            if (s[i][j] > maxscore) {
                max_xpos = i;
                max_ypos = j;
            }
            maxscore = max(s[i][j], maxscore);
        }
    }

    //now backtrace to align the sequences
    ret_seq1 = seq1;
    ret_seq2 = seq2;
    string t1holder = "";
    string t2holder = "";
    size_t i = max_xpos;
    size_t j = max_ypos;

    for (int x = max_xpos; x > 0;) {
        int flag = 0;
        for (int y = max_ypos; y > 0;) {
            if (s[x][y] == s[x-1][y]-gap) {
                t1holder = ret_seq1[x-1] + t1holder;
                t2holder = "-" + t2holder;
                x--;
            }
            else if (s[x][y] == s[x][y-1]-gap) {
                t2holder = ret_seq2[y-1] + t2holder;
                t1holder = "-" + t1holder;
                y--;
            }
            else if (s[x][y] == s[x-1][y-1]+match) {
                t1holder = ret_seq1[x-1] + t1holder;
                t2holder = ret_seq2[y-1] + t2holder;
                x--;
                y--;
            }
            else {
                x--;
                y--;
            }
            // cout << t1holder << "               " << t2holder << endl;
            if (s[x][y] == 0) {
                flag = 1;
                break;
            }
        }
        if (flag == 1) {
            break;
        }
    }
    ret_seq1 = t1holder;
    ret_seq2 = t2holder;
    cout << "debuf purposes" << endl;

    // printout the Dynamic programming table
    for (int i = 0; i < seq1_size+1; i++) {
        for (int j = 0; j < seq2_size+1; j++) {
            cout << s[i][j] << "\t";
        }
        cout << "" << endl;
    }

    //return last elment in table (i.e. score)
    return maxscore;
}


int main(int argc, char* argv[])
{
    //usage when incorrect input
    if (argc != 7) {
        printf("usage:   %s <program> [Sequence1] [Sequence2] [Match Score] [Mismatch Score] [Gap Penalty] [NW(NeedlemanWunsch)/SW(SmithWaterman)]\n", argv[0]);
        printf("example: %s sequence1.text sequence2.txt 1 -1 1 NW\n", argv[0]);
        return 1;
    }
    if (!((string(argv[6]) == "NW" || string(argv[6]) == "SW"))) {
        printf("usage:   %s <program> [Sequence1] [Sequence2] [Match Score] [Mismatch Score] [Gap Penalty] [NW(NeedlemanWunsch)/SW(SmithWaterman)]\n", argv[0]);
        printf("example: %s sequence1.text sequence2.txt 1 -1 1 NW\n", argv[0]);
        return 1;
    }

    //open textfile, and save sequence to string
    fstream seq1, seq2;
    seq1.open(argv[1]);
    seq2.open(argv[2]);
    string s1 = "";
    string s2 = "";
    string line;
    for (size_t i = 0; getline(seq1, line); i++) {
        s1 += line;
    }
    for (size_t i = 0; getline(seq2, line); i++) {
        s2 += line;
    }
    //initialize output string and convert arg to int
    string outseq1;
    string outseq2;
    int gap_pen = atoi(argv[5]);
    int match = atoi(argv[3]);
    int mismatch = atoi(argv[4]);

    //do NeedlemanWunsch / SmithWaterman Algorithm and find score
    int score = 0;
    string method = "";
    if (string(argv[6]) == "NW") {
        method = "Needleman-Wunsch Algorithm (Global Alignment)";
        score = NeedlemanWunsch(s1, s2, match, mismatch, gap_pen, outseq1, outseq2);
    }
    else if (string(argv[6]) == "SW") {
        method = "Smith-Waterman Algorithm (Local Alignment)";
        score = SmithWaterman(s1, s2, match, mismatch, gap_pen, outseq1, outseq2);
    }


    //print output
    cout << endl;
    cout << "Sequence1: " << s1 << endl;
    cout << "Sequence2: " << s2 << endl;
    cout << "Algorithm: " << method << endl;
    cout << "Match Score: " << match;
    cout << "   Mismatch Score: " << mismatch;
    cout << "   Gap Penalty: " << gap_pen << endl;
    cout << "Score: " << score << endl;
    cout << "Output: " << endl;
    cout << outseq1 << endl;
    cout << outseq2 << endl;
    cout << endl;
    //write Output
    cout << "Name for result file?" << endl;
    cout << "example: result" << endl;
    string fname;
    cin >> fname;
    fname = fname + ".txt";
    ofstream outfile(fname.c_str());
    outfile << "Sequence1: " << s1 << endl;
    outfile << "Sequence2: " << s2 << endl;
    outfile << "Algorithm: " << method << endl;
    outfile << "Match Score: " << match;
    outfile << "   Mismatch Score: " << mismatch;
    outfile << "   Gap Penalty: " << gap_pen << endl;
    outfile << "Score: " << score << endl;
    outfile << "Output: " << endl;
    outfile << outseq1 << endl;
    outfile << outseq2;

    outfile.close();
    seq1.close();
    seq2.close();

    cout << "Done! Your result has been saved into " << fname << "." << endl;
    return 0;
}

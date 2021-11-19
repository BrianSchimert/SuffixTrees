#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <iostream>
#include "GeneSequence.h"
#include "SuffixTree.h"


using namespace std;

vector<GeneSequence> genes;

void openGenomeFile(string myfile) {
	ifstream myFile(myfile);
	string line;
	//GeneSequence currentGene;
	GeneSequence currentGene;
	int time = 0;
	while (getline(myFile, line)) {
		if (line.empty()) {
			continue;
		}
		else if (line[0] == '>') { //new sequence found
			if (time != 0) {
				genes.push_back(currentGene);
				currentGene.sequence.clear();
			}
			string name = line.substr(1, line.find_first_of(" "));
			currentGene.name = name;
			time = 1;
		}
		else {  // (sequence lines) ie. lines that are not part of the name, or empty
			currentGene.sequence += line;
		}

	}
	genes.push_back(currentGene);

}


int main(int argc, char* argv[])
{
	fstream file;
	//file.open("Opsin1_colorblindness_gene_globalAlign.txt", ios::out);
	//file.open("Human-Mouse-BRCA2-cds_globalAlign.txt", ios::out);
	//file.open("Opsin1_colorblindness_gene_localAlign.txt", ios::out);
	//file.open("tester.txt", ios::out);

	//auto cout_buf = cout.rdbuf(file.rdbuf());

	string inputFile;
	string alphabetFile;
	if (argc > 1) { //if being run on command line
		//for (int i = 0; i < argc; i++)
		//	std::cout << argv[i] << "\n";
		inputFile = argv[1];
		alphabetFile = stoi(argv[2]);
		openGenomeFile(inputFile);

	}
	else {
		
		inputFile = "strings1.fas";
		//inputFile = "strings2.fas";
		//inputFile = "colorblind_human_gene.fasta";
		//inputFile = "colorblind_mouse_gene.fasta";
		//inputFile = "Human-BRCA2-cds.fasta";
		//inputFile = "TomatoChloroplast.fas";
		//inputFile = "yeastchr12.fas";
		//inputFile = "Covid_Wuhan.fasta";
		//inputFile = "Covid_USA.fasta";
		//inputFile = "Covid_Australia.fasta";
		//inputFile = "Covid_India.fasta";
		//inputFile = "Covid_Brazil.fasta";
		
		openGenomeFile(inputFile);

	}
	string alphabet;
	string s1 = genes.at(0).sequence;
	//cout << s1;
	string s1Name = genes.at(0).name;

	int m = s1.length();
	string outfile = inputFile.substr(0, inputFile.find('.'));
	//cout << outfile;
	outfile += "_BWT.txt";
	//cout << outfile;



	cout << "Suffix Tree String Info: " << "\n";
	cout << "Name: " << s1Name << "   Length: " << m;
	cout << "\n\n";
	//cout << s1;
	//cout << s1;
	cout << "\n\n";

	alphabet = "A B C D E F G H I J K L M N O P Q R S T U V W X Y Z";
	SuffixTree st(s1, alphabet);
	st.printBWTToFile(outfile);




	file.close();
	return 0;
}

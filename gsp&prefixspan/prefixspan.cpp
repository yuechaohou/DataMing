#include<fstream>
#include<iostream>
#include<string>
#include<vector>
#include<cstdlib>
#include<ctime>
using namespace std;

int compute_support(vector<string>& data, vector<int> index, char key){	
	int count=0;
	for(unsigned i=0; i<data.size(); i++){
		if(index[i]<0) 	continue;
		else{
			char* it = &data[i][index[i]];
			while(*it != '\0'){				
				if(*it == key){
					count++;
					break;
				}
				++it;
			}
		}
	}
	return count;
};


void trune(vector<string>& data, vector<int>& index, char key, bool& end){
	end =false;
	for(unsigned i=0; i<data.size(); i++){
		if(index[i]<0) {
			end  = (end | false); 
			continue;
		}
		else{
			int temp = index[i];
			char* it = &data[i][index[i]];
			while(*it != '\0'){				
				if(*it == key){
					index[i] = temp+1;
					it++;
					break;
				}
				temp++;
				++it;
			}
			if(*it == '\0'){
				index[i]=-1;
				end= (end | false);
			}
			else{
				end = (end | true);
			}			
		}
	}		
};



void PrefixSpan(vector<string>& data, vector<int> index, string alphabet, string current, int minsup, unsigned int i, bool stage, ofstream& ofile){
	if(stage){	
		for(i=0; i<alphabet.length(); i++){
		int count= compute_support(data, index, alphabet[i]);
		if (count >= minsup){
				string next = current + alphabet[i];
				ofile << next  << " " << count << endl;  

				stage = false;
				PrefixSpan(data, index, alphabet, next, minsup, i, stage, ofile);
			}
		}
	}
	else{		
			bool not_end = false;
			trune(data, index, alphabet[i], not_end);
			for(unsigned int j=0; j< alphabet.length(); j++){
				if(compute_support(data, index, alphabet[j]) < minsup){
					alphabet.erase(j,1);
				}
			}
			stage = true;
			if(not_end){
				int k=0;
				PrefixSpan(data, index, alphabet, current, minsup, k, stage, ofile);
			}
	}
};

int main(int argc, char** argv){
	ifstream iFile;
	ofstream oFile;
	string line;
	char n = '>';
	int minsup = atoi(argv[1]);
	vector<string> data;
	vector<int> index;
	string current = "";
	string alphabet = "ACGT";
	clock_t start, stop;

	
	iFile.open("sequencedb.txt");
	oFile.open("output.txt");

	while(getline(iFile, line)){
		if(line[0] == n ) {
				continue;
		}
		else {
				data.push_back(line);
				index.push_back(0);
			}
	}

	start= clock();
	PrefixSpan(data, index, alphabet, current, minsup, 0, true, oFile);
	stop=clock();
	cout << (double)(stop-start)/CLOCKS_PER_SEC <<endl;
	

	iFile.close();
	oFile.close();

	return 0;
}


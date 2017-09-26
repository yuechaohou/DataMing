#include<fstream>
#include<iostream>
#include<string>
#include<vector>
#include<unordered_map>
#include<cstdlib>
#include<ctime>
using namespace std;

struct node{
	string sequence;
	int sup;
	vector<int> index;
	vector<char> silbing;
};

//compute support
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

bool check_alpha(unordered_map<string, bool>& level, string candidator){
	for(unsigned int i=0; i<candidator.length(); i++ ){
		string next=candidator;
		next.erase(i,1);
		if(level.find(next)==level.end()){
			return false;
		}
	}
	return true;	
};


void extend_tree(vector<string>& data, unordered_map<string, bool>& level, vector<node>& leaf, int minsup, string alphabet, ofstream& ofile){
	vector<node> new_leaf;
	unordered_map<string, bool> new_level;

	for(unsigned int i=0; i<leaf.size(); i++){
		unordered_map<char, bool> check;
		vector<node> parent;
		for(unsigned int j=0; j< leaf[i].silbing.size(); j++){
			node temp;
			char new_char=leaf[i].silbing[j];

			temp.index = leaf[i].index;
			string new_seq =leaf[i].sequence + new_char;
			if(check_alpha(level, new_seq)){
				int sup = compute_support(data, temp.index, new_char);
				if(sup>=minsup){
					temp.sup=sup;
					temp.sequence = new_seq;
					bool end=false;
					if(check.find(new_char) == check.end()){
						pair<char, bool> tt(new_char, true); 
						check.insert(tt);
					}
					trune(data, temp.index, new_char, end);
					pair<string, bool> dd(new_seq, true);
					new_level.insert(dd);
					parent.push_back(temp);
					ofile << new_seq << " " << sup <<endl;
				}
			}
		}
	
		vector<char> brother;
		for(unsigned int j=0; j<alphabet.length();j++){
			if(check.find(alphabet[j])!=check.end()){
				brother.push_back(alphabet[j]);
			}
		}
		for(unsigned int j=0; j < parent.size(); j++){
			parent[j].silbing = brother;
			new_leaf.push_back(parent[j]);
		}

	}
	leaf=new_leaf;
	level=new_level;

};


void gsp(vector<string>& data, vector<int>& index, vector<node>& leaf, string alphabet, int minsup, ofstream& ofile){

	unordered_map<string, bool> level;
	vector<char> silb;
	
	for(unsigned int i=0;i < alphabet.length();i++){
		int count = compute_support(data, index, alphabet[i]);
		node candidator;
		if(count >= minsup){	
			string root = alphabet.substr(i,1);
			pair<string, bool> pp(root,true);
			level.insert(pp);

			candidator.index = index;
			bool end = false;
			trune(data, candidator.index, alphabet[i], end);
			ofile << root << " " << count <<endl;			

			candidator.sup=count;
			candidator.sequence = root;
			leaf.push_back(candidator);
			silb.push_back(alphabet[i]);
		}
	}
	
	for(unsigned int i=0; i < leaf.size(); i++ ){
			leaf[i].silbing = silb;
	}


	while(leaf.size()>0){
		extend_tree(data, level, leaf, minsup, alphabet, ofile);
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
	vector<node> leaf;
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
	gsp(data, index, leaf, alphabet, minsup, oFile);

	stop=clock();
	cout << (double)(stop-start)/CLOCKS_PER_SEC <<endl;
	
	iFile.close();
	oFile.close();

	return 0;
}


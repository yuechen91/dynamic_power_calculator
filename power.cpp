#include<iostream>
#include<fstream>
#include<string>
using namespace std;

int extract_number(string line){
	size_t pos = line.find("    ");
	string number = line.substr(pos);
//	cout<<number<<endl;
	return stoi(number);
}

int main(int argc, char *argv[]){

	string inputfile=argv[1];
	ifstream fileHandler;


	float f = 1e9;
	float inject = 0.3;
	float port = 5;

	fileHandler.open(inputfile);
	if(!fileHandler.is_open()){
		cout<<"Error when opening "<<inputfile<<endl;
		return 0;
	}
	if(argv[2]==NULL){
		cout<<"No set number!"<<endl;
		return 0;
	}
	else{
		cout<<"Stats File: "<<inputfile<<" Set = "<<argv[2]<<endl;
	}
	int set = atoi(argv[2]) - 1;

	//************ open file

	string marco[16];
	string start="system.ruby.network.routers";
	for(int i =0; i< 16; i++){
		if(i<10)
			marco[i] = start +"0" + to_string(i);
		else 
			marco[i] = start + to_string(i);
//	       cout<<marco[i]<<endl;
	}
	//************* preparing marco for searcing

	int read[16][4];
	int write[16][4];
	int in_activity[16][4];
	int out_activity[16][4];
	int cross_activity[16][4];
	string line;

	int a = 0;
	int b = 0;
	int c = 0;
	int d = 0;
	int e = 0;

	for(int i=0; i< 16; i++){
		while(getline(fileHandler,line)){

			if(line.find(marco[i]+".buffer_reads") != string::npos){
			//	cout<<line<<endl;
			//	cout<<extract_number(line)<<endl;
				read[i][a]=extract_number(line);
				a++;
			}
			if(line.find(marco[i]+".buffer_writes") != string::npos){
			//	cout<<line<<endl;
			//	cout<<extract_number(line)<<endl;
				write[i][b]=extract_number(line);
				b++;
			}
			if(line.find(marco[i]+".sw_input_arbiter_activity") != string::npos){
			//	cout<<line<<endl;
			//	cout<<extract_number(line)<<endl;
				in_activity[i][c]=extract_number(line);
				c++;
			}
			if(line.find(marco[i]+".sw_output_arbiter_activity") != string::npos){
			//	cout<<line<<endl;
			//	cout<<extract_number(line)<<endl;
				out_activity[i][d]=extract_number(line);
				d++;
			}
			if(line.find(marco[i]+".crossbar_activity") != string::npos){
			//	cout<<line<<endl;
			//	cout<<extract_number(line)<<endl;
				cross_activity[i][e]=extract_number(line);
				e++;
			}
		}
		fileHandler.clear();//reset getline()
		fileHandler.seekg(0,ios::beg);//set the curser to the top of file
		a=0;
		b=0;
		c=0;
		d=0;
		e=0;
//		cout<<endl;
	}
	//*********** extract numbers
	
	double dynamic_power = 0;
	double sa_dynamic = 0;
	double xbar_dynamic = 0;
	double buffer_dynamic = 0;

	for(int i=0; i < 16; i++){
		sa_dynamic = sa_dynamic + in_activity[i][set] * f * inject * port + out_activity[i][set] * f * inject * port;
	}
	
	for(int i=0; i < 16; i++){
		xbar_dynamic = xbar_dynamic + cross_activity[i][set] * f * inject;
	}

	for(int i=0; i < 16; i++){
		buffer_dynamic = buffer_dynamic + read[i][set] * f * inject * port + write[i][set] * f * inject * port ;
	}

//	for(int i=0; i < 16; i++){
//		cout<<"i= "<<i<<" "<<read[i][set]<<" " <<write[i][set]<<" "<<in_activity[i][set]<<" "
//			<<out_activity[i][set]<<"  "<<cross_activity[i][set]<<endl;
//	}
//	cout<<xbar_dynamic<<endl<<buffer_dynamic<<endl<<sa_dynamic<<endl;
	
	dynamic_power = xbar_dynamic + buffer_dynamic + sa_dynamic;
	cout<<"Dynamic Power ="<<dynamic_power<<endl;

	fileHandler.close();
	return 0;
}

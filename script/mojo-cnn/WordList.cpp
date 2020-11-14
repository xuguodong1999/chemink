#include<bits/stdc++.h>
using namespace std;
string s[10000];
string sstmp;
string mystring;
bool validchar(char c){
	if(!( 
	('0'<=c&&c<='9')||
	('a'<=c&&c<='z')||
	('A'<=c&&c<='Z')||
	(c=='(')||
	(c==')')
	))
		return 0;
	return 1;
}
int main(){
	
	FILE *fp=fopen("WordList.txt","r");
	FILE *fp2 = fopen("res.txt","w");
	char a[100];int count_mat=0;
	while(!feof(fp)){
		fscanf(fp,"%s",a);
		s[count_mat++] = a;
	}
	
	
	
	for (int i = 0; i < count_mat - 1; i++) {
		int flag = i;
		for (int j = i + 1; j < count_mat; j++) {
			if (s[flag]<s[j])
				flag = j;
		
				
		}
		if (flag != i) {
			sstmp = s[flag];
			s[flag] = s[i];
			s[i] = sstmp;
		}
	}
	for (int i = 0; i < count_mat - 1; i++) {
		if (s[i] == s[i + 1]) {
			for (int j = i + 1; j < count_mat; j++)
				s[j] = s[j + 1];
			s[count_mat - 1].clear();
			count_mat--;
			i--;
		}
	}
	int mycount=0;
	for(int i=0;i<count_mat;i++){
		int len = s[i].length();
		for(int j=0;j<len;j++)
			a[j] = s[i][j];
		a[len] = '\0';
		for(int j=0;j<len;j++)
			if(!validchar(a[j]))
				goto GG;
		mystring = a;
		while('0'<=mystring[0]&&mystring[0]<='9')
			mystring.erase(mystring.begin());
		fprintf(fp2,"\"");
		fprintf(fp2,"%s",(char*)mystring.c_str());
		fprintf(fp2,"\",");mycount++;
		if('a'<=mystring[0] && mystring[0]<='z'){
			mystring[0] = mystring[0] - 'a' + 'A';
			fprintf(fp2,"\"");
			fprintf(fp2,"%s",(char*)mystring.c_str());
			fprintf(fp2,"\",");mycount++;
		}
		GG:;
	}
	cout<<mycount<<"done."<<endl;
	int stop;cin>>stop;
	return 0;
}


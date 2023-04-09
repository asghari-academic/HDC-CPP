#include <stdio.h>
#include <bitset>
#include <stdlib.h>
#include <limits.h>

#define printInit
#define printVals
#define printVars
//#define printH
//#define printQuery

int mask[8]={1,1<<1,1<<2,1<<3,1<<4,1<<5,1<<6,1<<7};
//int mask_rev[8]={1<<7,1<<6,1<<5,1<<4,1<<3,1<<2,1<<1,1};

int bitAt(int a, int index){
	return (a & mask[index])>>index;
}

void printBinChar(char a){
	for(int i=7; i>=0; i--){
		printf("%d",bitAt(a,i));
	}
}
void printHV(char* hv, int d){
	int div= d/8;
	int r= d%8;
	if(r>0){
		for(int i=r-1; i>=0; i--){
			printf("%d",bitAt(hv[div],i));
		}	
	}
	for(int j=div-1;j>=0;j--){
		printBinChar(hv[j]);
	}
	//printf("\n");
}
void randomHV(int d, char* hv, int var){
	
	int div= d/8;
	int r= d%8;
	if(r>0){
		div++;
	}
	srand (var);
	for(int i=0;i<div;i++){
		char c=(char)(rand());
		//printf("%d\n",c);
		hv[i]= c;
	}
}

void xorHV(char* A, char* B, char* out, int size){
	for(int i=0;i<size; i++){
		out[i]= A[i] ^ B[i];
	}
}

void majorityHV( char **HVs, int numberHVs, char* out, int size){
	int m= (numberHVs/2)+(numberHVs%2);
	for(int i=0;i<size; i++){
		int s[8]={0,0,0,0,0,0,0,0};
		for(int j=0; j<numberHVs;j++){
			//char a=HVs[i+(j*size)];
			char a=HVs[j][i];
			for(int b=0; b<8; b++){
				s[b]+=bitAt(a,b);
			}
		}
		out[i]=0;
		for(int l=0;l<8;l++){
			if(s[l]>=m){
				out[i] = out[i] | mask[l];
			}
		}
	}
}
int sumBitsHV(char* hv, int d){
	int div= d/8;
	int r= d%8;
	int sum=0;
	if(r>0){
		for(int i=r-1; i>=0; i--){
			sum+=bitAt(hv[div],i);
		}	
	}
	for(int j=div-1;j>=0;j--){
		for(int i=0; i<8; i++){
			sum+=bitAt(hv[j],i);
		}
	}
	return sum;
}
int writeAppend(char *ch, char* addr)
{
   FILE *fptr;

   // use appropriate location if you are using MacOS or Linux
   fptr = fopen(addr,"a");

   if(fptr == NULL)
   {
      printf("Error!");   
      exit(1);             
   }

   fprintf(fptr, ch);
   fclose(fptr);

   return 0;
}
int write(char *ch, char* addr)
{
   FILE *fptr;

   // use appropriate location if you are using MacOS or Linux
   fptr = fopen(addr,"w");

   if(fptr == NULL)
   {
      printf("Error!!");   
      exit(1);             
   }

   fprintf(fptr, ch);
   fclose(fptr);

   return 0;
}
void test(int d, int numOfVar){
	//int d=1000;
	//int numOfVar=500;

	//
	int div= d/8;
	int r= d%8;
	if(r>0){
		div++;
	}
	
#ifdef printInit
	char addr[100]="";
	sprintf(addr, "F:\\test\\test_%dvar%dD.h", numOfVar,d);
	char s[300]="";
	sprintf(s, "int d=%d;\nint numOfVar=%d;\nint div=%d;\n", d, numOfVar,div);
	write(s, addr );
#endif
	
	//init
	char vals[numOfVar][div];
	char vars[numOfVar][div];

	char **temp = new char*[numOfVar];
	for(int i = 0; i < numOfVar; ++i)
    	temp[i] = new char[div];
	
	
	
	for(int i=0; i<numOfVar;i++){
		randomHV(d, &vals[i][0],(i+1)+11);
		//printHV(&vals[i][0], d);
	}
	//printf("\n");
	//random var
	for(int i=0; i<numOfVar;i++){
		randomHV(d, &vars[i][0],(i+1+numOfVar)+1);
		//printHV(&vars[i*div], d);
	}
	//printf("\n");
	
	//bind  var[i] = val[i]
	for(int i=0; i<numOfVar;i++){
		xorHV(&vars[i][0], &vals[i][0], temp[i], div);
		//printHV(temp[i], d);
	}
	
	//printf("\n");
	                                                      
	char H[div];
	majorityHV(temp, numOfVar, H, div);                                         
	//printHV(H, d);
	//printf("\n");
	
#ifdef printVals
	//whrite Vals;
	sprintf(s, "char vals[%d][%d]{\n", numOfVar,div);
	writeAppend(s, addr );
	for(int i=0; i<numOfVar;i++){
		sprintf(s, "{");
		writeAppend(s, addr );
		for(int j=0; j<div;j++){
			sprintf(s, "%d,", vals[i][j]);
			writeAppend(s, addr );
		}
		sprintf(s, "},\n");
		writeAppend(s, addr );
	}
	sprintf(s, "};\n");
	writeAppend(s, addr );
#endif
#ifdef printVars	
	//whrite Vars;
	sprintf(s, "char vars[%d][%d]{\n", numOfVar,div);
	writeAppend(s, addr );
	for(int i=0; i<numOfVar;i++){
		sprintf(s, "{");
		writeAppend(s, addr );
		for(int j=0; j<div;j++){
			sprintf(s, "%d,", vars[i][j]);
			writeAppend(s, addr );
		}
		sprintf(s, "},\n");
		writeAppend(s, addr );
	}
	sprintf(s, "};\n");
	writeAppend(s, addr );
#endif        
#ifdef printH	
	//whrite Vars;
	sprintf(s, "char H[%d][%d]{\n", numOfVar,div);
	writeAppend(s, addr );
	for(int i=0; i<div;i++){
		sprintf(s, "%d,", H[i]);
		writeAppend(s, addr );
	}
	sprintf(s, "};\n");
	writeAppend(s, addr );
#endif	
	//find in H
	char HX[div];
	char X[div];
	int acc=0;
	
	
	// H*var[i] --> ~val[i]
	for(int q=0; q<numOfVar; q++){
		//HX=H*var[q]
		xorHV(&vars[q][0], H, HX, div);
#ifdef printQuery	
	//whrite Vars;
	sprintf(s, "char HVar%d[%d]{\n",q, div);
	writeAppend(s, addr );
	for(int i=0; i<div;i++){
		sprintf(s, "%d,", HX[i]);
		writeAppend(s, addr );
	}
	sprintf(s, "};\n");
	writeAppend(s, addr );
#endif		
		int min=d;
		int min2=d;
		int max=0;
		int index=0;
		int index2=0;
		for(int i=0; i<numOfVar;i++){
			xorHV(&vals[i][0], HX, X, div);
			//printHV(&temp[i*div], d);
			int sum= sumBitsHV(X, d);
			
			if(min>sum){
				min2=min;
				index2=index;
			}
			index= min>sum? i:index;
			min  = min>sum? sum:min;
			max  = max<sum? sum:max;
		}
		if(index==q){
			acc++;
		}
		//char s[300];
		//sprintf(s, "%d: min: %d, max: %d, min2: %d, index: %d, index2: %d\n",q, min, max, min2, index, index2 );

	}
#ifndef printInit
	char s[300]="";
	char addr[100]="";
#endif
	sprintf(s, "%d,%f\n", d,((double)acc/(double)numOfVar)*100.0);

	sprintf(addr, "F:\\test\\test_%dvar.csv", numOfVar);
	writeAppend(s, addr );
}

void test_HDC_Var(){
	int D[13]={101, 201, 501, 1001,1501,2001,4001,5001,6001,7001,8001,9001,10001};
    int variables[4] ={100, 500, 1000, 2000};
	for(int i=3; i<4; i++){
		int numOfVar= variables[i];
		char addr[100]="";
		sprintf(addr, "F:\\test\\test_%dvar.csv", numOfVar);
		char s[100]="D,acc\n";
		write(s, addr );
		for(int j=12; j<13; j++){
			int d= D[j];
			test(d, numOfVar);
		}
	}
}



int main(){
	
	//testlfsr();
	//int d=100;
	//int numOfVar=500;
	
	test_HDC_Var();
	
	//test_Sobol_Seq();
}
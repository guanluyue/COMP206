//Gwen Guan
//260950108
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//char eMatrix;
//char dMatrix;
char inputData[500];
char matrix[500][500];
void errorMsg(char *msg){
	printf("%s\n",msg);
	//printf("Program syntax:\n");
	printf("./cipher SWITCH KEY LENGTH < FILENAME\n");
	//printf("Legal usage examples:\n");
	printf("\t./cipher -e 10 100 < filename.txt\n\t./cipher -d 10 200 < filename.e\n");
	exit(EXIT_FAILURE);
}

void encrypt(int key, int length){
	int rows = length/key;
	//printf("%d",rows);
//	printf("%s", inputData);
	int j,k=0;
	int num=0;
	int first=length%key;
	for (int j=0; j<rows+1; j++){
		for (int k=0; k<key; k++){
			matrix[j][k]=inputData[num];
			num++;
			if (num>=length){
                                break;
                        }	
		}
	}
	
	int a,b=0;
	int n=0;
	for (int a=0; a<key; a++){
		for (int b=0; b<rows+1; b++){
			if ((b==rows) && (a>=first)){
                                break;
                        }
			
			printf("%c",matrix[b][a]);
			n++;
			if (n>=length){
				break;
			}
		}
	}
}

void decrypt(int key, int length){
	int a,b=0;
	int rows = length/key;
	int num=0;
	int first= length%key;

	for (int a=0; a<key; a++){
		for (int b=0;b<rows+1; b++){
			if (b==rows && a>=first){
				break;
                        }
			matrix[b][a]=inputData[num];
			num++;
			if (num>=length){
                                break;
                        }

		}
	}
	int j,k=0;
	int n=0;
	for (int j=0; j<rows+1; j++){
		for (int k=0; k<key; k++){
			printf("%c",matrix[j][k]);
			n++;
			if (n>=length){
                                break;
			}
		}
	}
}

int main(int argc, char *argv[]){
	
	if (argc != 4){
		errorMsg("Wrong number of arguments provided.");
	}
	if (atoi(argv[3]) >= 500){
		errorMsg("Length must be less than 500.");
	}
	if (atoi(argv[2]) >= atoi(argv[3])){
		errorMsg("Key must be less than length");
	}
	if (strcmp(argv[1], "-e") == 0){
		int i=0;
		for (i=0; i<(atoi(argv[3])); i++){
                	inputData[i] = getc(stdin);
        	}
		encrypt(atoi(argv[2]), atoi(argv[3]));
	}
	else if (strcmp(argv[1], "-d") == 0){
		int i=0;
                for (i=0; i<(atoi(argv[3])); i++){
                        inputData[i] = getc(stdin);
                }
		decrypt(atoi(argv[2]), atoi(argv[3]));
	}
	else{
		errorMsg("Invalid switch provided.");	
	}
}


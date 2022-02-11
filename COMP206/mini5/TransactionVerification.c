//Gwen Guan
//260950108
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void errorMsg(char *msg, int code, char *filename){
	if (filename==NULL){
		printf("%s\n",msg);
	}else{
		printf("Unable to open filename %s\n", filename);
	}
       	printf("./tv STATE TRANSACTIONS\n");
	printf("\t./tv state.csv transaction.csv\n\t./tv ../state.csv /data/log/transaction.csv\n");
	exit(code);
}

struct ACCOUNT {
	int accountNumber;
	double startingBalance;
	double endingBalance;
	struct ACCOUNT *next;
}ACCOUNT;


void traverse(struct ACCOUNT* head, char* msg){
        struct ACCOUNT* ptr=head;
	printf("%s",msg);
        while (ptr!=NULL){
                printf("%d %.2f %.2f\n",ptr->accountNumber, ptr->startingBalance, ptr->endingBalance);
        	ptr=ptr->next;
	}
	printf("----Traversal Finished----\n\n");
}

struct ACCOUNT* addNode (struct ACCOUNT* head, struct ACCOUNT* tmp){

	if (head==NULL){
		head=tmp;
	}else{
		if (tmp->accountNumber < head->accountNumber){
			tmp->next=head;
			head=tmp;
		}else{
			struct ACCOUNT* ptr=head;
			if (ptr->accountNumber == tmp->accountNumber){
			
                                printf("Duplicate account number [account, start, end]: %06d %.2f %.2f\n", tmp->accountNumber, tmp->startingBalance, tmp->endingBalance);
				return head;
			}
			while (ptr->next!=NULL){
				if (ptr->accountNumber == tmp->accountNumber){
					printf("Duplicate account number [account, start, end]: %06d %.2f %.2f\n", tmp->accountNumber, tmp->startingBalance, tmp->endingBalance);
					break;
				}else{
					if (tmp->accountNumber < ptr->next->accountNumber){
						tmp->next=ptr->next;
						ptr->next=tmp;
						break;
					}
					ptr=ptr->next;
				}
			}
			if (ptr->next==NULL){
				ptr->next=tmp;
			}
			
		}
	}
	return head;
}


struct ACCOUNT* readState(FILE* file){
	
	struct ACCOUNT* head=NULL;

	char line[1000];

	fgets(line,1000,file);

	while (fgets(line, 1000, file)!=NULL&&line[0]>=33){
		struct ACCOUNT* acc=(struct ACCOUNT*)malloc(sizeof(struct ACCOUNT));
		if (acc==NULL){
			exit(1);
		}
		acc->next=NULL;		
		
		int result=sscanf(line,"%d,%lf,%lf",&acc->accountNumber,&acc->startingBalance,&acc->endingBalance);
		if (!acc->accountNumber>0){
			break;
		}	
		head=addNode(head,acc);
	}
		
	
	fclose(file);
	//traverse(head,"After State:\n");
	return head;
}

void freeList(struct ACCOUNT *head){

	struct ACCOUNT *tmp;
	while (head!=NULL){
		tmp=head;
		head=head->next;
		free(tmp);
	}
}

void readTransaction(FILE* file, struct ACCOUNT* head){

	char line[1000];
	int num;
	char mode;
	double value;

	fgets(line,1000,file);
	while (fgets(line,1000,file)!=NULL&&line[0]>=33){
		int result=sscanf(line,"%d,%c,%lf",&num,&mode,&value);
		struct ACCOUNT* ptr=head;
       		while (ptr->accountNumber != num){
			if (ptr->next==NULL){
                		printf("Account not found (account, mode, amount): %06d %c %.2f\n",num,mode,value);
				ptr=NULL;
				break;
			}
			ptr=ptr->next;
		}
		if (ptr==NULL){
			continue;
		}
		if (mode=='d'){
			ptr->startingBalance=ptr->startingBalance + value;
			
		}else if(mode=='w'){
			double before = ptr->startingBalance;
			ptr->startingBalance=ptr->startingBalance - value;
			if (ptr->startingBalance < 0){
				printf("Balance below zero error (account,mode, transaction, starting balance before): %06d %c %.2f %.2f\n",ptr->accountNumber,mode,value,before);
				ptr->startingBalance=0.00;
			}
		}else{
				printf("Please enter a valid mode.\n");
			}
		
	}	
		fclose(file);
	//	traverse(head,"After Transaction:\n");


}

void finalStats(struct ACCOUNT* head){
	struct ACCOUNT* ptr=head;
	while (ptr!=NULL){
		if (ptr->startingBalance!=ptr->endingBalance){
			printf("End of day balance do not agree (account, starting, ending): %06d %.2f %.2f\n",ptr->accountNumber,ptr->startingBalance,ptr->endingBalance);
		}
		ptr=ptr->next;
	}
}

int main(int argc, char *argv[]){

	if (argc != 3){
		errorMsg("Wrong number of arguments!", 1, NULL);
	}

	//char* file1=argv[1];
	//char* file2=argv[2];
	FILE *state;
	FILE *transaction;
	state = fopen(argv[1],"r");
	if (state == NULL){
	       errorMsg(NULL,2,argv[1]);
	}
	transaction = fopen(argv[2],"r");
	if (transaction == NULL){
	       errorMsg(NULL,2,argv[2]);
	}

	char c1=getc(state);
	char c2=getc(transaction);
	if (c1 < 33 && c2 >=33){
		printf("File %s is empty, Unable to validate %s.",argv[1],argv[2]);
		exit(3);
	}

	struct ACCOUNT *head=readState(state);
	readTransaction(transaction,head);
	finalStats(head);
	freeList(head);

	return 0;
}

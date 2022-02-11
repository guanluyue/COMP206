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
};

struct ACCOUNT* addNode (struct ACCOUNT* head, int num, double sBalance, double eBalance){
        struct ACCOUNT* tmp=(struct ACCOUNT*)malloc(sizeof(struct ACCOUNT));
        if (tmp==NULL){
                return NULL;
        }
        tmp->accountNumber=num;
        tmp->startingBalance=sBalance;
        tmp->endingBalance=eBalance;
        if (head==NULL){
                tmp->next=NULL;
        }else{
                if (tmp->accountNumber < head->accountNumber){
                        tmp->next=head;
                        head=tmp;
                }else if (tmp->accountNumber >= head->accountNumber){
                        struct ACCOUNT* ptr=head;
                        struct ACCOUNT* prev=NULL;
                        while (ptr->accountNumber < tmp->accountNumber){
                                if (ptr->accountNumber == tmp->accountNumber){
                                        printf("Duplicate account number [account, start, end]: %d %f %f", ptr->accountNumber, ptr->startingBalance, ptr->endingBalance);
                                        ptr=ptr->next;
                                        continue;
                                }else{
                                        prev=ptr;
                                        ptr=ptr->next;
                                }
                        }
                        prev->next=tmp;
                        tmp->next=ptr;
                }
        }
        return tmp;

}

void traverse(struct ACCOUNT* head){
        struct ACCOUNT* ptr=head;
        while (ptr->next!=NULL){
                printf("%d %f %f",ptr->accountNumber, ptr->startingBalance, ptr->endingBalance);
        }
}

struct ACCOUNT* readState(FILE *file){

        struct ACCOUNT* head = NULL;
        char line[1000];

        fgets(line,1000,file);
        if (line==NULL){
                printf("Empty state.csv");
                return NULL;
        }
        while (fgets(line, 1000, file)!=NULL){
                struct ACCOUNT *acc;
                acc=(struct ACCOUNT*)malloc(sizeof(struct ACCOUNT));
                if (acc==NULL){
                        exit(1);
                }

                acc->next=NULL;
                int i,j,k=0;
                char num[100];
                char sBalance[100];
                char eBalance[100];


/*              for (i=0; array[i]!=','; i++){
                        num[i]=array[i];
                }
                i=i+1;
                num[i]='\0';
                for (j=0; array[i]!=','; i++, j++){
                        sBalance[j]=array[i];
                }
                i=i+1;
                j=j+1;
                sBalance[j]='\0';
                for (k=0; array[i]!=','; i++, k++){
                        eBalance[k]=array[i];
                }
                k=k+1;
                eBalance[k]='\0';
*/
                int result=sscanf(line, "%[^','], %[^','], %s",num,sBalance,eBalance);
                int accNum=atoi(num);
                double stBalance=atof(sBalance);
                double edBalance=atof(eBalance);

                head=addNode(head,accNum,stBalance,edBalance);
        }
             traverse(head);
        fclose(file);
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
        struct ACCOUNT *head=readState(state);
        readState(state);
        freeList(head);
        return 0;
}
                   
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAIN_MENU "\n< Run, HOBANU!!! >\n1. Game Start\n2. Score\n3. Quit\n>> "
#define RANKING "\n#Ranking\n         Score(Attempts)\nTop       %d(%d)\nLatest    %d(%d)\n"
#define NO_RANKING_RESULTS "#Ranking\n\nNo Ranking Results\n"

int flag;

int main(int argc, char *argv[])
{
    int input;
    pid_t pid;
    FILE* fp = NULL;
    int cur_score; int top_score; int trial;
    
    while(1)
    {
        printf(MAIN_MENU);
        scanf("%d", &input);
        
        switch (input) {
            case 1:
                if ((pid = fork()) < 0)
                {
                    perror("fork error");
                    exit(1);
                }
                else if (pid == 0) // Child
                {
                    system("./game");
                    return 0;
                }
                else{ //Parent
                    wait(&cur_score);
                    
                    fp = fopen("SCORE.txt","wt");
                    if(flag == 0){
                        fprintf(fp,"%d %d\n",cur_score,flag);
                        top_score = cur_score;
                        flag++;
                    }
                    else{
                        if(top_score < cur_score){
                            top_score = cur_score;
                        }
                        flag++;
                        fprintf(fp,"%d %d\n",top_score,flag);
                    }
                    
                    fclose(fp);
                }
                
                break;//case1
                
                
            case 2:
                fp = fopen("SCORE.txt","rt");
                if(fp == NULL){
                    printf(NO_RANKING_RESULTS);
                    break;
                }
                fscanf(fp,"%d %d",&top_score,&trial);
                printf(RANKING,top_score,trial,cur_score,flag);
                fclose(fp);
                break;
            case 3:
                return 0;
            default:
                puts("Enter the number \'1 - 3\'");
                break;
        }
    }//while
}//main


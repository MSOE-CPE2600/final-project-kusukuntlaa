/*
 * Author: Anish Kusukuntla
 * Filename: VotingMachine.c
 * Lab13 - Voting Machine
 * CPE 2600 121
*/
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <string.h>
//Created an enum for what vote would equal
typedef enum votes{
    Zero, One, Two
} votes;
votes vote;
char mail[100];
//Handles the action for the votes
void handle_signal(int signum, siginfo_t *info, void *context){
    (void)(signum);
    (void)(context);
    
    if(info->si_value.sival_int == 1){
        vote = One;
        printf("You voted for Batman\n");
    } else if(info->si_value.sival_int == 2){
        vote = Two;
        printf("You voted for Superman\n");
    } else {
        vote = Zero;
        printf("Your vote is invalid\n");
    }   
    
}
//main would apply the sigaction for recieving the votes. 
//The PID would be printed for the user to use in another terminal to communicate with this one. 
//The voting would keep looping until one of the sides are at 5 votes. 
//The file be updating the score in TheBest.csv and would close at the end just so the file is cleared. 
//The file TheUser.csv would close out once the program ends.
//Would print out who won in votes at the end.  
int main(void){
    struct sigaction action;
    //Register with signal
    action.sa_flags = SA_SIGINFO;
    action.sa_sigaction = handle_signal;
    sigemptyset(&action.sa_mask);
    
    int amount1 = 0;
    int amount2 = 0;
    
    if(sigaction(SIGUSR1, &action, NULL) == -1){
        perror("Trouble with sigaction");
        return -1;
    }
    FILE *file = fopen("TheBest.csv", "w");
    if (!file) {
        fprintf(stderr, "Error: Could not open this file: TheBest.csv\n");
        return 1;
    }
    fprintf(file, "Batman: 0 || Superman: 0\n");
    fclose(file);

    printf("Example Format should be <./StoringData username 1>\n");
    
    while(amount1 < 5 && amount2 < 5){
        printf("Your PID is %d\n", getpid());
        printf("Who do you like better, Batman or Superman?\n");
        
        printf("Send SIGUSR1 with value 1 for Batman or 2 for Superman followed by username.\n");
        
        printf("Enter the PID afterwards\n");
        //pause() would make sure the sigaction is being processed. 
        pause();

        if (vote == One) {
            amount1++;
            
        } else if (vote == Two) {
            amount2++;
            
        } else {
            printf("Invalid vote received. No increment.\n");
        }

        
        file = fopen("TheBest.csv", "a");
        if (!file) {
            fprintf(stderr, "Error: Could not open this file: TheBest.csv\n");
            return 1;
        }
        printf("Batman: %d || Superman: %d\n", amount1, amount2);
        fprintf(file, "Batman: %d || Superman: %d\n", amount1, amount2);
        fclose(file);
        vote = Zero;
        
        
    } 
    FILE *f = fopen("TheUser.csv", "w");
    if (!f) {
        fprintf(stderr, "Error: Could not open this file: TheBest.csv\n");
    }
    if(amount1 == 5){
        printf("Batman has hit 5 votes and most people believe that Batman would be better.\n");
        
    } else if(amount2 == 5){
        printf("Superman has hit 5 votes and most people believe that Superman would be better.\n");
        
    }
    fclose(f);
    
    return 0;

}
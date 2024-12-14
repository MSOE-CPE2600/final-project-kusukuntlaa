/*
 * Author: Anish Kusukuntla
 * Filename: StoringData.c
 * Lab13 - Voting Machine
 * CPE 2600 121
*/

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
//Checks to see if the username has not been used already
int check_username(char *filename, char * username){
    FILE *check = fopen(filename, "r");
    if (!check) {
        fprintf(stderr, "Could not open TheBest.csv\n");
        return 0;
    }
    char name[256];
    char line[100];
    while(fgets(name, sizeof(name), check)){
        
        if (sscanf(name, "The username was %s and the vote was ", line) == 1){
            if(strcmp(line, name) == 0){
                
                fclose(check);
                return 1;
            }
        }
    }
    fclose(check);
    return 0;
}
//main would apply the sigqueue so that it can communicate with VotingMachine by using that PID.
//Would use the command line to recieve the username and their vote whether it is 1 for Batman or 2 for Superman.
//The file would keep appending their username and who they voted to TheUser.csv 
//since the information would still be saved after I do every command line for StoringData.c. 
int main(int argc, char *argv[]){
    if (argc != 3){
        fprintf(stderr, "Incorrect command: %s\n", argv[0]);
        return 1;
    }
    
    
    char *user = argv[1];
    
    int ans = atoi(argv[2]);
    
    if(check_username("TheBest.csv", user)){
        fprintf(stderr, "Error: The username %s has already been used.\n", user);
        return 1;
    }
    pid_t pid;
    printf("Enter the PID of the VotingMachine process: ");
    scanf("%d", &pid);
    union sigval sig;
    sig.sival_int = ans;
    
    

    if(sigqueue(pid, SIGUSR1, sig) == -1){
        perror("Error with the signal");
        return 1;
    }
    
    //Needed to append so that old data would not get lost
    FILE *fp = fopen("TheUser.csv", "a");
    if (!fp) {
        fprintf(stderr, "Error: Could not open this file: TheUser.csv\n");
        return 1;
    }
    
    
    

    if(ans == 1){
        printf("You voted for Batman, your username was %s\n", user);
        fprintf(fp, "The username was %s and the vote was Batman\n", user);
            
    } else if(ans == 2){
        printf("You voted for Superman, your username was %s\n", user);
        fprintf(fp, "The username was %s and the vote was Superman\n", user);
    } else {
        printf("Your vote is invalid\n");
        fprintf(fp, "Your vote is invalid\n");
    }
    fclose(fp);
    
    return 0;
    

}
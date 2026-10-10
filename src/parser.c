#include "parser.h"
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

//Command should have format -> set app/brake to n%

//Space separates words and removing % sign from number strings to read
#define DELIMS " %"

command parseCommand(char*line){
    //Assume cmd is an error before deciding on whether it works or not
    command cmd = {COMMAND_ERROR, 0};

    //Make everything uppercase letters to make parsing much easier
    for(char* c = line; *c != '\0'; c++){
        *c = (char)toupper((unsigned char)*c);
    }

    //Get first one in set
    char* word = strtok(line, DELIMS);
    if(word == NULL){
        cmd.type = COMMAND_NONE;
        return cmd;
    }

    if(strcmp(word, "SET") == 0){
        word = strtok(NULL, DELIMS);
    }

    //APPS or BRAKE
    char* name = word; 
    char* number = strtok(NULL, DELIMS);
    if(number != NULL && strcmp(number, "TO") == 0){
        number = strtok(NULL, DELIMS);
    }

    //Check if either part of command is missing
    if(name == NULL || number == NULL){
        return cmd;
    }

    //Check for extra characters after the number
    if(strtok(NULL, DELIMS) != NULL){
        return cmd;
    }

    //Check that number is maximum 3 digits
    if(strlen(number) > 3){
        return cmd;
    }

    //check if all chars in the number are actually numbers
    for(char* c = number; *c != '\0'; c++){
        if(!isdigit((unsigned char)*c)){
            return cmd;
        }
    }

    //check if the number is in the valid range of 0-100
    int percent = atoi(number);
    if(percent > 100){
        return cmd;
    }

    if(strcmp(name, "APPS") == 0){
        cmd.type = COMMAND_SET_APPS;
        cmd.percent = (uint32_t)percent;
    }else if(strcmp(name, "BRAKE") == 0){
        cmd.type = COMMAND_SET_BRAKE;
        cmd.percent = (uint32_t)percent;
    }
    

    return cmd;
}

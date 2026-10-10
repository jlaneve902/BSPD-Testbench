#ifndef PARSER_H
#define PARSER_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"{
#endif

typedef enum{
    COMMAND_NONE,
    COMMAND_ERROR,
    COMMAND_SET_APPS,
    COMMAND_SET_BRAKE
}commandType;

typedef struct{
    commandType type;
    uint32_t percent;
}command;

command parseCommand(char* line);

#ifdef __cplusplus
}
#endif
#endif
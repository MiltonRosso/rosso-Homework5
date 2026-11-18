#ifndef ROOMMANIP_H
#define ROOMMANIP_H

typedef struct Room Room;

struct Room {
    char name[50];
    char code[3];
    char description[500];
    Room *north, *east, *south, *west;
};

Room* room_create(Room *next_room);

Room* read_room_file(char *str, int *ptr);

#endif
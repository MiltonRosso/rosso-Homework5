#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "StringManip.h"
#include "RoomManip.h"

/*
 * Takes in a pointer to a Room, dymanically allocates memory for a
 * copy of the Room and then returns it.
**/
Room* room_create(Room *room) {

    // Checks to see if our input points at anything.
    if(room == NULL) {
        printf("Error: enter a valid pointer\n");
        return 0;
    }

    // Dynamically allocates room in memory for one Room.
    Room *new_room = malloc(sizeof(Room));

    // Checks to see if memeory was allocated.
    if(new_room == NULL) {
        printf("Error: memory allocation failed.\n");
        return 0;
    }

    // Stores a copy of the original room.
    *new_room = *room;

    // Reset directional pointers.
    new_room->north = NULL;
    new_room->east = NULL;
    new_room->south = NULL;
    new_room->west = NULL;

    return new_room;
}

/*
 * Reads file and creates a dynamically allocated array containing
 * all available rooms.
**/
Room* read_room_file(char *str, int *ptr) {

    // Checks to see if the inputs point to anything.
    if(str == NULL || ptr == NULL) {
        return 0;
    }

    *ptr = 0;

    // Opens a file in read mode.
    FILE *file = fopen(str, "r");

    // Checks to see if the file is empty.
    if(file == NULL) {
        printf("Error opening file.\n");
        return 0;
    }

    // Dynamically allocates memory for rooms.
    int capacity = 2;
    int num_of_rooms = 0;
    Room *rooms = malloc(sizeof(Room) * capacity);

    // Checks to see if rooms points at anything.
    if(rooms == NULL) {
        fclose(file);
        return 0;
    }

    // Temporary variables for creating a room.
    Room temp_room = {0};
    bool has_name = false;
    bool has_code = false;
    bool has_description = false;
    char buffer[1024];

    // Loops through the entire file.
    while(fgets(buffer, sizeof(buffer), file) != NULL) {

        char *value = strchr(buffer, ':');

        // Checks to see if value points to anything.
        if(value == NULL) {
            continue;
        }

        *value = '\0';
        value++;

        // Trims whitespace.
        char *label = str_trim(buffer);
        value = str_trim(value);

        // Checks to see if each of the strings are too long.
        if(strcmp(label, "Room Name") == 0) {
            
            // Checks to see if the room name is too long.
            if(strlen(value) >= sizeof(temp_room.name)) {
                printf("Error: room name too long\n");
                free(rooms);
                fclose(file);
                return 0;
            }

            // Stores the room name.
            strcpy(temp_room.name, value);
            has_name = true;

        } else if(strcmp(label, "Room Code") == 0) {
            
            // Checks to see if the room code is too long.
            if(strlen(value) >= sizeof(temp_room.code)) {
                printf("Error: room code too long\n");
                free(rooms);
                fclose(file);
                return 0;
            }

            // Stores the room code.
            strcpy(temp_room.code, value);
            has_code = true;

        } else if(strcmp(label, "Room Description") == 0) {

            // Checks to see if the room code is too long.
            if(strlen(value) >= sizeof(temp_room.description)) {
                printf("Error: room description too long\n");
                free(rooms);
                fclose(file);
                return 0;
            }

            // Stores the room description.
            strcpy(temp_room.description, value);
            has_description = true;
        }

        // Checks to see if Room contains information.
        if(has_name && has_code && has_description) {

            // Expads array's capacity as needed.
            if(num_of_rooms == capacity) {
                int new_capacity = capacity * 2;
                Room *temp = realloc(rooms, new_capacity * sizeof(Room));

                // Checks to see if memory allocations worked.
                if(temp == NULL) {
                    free(rooms);
                    fclose(file);
                    return 0;
                }

                rooms = temp;
                capacity = new_capacity;
            }

            // Store the new room.
            rooms[num_of_rooms] = temp_room;
            num_of_rooms++;

            // Resets temp_room.
            temp_room = (Room){0};
            has_name = false;
            has_code = false;
            has_description = false;
        }
    }

    // Closes the file.
    fclose(file);

    // Return the number of rooms.
    *ptr = num_of_rooms;

    // Checks to see if the file contained any rooms.
    if(num_of_rooms == 0) {
        free(rooms);
        return 0;
    }

    return rooms;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <errno.h>
#include <limits.h>
#include "RoomManip.h"
#include "StringManip.h"

// Forward declared functions.
Room* create_dungeon(Room *rooms, int room_count, int dungeon_size);
void print_dungeon(Room *start);
void delete_dungeon(Room *start);

/*
 * Main function.
**/
int main(int argc, char **argv) {

    // Checks to see if the user puts in exactly 2 arguments.
    if(argc != 2) {
        printf("Error: you muster enter exactly 2 arguments\n");
        return 0;
    }

    // Read the original room array.
    int num_rooms = 0;

    Room *rooms = read_room_file(argv[1], &num_rooms);

    // Checks to see if rooms were loaded.
    if(rooms == NULL) {
        printf("Unable to load rooms.\n");
        return 0;
    }

    printf("Successfully loaded %d rooms.\n", num_rooms);


    // Ask the user for dungeon size.
    char buffer[100];

    printf("Enter the size of your dungeon: ");

    if(fgets(buffer, sizeof(buffer), stdin) == NULL) {
        free(rooms);
        return 0;
    }

    // Convert input into an integer.
    char *endptr;

    errno = 0;

    long dungeon_size = strtol(buffer, &endptr, 10);

    // Ignore trailing whitespace.
    while(isspace((unsigned char) * endptr)) {
        endptr++;
    }

    // Validate input.
    if(errno != 0 || endptr == buffer || *endptr != '\0' || dungeon_size <= 0 || dungeon_size > INT_MAX) {
        printf("Invalid dungeon size.\n");
        free(rooms);
        return 0;
    }


    // Seed the random number generator.
    srand((unsigned int)time(NULL));


    // Create the dungeon.
    Room *dungeon = create_dungeon(
        rooms,
        num_rooms,
        (int)dungeon_size
    );

    if(dungeon == NULL) {

        printf("Failed to create dungeon.\n");

        free(rooms);

        return 1;
    }


    // Print the dungeon.
    printf("\nYour dungeon:\n");

    print_dungeon(dungeon);


    // Free the dungeon.
    delete_dungeon(dungeon);


    // Free the original room array.
    free(rooms);

    return 0;
}

/*
 * Creates a doubly linked dungeon using randomly selected rooms from
 * the original room array.
**/
Room* create_dungeon(Room *rooms, int room_count, int dungeon_size) {

    // Checks to see if all the inputs were valid.
    if(rooms == NULL || room_count <= 0 || dungeon_size <= 0) {
        return 0;
    }

    Room *start = NULL;
    Room *previous = NULL;

    // Create the requested number of rooms.
    for(int i = 0; i < dungeon_size; i++) {

        // Randomly select an available room.
        int random_index = rand() % room_count;

        // Create a dynamically allocated copy.
        Room *new_room = room_create(&rooms[random_index]);

        // Checks to see if memory was allocated.
        if(new_room == NULL) {
            delete_dungeon(start);
            return 0;
        }

        // First room becomes the beginning of the dungeon.
        if(start == NULL) {
            start = new_room;
        }

        // Connect the previous room to the new room.
        if(previous != NULL) {

            previous->east = new_room;

            new_room->west = previous;
        }

        // Move previous to the new room.
        previous = new_room;
    }

    return start;
}

/*
 * Prints the entire dungeon.
**/
void print_dungeon(Room *start) {

    Room *current = start;

    // Prints all the linked Rooms.
    while (current != NULL) {
        printf("%s:%s\n", current->code, current->name);
        current = current->east;
    }
}

/*
 * Frees all dynamically allocated rooms in the dungeon.
**/
void delete_dungeon(Room *start) {

    Room *current = start;

    while (current != NULL) {

        // Save the next room before freeing current.
        Room *next = current->east;
        free(current);

        current = next;
    }
}
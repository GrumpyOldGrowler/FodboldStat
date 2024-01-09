#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_NAME_LGT 50

typedef struct{
    char fornavn[MAX_NAME_LGT];
    char efternavn[MAX_NAME_LGT];
    char vejnavn[MAX_NAME_LGT];
    int vejnummer;
    int postnummer;
    char bynavn[MAX_NAME_LGT];
} person_t;

void print_person(person_t p); //This function creates a box named p with a size of person_t.
//It is its own copy, of the data type person_t, so I can play around with p, without changing where it came from.

person_t read_people_from_file(FILE *filepointer); //1
//the function is given the location of the file to be used through the pointer.
//the function will read from this file location, and rearrange the data in to a box of size person_t.
//the function will then return this box to the caller. semi colon is decleration, not a call.
//basically, I tell where something is by pointing to it with the pointer and give the location with the data type
//in this instance, the data type is FILE. When the function returns, it will return data the size of the data type
//person_t.
//This entire assignment is basically making smaller boxes within boxes.

int main(void) {
    person_t input_people[100]; //creation of a hundred boxes of input_people, with the size person_t.
    FILE *input_file; //1 decleration of the pointer input_file.

    input_file = fopen("input_test.txt", "r");

    if (input_file == NULL) {
        printf("Could not open file.");
        exit(EXIT_FAILURE);
    }

    person_t people_info = read_people_from_file(input_file); //1. The reason there reads input_file as an argument
    //in the function, is because we have declared input_file as being a pointer to a file location.
    //So when we give our input_file as an, we are effectively giving the returned value of fopen, which is
    //a pointer to a FILE structure. Which, as we have established before, is identical to the arguments in our
    //read_people_from_file function. read_people_from_file then returns a box of size person_t because thats what we
    //told the compiler in the beginning. We then set people_info, a variable of size person_t to be equal
    //to that returned value. We have essentially passed multiple returned values all the way to people_info.

    // fclose(input_file);
    //printf("%s %s %s %d %d %s\n", people_info.fornavn,people_info.efternavn,people_info.vejnavn,people_info.vejnummer,people_info.postnummer,people_info.bynavn);
    print_person(people_info);
    people_info = read_people_from_file(input_file);
    fclose(input_file);
    printf("%s %s %s %d %d %s\n", people_info.fornavn,people_info.efternavn,people_info.vejnavn,people_info.vejnummer,people_info.postnummer,people_info.bynavn);
}

void print_person(person_t p) {
    printf("%s %s %s %d %d %s\n", p.fornavn, p.efternavn, p.vejnavn,
           p.vejnummer, p.postnummer, p.bynavn);
    return;
}


person_t read_people_from_file(FILE *filepointer) {
    person_t people;

    fscanf(filepointer, "%s %[^,]s", people.fornavn,people.efternavn);
    fgetc(filepointer); // skip comma
    fscanf(filepointer, "%s %d", people.vejnavn,&people.vejnummer);
    fgetc(filepointer); // skip comma
    fscanf(filepointer, "%d %[^.]s", &people.postnummer,people.bynavn);
    fgetc(filepointer);  // skip .

    return people;
}
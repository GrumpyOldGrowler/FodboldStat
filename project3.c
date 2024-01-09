#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//create struct with match information
//read from "kampe-2022-2023.txt"file.
//add results from file to struct
#define WEEKDAYSIZE 8
#define DATESIZE 8
#define TIMESIZE 8
#define TEAMNAMESIZE 5
#define NUMBEROFMATCHES 132
#define NUMBEROFTEAMS 12

typedef struct {
    char weekday[WEEKDAYSIZE];
    char date[DATESIZE];
    char time[TIMESIZE];
    char hometeam[TEAMNAMESIZE];
    char guestteam[TEAMNAMESIZE];
    int  homescore;
    int  guestscore;
    int  attendes;

}match_info_t;

typedef struct {
    char teamname[TEAMNAMESIZE];
    int point;
    int goals_scored;
    int goals_against;

}teamscore_info_t;

void read_matchInfo(char *inputfile, match_info_t matchInfo[], int size) {
    FILE *input_file;
    int i;
    input_file = fopen( inputfile, "r");

    if (input_file == NULL) {
        printf("Could not open file.");
        exit(EXIT_FAILURE);
    }
    //printf("hi\n");

    for (i = 0; i < size; i++) {
        fscanf(input_file, "%s %s %s %s", matchInfo[i].weekday,matchInfo[i].date,matchInfo[i].time,matchInfo[i].hometeam);
        // printf("hi igen");
        // printf("%s\n", matchInfo[0].weekday);
        fgetc(input_file); // skip space
        fgetc(input_file); // skip -
        fscanf(input_file, "%s %d", matchInfo[i].guestteam, &matchInfo[i].homescore);
        fgetc(input_file); // skip space
        fgetc(input_file); // skip -
        fscanf(input_file, "%d %d", &matchInfo[i].guestscore,&matchInfo[i].attendes);
        fgetc(input_file);  // skip \n
    }
}


void create_team_index(match_info_t *m, char t[][TEAMNAMESIZE],teamscore_info_t *s, int size){
    int game_counter = 0;
    int team_counter = 0;

    //In the first match, the first two team names are always different from each other, and will always
    //occupy an index of their own. So home team gets the 0 index and the guest team gets the 1 index.
    for (game_counter = 0; game_counter < 6; game_counter++) {
        strncpy(t[team_counter], m[game_counter].hometeam, TEAMNAMESIZE);
        //printf("%s", t[0]);
        strncpy(s[team_counter].teamname, t[team_counter], TEAMNAMESIZE); //Copies the teamname to the score structure.
        s[team_counter].goals_against = 0;
        s[team_counter].goals_scored = 0;
        s[team_counter].point = 0;
        team_counter++;
        strncpy(t[team_counter], m[game_counter].guestteam, TEAMNAMESIZE);
        strncpy(s[team_counter].teamname, t[team_counter], TEAMNAMESIZE); //Copies the teamname to the score structure.
        s[team_counter].goals_against = 0;
        s[team_counter].goals_scored = 0;
        s[team_counter].point = 0;
        team_counter++;
    }
    int i;
    for (i = 0; i < size; i++)
        printf("%i %s \n", i, t[i]);
}

int string_2_team_nr(char *s, char t[NUMBEROFTEAMS][TEAMNAMESIZE], int size){
    int i;
    for(i=0; i < size;i++) {
        if (strncmp(s, t[i], TEAMNAMESIZE) == 0) return(i);
    }
    exit(EXIT_FAILURE);
}


void print_matches(match_info_t p[], int size){
    int i;

    for (i=0; i < size; i++) {
        printf("weekday: %s\n", p[i].weekday);
        printf("Date: %s\n", p[i].date);
        printf("hometeam: %s\n", p[i].hometeam);
        printf("guestteam: %s\n", p[i].guestteam);
        printf("homescore: %d\n", p[i].homescore);
        printf("guestscore: %d\n", p[i].guestscore);
        printf("attendes: %d\n", p[i].attendes);
    }
}


void process_matches(match_info_t *m, teamscore_info_t *ts,char tn[][TEAMNAMESIZE], int size){
    int i, index;
    for(i=0; i < size; i++){
        index = string_2_team_nr(m[i].hometeam, tn, NUMBEROFTEAMS);
        ts[index].goals_scored += m[i].homescore;
        ts[index].goals_against += m[i].guestscore;
        if(m[i].homescore == m[i].guestscore){
            ts[index].point += 1;
            ts[string_2_team_nr(m[i].guestteam, tn, NUMBEROFTEAMS)].point += 1;
        } else if (m[i].homescore > m[i].guestscore){
            ts[index].point += 3;
        } else {
            ts[string_2_team_nr(m[i].guestteam, tn, NUMBEROFTEAMS)].point += 3;
        }

        index = string_2_team_nr(m[i].guestteam, tn, NUMBEROFTEAMS);
        ts[index].goals_scored += m[i].guestscore;
        ts[index].goals_against += m[i].homescore;
    }
}

void print_match_scores( teamscore_info_t *ts, int score_placement[], int size){
    int i;

    printf("team\tPoints\tgoals\tlet in\n");
    for (i = 0; i < size; i++){
        printf("%s\t%i\t%i\t%i \n", ts[score_placement[i]].teamname, ts[score_placement[i]].point,ts[i].goals_scored, ts[score_placement[i]].goals_against);
    }

}


void sort_matches(teamscore_info_t *ts, int score_placement[], int size){
    int i,j;
    int hold;

    for (i = 0; i < size; i++)
        score_placement[i] = i;

    for (i = 0; i < size - 1; i++)
        for (j= i + 1; j < size; j++){
            if (ts[score_placement[i]].point < ts[score_placement[j]].point){
                hold = score_placement[i];
                score_placement[i] = score_placement[j];
                score_placement[j] = hold;
            }
        }
    // for (i = 0; i < size; i++)
    //    printf("%i\n", score_placement[i]);

    // if points are the same, we need to look at which team has the best goal score
    // and bump up that team.
    for (i = 0; i < size - 1; i++){
        if (ts[score_placement[i]].point == ts[score_placement[i+1]].point){
            hold = score_placement[i];
            score_placement[i] = score_placement[i + 1];
            score_placement[i + 1] = hold;
        }
    }

}


int main(void) {
    match_info_t matchInfo[NUMBEROFMATCHES];
    char inputfile[25] = "kampe-2022-2023.txt";
    teamscore_info_t teamscore[NUMBEROFTEAMS];
    //A big box has been created, with space for 12 teams. That is, since we defined NUMBEROFTEAMS as 12
    //that is the size of the array.
    char teamnames[NUMBEROFTEAMS][TEAMNAMESIZE];
    int score_placement[NUMBEROFTEAMS];


    // printf("%u\n",inputfile); //points to the adress of the first element in inputefile
    // printf("%c\n",*inputfile);// The first element of inputfile, NOT the adress
    // printf("%c\n",inputfile[0]);//returns the element of the given index.
    // printf("%c\n",*(inputfile+1));// The second element
    // printf("%u\n",&inputfile[0]);//points to the first element
    // printf("%s\n",inputfile);//prints the string until null element is detected


    read_matchInfo(inputfile, matchInfo, NUMBEROFMATCHES);
    //print_matches(matchInfo, NUMBEROFMATCHES);
    //After reading the match info, then I suppose its time to index the teams with a function?
    create_team_index(matchInfo, teamnames, teamscore, NUMBEROFTEAMS);
    process_matches(matchInfo, teamscore, teamnames, NUMBEROFMATCHES);
    // printf("%i \n", string_2_team_nr("AaB",teamnames,NUMBEROFTEAMS));
    sort_matches(teamscore, score_placement, NUMBEROFTEAMS);
    print_match_scores(teamscore, score_placement, NUMBEROFTEAMS);
}



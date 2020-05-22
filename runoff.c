/*
Current state of play
The program gets stuck in an infinite loop

You should do some debugging on the tabulate function & then see if you can go onto iteration 3 (print_winner)

I need to figure out how the program is checking the preferences

*/


// https://cs50.harvard.edu/x/2020/psets/3/runoff/

/* Include the two lines below for Visual Studio Code */
#include "cs50-2019.h"
#include "cs50-2019.c"
// #include <cs50.h> // <= Include for the CS50 IDE
#include <stdio.h>
// Include string.h for strlen
#include <string.h>

// Define two constants: maximum number of voters and maximum number of candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// Define two-dimensional array preferences
// preferences[i][j] is jth preference for voter i
// In other words, i should be used for votes; j should be used for candidates
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
}
candidate;

// Array of candidates with struct candidate
candidate candidates[MAX_CANDIDATES];

// Declare global variables: numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

// Declare function prototype for own version of string compare
int my_str_cmp(const char *s1, const char *s2);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    // Loop through the candidates given in argv and set the relevant properties using dot notation (namely name, votes, elimated)
    for (int i = 0; i < candidate_count; i++)
    {
        // Assign candidates names according to the argument vectors
        candidates[i].name = argv[i + 1];
        // Initialise the votes of the i'th candidate to zero
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    // Get user input to determine how many voters there are
    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Begin main voting loop
    // Keep querying for votes. i refers to the number of voters
    for (int i = 0; i < voter_count; i++)
    {
        // Query for each rank. The number of positions that the voter has to vote for is the same as the number of candidates
        // j referes to the number of candidates
        for (int j = 0; j < candidate_count; j++)
        {
            /* Edit - 29/03/20. I cannot get get_string to compile on Visual Studio Code so am using fgets instead
            Therefore commenting out the line below
            string name = get_string("Rank %i: ", j + 1);
            */
            // string name = get_string("Rank %i: ", j + 1);
            /* --- Request name from user & store using fgets --- */
                
            char name_pointer[100];
            printf("Rank %i: for voter #%i ", j + 1, i);
            fgets(name_pointer, sizeof(name_pointer) - 1, stdin);
            int len = strlen(name_pointer) - 1;
            // printf("length of array of chars from fgets= %d\n", len);

            /*
            Parse the input from fgets until the terminating characters are detected.
            Note1: when the user input is less than the array size, a newline will get stored.
            Note2: when the user input is greater than the array size, it is protected from overflow, but a newline will not get stored
            Ref: https://cboard.cprogramming.com/c-programming/151288-scanning-new-line.html
            */
            // As I am using fgets(), there is only one position for the newline (strlen() -1). It can't be located anywhere else. Therefore, the parsing you need to do is to check if the character at the position of array[length] is equal to the newline character. If it is equal to the newline character, then you should replace that with a null character to close off the string.

            if (name_pointer[len] == '\n')
            {
                name_pointer[len] = '\0';
            }

            // Record vote, unless it's invalid
            // Edit (29/03/20): replace name with name_pointer
            if (!vote(i, j, name_pointer))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }
        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates. This function should look at all the voter's preferences and compute the current vote totals
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            // If election has been won, break the while loop
            break;
        }

        // Eliminate last-place candidates
        // Declare variable (integer) which holds the number of votes given to the candidate who has received the fewest votes
        int min = find_min();
        // printf("min value is %d\n", min);
        // Declare variable (boolean) which holds the value of whether there has been a tie or not
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                // If the candidate hasn't been eliminated, print his/her name
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // If there is not a tie, then eliminate anyone with the minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
/* Instructions:
The vote function takes arguments 'voter', 'rank', and 'name'. If 'name' is a match for the name of a valid candidate, then you should update the global preferences array to indicate that the voter 'voter' has that candidate as their rank preference (where 0 is the first preference, 1 is the second preference, etc.).
If the preference is successfully recorded, the function should return true; the function should return false otherwise (if, for instance, 'name' is not the name of one of the candidates).
You may assume that no two candidates will have the same name. */

// Function returns true or false; false indicates that the voter has given a name that is not valid

// preferences[i][j] is jth preference for voter i

// voter = i
// preference / rank = j

bool vote(int voter, int rank, string name)
{
    // If the 'name' that you are voting for is equal to a valid candidate, then you should update the voter count for that candidate

    // First loop (loop through the candidates)
    for (int j = 0; j < candidate_count; j++)
    {
        // printf("this is candidate %d's name: %s\n", j, candidates[j].name);
        // printf("this is the name of the candidate that voter i voted for: %s\n", name);
        /* --- start debugging --- *//*
        int result_of_strcmp;
        result_of_strcmp = my_str_cmp(candidates[j].name, name);
        printf("result of string comp: %d\n", result_of_strcmp);
        // https://www.tutorialspoint.com/c_standard_library/c_function_strcmp.html
        *//* --- end debugging -- */
        if (my_str_cmp(candidates[j].name, name) == 0)
        
        {
            // Conditional to check if the i'th candidate is equal to the name that the voter gave. Remember, the votes do not have names. They are numbers (integers).
            // If the name is equal to a candidate, do two things
            // (1) candidates[i]
            // (2) Update the global 2d preferences array to indidate that voter i's j'th preference is the name of the candidate
            preferences[voter][rank] = j;
            // printf("Index of Voter: %d | Rank: %d | Index of Candidate i: %d | Name of Candidate: %s \n", voter, rank, j, candidates[j].name);
            // Once those two actions are done, you should return true
            return true;
        }
    }
    // TODO
    return false;
}

// Tabulate votes for non-eliminated candidates
/*
The function should update the number of votes each candidate has at this stage in the runoff.
Recall that at each stage in the runoff, every voter effectively votes for their top-preferred candidate who has not already been eliminated.
*/


/*
Debugging the tabulate formula
(1) You want to find a way where the vote candidate of a candidate is increased if that candidate happens to be the first choice of a voter
(2) Add an if function that performs a check on the said candidate to make sure that they are not eliminated
    (i) If the candidate is eliminated, then the second choice candidate should receive a vote
*/
void tabulate(void)
{
    // Loop through the voters' preferences using a length of the constant number of voters. voter = i
    for (int i = 0; i < voter_count; i++)
    {
        // printf("Start looping through the votes for voter %d\n", i);
        // k refers to the preference / rank. k = 0 is the voter's first choice. k = 1 is the voter's second choice
        for (int k = 0; k < candidate_count; k++)
        {
            int index = preferences[i][k];
            
            // printf("inner loop for voter #'%d' looking at preference %d. That preference (index) is: %d (aka %s) \n", i, k, preferences[i][k], candidates[preferences[i][k]].name);
            // printf("therefore we want to find where %d = %d\n", k, preferences[i][k]);
            // printf("inside inner loop | checking for preferences [i:%d] [j:%d]\n", i, j);
            // preferences[i][j] stores the index of the candidate who is the jth preference for voter i
            if (!candidates[index].eliminated)
            {
                // If the first choice vote is equal to the name of the candidate, then increase the number of votes that the candidate has received
                candidates[preferences[i][k]].votes++;
                // printf("increased votes of candidate '%s' to %d votes\n", candidates[preferences[i][k]].name, candidates[preferences[i][k]].votes);
                break;
                // break;
            }
            /* Don't think an else statement is required here thus commenting it out */
            // else
            {
                printf("Candidate has been eliminated\n");
                // break;
            }
        }

        // printf("inner loop #%d of tabulate finished\n", i);
        // break;

        
    }
    // return void. This function is just to tabulate
    /* Before returning, let's print the vote totals */

    /*
    printf("Below are the votes totals\n");
    for (int z = 0; z < candidate_count; z++)
    {
        printf("Name: %s; Votes: %d\n", candidates[z].name, candidates[z].votes);
    }
    */

    return;
}

// Print the winner of the election, if there is one
/*
If any candidate has more than half of the vote, their name should be printed to stdout and the function should return true.
If nobody has won the election yet, the function should return false.
*/
bool print_winner(void)
{
    // Loop through the candidates. If any candidate has more than half of the vote, print their name & return true
    for (int j = 0 ; j < candidate_count; j++) 
    {
        // printf("voter count is: %d | j is %d | candidate count is %d\n", voter_count, j, candidate_count);
        // printf("candidate name is: '%s', candidate vote(s) are: %d\n", candidates[j].name, candidates[j].votes);

        if (candidates[j].votes > (voter_count / 2))
        {
            // printf("The winner is '%s' (index: %d) with %d votes\n", candidates[j].name, j, candidates[j].votes);
            printf("%s\n", candidates[j].name);
            return true;
        }
    }

    // TODO
    return false;
}

// Return the minimum number of votes any remaining candidate has
// Loop through the candidates and return the number for the minumum
int find_min(void)
{
    int min;

    // Assign min a value that cannot be achieved. Ideally, I would assign it to 'null' but apparently this is not possible in C (https://www.quora.com/How-can-we-assign-null-value-to-an-integer-variable-in-C?share=1)
    min = 1000;

    // Loop through the candidates and assign the number of votes that candidate 0 received to the min value.
    // If there is a candidate who has received fewer votes, then the min value should be updated to reflect that number of votes
    
    for (int i = 0; i < candidate_count; i++)
    {
        if (!candidates[i].eliminated)
        {
            // Check if the number of votes is still the same as the initial. Note, this is a hack. There should be a better way of doing this.
            if (min == 1000) 
            {
                min = candidates[i].votes;
            } 
            else if (candidates[i].votes < min)
            {
                min = candidates[i].votes;
            } 
            else
            {
                printf("min does not need updating as candidate %d's votes is/are greater than the min of %d\n", i, min);
            }
        }
    }

    // Return the min value
    printf("returning a value of %d for min\n", min);
    return min;
}

// Return true if the election is tied between all candidates, false otherwise
/*
The function takes an argument min, which will be the minimum number of votes that anyone in the election currently has.
*/
bool is_tie(int min)
{
    // Declare variable (integer) which counts the number of candidates remaining
    int candidates_remaining, candidates_with_min_votes;
    candidates_remaining = 0;
    candidates_with_min_votes = 0;

    for (int i = 0; i < candidate_count; i++)
    {
        // printf("bool value for candidate with index %d: %d\n", i, candidates[i].eliminated);

        // If the candidate not yet has been eliminated, then increase the counter of candidates remaining
        if (!candidates[i].eliminated)
        {
            candidates_remaining++;
        }
        // If the number of votes the candidate has is equal to the minimum number & the candidate has not yet been eliminated, then increase the counter by one
        if (candidates[i].votes == min && !candidates[i].eliminated)
        {
            // printf("increasing candidates with min votes to %d\n", candidates_with_min_votes);
            candidates_with_min_votes++;
        }
    }

    // Return true if every candidate remaining in the election has the same number of votes, and return false otherwise.
    if (candidates_remaining == candidates_with_min_votes)
    {
        printf("It is a tie: remaining candidates: %d vs min votes: %d\n", candidates_remaining, candidates_with_min_votes);
        return true;
    }   
    else
    {
        printf("It is a not a tie: remaining candidates: %d vs min votes: %d", candidates_remaining, candidates_with_min_votes);
        return false;
    }
}

// Eliminate the candidate (or candidiates) in last place
void eliminate(int min)
{

    // Loop through candidates and see if the i'th candidate has the same number of votes as the variable 'min'
    for (int i = 0; i < candidate_count; i++)
    {  
        // If the candidate has the minimum amounts of votes, then elimate that candidate by assigning a truthy value to the bool eliminated
        if (candidates[i].votes == min)
        {
            candidates[i].eliminated = true;
            printf("candidate '%s' has been eliminated\n becuase they had the minimum (%d) number of votes\n", candidates[i].name, min);
        }
    }
    // TODO
    return;
}

// Create own version of strcmp so that I can troubleshoot issues I had with comparing strings.
int my_str_cmp(const char *s1, const char *s2) 
{
    const unsigned char *p1 = (const unsigned char *)s1;
    const unsigned char *p2 = (const unsigned char *)s2;

    /* 
    Debugging to see the lengths. Turns out that fgets was adding a newline to the input taken from the human. This \n character needed to be removed (By pasing it with a conditional parse)
    int len_s1, len_s2;

    len_s1 = strlen(s1);
    len_s2 = strlen(s2);

     
    printf("length of s1 [%s] is %d\n", s1, len_s1);
    printf("length of s2 [%s] is %d\n", s2, len_s2);
    */

    // Each character is compared in turn and a number is returned based on whether ther first of second string is greater
    // Only if the characters are identical do you move to the next character
    while (*p1 != '\0') 
    {
        if (*p2 == '\0') 
        {
            return  1;
        }

        if (*p2 > *p1)
        {
            return -1;
        }

        if (*p1 > *p2)
        {   
            return  1;
        }

        p1++;
        p2++;
    }

    if (*p2 != '\0')
    {
        return -1;
    }

    // If all the characters are identical, return 0 to indicate success
    return 0;
}

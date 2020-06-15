/*Anh Tuan Pham
*30092540
*Clarification: This is the game with the bonus. I want to clarify some value before you go into my code:
*The bonus time if the player get the time packet is 10s
*There is only 1 of each type of suprised
*Because you want to use shift so the score would be inaccrurated. It would be 0.1 or 0.2 different
*I use time_t which is a Unix library to compare real time. I'm not sure if every computer has it
*Because negative value is "randomly" generated so I don't make any condition to restrict the case that
*there is no negative value. There's possibility that there will be a board with no negative value
*I sort the high score accordingly to the score. If it has the same score, it will display the player who ever
*play first (since there is no requirement to sort others)
*I checked the lenght of the player's name string and restrict it to below 16 and it can't be all number.
*When you have opened all the board, you need to input 1 last time in order for the program to recheck if
*there is any empty place left.
* You can exit the program anytime by enterting 69 in either col or row input.
*
*
*Glhf! 
*
*/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
/*
 *Function: randomNumber 
 *-----------------------------------------------------------------------------------------------------------------------
 * Random number from range m to n
 * initialize good supprise is 100.00 and bad suprise is 100.01
 * check if the take can still take negative value
 * only generate 1 good suprise and 1 bad supris
 * only generate at most 20% of negative value
 * 
 * int m,n: upper bounds and lower bounds of the number
 * bool neg: check if the board has below 20% of negative value where true is still generating negative.
 * 
 * return: score2
 */

float randomNumber(int m, int n, bool neg)
{
 
    int random_score = 1+rand()&(m-n);
    int random_value = rand() % abs(4);
    int score1;
    float score2;
    if (neg == true)
    {
        if (random_value == 1)
        {
            score1 = random_score;
            score2 = -score1 / 100.00;
        }
        else
        {
            score1 = random_score;
            score2 = score1 / 100.00;
        }
    }
    else if (neg == false)
    {
        score1 = random_score;
        score2 = score1 / 100.0;
    }
    return score2;
}

/*
 *Function: chooseGoodSup
 *-----------------------------------------------------------------------------------------------------------------------
 * random the position of good suprise
 * then overwrite the value with 100.00
 * 
 * int N: The size of the board
 * float *baseMapAddress: the numberic value of the score on the board
 * 
 * return: position
 */

int chooseGoodSup(int N, float *baseMapAddress)
{
    int randomCol = rand() % abs(N);
    int randomRow = rand() % abs(N);
    int position = randomCol + N * randomRow;
    *(baseMapAddress + position) = 100.00;
    return position;
}
/*
 *Function: chooseBadSup
 *-----------------------------------------------------------------------------------------------------------------------
 * random the position of bad suprise
 * then overwrite the value with 101.00
 * using recursion to check if the generated position is overlapped with the good suprise
 * 
 * int N: The size of the board
 * float *baseMapAddress: the numberic value of the score on the board
 * int takePos: The position of good sup
 * 
 * return: position
 */

int chooseBadSup(int N, float *baseMapAddress, int takePos)
{
    int randomCol = rand() % abs(N);
    int randomRow = rand() % abs(N);
    int position = randomCol + N * randomRow;
    if (takePos != position)
    {
        *(baseMapAddress + position) = 101.00;
    }
    else
    {
        chooseBadSup(N, baseMapAddress, takePos);
    }
    return position;
}

int timeBonus(int N, float *baseMapAddress, int takePos1, int takePos2)
{
    int randomCol = rand() % abs(N);
    int randomRow = rand() % abs(N);
    int position = randomCol + N * randomRow;
    if (takePos1 != position || takePos2 != position)
    {
        *(baseMapAddress + position) = 102.00;
    }
    else
    {
        timeBonus(N, baseMapAddress, takePos1, takePos2);
    }
    return position;
}

/*
 *Function: init
 *-----------------------------------------------------------------------------------------------------------------------
 * initialize the numberic value of the board
 * check the value of the position then overwrite it with X to appear to the player
 * check the number of negative value then pass the boolean to randomNumber function
 * check the number of goodsup and badsup then pass the counter into the randomNum function
 * overwrite the goodsup and badsup with chooseGoodSup and chooseBadSup function
 * 
 * int N: The size of the board
 * char *board1: The X board
 * float *board2: the numberic board 
 * 
 * return: nothing
 */

void init(char *board1, float *board2, int N)
{
    srand(time(NULL));
    bool neg = true;
    int counter = 0;
    for (int position = 0; position < (N * N); position++)
    {
        *(board1 + position) = 'X';
    }
    for (int position = 0; position < (N * N); position++)
    {
        *(board2 + position) = randomNumber(1500, 1, neg);

        if ((*(board2 + position)) < 0)
        {
            counter += 1;
        }
        if (counter >= (N * N) / 5)
        {
            neg = false;
        }
    }
    int takeposition1 = chooseGoodSup(N, board2);
    int takenposition2 = chooseBadSup(N, board2, takeposition1);
    timeBonus(N, board2, takeposition1, takenposition2);
}

/*
 *Function: display
 *-----------------------------------------------------------------------------------------------------------------------
 * Show the board to the player
 * Display the score on the terminal
 * 
 * int N: The size of the board
 * char *board1: The X board
 * float *board2: the numberic board 
 * float score: current score of the player
 * 
 * return: nothing
 */
void display(int N, char *board1, float *board2, float score)
{

    for (int row = 0; row < N; row++)
    {
        for (int col = 0; col < N; col++)
        {
            printf("%c ", *(board1 + row * N + col));
            printf("   ");
        }
        printf("\n");
    }
    printf("You score is %.2f \n", score);
}

/*
 *Function: displaytopscore
 *-----------------------------------------------------------------------------------------------------------------------
 * Show the top score of the player if the choose yes to the player
 * 
 * int top: The top n score player
 * char *rank[]: the rank of the player
 * 
 * return: nothing
 */
void discplaytopscore(int top, char *rank[])
{
    for (int i = 0; i < (top); i++)
    {
        printf("%s", rank[i]);
        printf("\n");
    }
}

/*
 *Function: exitgame
 *-----------------------------------------------------------------------------------------------------------------------
 * print the game is over then exit the game
 * 
 * return: nothing
 */
void exitgame()
{
    printf("The game is over. Existing!");
    exit(0);
}

/*
 *Function:logscore
 *-----------------------------------------------------------------------------------------------------------------------
 * save the score in the log file
 * then if the player choose to show the score, use the boolean to open the file then read it, then sort it accordingly to
 * the score of the player. It will show Score, name, and time.
 * 
 *float totalscore: score of the player
 *char *name : name of the player
 *bool read: determine if it's read or write to file
 *int n: the number the player enter to show the sorted file
 *int time: time of the player
 * 
 * return: nothing
 */
void logscore(float totalscore, char *name, bool read, int n, time_t t)
{
    if (read == false)
    {
        FILE *fp;

        fp = fopen("highscore.log", "a");
        fprintf(fp, " %.2f, %s, %ld", totalscore, name, t);
        fprintf(fp, "\n");
        fclose(fp);
    }
    if (read == true)
    {
        char ch;
        FILE *fp;
        int max_len = 100;
        char strTempData[max_len];
        char **strData = NULL;
        int num_of_lines = 0;
        fp = fopen("highscore.log", "r");

        while (fgets(strTempData, max_len, fp) != NULL)
        {
            // Remove the trailing newline character
            if (strchr(strTempData, '\n'))
                strTempData[strlen(strTempData) - 1] = '\0';
            strData = (char **)realloc(strData, sizeof(char **) * (num_of_lines + 1));
            strData[num_of_lines] = (char *)calloc(max_len, sizeof(char));
            strcpy(strData[num_of_lines], strTempData);
            num_of_lines++;
        }

        char *temp;
        for (int i = 0; i < (num_of_lines - 1); ++i)
        {
            for (int j = 0; j < (num_of_lines - i - 1); ++j)
            {
                float J = strtof(strData[j], NULL);
                float Jplus1 = strtof(strData[j + 1], NULL);

                if (J < Jplus1)
                {
                    temp = strData[j];
                    strData[j] = strData[j + 1];
                    strData[j + 1] = temp;
                }
            }
        }
        printf("This is the top %d high score:", n);
        printf("\n");
        discplaytopscore(n, strData);
        fclose(fp);
    }
}

/*
 *Function: Calculate the score
 *-----------------------------------------------------------------------------------------------------------------------
 * This is the game loop where it updates the move, calculate the score and time
 * Use time_t which is a library to update time for unix to update tike
 * check the play condition such as the available move, exit condition,etc
 * if the board is full, the player has to enter 1 more time in order to check if there is any other empty position
 * 
 * int N: size of the board
 * char *board1: X board
 * float *board2: the numberic board
 * char *name: name of the player
 * 
 * return: nothing
 */
void calculatescore(int N, char *board1, float *board2, char *name)
{
    int col;
    int row;
    int input[N * N + 1];
    int counter = 0;
    int mov_counter = 0;
    float score = 0.00;
    char option;
    time_t begin, currenttime;
    time_t time_spent = 0;
    long int used_time;
    unsigned int sec;
    int limit_time = 60;
    int counter_negative=0;

    if (N > 10)
    {
        limit_time = 90;
    }
    else if (N > 15)
    {
        limit_time = 120;
    }

    printf("Enter your col: ");
    begin = time(NULL);
    while (scanf("%d", &col) == 1 && printf("Enter your row: ") && scanf("%d", &row) == 1 && (col != 69 && row != 69))
    {   
        counter_negative++;
        if (counter_negative >1 && score<0){printf("Your score is less than 0. You lost!");break;}
        printf("Your limit time is %d \n", limit_time);
        if (time_spent >= limit_time)
        {
            printf("You run out of time\n");
            break;
        }
        if (*(board1 + row * N + col) != 'X')
        {
            if (mov_counter >= N * N)
            {
                break;
            }
            else
            {
                printf("\nInvalid Input. Please try again!!! \n");
                printf("Enter you col: ");
                continue;
            }
        }
        if (col >= 0 && col < N & row >= 0 & row < N)
        {
            mov_counter++;
            *(board1 + row * N + col) = *(board2 + row * N + col);
            if (*(board2 + row * N + col) == 100.00)
            {
                *(board1 + row * N + col) = '$';
                int temp = score * 100.00;
                score = (temp << 1) / 100.00;

                printf("\n");
                printf("You got double score");
                printf("\n");
            }
            else if (*(board2 + row * N + col) == 101.00)
            {
                *(board1 + row * N + col) = '!';
                int temp = score * 100.00;

                score = (temp >> 1) / 100.00;
                printf("\n");
                printf("You lost half of your score");
                printf("\n");
            }
            else if (*(board2 + row * N + col) == 102.00)
            {
                *(board1 + row * N + col) = '@';
                limit_time = limit_time + 10;
                printf("\n");
                printf("You have extra 10 seconds to play");
                printf("\n");
            }
            else
            {
                score += *(board2 + row * N + col);
                if (*(board2 + row * N + col) >= 0)
                {
                    *(board1 + row * N + col) = '+';
                    printf("\n");
                    printf("Uncovered a reward of %.2f", *(board2 + row * N + col));
                    printf("\n");
                }
                else
                {
                    *(board1 + row * N + col) = '-';
                    printf("\n");
                    printf("Bang!! You lost  %.2f", *(board2 + row * N + col));
                    printf("\n");
                }
            }

            display(N, board1, board2, score);
        }
        else
        {
            printf("Invalid input. Please try again");
        }
        currenttime = time(NULL);
        time_spent = (double)(currenttime - begin);
        printf("you have spent %ld second\n", time_spent);

        printf("Enter you col: ");
    }
    logscore(score, name, false, 0, time_spent);
    while (1)

    {
        printf("Game is over \n");
        printf("Do you want to see the see the high score? (Y/N) (Case sensitive) ");
        scanf("%s", &option);
        if (option == 'Y')
        {
            int n;
            printf("Enter the number of top player you want to display: ");
            scanf("%d", &n);
            logscore(score, name, true, n, time_spent);
            exitgame();
        }
        else if (option == 'N')
        {
            exitgame();
        }
        else
        {
            printf("Invalid command. Please try again!");
        }
    }
}

/*
 *Function: isNumber
 *-----------------------------------------------------------------------------------------------------------------------
 * check if the argument is a number or not
 * 
 *char argument[]: the argument passed from main
 * 
 * return: nothing
 */
bool isNumber(char argument[])
{
    int i = 0;

    if (argument[0] == '-')
        i = 1;

    for (; argument[i] != 0; i++)
    {
        //if (argument[i] > '9' || number[i] < '0')
        if (!isdigit(argument[i]))
            return false;
    }
    return true;
}

/*
 *Function: my_strlen
 *-----------------------------------------------------------------------------------------------------------------------
 * check the lenght of the argument
 * 
 * const char *argument: the argument
 * 
 * return: nothing
 */
size_t my_strlen(const char *argument)
{
    const char *p = argument;
    //when argument is not empty
    while (*p != '\0')
        ++p;
    return p - argument;
}

/*
 *Function: main
 *-----------------------------------------------------------------------------------------------------------------------
 * check if there are 3 arguments, second argument is number, the name lenght is less than 16, and if the name is all number
 * check if 5<=N<=20
 * start the game
 * 
 * int argc: number of argument
 * char **argv: argument
 * 
 * return: nothing
 */

void main(int argc, char **argv)
{
    if (argc == 3 && isNumber(argv[2]) && (int)strlen(argv[1]) <= 16 && !isNumber(argv[1]))
    {
        int N = atoi(argv[2]);
        if (N >= 5 && N <= 20)
        {
            bool functions;
            char *name = argv[1];
            if (N > 0)
            {
                char array1[N][N];
                char *board1 = &array1[0][0];
                float array2[N][N];
                float *board2 = &array2[0][0];
                init(board1, board2, N);
                display(N, board1, board2, 0.00);
                calculatescore(N, board1, board2, name);
            }
        }
        else{
            printf("Invalid N");
        }
    }
    else
    {
        printf("Invalid argument");
    }
}

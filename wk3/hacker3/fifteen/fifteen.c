/**
 * fifteen.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 5
#define PTR_MAX 1048576 // TODO: wat
#define SEARCH_MAX 65536 // TODO: how big?

// board
int brd[DIM_MAX][DIM_MAX];

// dimensions
int d;

// prototypes
void clear(void);
void greet(void);
void init(void);
void base_brd(void);
void randomize_brd(void);
void draw(void);
bool move(int tile);
int find(int tile);
bool neighbours(int pos_one, int pos_two);
void swap(int pos_one, int pos_two);
bool won(void);
void god(void);
int search(int winning_indices[]);
//bool position_considered(int*** board_positions, int size);
//bool position_considered(int* board_positions[][], int size);
//bool position_considered(int board_positions[][DIM_MAX][DIM_MAX], int size);
bool position_considered(int board_positions[], int size);
int position_cost(void);
//void add_position_data(bool explored[], int tiles[], int tile, int costs[], int cost, int parents[], int parent, int*** board_positions, int size);
//void add_position_data(bool explored[], int tiles[], int tile, int costs[], int cost, int parents[], int parent, int board_positions[][DIM_MAX][DIM_MAX], int size);
void add_position_data(bool explored[], int tiles[], int tile, int costs[], int cost, int parents[], int parent, int board_positions[], int size);
void populate_valid_tiles(int tiles[], int zero_pos);
//int parent_and_brd_to_lowest_cost(bool explored[], int costs[], int*** board_positions, int size);
//int parent_and_brd_to_lowest_cost(bool explored[], int costs[], int board_positions[][DIM_MAX][DIM_MAX], int size);
int parent_and_brd_to_lowest_cost(bool explored[], int costs[], int* board_positions, int size);

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    greet();
    init();

    while (true)
    {
        clear();
        draw();

        if (won())
        {
            printf("ftw!\n");
            break;
        }

        printf("Tile to move: ");
        char* tile = GetString();

        if (strcmp(tile, "GOD") == 0)
        {
            god();
        }
        else if (!move(atoi(tile)))
        {
            printf("\nIllegal move.\n");
            usleep(200000);
        }

        usleep(200000);
    }

    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(400000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */
void init(void)
{
    base_brd();
    randomize_brd();
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * in reverse position.
 */
void base_brd(void)
{
    int tile = d * d - 1;
    bool odd = tile % 2;
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            brd[i][j] = tile--;
        }
    }
    if (odd)
    {
        brd[d - 1][d - 3] = 1;
        brd[d - 1][d - 2] = 2;
    }
}

/**
 * Randomly moves board tiles a random number of times to create a pseudo-
 * random positition that is knowably reachable from it's starting position.
 */
void randomize_brd(void)
{
    // use srand something so this is actually more random
    srand((int) time(NULL) % 32768);
    int move_count = rand() % (d * 10) + (d * 10);
    int last_move = 0;
    for (int i = 0; i < move_count; i++)
    {
        int valid_moves[4] = { 0 };
        int zero_pos = find(0);
        int move_ctr = 0;
        for (int j = 1; j < d * d - 1; j++)
        {
            if (neighbours(zero_pos, find(j)) && j != last_move)
            {
                valid_moves[move_ctr] = j;
                move_ctr++;
            }
        }
        last_move = valid_moves[rand() % move_ctr];
        move(last_move);
    }
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    char first, second, between;
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            first = (brd[i][j] < 10) ? ' ' : (char) (brd[i][j] / 10 + 48);
            second = (brd[i][j]) ? (char) (brd[i][j] % 10 + 48) : '_';
            between = (j < d - 1) ? '|' : '\0';
            printf(" %c%c %c", first, second, between);
        }
        printf("\n");
    }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    int tile_pos = find(tile);
    if (tile_pos == -1 || tile == 0)
    {
        printf("someone is playing silly buggers\n");
        return false;
    }
    
    int blank_pos = find(0);
    
    if (neighbours(tile_pos, blank_pos))
    {
        swap(tile_pos, blank_pos);
        return true;
    }
    return false;
}

/**
 * Find the position (1D unwound array index) of a tile and return it, or if
 * the tile is not present, return -1.
 */
int find(int tile)
{
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            if (brd[i][j] == tile)
            {
                return i * d + j;
            }
        }
    }
    return -1;
}

/**
 * Returns true if the unwound 1D positions neighbour, else false.
 */
bool neighbours(int pos_one, int pos_two)
{
    return (pos_two == pos_one - d) || 
           (pos_two == pos_one + 1 && pos_two % d != 0) || 
           (pos_two == pos_one - 1 && pos_one % d != 0) || 
           (pos_two == pos_one + d);
}

/**
 * Swaps the tiles at the two unwound 1D positions.
 */
void swap(int pos_one, int pos_two)
{
    int tmp = brd[pos_one / d][pos_one % d];
    brd[pos_one / d][pos_one % d] = brd[pos_two / d][pos_two % d];
    brd[pos_two / d][pos_two % d] = tmp;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            if (brd[i][j] != i * d + j + 1 && brd[i][j] != 0)
            {
                return false;
            }
        }
    }
    return true;
}

/**
 * ok so the N-puzzle conveniently provides a taxicab distance sum heuristic
 * (for misplaced tiles) that is "admissible", or never overestimates the cost
 * with respect to remaining distance to a solution.
 * 
 * This ensures optimality for the A* search algorithm, which I will
 * attempt to implement here without reading any code description.
 * 
 * PS: Don't worry, I had no idea what any of that meant before I read the
 * wikipedia page either.
 */
void god(void)
{
    int winning_moves[4096];
    int starting_position[d][d];
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            starting_position[i][j] = brd[i][j];
        }
    }
    printf("commencing search\n");
    int win_c = search(winning_moves);
    printf("search complete\n");
    for (int i = 0; i < win_c; i++)
    {
        printf("%d\n", winning_moves[i]);
    }
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            brd[i][j] = starting_position[i][j];
        }
    }
    printf("holding pattern\n");
    GetString();
}

int search(int winning_moves[])
{
    printf("setting up search vars\n");
    //int tiles[SEARCH_MAX];
    //int costs[SEARCH_MAX];
    //int parents[SEARCH_MAX];
    //bool explored[SEARCH_MAX];
    //int board_positions[SEARCH_MAX][DIM_MAX][DIM_MAX];
    int* tiles = malloc(PTR_MAX * sizeof(int));
    int* costs = malloc(PTR_MAX * sizeof(int));
    int* parents = malloc(PTR_MAX * sizeof(int));
    bool* explored = malloc(PTR_MAX * sizeof(int));
    int* board_positions = malloc(PTR_MAX * DIM_MAX * DIM_MAX * sizeof(int));
    
    int size = 0;
    int parent = -1;
    int won = 0;
    printf("set up\n");
    
    while (true)
    {
        // for the current position of the board, find possible moves
        int zero_pos = find(0);
        int local_tiles[4];
        populate_valid_tiles(local_tiles, zero_pos);
        
        // for each move
        for (int i = 0; i < 4; i++)
        {
            // if it's a valid game move
            if (local_tiles[i] != -1)
            {
                // pop the board into that position
                swap(zero_pos, local_tiles[i]);
                // check if the board position has been seen before
                if (position_considered(board_positions, size))
                {
                    //printf("board already considered\n");
                    // just pop back if so
                    swap(zero_pos, local_tiles[i]);
                }
                else
                {
                    // check for search win
                    if (position_cost() == 0)
                    {
                        won = size;
                    }
                    //printf("adding board position %d\n", size);
                    // otherwise, add the board position, its parent idx, costs and tile
                    add_position_data(explored, tiles, local_tiles[i], costs, position_cost(), parents, parent, board_positions, size);
                    size++;
                    // then swap back
                    swap(zero_pos, local_tiles[i]);
                }
            }
        }
        
        if (won)
        {
            break;
        }
        
        // find the best available move that isn't already a parent
        // move the board and parent to that position
        parent = parent_and_brd_to_lowest_cost(explored, costs, board_positions, size);
    }
    
    printf("populating winning moves\n");
    // trace the moves back from won
    int win_c = 0;
    printf("won is: %d\n", won);
    while (won > 0)
    {
        printf("adding winning move\n");
        winning_moves[win_c] = tiles[won];
        won = parents[won];
        
        win_c++;
    }
    
    // TODO: free board positions?
    return win_c;
}

//void add_position_data(bool explored[], int tiles[], int tile, int costs[], int cost, int parents[], int parent, int*** board_positions, int size)
//void add_position_data(bool explored[], int tiles[], int tile, int costs[], int cost, int parents[], int parent, int board_positions[][DIM_MAX][DIM_MAX], int size)
void add_position_data(bool explored[], int tiles[], int tile, int costs[], int cost, int parents[], int parent, int board_positions[], int size)
{
    //printf("adding position data\n");
    //printf("tile: %d\n", tile);
    //printf("parent: %d\n", parent);
    explored[size] = false;
    tiles[size] = tile;
    costs[size] = (parent >= 0) ? costs[parents[parent]] + cost : cost;
    //costs[size] = (parent >= 0) ? costs[parents[parent]] * 2 + cost : cost;
    //costs[size] = cost;
    printf("cost: %d\n", cost);
    //printf("size: %d\n", size);
    //printf("costs[size]: %d\n", costs[size]);
    costs[size] = cost;
    parents[size] = parent;
    //printf("doing board position\n");
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            board_positions[size][i][j] = brd[i][j];
        }
    }
}

/**
 * Checks if the current board configuration has been considered previously
 */
//bool position_considered(int*** board_positions, int size)
//bool position_considered(int board_positions[][DIM_MAX][DIM_MAX], int size)
bool position_considered(int board_positions[], int size)
{
    for (int i = 0; i < size; i++)
    {
        int matching = 1;
        for (int j = 0; j < d; j++)
        {
            for (int k = 0; k < d; k++)
            {
                if (brd[j][k] != board_positions[i][j][k])
                {
                    matching = 0;
                }
            }
        }
        if (matching)
        {
            return true;
        }
    }
    return false;
}

/**
 * Determines the value of the aforementioned heuristic determining the "cost"
 * of a board position, or it's total distance from the solution.
 * 
 * That yuck line computes the taxicab distance between the tile at [i][j]'s
 * real and target position. Optimisation is important here soz.
 */
int position_cost(void)
{
    int acc = 0;
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            if (brd[i][j] != 0)
            {
                acc += abs((brd[i][j] - 1) / d - i) + abs((brd[i][j] - 1) % d - j); // strict taxicab distance
                //int cost = abs((brd[i][j] - 1) / d - i) + abs((brd[i][j] - 1) % d - j); // weighting on low numbers
                //int weight = d * d - brd[i][j];
                //acc += cost * weight;
            }
        }
    }
    return acc;
}

/**
 * Populates the valid moves for a given dimensionality.
 */
void populate_valid_tiles(int tiles[], int zero_pos)
{
    int i = zero_pos / d;
    int j = zero_pos % d;
    tiles[0] = (i == 0) ? -1 : brd[i - 1][j]; // N
    tiles[1] = (j == d - 1) ? -1 :brd[i][j + 1]; // E
    tiles[2] = (i == d - 1) ? -1 : brd[i + 1][j]; // S
    tiles[3] = (j == 0) ? -1 : brd[i][j - 1]; // W
}

//int parent_and_brd_to_lowest_cost(bool explored[], int costs[], int*** board_positions, int size)
//int parent_and_brd_to_lowest_cost(bool explored[], int costs[], int board_positions[][DIM_MAX][DIM_MAX], int size)
int parent_and_brd_to_lowest_cost(bool explored[], int costs[], int* board_positions, int size)
{
    int lowest_cost_parent = -1;
    int lowest_cost = SEARCH_MAX;
    
    // get indices that aren't in parents, and get their costs
    for (int i = 0; i < size; i++)
    {
        if (!explored[i])
        {
            if (costs[i] < lowest_cost)
            {
                lowest_cost_parent = i;
                lowest_cost = costs[i];
            }
        }
    }
    
    // set board position
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            brd[i][j] = board_positions[lowest_cost_parent][i][j];
        }
    }
    
    explored[lowest_cost_parent] = true;
    
    return lowest_cost_parent;
}

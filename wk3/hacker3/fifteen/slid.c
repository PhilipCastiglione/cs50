#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

#define _XOPEN_SOURCE 500
#define DIM_MIN 3
#define DIM_MAX 5
#define LOTS 65536

int validate_args(int argc, char *argv[]);
void initialize_board(int **board, int dim);
void set_starting_board(int **board, int dim);
void randomize_board(int **board, int dim);
void run_game(int **board, int dim);
void draw_board(int **board, int dim);
int is_board_correct(int **board, int dim);
void accept_user_move(int **board, int dim);
int find(int **board, int dim, int tile);
void move_tile_at_idx(int **board, int dim, int tile_idx);
void god(int **board, int dim);
int is_valid_move(int **board, int dim, int tile_idx);
void malloc_arrays(int *tiles, int *costs, int *parents, int *explored, int ***board_positions, int dim);
void populate_move_idxs(int **board, int dim, int *move_idxs);
void add_new_moves(int **board, int dim, int *tiles, int *costs, int *parent_idxs, int *explored, int ***board_positions, int *local_move_idxs, int *size, int *won);
int is_new_valid_move(int **board, int dim, int move_idx, int ***board_positions, size);

void swap(int pos_one, int pos_two);
void god(void);
int search(int winning_indices[]);
bool position_considered(int ***board_positions, int size);
int position_cost(void);
void add_position_data(bool explored[], int tiles[], int tile, int costs[], int cost, int parents[], int parent, int size);
void populate_valid_tiles(int tiles[], int zero_pos);
int parent_and_brd_to_lowest_cost(bool explored[], int costs[], int ***board_positions, int size);

int main(int argc, string argv[])
{
    if (int code = validate_args(argc, argv))
    {
      return code;
    }

    int dim = atoi(argv[1]);
    int **current_board = malloc(dim * dim * sizeof(int));

    initialize_board(current_board, dim);

    run_game(current_game, dim);

    return 0;
}

int validate_args(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }
    else if (atoi(argv[1]) < DIM_MIN || atoi(argv[1]) > DIM_MAX)
    {
        printf("Usage: fifteen d\n");
        printf("d must be between %i and %i inclusive.\n", DIM_MIN, DIM_MIN);
        return 2;
    }
    return 0;
}

void initialize_board(int **board, int dim)
{
    set_starting_board(board, dim);
    randomize_board(board, dim);
}

void set_starting_board(int **board, int dim)
{
    int tile = dim * dim - 1;
    bool odd = tile % 2;
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            board[i][j] = tile--;
        }
    }
    if (odd)
    {
        board[dim - 1][dim - 3] = 1;
        board[dim - 1][dim - 2] = 2;
    }
}

void randomize_board(int **board, int dim)
{
    srand((int) time(NULL) % 32768);
    int move_count = rand() % (dim * 10) + (dim * 10);
    for (int i = 0; i < move_count; i++)
    {
        int *local_move_idxs = malloc(4 * sizeof(int));
        populate_move_idxs(board, dim, local_move_idxs);

        int random_index;
        do
        {
          random_index = rand() % 4;
        }
        while (local_move_idxs[random_index] != -1)
        move_tile_at_idx(board, dim, local_move_idxs[random_index]);
    }
}

void run_game(int **board, int dim)
{
    while (true)
    {
        draw_board(board, dim);

        if (is_board_correct(board, dim))
        {
            printf("You win. Congratulations. I hope it was worth it.\n");
            break;
        }

        accept_user_move(board, dim);
    }
}

void draw_board(int **board, int dim)
{
    usleep(200000);
    char c1, c2, sep;
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            c1 = (board[i][j] < 10) ? ' ' : (char) (board[i][j] / 10 + 48);
            c2 = (board[i][j]) ? (char) (board[i][j] % 10 + 48) : '_';
            sep = (j < dim - 1) ? '|' : '\0';
            printf(" %c%c %c", c1, c2, sep);
        }
        printf("\n");
    }
}

int is_board_correct(int **board, int dim)
{
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            if (board[i][j] != i * dim + j + 1 && board[i][j] != 0)
            {
                return false;
            }
        }
    }
    return true;
}

void accept_user_move(int **board, int dim)
{
    char *move = "";
    do
    {
        printf("Tile to move: ");
        char *input;
        // TODO: read in move
        int tile_idx = find(board, dim, atoi(input))
        if (strcmp(input, "GOD") == 0)
        {
           god(board, dim);
           // TODO: first, just print the moves, later refactor to make them and break
           // break;
        }
        else if (is_valid_move(board, dim, tile_idx))
        {
            move_tile_at_idx(board, dim, tile_idx);
            strcpy(move, input);
        }
        else
        {
           printf("Illegal move.\n");
        }
    }
    while(strcmp(move, "") == 0);
}

int find(int **board, int dim, int tile)
{
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            if (board[i][j] == tile)
            {
                return i * dim + j;
            }
        }
    }
    return -1;
}

void god(int **board, int dim)
{
    int* tiles, costs, parent_idxs, explored;
    int ***board_positions;
    malloc_arrays(tiles, costs, parent_idxs, explored, board_positions, dim);

    int size = 0;
    int parent_idx = -1;
    int won = 0;

    while (!won)
    {
        int *local_move_idxs = malloc(4 * sizeof(int));

        populate_move_idxs(board, dim, local_move_idxs);

        add_new_moves(board, dim, tiles, costs, parent_idxs, explored, board_positions, local_move_idxs, &size, &won);
    }

    printf("winning moves, in reverse order!\n");
    // TODO: change printing the won stuff to returning/setting/doing whatever
    while (size > 0)
    {
        printf("%d\n", tiles[size]);
        size = parents[size];
    }
}

void move_tile_at_idx(int **board, int dim, int tile_idx)
{
    int zero_idx = find(board, dim, 0);
    int tmp = board[zero_idx / dim][zero_idx % dim];
    board[zero_idx / d][zero_idx % dim] = board[tile_idx / dim][tile_idx % dim];
    board[tile_idx / dim][tile_idx % dim] = tmp;
}

int is_valid_move(int **board, int dim, int tile_idx)
{
    int zero_idx = find(board, dim, 0);
    if (tile_idx == -1 || tile_idx == zero_idx)
    {
      return 0;
    }
    return (tile_idx == zero_idx - dim) || // North neighbour
           (tile_idx == zero_idx + 1 && tile_idx % dim != 0) || // East neighbour
           (tile_idx == zero_idx + dim) || // South neighbour
           (tile_idx == zero_idx - 1 && zero_idx % dim != 0); // West neighbour
}

void malloc_arrays(int *tiles, int *costs, int *parents, int *explored, int ***board_positions, int dim)
{
    *tiles = malloc(LOTS * sizeof(int));
    *costs = malloc(LOTS * sizeof(int));
    *parent_idxs = malloc(LOTS * sizeof(int));
    *explored = malloc(LOTS * sizeof(int));
    ***board_positions = malloc(LOTS * dim * dim * sizeof(int*));
    for (int i = 0; i < LOTS; i++)
    {
        board_positions[i] = malloc(dim * dim * sizeof(int*));
        for (int j = 0; j < dim; j++)
        {
            board_positions[i][j] = malloc(dim * sizeof(int));
        }
    }
}

void populate_move_idxs(int **board, int dim, int *move_idxs)
{
    int zero_idx = find(board, dim, 0);
    int i = zero_idx / dim;
    int j = zero_idx % dim;
    move_idxs[0] = (i == 0) ? -1 : zero_idx - dim; // North idx
    move_idxs[1] = (j == dim - 1) ? -1 : zero_idx + 1; // East idx
    move_idxs[2] = (i == dim - 1) ? -1 : zero_idx + dim; // South idx
    move_idxs[3] = (j == 0) ? -1 : zero_idx - 1; // West idx
}

void add_new_moves(int **board, int dim, int *tiles, int *costs, int *parent_idxs, int *explored, int ***board_positions, int *local_move_idxs, int *size, int *won)
{
    for (int i = 0; i < 4; i++)
    {
        if (!won && is_new_valid_move(board, dim, local_move_idxs[i], board_positions, size))
        {
            add_move_to_arrays(board, dim, tiles, costs, parent_idxs, explored, board_positions, local_move_idxs, &won, &size);
        }
    }
}

int is_new_valid_move(int **board, int dim, int move_idx, int ***board_positions, size)
{
    return is_valid_move(board, dim, local_move_idxs[i]) &&
      !is_position_for_move_idx_checked(board, dim, local_move_idxs[i], board_positions, size);
}

// TODO: is_position_for_move_idx_checked


// TODO add_move_to_arrays(board, dim, tiles, costs, parent_idxs, explored, board_positions, local_move_idxs, &won, &size);





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
    int LOTS = 65536;
    printf("setting up search vars\n");
    int* tiles = malloc(LOTS * sizeof(int));
    int* costs = malloc(LOTS * sizeof(int));
    int* parents = malloc(LOTS * sizeof(int));
    bool* explored = malloc(LOTS * sizeof(int));
    int ***board_positions = malloc(LOTS * d * d * sizeof(int*));
    for (int i = 0; i < LOTS; i++)
    {
        board_positions[i] = malloc(d * d * sizeof(int*));
        for (int j = 0; j < d; j++)
        {
            board_positions[i][j] = malloc(d * sizeof(int*));
        }
    }
    
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
                swap(zero_pos, find(local_tiles[i]));
                // check if the board position has been seen before
                if (position_considered(board_positions, size))
                {
                    // just pop back if so
                    swap(zero_pos, find(local_tiles[i]));
                }
                else
                {
                    int cost = position_cost();
                    // add the tile, its parent idx, costs and set explored
                    printf("%d%d%d%d%d%d%d%d%d\n", brd[0][0], brd[0][1], brd[0][2], brd[1][0], brd[1][1], brd[1][2], brd[2][0], brd[2][1], brd[2][2]);
                    add_position_data(explored, tiles, local_tiles[i], costs, cost, parents, parent, size);
                    // check for search win
                    if (cost == 0)
                    {
                        printf("exiting the search\n");
                        won = size;
                        break;
                    }
                    // increment size given that we added a new position when we checked position considered
                    size++;
                    // then swap back
                    swap(zero_pos, find(local_tiles[i]));
                }
            }
        }
        
        if (won)
        {
            printf("exiting the search\n");
            break;
        }
        
        // find the best available move, move the board and parent to that position
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
    
    // TODO:
    //free(tiles);
    //free(costs);
    //free(parents);
    //free(explored);
    //free(board_positions);
    return win_c;
}

void add_position_data(bool explored[], int tiles[], int tile, int costs[], int cost, int parents[], int parent, int size)
{
    explored[size] = false;
    tiles[size] = tile;
    costs[size] = (parent >= 0) ? costs[parents[parent]] + cost : cost;
    parents[size] = parent;
}

/**
 * Checks if the current board configuration has been considered previously and adds it if not
 */
bool position_considered(int ***board_positions, int size)
{
    // for each recorded board position
    for (int i = 0; i < size; i++)
    {
        bool matching = true;
        for (int j = 0; j < d; j++)
        {
            for (int k = 0; k < d; k++)
            {
                if (board_positions[i][j][k] != brd[j][k])
                {
                    matching = false;
                    break; // early exit
                }
            }
            if (!matching)
            {
              break; // early exit
            }
        }
        if (matching)
        {
            return true;
        }
    }
    // add the current position to the recorded positions
    for (int j = 0; j < d; j++)
    {
        for (int k = 0; k < d; k++)
        {
            board_positions[size][j][k] = brd[j][k];
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
                // strict taxicab distance accumulative A*
                acc += abs((brd[i][j] - 1) / d - i) + abs((brd[i][j] - 1) % d - j);
                // weighting on low numbers
                //int cost = abs((brd[i][j] - 1) / d - i) + abs((brd[i][j] - 1) % d - j);
                //int weight = d * d - brd[i][j];
                //acc += cost * weight;
            }
        }
    }
    return acc;
}

/**
 * Populates the valid moves for a given dimensionality. An invalid move is
 * represented by -1.
 */
void populate_valid_tiles(int tiles[], int zero_pos)
{
    int i = zero_pos / d;
    int j = zero_pos % d;
    tiles[0] = (i == 0) ? -1 : brd[i - 1][j]; // North tile
    tiles[1] = (j == d - 1) ? -1 :brd[i][j + 1]; // East tile
    tiles[2] = (i == d - 1) ? -1 : brd[i + 1][j]; // South tile
    tiles[3] = (j == 0) ? -1 : brd[i][j - 1]; // West tile
}

/**
 * Finds the lowest cost unexplored position, to look at next. Sets the board and returns
 * the parent index.
 */
int parent_and_brd_to_lowest_cost(bool explored[], int costs[], int ***board_positions, int size)
{
    int lowest_cost_parent = -1;
    int lowest_cost = INT_MAX;
    
    // get unexplored indices and get their costs
    // update lowest cost if found
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

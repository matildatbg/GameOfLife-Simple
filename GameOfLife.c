
/*  Conway's Game of Life - OPTIMIZED, PARALLEL VERSION

    The cells live in a NxN matrix, and are initilized 
    to 0 (dead) or alive (1) randomly.
    The cells statuses are updated accoring to the following rules: 
        alive #neighbors < 2 -> dies
        alive #neighbors == 2 || 3 -> lives
        alive #neighbors > 3 -> dies
        dead #neighbors == 3 -> lives
    
    The code is run by entering the following in the terminal:

    ./GameOfLife [N] [steps] [probability] [numberOfThreads]
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>

//Global variables 
int sizeWorld;

/*Keep tracks of each cell in world, and its neighbors.
    Status = 1 --> alive, 
    Status = 0 --> dead
*/
typedef struct cell { 
    int neighbors;
    int status;
} cell_t;

void printGrid(cell_t** __restrict cells);
void initialPattern(cell_t** __restrict cells, int p);
void updatePattern(cell_t** __restrict cells);

static double get_wall_seconds() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    double seconds = tv.tv_sec + (double)tv.tv_usec / 1000000;
    return seconds;
}


//Main program
int main(int argc, char* argv[]){
    double time_start = get_wall_seconds();
    if (argc != 5){
        printf("Wrong input! \nFollowing arguents required: \n");
        printf("    N: size of world\n");
        printf("    s: number of steps in the simulation. Integer.\n");
        printf("    p: chance of living cell. Given as integer [1,10]\n");
        printf("    t: number of threads for parallelization\n");
        printf("\nEnding program.\n");
        return -1;
    }
    
    sizeWorld = atoi(argv[1]);
    int steps = atoi(argv[2]);
    int probability = atoi(argv[3]);
    int numThreads = atoi(argv[4]);
    omp_set_num_threads(numThreads);

    cell_t** cells = (cell_t**)malloc((sizeWorld+2)*sizeof(cell_t*));
    for(int i = 0; i < sizeWorld+2; i++){
        cells[i]=(cell_t*)malloc((sizeWorld+2)*sizeof(cell_t));
        //Sets all initial values to zero
         for (int j = 0; j < sizeWorld+2;j++){
            cells[i][j].status = 0;
            cells[i][j].neighbors = 0;
        }
    }

    initialPattern(cells,probability);
    
    int i = 0;
    /*  The commented lines in this section should be 
        uncommented for a visual representation.*/
    while (i < steps){
        system("clear");
        printf("\n--------------NEW GRID----------------\n");
        printGrid(cells);
        updatePattern(cells);
        usleep(600000);
        i = i+1;
    }

    for (int i=0; i<sizeWorld+2; i++){
        free(cells[i]);
    }
    free(cells);
    
    printf("\nProgram ended. Have a Good Life! \n");
    printf("GameOfLife main took %7.3f wall seconds.\n", get_wall_seconds()-time_start);
    return 0;
}


// ==================== FUNCTIONS ===================================
void printGrid(cell_t** __restrict cells){
    for (int row=1; row<sizeWorld+1; row++){
        for(int col=1; col < sizeWorld+1; col++){
            if(cells[row][col].status == 1){
                printf("\u2B1C ");
            }
            else{
                printf("   ");
            }
        }
        printf("\n");
    }
}

// Creates a random pattern with a set size
void initialPattern(cell_t** __restrict cells, int p){
    int prob = 10-p;
    int chance;
    srand(time(0));
    for(int row = 1; row < sizeWorld+1; row++){
        for (int col = 1; col < sizeWorld+1; col++){
            chance = rand()%10;
            if (chance >= prob){
                cells[row][col].status = 1;
            }
        }
    }   
}


void updatePattern(cell_t** __restrict cells){
    //Counts neighbors
    int row, col;

    #pragma omp parallel for private(row, col)
    for(row=1; row<sizeWorld+1; row++){
        for (col = 1; col < sizeWorld+1; col++){
            cells[row][col].neighbors = 
                    cells[row][col-1].status + cells[row][col+1].status + 
                    cells[row-1][col-1].status + cells[row-1][col].status + cells[row-1][col+1].status+
                    cells[row+1][col-1].status + cells[row+1][col].status + cells[row+1][col+1].status;
        }
    }

    //Updates statuses
    #pragma omp parallel for private(row, col)
    for(row=1; row<sizeWorld+1; row++){                 
        for (col = 1; col < sizeWorld+1; col++){
            if (cells[row][col].status == 0){
                if (cells[row][col].neighbors == 3){
                    cells[row][col].status = 1;
                }
            }
            else{
                if (cells[row][col].neighbors > 3){
                    cells[row][col].status = 0;
                }
                else if (cells[row][col].neighbors < 2){
                    cells[row][col].status = 0;
                }
            }
        }
    }
}

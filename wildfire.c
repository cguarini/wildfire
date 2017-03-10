//file: wildfire.c
//Simulates a wildfire using modified shiflet rules
//author: Chris Guarini

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //used for usleep
#include <getopt.h> //processing for "-fN" command line args
#include "display.h"//Used to display the grid on the terminal


//defines
//DEFUALTS
#define DEFAULT_BURN 10 ///Proportion of trees initially burning
#define DEFAULT_PROB_CATCH 30///Probability of a tree to catch fire
#define DEFAULT_DENSITY 50 ///Proportion of trees to empty cells on map
#define DEFAULT_PROB_NEIGHBOR 25///Proportion of neighbors on fire to trigger prob_catch
#define DEFAULT_PRINT_COUNT  0///Toggles if printing is on or off, default off
#define DEFAULT_SIZE 10///Default size of 10x10 grid
//Symbols used for backend grid only
#define TREE  '1'  ///symbol for a noninflamed tree
#define EMPTY '0'  ///Symbol for an empty cell
#define FIRE1 '2'  ///Symbol for fire in first cycle
#define FIRE2 '3'  ///Symbol for fire in second cycle
#define BURNT '5'  ///Symbol for burnt out cell
//Symbols used for display purposes
#define TREE_DISPLAY 'Y' ///Symbol for noninflamed tree
#define EMPTY_DISPLAY ' '///Symbol for empty cell
#define FIRE_DISPLAY '*' ///Symbol for fire
#define BURNT_DISPLAY '.'///Symbol for burnt out cell
#define ERROR_DISPLAY 'N'///Symbol if grid was incorrectly populated
//FUNCTION DECLARATIONS
int usleep(useconds_t usec);
void *memcpy(void *str1, const void *str2, size_t n);
//Command line arguments with defaults
static int burning=DEFAULT_BURN;      //percentage of population that is initially burning, modified by -bN argument
static int poc=DEFAULT_PROB_CATCH;    //Probability of catching fire, chance for each tree to catch fire, modified by -cN argument
static int occ=DEFAULT_DENSITY;       //proportion of simulation space that is occupied, modified by -dN argument
static int neigh=DEFAULT_PROB_NEIGHBOR;//Proportion of neighbors that above the tree may catch fire, modified by -nN argument
static int print=DEFAULT_PRINT_COUNT; //Toggles print mode, if >0, simulation will print that many iterations
static int size=DEFAULT_SIZE;         //size of the simulation grid's rows and columns. Grid will be size*size
static int damp=0;                    //Dampness factor reduces the likelyhood a tree will catch fire, initializes to 0 (off)
static int strike=0;                  //Toggles lightning strikes, which will set random trees on fire, initializes to 0 (off)
//Global Variables
static int Cycle=0;//Cycle the simulation is on
static int changes=0;//Changes this cycle
static int cumulativeChanges=0;//Total changes
///////////////////////////////////////////////



/*Initializes the game board by populating it with empty cells, trees amd fires
**Will add trees and fires  randomly to the board until it is populated
**to the proportion of the occ variable.
**@param grid: The board to populate with trees.
*/
void initialize(char grid[size][size]){
  int cells=size*size;//total number of cells
  int numOfTrees=cells*occ/100;//Total number of trees
  int onFire=numOfTrees*burning/100;//Number of trees initially on fire

  //Populate board with empty cells
  for(int i=0;i<size;i++){//for every row, top to bottom
    for(int j=0;j<size;j++){//for every col, left to right
      grid[i][j]=EMPTY;//Populate grid with empty cells
    }
  }

  
  //Populate board with trees
  while(numOfTrees>0){//While there are trees left to add
    int temp1=random();
    int temp2=random();
    int i=temp1%(size);//gets a row of a random cell
    int j=temp2%(size);//gets a col of a random cell
    if(grid[i][j]==TREE){
      //Already tree there, loop again    
    }
    else{//Cell is empty
      grid[i][j]=TREE;//Add tree
      numOfTrees--;//decriment counter
    }
  }


  //inflame initial trees
  while(onFire>0){//While there are still fires to set
    int i=random()%(size);//gets a row of a random cell
    int j=random()%(size);//gets a col of a random cell
    if(grid[i][j]==TREE){//Check if cell is a tree
      grid[i][j]++;//Increment the cell to show tree is on fire
      onFire--;//decrement fire counter
    }
    else{//Cell is not a tree, pass over
      //No tree, loop again
    }

  }
//End initialize
}

/*Advances the grid one cycle, spreading fires and taking in to account
**the variables set in the arguments. 
**A cell with a 0 is empty, a cell with a 1 is a tree. For a tree on fire,
**A cell can be 2 or 3, which corresponds to how many cycles it has been
**on fire. A cell with a value of 4 has been burnt out.
**@param: grid - Map of the simulation
**@returns: Returns 1 if any fires left, 0 if all fires are out.
*/
int advance(char grid[size][size]){
  changes=0;//reset changes
  int onFire=0;//Number of fires in the grid, if 0 triggers end of simulation.
  for(int i=0;i<size;i++){//Loop through every row, top to bottom
    for(int j=0;j<size;j++){//Loop through every column, left to right
      int trees=0;//Number of adjacent trees
      int fires=0;//Number of adjacent fires
      if(grid[i][j]==EMPTY || grid[i][j]==BURNT){//Empty or burnt cell
        //Nothing happens
      }
      else if(grid[i][j]>TREE && grid[i][j]<BURNT){//Cell is burning but not burnt out
        grid[i][j]++;//increment the fire a cycle
        onFire++;
        if(grid[i][j]==BURNT){//Went from fire to burnt
          //Increment change counters
          changes++;
          cumulativeChanges++;
        }
      }
      else if(grid[i][j]==TREE){//Cell holds a tree that is not on fire
        
        //Check adjacent cells starting at north and working clockwise
        
        //North
        if(i>0){//if cell exists
          if(grid[i-1][j]==TREE){//adjacent cell is a tree
            trees++;//increment tree counter
          }
          if(grid[i-1][j]>TREE &&grid[i-1][j]<BURNT){//adjacent cell is on fire
            fires++;//increment fire counter
          }
        }
        
        //North East
        if(i>0&& j<size-1){//if cell exists
          if(grid[i-1][j+1]==TREE){//adjacent cell is a tree
            trees++;//increment tree counter
          }
          if(grid[i-1][j+1]>TREE &&grid[i-1][j]<BURNT){//adjacent cell is on fire
            fires++;//increment fire counter
          }
        }

        //East
        if(j<size-1){//if cell exists
          if(grid[i][j+1]==TREE){//adjacent cell is a tree
            trees++;//increment tree counter
          }
          if(grid[i][j+1]>TREE &&grid[i-1][j]<BURNT){//adjacent cell is on fire
            fires++;//increment fire counter
          }
        }

        //South East
        if(i<size-1 && j<size-1){//if cell exists
          if(grid[i+1][j+1]==TREE){//adjacent cell is a tree
            trees++;//increment tree counter
          }
          if(grid[i+1][j+1]>TREE &&grid[i-1][j]<BURNT){//adjacent cell is on fire
            fires++;//increment fire counter
          }
        }
        
        //South
        if(i<size-1){//if cell exists
          if(grid[i+1][j]==TREE){//adjacent cell is a tree
            trees++;//increment tree counter
          }
          if(grid[i+1][j]>TREE &&grid[i-1][j]<BURNT){//adjacent cell is on fire
            fires++;//increment fire counter
          }
        }

        //South West
        if(i<size-1 && j>0){//if cell exists
          if(grid[i+1][j-1]==TREE){//adjacent cell is a tree
            trees++;//increment tree counter
          }
          if(grid[i+1][j-1]>TREE &&grid[i-1][j]<BURNT){//adjacent cell is on fire
            fires++;//increment fire counter
          }
        }

        //West
        if(j>0){//if cell exists
          if(grid[i][j-1]==TREE){//adjacent cell is a tree
            trees++;//increment tree counter
          }
          if(grid[i][j-1]>TREE &&grid[i-1][j]<BURNT){//adjacent cell is on fire
            fires++;//increment fire counter
          }
        }

        //North West
        if(i>0 && j>0){//if cell exists
          if(grid[i-1][j-1]==TREE){//adjacent cell is a tree
            trees++;//increment tree counter
          }
          if(grid[i-1][j-1]>TREE &&grid[i-1][j]<BURNT){//adjacent cell is on fire
            fires++;//increment fire counter
          }
        }
        //END CHECK

        float fireProportion=0;
        if(fires>0){//So that it doesn't divide by zero
          fireProportion =(float) (trees+fires)/fires;//Proportion of neighbors on fire
        }
        float neighProp=(float)neigh/100;
        int catchChance =rand()%100;//Random number between 1 and 100, used to check if tree will catch fire
        if(fireProportion>neighProp/100 && catchChance<poc){
          grid[i][j]++;//Tree catches fire
          onFire++;//Add fire to the counter that determines end of game
          //increment change counters
          changes++;
          cumulativeChanges++;
        }
      }
    }
  }
  //end of loops
  if(onFire>0){//Any fires left?
    return 1;
  }
  return 0;//end of simulation
}


/**Prints the grid to the terminal using stdio functions. Will display the grid
** in a way that the terminal can scroll through each iteration.
**@param: grid - grid to print to terminal
**/
void printGrid(char grid[size][size]){
  printf("\n");//Start with new line
  //Print the grid
  for(int i=0; i<size;i++){//loop through grid from top to bottom
    for(int j=0;j<size;j++){//left to right
      if(grid[i][j]==EMPTY){//empty cell
        printf("%c",EMPTY_DISPLAY);
      }
      else if(grid[i][j]==TREE){//tree in cell
        printf("%c",TREE_DISPLAY);
      }
      else if(grid[i][j]==BURNT){//Burnt out cell
        printf("%c",BURNT_DISPLAY);
      }
      else if(grid[i][j]>TREE && grid[i][j ]< BURNT){//Cell on fire
        printf("%c",FIRE_DISPLAY);
      }
      else{
        printf("%c",ERROR_DISPLAY);//shouldn't be reached
      }
    }
    printf("\n");//newline after every row
  }
  //Grid printed
}

/**Prints the information string that contains the global variables and cycle
** numbers
**/
void printInfo(void){
  printf("size %d, pCatch %d%%, density %d%%, pBurning %d%%, pNeighbor %d%%\n",size,poc,occ,poc,neigh);
  printf("cycle %d, changes %d, cumulative changes %d.\n",Cycle,changes,cumulativeChanges);
}

/**Displays the simulation using the code given in display.c and display.h
** @param: grid - grid to display to the terminal
**/
void displayOverlay(char grid[size][size]){
  clear();//clear the terminal
  set_cur_pos(1,1);//set cursor to top left position
  for(int i=0;i<size;i++){//loop through grid from top to bottom
    for(int j=0;j<size;j++){//left to right
      set_cur_pos(i+1,j+1);//set cursor to position in grid
      if(grid[i][j]==EMPTY){//Empty cell
        put(EMPTY_DISPLAY);
      }
      else if(grid[i][j]==TREE){//Tree
        put(TREE_DISPLAY);
      }
      else if(grid[i][j]>TREE && grid[i][j]<BURNT){//Burning
        put(FIRE_DISPLAY);
      }
      else if(grid[i][j]==BURNT){//Burnt cell
        put(BURNT_DISPLAY);
      }
      else{
        put(grid[i][j]);
        //put(ERROR_DISPLAY);//Shouldn't be reached
      }
    }

  }
  set_cur_pos(size+1,1);//cursor right bellow output
}

/**Test function that prints out the grid without converting it to display
** values
*/
void testDisplay(char grid[size][size]){
  clear();//clear display
  set_cur_pos(1,1);//initialize cursor
  for(int i=0;i<size;i++){//loop through grid
    for(int j=0;j<size;j++){
      set_cur_pos(i+1,j+1);//set cursor position
      put(grid[i][j]);//put the value
    }
  }
  set_cur_pos(size+1,1);
}


/**Prints the usage message, for use when the user enters the H argument
*/
static void usage() {
  fprintf(stderr, "usage: wildfire [options]\n ");
  fprintf(stderr, "By default, the simulation runs in overlay display mode.\n");
  fprintf(stderr, "The -pN option makes the simulation run in print mode for up to N cycles.\n\n");
  fprintf(stderr, "Simulation Configuration Options:\n");
  fprintf(stderr, "\t-H # View simulation options and quit.\n");
  fprintf(stderr, "\t-bN # proportion of trees that are already burning. 0 < N < 101.\n");
  fprintf(stderr, "\t-cN # probability that a tree will catch fire. 0 < N < 101.\n");
  fprintf(stderr, "\t-dN # density/proportion of trees in the grid. 0 < N < 101.\n");
  fprintf(stderr, "\t-nN # proportion of neighbors that influence a tree catching fire. -1 < N < 101.\n");
  fprintf(stderr, "\t-pN # number of cycles to print before quitting. -1 < N < ...\n");
  fprintf(stderr, "\t-sN # simulation grid size. 4 < N < 41.\n");
}
/**Main function of wildfire. This will have the main loop of the simulation,
** which based on the arguments will determine initial conditions, and output
** conditions.
**/
int main(int argc, char * argv[]){
  srandom(528);//seed the random number generator
  int opt;
  while((opt=getopt(argc,argv,"Hb:c:d:n:p:s:")) != -1){
    switch( opt ) {
      case 'H'://Help command
        usage();
        return 0;
        break;
      case 'b'://set burning
        burning = (int)strtol(optarg, NULL, 10);//set burning to value
        if(burning > 0 && burning < 101){//check if in parameters
          break;//all is good
        }
        else{//Bad input
          fprintf(stderr, "\t(-bN) proportion already burning. must be an integer in [1...100].\n");
          usage();//print usage
          return EXIT_FAILURE;
        }
      case 'c'://probability to catch fire (poc)
        poc = (int)strtol(optarg, NULL, 10);//set poc to value
        if(poc > 0 && poc < 101){//check if withing parameters
        break;//all is well
        }
        else{//Bad input
          fprintf(stderr, "\t(-cN) probability a tree will catch fire. must be an integer in [1...100].\n");      
          usage();//print usage information
          return EXIT_FAILURE;
        }
      case 'd'://proportion of trees to grid size
        occ= (int)strtol(optarg, NULL, 10);//set occ to value
        if(occ > 0 && occ < 101){//within parameters
          break;//all is fine
        }
        else{//not within parameters
          fprintf(stderr, "\t(-dN) density of trees in the grid must be an integer in [1...100].\n");
          usage();//usage info
          return EXIT_FAILURE;
        }
      case 'n':
        neigh= (int)strtol(optarg, NULL, 10);//set neigh to value
        if(neigh > -1 && neigh < 101){//check if within parameters
          break;//all is great
        }
        else{//all is not great
          fprintf(stderr, "\t(-nN) neighbors influence catching fire must be an integer in [0...100].\n");
          usage();//print help
          return EXIT_FAILURE;
        }
       case 'p':
        print= (int)strtol(optarg, NULL, 10);//set print to value
        if(print > -1 && print < 10000){//within parameters
          break;//all is spectacular
        }
        else{//all is not spectacular
          fprintf(stderr, "\t(-pN) number of cycles to print. must be an integer in [0...10000].\n");
          usage();
          return EXIT_FAILURE;
        }
       case 's':
        size = (int)strtol(optarg, NULL, 10);//set size to value
        if(size > 4 && size < 41){//within parameters
          break;//all is okay
        }
        else{//all is not okay
          fprintf(stderr, "\t(-sN) simulation grid size must be an integer in [5...40].\n");
          usage();
          return EXIT_FAILURE;
        }
    }//end of switch
  }//end of while

  /////////////////////////////////////////////////
  //Start of simulation
  char grid[size][size];//allocate the grid
  initialize(grid);//initialize the grid
  if(print > 0){//Print output instead of display overlay
    printGrid(grid);//print the initial state of the simulation
    printInfo();//Print initial information
    for(int i=0;i<print;i++){//Run for as many cycles as determined by arguments
      usleep(750000);//wait
      advance(grid);//Do a simulation cycle
      Cycle++;//increment cycle counter
      printGrid(grid);//print the simulation
      printInfo();//Print the cycle information
    }
    return 0;
    //END OF SIMULATION
  }
  else{//Display overlay
    int cont=1;//output of advance, 0 if end of game
    displayOverlay(grid);//display initial state
    printInfo();//Print initial info
    while(cont!=0){//Simulation loop
      usleep(750000);//wait
      cont=advance(grid);//Do a simulation cycle
      Cycle++;//Increment cycle counter
      displayOverlay(grid);//Display the simulation
      printInfo();//Print cycle informatioin
      ///////////////////////////////////////
      /*FOR TESTING ONLY
      ** Will flash the value of every cell after it displays the grid
      usleep(750000);
      testDisplay(grid);
      printInfo();
      */
    }
    printf("Fires are out.\n");//end of simulation print
    return 0;
  }

}





















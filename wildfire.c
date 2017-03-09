//file: wildfire.c
//Simulates a wildfire using modified shiflet rules
//author: Chris Guarini

#include <stdio.h>
#include <stdlib.h>

#include <getopt.h> //processing for "-fN" command line args

//defines
//DEFUALTS
#define DEFAULT_BURN 10 ///Proportion of trees initially burning
#define DEFAULT_PROB_CATCH 30///Probability of a tree to catch fire
#define DEFAULT DENSITY 50 ///Proportion of trees to empty cells on map
#define DEFAULT_PROP_NEIGHBOR 25///Proportion of neighbors on fire to trigger prob_catch
#define DEFAULT_PRINT_COUNT  0///Toggles if printing is on or off, default off
#define DEFAULT_SIZE 10///Default size of 10x10 grid
//Symbols used for backend grid only
#define TREE  1  ///symbol for a noninflamed tree
#define EMPTY 0  ///Symbol for an empty cell
#define FIRE1 2  ///Symbol for fire in first cycle
#define FIRE2 3  ///Symbol for fire in second cycle
#define BURNT 4  ///Symbol for burnt out cell
//Symbols used for display purposes
#define TREE_DISPLAY 'Y' ///Symbol for noninflamed tree
#define EMPTY_DISPLAY ' '///Symbol for empty cell
#define FIRE_DISPLAY '*' ///Symbol for fire
#define BURNT_DISPLAY '.'///Symbol for burnt out cell
//Command line arguments with defaults
int burning=DEFUALT_BURN;      //percentage of population that is initially burning, modified by -bN argument
int poc=DEFAULT_PROB_CATCH;    //Probability of catching fire, chance for each tree to catch fire, modified by -cN argument
int occ=DEFAULT_DENSITY;       //proportion of simulation space that is occupied, modified by -dN argument
int neigh=DEFAUT_PROB_NEIGHBOR;//Proportion of neighbors that above the tree may catch fire, modified by -nN argument
int print=DEFAULT_PRINT_COUNT; //Toggles print mode, if >0, simulation will print that many iterations
int size=DEFAULT_SIZE;         //size of the simulation grid's rows and columns. Grid will be size*size
int damp=0;                    //Dampness factor reduces the likelyhood a tree will catch fire, initializes to 0 (off)
int strike=0;                  //Toggles lightning strikes, which will set random trees on fire, initializes to 0 (off)
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
    int i=rand()%size-1;//gets a row of a random cell
    int j=rand()%size-1;//gets a col of a random cell
    if(grid[i][j]==TREE{
      //Already tree there, loop again    
    }
    else{//Cell is empty
      grid[i][j]=TREE;//Add tree
      numOfTrees--;//decriment counter
    }
  }


  //inflame initial trees
  while(onFire>0){//While there are still fires to set
    int i=rand()%size-1;//gets a row of a random cell
    int j=rand()%size-1;//gets a col of a random cell
    if(grid[i][j]==TREE{//Check if cell is a tree
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
**the variables set in the arguments. Creates a temporary copy of the grid,
**so that changes during the current cycle don't effect other changes
**as the program loops through the grid.
**A cell with a 0 is empty, a cell with a 1 is a tree. For a tree on fire,
**A cell can be 2 or 3, which corresponds to how many cycles it has been
**on fire. A cell with a value of 4 has been burnt out.
**@param: grid - Map of the simulation
**@returns: Returns 1 if any fires left, 0 if all fires are out.
*/
int advance(char grid[size][size]{
  char copy[size-1][size-1];//Copy of the simulation map
  memcpy(copy,grid,size*size);//create the copy
  int onFire=0;//Number of fires in the grid, if 0 triggers end of simulation.
  for(int i=0;i<size;i++){//Loop through every row, top to bottom
    for(int j=0;j<size;j++){//Loop through every column, left to right
      int trees=0;//Number of adjacent trees
      int fires=0;//Number of adjacent fires
      if(copy[i][j]>1&&copy[i][j]<4){//Cell is burning but not burnt out
        grid[i][j]++;//increment the fire a cycle
        onFire++;
      }
      else if(copy[i][j]==1){//Cell holds a tree that is not on fire
        
        //Check adjacent cells starting at north and working clockwise
        
        //North
        if(i>0){//if cell exists
          if(copy[i-1][j]==TREE){//adjacent cell is a tree
            trees++;//increment tree counter
          }
          if(copy[i-1][j]>TREE &&copy[i-1][j]<BURNT){//adjacent cell is on fire
            fires++;//increment fire counter
          }
        }
        
        //North East
        if(i>0&& j<size-1){//if cell exists
          if(copy[i-1][j+1]==TREE){//adjacent cell is a tree
            trees++;//increment tree counter
          }
          if(copy[i-1][j+1]>TREE &&copy[i-1][j]<BURNT){//adjacent cell is on fire
            fires++;//increment fire counter
          }
        }

        //East
        if(j<size-1){//if cell exists
          if(copy[i][j+1]==TREE){//adjacent cell is a tree
            trees++;//increment tree counter
          }
          if(copy[i][j+1]>TREE &&copy[i-1][j]<BURNT){//adjacent cell is on fire
            fires++;//increment fire counter
          }
        }

        //South East
        if(i<size-1 && j<size-1){//if cell exists
          if(copy[i+1][j+1]==TREE){//adjacent cell is a tree
            trees++;//increment tree counter
          }
          if(copy[i+1][j+1]>TREE &&copy[i-1][j]<BURNT){//adjacent cell is on fire
            fires++;//increment fire counter
          }
        }
        
        //South
        if(i<size-1){//if cell exists
          if(copy[i+1][j]==TREE){//adjacent cell is a tree
            trees++;//increment tree counter
          }
          if(copy[i+1][j]>TREE &&copy[i-1][j]<BURNT){//adjacent cell is on fire
            fires++;//increment fire counter
          }
        }

        //South West
        if(i<size-1 && j>0){//if cell exists
          if(copy[i+1][j-1]==TREE){//adjacent cell is a tree
            trees++;//increment tree counter
          }
          if(copy[i+1][j-1]>TREE &&copy[i-1][j]<BURNT){//adjacent cell is on fire
            fires++;//increment fire counter
          }
        }

        //West
        if(j>0){//if cell exists
          if(copy[i][j-1]==TREE){//adjacent cell is a tree
            trees++;//increment tree counter
          }
          if(copy[i][j-1]>TREE &&copy[i-1][j]<BURNT){//adjacent cell is on fire
            fires++;//increment fire counter
          }
        }

        //North West
        if(i>0 && j>0){//if cell exists
          if(copy[i-1][j-1]==TREE){//adjacent cell is a tree
            trees++;//increment tree counter
          }
          if(copy[i-1][j-1]>TREE &&copy[i-1][j]<BURNT){//adjacent cell is on fire
            fires++;//increment fire counter
          }
        }
        //END CHECK
      

        int fireProportion = (trees+fires)/fires;//Proportion of neighbors on fire
        int catchChance =rand()%100;//Random number between 1 and 100, used to check if tree will catch fire
        if(fireProportion>neigh && catchChance<poc){
          grid[i][j]++;//Tree catches fire
          onFire++;//Add fire to the counter that determines end of game
        }
      }
    }
  }
  //end of loops
  if(onFire>0){
    return 1;
  }
  return 0;
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
        printf(EMPTY_DISPLAY);
      }
      if(grid[i][j]==TREE){//tree in cell
        printf(TREE_DISPLAY);
      }
      if(grid[i][j]==BURNT){//Burnt out cell
        printf(BURNT_DISPLAY);
      }
      if(grid[i][j]==FIRE1 || grid[i][j]==FIRE2){//Cell on fire
        printf(FIRE_DISPLAY);
      }
    }
    printf("\n");//newline after every row
  }
  //Grid printed
}


















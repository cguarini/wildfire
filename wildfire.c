//file: wildfire.c
//Simulates a wildfire using modified shiflet rules
//author: Chris Guarini

#include <stdio.h>
#include <stdlib.h>

#include <getopt.h> //processing for "-fN" command line args

//defines
#define TREE  1  //symbol for a noninflamed tree
#define EMPTY 0  //Symbol for an empty cell
#define FIRE1 2  //Symbol for fire in first cycle
#define FIRE2 3  //Symbol for fire in second cycle
#define BURNT 4  //Symbol for burnt out cell
//Command line arguments with defaults
int burning=15//percentage of population that is initially burning, modified by -bN argument
int poc=60    //Probability of catching fire, chance for each tree to catch fire, modified by -cN argument
int occ=75    //proportion of simulation space that is occupied, modified by -dN argument
int neigh=50  //Proportion of neighbors that above the tree may catch fire, modified by -nN argument
int print=0   //Toggles print mode, if >0, simulation will print that many iterations
int size=15   //size of the simulation grid's rows and columns. Grid will be size*size
///////////////////////////////////////////////



/*Initializes the game board by populating it with trees
**Will add trees randomly to the board until it is populated
**to the proportion of the occ variable.
**@param grid: The board to populate with trees.
*/
void initialize(char grid[size-1][size-1]){
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















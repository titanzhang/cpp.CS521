#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#define SCALE_MAP 2
#define GRID_ROWS 1000 
#define GRID_COLS 1000

// You can change this to use dynamic memory, if you like.
float gridMap[GRID_ROWS][GRID_COLS];


/*************************************************************************
 *                                                                        *
 * Function inputMap converts the input map information into an           *
 * initial occupancy grid, which is stored in gridMap.                    *
 *                        
 * (Here, the "output" parameter says whether to print out the scaled     *
 *  map; output = 1 ==> yes, print;  output = 0 ==> no, don't print.)     *
 *************************************************************************/

void inputMap(int output) 
{
    int i, j, m, n;
    char inputLine1[80], nextChar;
    int width, height, maxVal;

    ifstream inFile("hospital_section.pnm");

    /* Initialize map to 0's, meaning all free space */

    for (m=0; m<GRID_ROWS; m++)
        for (n=0; n<GRID_COLS; n++)   
            gridMap[m][n] = 0.0;

    /* Read past first line */
    inFile.getline(inputLine1,80);

    /* Read in width, height, maxVal */
    inFile >> width >> height >> maxVal;
    cout << "Width = " << width << ", Height = " << height << endl;

    /* Read in map; */
    for (i=0; i<height; i++)    
        for (j=0; j<width; j++) {
	  inFile >> nextChar;
	  if (!nextChar)  
	    gridMap[i/SCALE_MAP][j/SCALE_MAP] = 1.0;
	}
    cout << "Map input complete.\n";

    if (output)  {
      ofstream outFile("scaled_hospital_section.pnm");
      outFile << inputLine1 << endl;
      outFile << width/SCALE_MAP << " " << height/SCALE_MAP << endl
	      << maxVal << endl;

      for (i=0; i<height/SCALE_MAP; i++)
	for (j=0; j<width/SCALE_MAP; j++) {
	  if (gridMap[i][j] == 1.0)
	    outFile << (char) 0;
	  else
	    outFile << (char) -1;
	}
       cout << "Scaled map output to file.\n";
    }
}

int main(int argc, char *argv[])
{
  inputMap(1);  // Here, '1' means to print out the scaled map to a file;
                // If you don't want the printout, pass a parameter of '0'.
}


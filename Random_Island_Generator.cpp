/*
* Christian Shimko
* November 1st, 2021
* This program takes in the users input of many dimensions to randomly generate an island that is specified based on the user input
* Usage: this program, when executed, will output a file named island.txt which has the outputted island generated
*/

#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <time.h>
#include <fstream>

using namespace std;

//Particles that roll to generate the island
//this has a current column and current row to easily search the indexes/positions around it and move to them
struct Particle{
	short maxLife;
	short currCol;
	short currRow;
};

//Rather than an array of shorts I used an array of Cell structs to easily acquire ones index
struct Cell{
	short value;
	short position;
};

//Step one: make grid
Cell** makeParticleMap(short gridWidth, short gridHeight, short dropX,
					short dropY, short dropWidth, short dropHeight,
					short numParticles,short maxLife)
{

	bool isValid; //used to break out of the loop when seaching for a single valid move
	short randRow;	//randomizing initial row
	short randCol;  //randomizing initial col
	randRow = (rand() % dropHeight) + dropY;
	randCol = (rand() % dropWidth) + dropX;
	short positionCount = 1;
	short* randArray = new short[8];
	//create array of particles based on the users input
	Particle* particles;
	particles = new Particle[numParticles];
	//create the array that will further be randomized to randomize the order of which moves get checked in what order
	for(short i = 0; i < 8; i++){
			randArray[i] = i + 1;
	}


	//Set 2D grid
	Cell** Grid;
	Grid = new Cell*[gridHeight];
	//Fill grid
	for (short i = 0;i < gridHeight;i++){
		Grid[i] = new Cell[gridWidth];
	}
	for (short y = 0; y < gridHeight; y++)
	{
		for (short x = 0; x < gridWidth; x++)
		{
			Grid[y][x].value = 0;
			Grid[y][x].position = positionCount;
			positionCount++;
		}
	}

	//Increment initial drop locations value
	Grid[randRow][randCol].value++;

	//this for loop with run as many times as there are particles to be dropped
	for(short i = 0; i < numParticles;i++){
		//since the first particles position was already randomized we now will only randomize the particles past index 0
		if (i > 0){
			randCol = (rand() % dropWidth) + dropX;
			randRow = (rand() % dropHeight) + dropY;
		}
		particles[i].maxLife = maxLife;
		particles[i].currRow = randRow;
		particles[i].currCol = randCol;
		for (short x = particles[i].maxLife; x > 0 ; x--){
			isValid = false;
			//Randomize the contents of randArray each time the particle moves so that when it reaches the switch statement below it will check positions at random
			for(short y = 7; y > 0; y--){
				short temp = 0;
				short swapper = (rand() % 7);
				temp = randArray[y];
				randArray[y] = randArray[swapper];
				randArray[swapper] = temp;
			}

			//this switch will check for valid moves at random because it will iterate through the randomized array above
			//each case is an adjacent position
			for(short p = 0; p < 8; p++){
				switch (randArray[p]){

				case 1:
					if (particles[i].currCol - 1 > -1 && Grid[particles[i].currRow][particles[i].currCol - 1].value <= Grid[particles[i].currRow][particles[i].currCol].value){
						particles[i].currCol -= 1;
						isValid = true;
					}
					break;
				case 2:
					if (particles[i].currCol + 1 < gridWidth && Grid[particles[i].currRow][particles[i].currCol + 1].value <= Grid[particles[i].currRow][particles[i].currCol].value){
						particles[i].currCol += 1;
						isValid = true;
					}
					break;
				case 3:
					if (particles[i].currRow + 1 < gridHeight && Grid[particles[i].currRow + 1][particles[i].currCol].value <= Grid[particles[i].currRow][particles[i].currCol].value){
						particles[i].currRow += 1;
						isValid = true;
					}
					break;
				case 4:
					if (particles[i].currRow - 1 > -1 && Grid[particles[i].currRow - 1][particles[i].currCol].value <= Grid[particles[i].currRow][particles[i].currCol].value ){
						//cout << "true 4";
						particles[i].currRow -= 1;
						isValid = true;
					}
					break;
				case 5:
					if (particles[i].currRow - 1 > -1 && particles[i].currCol - 1 > -1  && Grid[particles[i].currRow - 1][particles[i].currCol - 1].value <= Grid[particles[i].currRow][particles[i].currCol].value){
						particles[i].currRow -= 1;
						particles[i].currCol -= 1;
						isValid = true;
					}
					break;
				case 6:
					if (particles[i].currCol + 1 < gridWidth && particles[i].currRow - 1 > -1 && Grid[particles[i].currRow - 1][particles[i].currCol + 1].value <= Grid[particles[i].currRow][particles[i].currCol].value ){
						particles[i].currRow -= 1;
						particles[i].currCol += 1;
						isValid = true;
					}
					break;
				case 7:
					if ( particles[i].currRow + 1 < gridHeight && particles[i].currCol - 1 > -1 && Grid[particles[i].currRow + 1][particles[i].currCol - 1].value <= Grid[particles[i].currRow][particles[i].currCol].value){
						particles[i].currRow += 1;
						particles[i].currCol -= 1;
						isValid = true;
					}
					break;
				case 8:
					if (particles[i].currRow + 1 < gridHeight && particles[i].currCol + 1 < gridWidth && Grid[particles[i].currRow + 1][particles[i].currCol + 1].value <= Grid[particles[i].currRow][particles[i].currCol].value ){
						particles[i].currRow += 1;
						particles[i].currCol += 1;
						isValid = true;
					}
					break;
				}
				if(isValid == true){
					break;
				}
				else{
					x = 0;
				}
			}
			cout << particles[i].currRow << "\n" << particles[i].currCol;
			Grid[particles[i].currRow][particles[i].currCol].value++;
		}
	}
	return Grid;
}


//Step 2: find the max value in the grid in order to normalize
short maxVal(Cell** Array, short dropX, short dropY, short dropWidth, short dropHeight)
{
	short max = 0;
	//I shrunk the algorithm down to just searching inside the drop box because the highest value must be the most frequently hit cell in the most densely populated area of the grid
	for (short y = dropY; y < dropHeight + dropY;y++){
		for (short x = dropX; x < dropWidth + dropX;x++){
			if (Array[y][x].value > max){
				max = Array[y][x].value;
			}
		}
	}

	return max;
}


//Step 3: normalize the values in the map before polish
Cell** normalizeMap(Cell** Array, short max, short Width, short Height)
{
	Cell** newArray = Array;
	double conversion = 255/max;

	for (short y = 0; y < Height; y++){
		for (short x = 0; x < Width; x++){
			newArray[y][x].value = newArray[y][x].value*conversion;
		}
	}

	return newArray;
}


//Step 4: convert to symbols
char** polishMap(Cell** Array, short Width, short Height, short water)
{
	char** polishedMap = new char*[Height];

	for (short y = 0; y < Height; y++){
		polishedMap[y] = new char[Width];
	}



	for (short y = 0; y < Height; y++){
		for (short x = 0; x < Width; x++){

			if (Array[y][x].value < water){
				if (Array[y][x].value < water/2){
					polishedMap[x][y] = '#';
				}
				else{
					polishedMap[x][y] = '~';
				}
			}
			else if (Array[y][x].value < (water + (((255-water)/100) * 15))){
				polishedMap[x][y] = '.';
			}
			else if (Array[y][x].value < (water + (((255-water)/100) * 40))){
				polishedMap[x][y] = '-';
			}
			else if (Array[y][x].value < (water + (((255-water)/100) * 80))){
				polishedMap[x][y] = '*';
			}
			else{
				polishedMap[x][y] = '^';
			}

		}


	}
	return polishedMap;

}



int main()
{
	short
	gridWidth,
	gridHeight,
	dropX,
	dropY,
	dropWidth,
	dropHeight,
	numParticles,
	maxLife,
	waterLine;
	Cell** ParticleMap;
	char** polishedMap;
	ofstream out;

	srand(time(0));

	//Prompt for input
	cout << "Welcome to Christian�s CSE240 island generator!\n\n" <<
			"Enter grid width: \n";
	cin >> gridWidth;
	cout << "Enter grid height: \n";
	cin >> gridHeight;
	cout << "Enter drop-window x-coordinate (0 - <width>): \n";
	cin >> dropX;
	cout << "Enter drop-window y-coordinate (0 - <height>): \n";
	cin >> dropY;
	cout << "Enter drop-window width (>= 1): \n";
	cin >> dropWidth;
	cout << "Enter drop-window height (>= 1): \n";
	cin >> dropHeight;
	cout << "Enter number of particles to drop: \n";
	cin >> numParticles;
	cout << "Enter max life of particles: \n";
	cin >> maxLife;
	cout << "Enter value for waterline (40-200): \n";
	cin >> waterLine;
	while (waterLine > 200 || waterLine < 40){
		cout << "The water line must be between 40 and 200.\n";
		cin >> waterLine;
	}


	ParticleMap = makeParticleMap(gridWidth,gridHeight,dropX, dropY,dropWidth,dropHeight,numParticles, maxLife);
	polishedMap = polishMap(normalizeMap(ParticleMap, maxVal(ParticleMap, dropX, dropY, dropWidth, dropHeight), gridWidth, gridHeight), gridWidth, gridHeight, waterLine);

	for (short y = 0;y < gridHeight; y++){
			for (short x = 0;x < gridWidth; x++){
				cout << polishedMap[y][x];
			}
			cout << "\n";
	}

	out.open("Island.txt");

	for (short y = 0;y < gridHeight; y++){
			for (short x = 0;x < gridWidth; x++){
				out << polishedMap[y][x];
			}
			out << "\n";
	}

	out.close();




	return 0;
}



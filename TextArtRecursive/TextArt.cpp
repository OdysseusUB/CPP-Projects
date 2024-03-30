/*
* This program allows display and editing of text art (also called ASCII art).
*/

#include <iostream>
#include <fstream>
#include <cctype>
#include <windows.h>
#include <conio.h>
#include "Definitions.h"
#pragma warning(disable:4996)
using namespace std;


int main()
{
	// Creates two canvas':
	//		main canvas that will be displayed at all times
	//		old canvas that will store previous state of canvas
	char canvas[MAXROWS][MAXCOLS];
	char oldCanvas[MAXROWS][MAXCOLS];

	initCanvas(canvas) ;
	initCanvas(oldCanvas) ;

	bool animate = false ;

	char user_entry = 'a';

	while (user_entry != 'q')
	{
		// Displays empty canvas to user
		displayCanvas(canvas);
		cout << "\n";

		// Displays menu to user
		printMenu(mainMenu) ;
		cin >> user_entry;

		user_entry = tolower(user_entry) ;

		// checks which menu user wants
		if (user_entry != 'd')
		{
			menuOne(canvas, oldCanvas, user_entry);
		}
		else
		{
			menuTwo(canvas, oldCanvas, animate);
		}
 
	}

	return 0;
}


void gotoxy(short row, short col)
{
	COORD pos = { col, row };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void clearLine(int lineNum, int numOfChars)
{
	// Move cursor to the beginning of the specified line on the console
	gotoxy(lineNum, 0);

	// Write a specified number of spaces to overwrite characters
	for (int x = 0; x < numOfChars; x++)
		cout << " ";

	// Move cursor back to the beginning of the line
	gotoxy(lineNum, 0);
}

void replace(char canvas[][MAXCOLS], char oldCh, char newCh)
{
	// finds every instance of the old character given by user and 
	// replaces with the new character given by user
	for (int i = 0; i < MAXROWS * MAXCOLS; i++)
	{
		if (canvas[0][i] == oldCh)
		{
			canvas[0][i] = newCh;
		}
	}
}

void editCanvas(char canvas[][MAXCOLS])
{
	char input = 'a';
	int row = 0, col = 0;

	// Move cursor to row,col and then get
	// a single character from the keyboard
	clearLine(MAINSCREENLENGTH, MAINSCREENWIDTH); 
	printf("Press <ESC> to stop editing");

	gotoxy(row, col);

	while (input != ESC)
	{

		switch (input = _getch())
		{
		case 0:
			cin.ignore(input);
			input = _getch();
			break;

		case UPARROW:
			if (row > 0)
			{
				gotoxy(--row, col);        // Up arrow
			}
			break;

		case DOWNARROW:
			if (row < MAXROWS - 1)
			{
				gotoxy(++row, col);        // Down arrow
			}
			break;

		case LEFTARROW:
			if (col > 0)
			{
				gotoxy(row, --col);        // Left arrow
			}
			break;

		case RIGHTARROW:
			if (col < MAXCOLS - 1)
			{
				gotoxy(row, ++col);        // Right arrow
			}
			break;

		case ESC:						  // ESC to return to main function
			return ;

		default:						  // default case to allow user to input characters using gotoxy and swap functions.
			if (input >= 32 && input <= 126)
			{
				cout << input;
				swap(canvas[row][col], input);
				gotoxy(row, col);
			}
		}
	}
}

void moveCanvas(char canvas[][MAXCOLS], int rowValue, int colValue)
{
	// Creates a new canvas that is initialized same as orginal
	char childCanvas[MAXROWS][MAXCOLS];
	initCanvas(childCanvas);

	for (int i = 0; i < MAXROWS; i++) // loops through each row
	{
		for (int j = 0; j < MAXCOLS; j++) // loops through each column
		{
			int rowBoundary = i + rowValue; // sets boundary for rows
			int colBoundary = j + colValue; // sets boundary for columns

			if (rowBoundary < MAXROWS && colBoundary < MAXCOLS)
			{
				if (rowBoundary >= 0 && colBoundary >= 0)
				{
					childCanvas[i + rowValue][j + colValue] = canvas[i][j]; // Transfers contents of cell to what we want if it's within boundaries
				}
			}
		}
	}

	// Transfers the 'moved' canvas into our real canvas ; psuedo return for canvas
	copyCanvas(canvas, childCanvas);
}

void initCanvas(char canvas[][MAXCOLS])
{	
	// Inserts an empty space character into each cell of the canvas
	for (int i = 0; i < MAXROWS; i++)
	{
		for (int j = 0; j < MAXCOLS; j++)
		{
			canvas[i][j] = ' ';
		}
	}
}

void displayCanvas(char canvas[][MAXCOLS])
{
	// Clear the screen
	system("cls");

	// Prints the array of i rows and j columns to the screen
	for (int i = 0; i < MAXROWS; i++)
	{
		for (int j = 0; j < MAXCOLS; j++)
		{
			cout << canvas[i][j];
		}
		cout << endl;
	}

	// uses gotoxy function to go to the last row and prints the boundary line along each column value
	for (int i = 0; i < MAXCOLS; i++)
	{
		gotoxy(MAXROWS, i);
		cout << '-';
	}

	// uses gotoxy function to go to the last column and print the boundary line along each row value.
	for (int i = 0; i < MAXROWS; i++)
	{
		gotoxy(i, MAXCOLS);
		cout << '|';
	}

	cout << endl;

}

void copyCanvas(char to[][MAXCOLS], char from[][MAXCOLS])
{
	// Copies contents of the from canvas into the to canvas
	for (int i = 0; i < MAXROWS * MAXCOLS; i++)
	{
		to[0][i] = from[0][i];
	}
}

void saveCanvas(char canvas[][MAXCOLS])
{

	char entry[FILENAMESIZE - 14]; // Raw user entry
	char savedFiles[] = "SavedFiles\\"; // beginning text to finalEntry 
	char txt[] = ".txt"; // ending text to finalEntry

	char finalEntry[FILENAMESIZE]; // Used for finding the actual file

	// Output / Input or file name
	printf("Please enter the name of the file to save: (without .txt): ");
	cin.getline(entry, FILENAMESIZE - 14);

	strcpy(finalEntry, savedFiles); // Copies the savedFile to our finalEntry

	strcat(finalEntry, entry); // Adds our raw user text into the finalEntry after the savedFile

	strcat(finalEntry, txt); // Adds .txt to the end to make it useable

	ofstream outFile; // Creates the output file

	outFile.open(finalEntry); // Tries to open the file we've creates

	if (!outFile) // Tests connection
	{
		cerr << "ERROR: FILE CANNOT BE WRITTEN";
		cout << endl;
		system("pause");
	}

	else // Triggers if there is a connection
	{
		int colNum = 0;
		int rowNum = 0;

		// Loops through every position of canvas and outputs it onto the file
		while (rowNum < 22)
		{
			while (colNum < 80)
			{
				outFile << canvas[rowNum][colNum];
				colNum++;
			}
			outFile << '\n';
			colNum = 0;
			rowNum++;
		}

		// Output to user
		cout << "File has been created!\n";
		cin.ignore();

		outFile.close();

	}

}

void loadCanvas(char canvas[][MAXCOLS])
{
	char entry[FILENAMESIZE - 14]; // Raw user entry
	char savedFiles[] = "SavedFiles\\"; // beginning text to finalEntry 
	char txt[] = ".txt"; // ending text to finalEntry

	char finalEntry[FILENAMESIZE]; // Used for finding the actual file

	// Output / Input or file name
	printf("Enter a filename: (without .txt): ");
	cin.getline(entry, FILENAMESIZE);

	strcpy(finalEntry, savedFiles); // Copies the savedFile to our finalEntry

	strcat(finalEntry, entry); // Adds our raw user text into the finalEntry after the savedFile

	strcat(finalEntry, txt); // Adds .txt to the end to make it useable

	ifstream inFile; // Creates the input file variable

	inFile.open(finalEntry); // Establishes a connection between our variable and the file

	if (!inFile) // Triggers if file cannot be opened
	{
		cerr << "ERROR: CANNOT OPEN FILE";
		cout << endl;
		system("pause");
	}

	else
	{
		initCanvas(canvas); // Creates a clean slate to open the file onto

		char c = inFile.get();
		int colNum = 0;
		int rowNum = 0;

		// While we havent reached the end of the file
		while (!inFile.eof())
		{
			if (c != '\n') // Makes sure not to store new-line characters
			{
				canvas[rowNum][colNum] = c;
				colNum++;
			}
			else if (colNum == 79)
			{
				colNum = 0;
				rowNum++;
			}
			else
			{
				colNum = 0;
				rowNum++;
			}
			c = inFile.get();
		}

		inFile.close(); // Closes file
	}
}

void printMenu(const char menu[])
{
	for (int i = 0; i < strlen(menu); i++)
	{
		cout << menu[i];
	}

}

void menuOne(char canvas[][MAXCOLS], char oldCanvas[][MAXCOLS], char entry)
{

	// Used to track user changes in cases of replace and move
	char oldChar, newChar;
	int oldRow, oldCol;

	cin.ignore();

	switch (entry)
	{
	case 'e': // Triggers the edit function
		copyCanvas(oldCanvas, canvas);
		editCanvas(canvas);
		break;

	case 'm': // Triggers the move function
		clearLine(MAINSCREENLENGTH, MAINSCREENWIDTH);

		// Output / Input for row number 
		cout << "Please enter row number: " << endl;
		cin >> oldRow;

		// Data Validation
		while (cin.fail())
		{
			cin.clear();
			cin.ignore((numeric_limits<streamsize>::max)(), '\n');
			cin >> oldRow;
		}

		// Output / Input for column number
		cout << "Please enter a column number: " << endl;
		cin >> oldCol;

		// Data Validation
		while (cin.fail())
		{
			cin.clear();
			cin.ignore((numeric_limits<streamsize>::max)(), '\n');
			cin >> oldCol;
		}

		copyCanvas(oldCanvas, canvas);
		moveCanvas(canvas, oldRow, oldCol);
		displayCanvas(canvas);

		break;

	case 'r':// Triggers the replace function

		clearLine(MAINSCREENLENGTH, MAINSCREENWIDTH);

		// Output / Input for old and new character from user
		cout << "Please enter the character you want to replace: " << endl;
		oldChar = getch() ;
		cout << oldChar << endl ;

		cout << "Please enter the new character: " << endl;
		newChar = getch() ;
		cout << newChar << endl ;

		copyCanvas(oldCanvas, canvas);
		replace(canvas, oldChar, newChar);

		break;

	case 'u':// Triggers the undo function
		// Copies the old canvas to the current canvas and displays it
		copyCanvas(canvas, oldCanvas);
		displayCanvas(canvas);
		break;

	case 'l': // Triggers the load function
		clearLine(MAINSCREENLENGTH, MAINSCREENWIDTH);
		copyCanvas(oldCanvas, canvas);
		loadCanvas(canvas);
		break;

	case 's': // Triggers the save function
		clearLine(MAINSCREENLENGTH, MAINSCREENWIDTH) ;
		saveCanvas(canvas);
		break;
	case 'c':
		copyCanvas(oldCanvas, canvas);
		initCanvas(canvas);

	case 'q': // Used for ending program
		break;

	default: // Standard error message for wrong input
		cout << "ERROR: INCORRECT INPUT";
		cout << endl;
		system("pause");
		clearLine(MAINSCREENLENGTH, MAINSCREENWIDTH) ;
		break;
	}
}


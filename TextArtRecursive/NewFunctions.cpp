#include <iostream>
#include <windows.h>
#include <conio.h>
#include "Definitions.h"
using namespace std;


Point::Point(DrawPoint p)
{
	row = (int)round(p.row);
	col = (int)round(p.col);
}

// https://math.stackexchange.com/questions/39390/determining-end-coordinates-of-line-with-the-specified-length-and-angle
DrawPoint findEndPoint(DrawPoint start, int len, int angle)
{
	DrawPoint end;
	end.col = start.col + len * cos(degree2radian(angle));
	end.row = start.row + len * sin(degree2radian(angle));
	return end;
}

// Use this to draw characters into the canvas, with the option of performing animation
void drawHelper(char canvas[][MAXCOLS], Point p, char ch, bool animate)
{
	// Pause time between steps (in milliseconds)
	const int TIME = 50;

	// Make sure point is within bounds
	if (p.row >= 0 && p.row < MAXROWS && p.col >= 0 && p.col < MAXCOLS)
	{
		// Draw character into the canvas
		canvas[p.row][p.col] = ch;

		// If animation is enabled, draw to screen at same time
		if (animate)
		{
			gotoxy(p.row, p.col);
			printf("%c", ch);
			Sleep(TIME);
		}
	}
}

// Fills gaps in a row caused by mismatch between match calculations and screen coordinates
// (i.e. the resolution of our 'canvas' isn't very good)
void drawLineFillRow(char canvas[][MAXCOLS], int col, int startRow, int endRow, char ch, bool animate)
{
	// determine if we're counting up or down
	if (startRow <= endRow)
		for (int r = startRow; r <= endRow; r++)
		{
			Point point(r, col);
			drawHelper(canvas, point, ch, animate);
		}
	else
		for (int r = startRow; r >= endRow; r--)
		{
			Point point(r, col);
			drawHelper(canvas, point, ch, animate);
		}
}

// Draw a single line from start point to end point
void drawLine(char canvas[][MAXCOLS], DrawPoint start, DrawPoint end, bool animate)
{
	char ch;

	Point scrStart(start);
	Point scrEnd(end);

	// vertical line
	if (scrStart.col == scrEnd.col)
	{
		ch = '|';

		drawLineFillRow(canvas, scrStart.col, scrStart.row, scrEnd.row, ch, animate);
	}
	// non-vertical line
	else
	{
		int row = -1, prevRow;

		// determine the slope of the line
		double slope = (start.row - end.row) / (start.col - end.col);

		// choose appropriate characters based on 'steepness' and direction of slope
		if (slope > 1.8)  ch = '|';
		else if (slope > 0.08)  ch = '`';
		else if (slope > -0.08)  ch = '-';
		else if (slope > -1.8) ch = '\'';
		else ch = '|';

		// determine if columns are counting up or down
		if (scrStart.col <= scrEnd.col)
		{
			// for each column from start to end, calculate row values
			for (int col = scrStart.col; col <= scrEnd.col; col++)
			{
				prevRow = row;
				row = (int)round(slope * (col - start.col) + start.row);

				// draw from previous row to current row (to fill in row gaps)
				if (prevRow > -1)
				{
					drawLineFillRow(canvas, col, prevRow, row, ch, animate);
				}
			}
		}
		else
		{
			// for each column from start to end, calculate row values
			for (int col = scrStart.col; col >= scrEnd.col; col--)
			{
				prevRow = row;
				row = (int)round(slope * (col - start.col) + start.row);

				// draw from previous row to current row (to fill in row gaps)
				if (prevRow > -1)
				{
					drawLineFillRow(canvas, col, prevRow, row, ch, animate);
				}
			}
		}
	}
}

// Draws a single box around a center point
void drawBox(char canvas[][MAXCOLS], Point center, int height, bool animate)
{
	int sizeHalf = height / 2;
	int ratio = (int)round(MAXCOLS / (double)MAXROWS * sizeHalf);

	// Calculate where the four corners of the box should be
	DrawPoint points[4];
	points[0] = DrawPoint(center.row - sizeHalf, center.col - ratio);
	points[1] = DrawPoint(center.row - sizeHalf, center.col + ratio);
	points[2] = DrawPoint(center.row + sizeHalf, center.col + ratio);
	points[3] = DrawPoint(center.row + sizeHalf, center.col - ratio);

	// Draw the four lines of the box
	for (int x = 0; x < 3; x++)
	{
		drawLine(canvas, points[x], points[x + 1], animate);
	}
	drawLine(canvas, points[3], points[0], animate);

	// Replace the corners with a better looking character
	for (int x = 0; x < 4; x++)
	{
		drawHelper(canvas, points[x], '+', animate);
	}
}

// Menu for the drawing tools
void menuTwo(char canvas[][MAXCOLS], char oldCanvas[][MAXCOLS], bool& animate)
{
	// Message used for box function
	char boxSizeMessage[] = "Enter Size: ";

	// Message used for nested box function
	char largestBoxMessage[] = "Enter size of the largest box: ";

	// Message used for tree function
	char treeSizeMessage[] = "Enter approximate tree height : ";
	char branchAngleMessage[] = "Enter branch angle: ";

	// Points used for 2-point functions
	Point startPoint ;
	Point endPoint ;

	// Point for 1-point functions
	Point centerPoint ; 

	// User-entered variable used for box functions
	int boxSize ;

	// User-entered variables used for tree functions
	int treeSize ;
	int branchAngle ; 

	// User-entered variables used for fill function
	char oldChar;
	char newChar;

	// Used to check if the user entered the C option for certain functions
	char cCheck; 

	char user_entry = 'g';

	while (user_entry != 'm')
	{
		clearLine(ALTSCREENLENGTH, ALTSCREENWIDTH);

		// Checks which menu we should display based off of animate
		if (!animate)
			printMenu(altMenu1);
		else
			printMenu(altMenu2);

		cin.ignore();

		cin >> user_entry;

		switch (user_entry)
		{

		case 'a': // Antimate function

			clearLine(ALTSCREENLENGTH, ALTSCREENWIDTH);

			// Switches animate variable if pressed by user
			if (!animate)
				animate = true;
			else
				animate = false;

			break;

		case 'f': // Fill function

			// Backs up current canvas in case of an undo
			clearLine(ALTSCREENLENGTH, ALTSCREENWIDTH);

			// Gathers box size from user
			printf("Enter character to fill with from current location / <ESC> to cancel");
			newChar = getPoint(centerPoint);

			// Gathers previous character in the point from old canvas
			oldChar = oldCanvas[centerPoint.row][centerPoint.col];

			// Validates the point to make sure its not ESC
			if (!pointValidation(centerPoint, canvas, oldCanvas))
			{
				displayCanvas(canvas) ;
				break;
			}
						
			// Draws box and displays it to the user 
			copyCanvas(oldCanvas, canvas);
			fillRecursive(canvas, centerPoint.row, centerPoint.col, oldChar, newChar, animate);
			displayCanvas(canvas);

			break;

		case 'l': // Line function

			// Backs up current canvas in case of an undo
			clearLine(ALTSCREENLENGTH, ALTSCREENWIDTH);

			// Gathers first point from user
			printf("Type any letter to choose a start point / <ESC> to cancel");
			getPoint(startPoint); 

			// Validates the point to make sure its not ESC
			if (!pointValidation(startPoint, canvas, oldCanvas))
			{
				displayCanvas(canvas);
				break;
			}

			// Gathers second point from user
			clearLine(ALTSCREENLENGTH, ALTSCREENWIDTH);
			printf("Type any letter to choose a end point / <ESC> to cancel");
			getPoint(endPoint);

			// Validates the point to make sure its not ESC
			if (!pointValidation(endPoint, canvas, oldCanvas))
			{
				canvas[startPoint.row][startPoint.col] = oldCanvas[startPoint.row][startPoint.col] ;
				displayCanvas(canvas) ;
				break;
			}

			// Draws the line and displays it to the user
			copyCanvas(oldCanvas, canvas);
			drawLine(canvas, startPoint, endPoint, animate);
			displayCanvas(canvas);

			break;

		case 'b': // Box function

			// Backs up current canvas in case of an undo
			clearLine(ALTSCREENLENGTH, ALTSCREENWIDTH);

			// Displays message to gather info from user
			displayMessage(boxSizeMessage) ;

			// Gathers boxSize from user
			boxSize = dataValidation(boxSizeMessage, ALTSCREENLENGTH, ALTSCREENWIDTH) ;

			// Gathers center point from user
			clearLine(ALTSCREENLENGTH, ALTSCREENWIDTH);
			printf("Type any letter to choose box center, or <C> for screen center / <ESC> to cancel");
			cCheck = getPoint(centerPoint) ;

			// Validates the point to make sure its not ESC
			if (!pointValidation(centerPoint, canvas, oldCanvas))
			{
				displayCanvas(canvas);
				break;
			}
			
			// Checks to see if C was entered and centers the point if it was
			checkForCenter(centerPoint, cCheck);

			// Draws box and displays it to the user 
			copyCanvas(oldCanvas, canvas) ;
			drawBox(canvas, centerPoint, boxSize, animate);
			displayCanvas(canvas);

			break;

		case 'n': // Nested Boxes function

			// Backs up current canvas in case of an undo
			clearLine(ALTSCREENLENGTH, ALTSCREENWIDTH);

			// Displays message to gather info from user
			displayMessage(largestBoxMessage);

			// Gathers boxSize from user
			boxSize = dataValidation(largestBoxMessage, ALTSCREENLENGTH, ALTSCREENWIDTH);

			// Gathers center point from user
			clearLine(ALTSCREENLENGTH, ALTSCREENWIDTH);
			printf("Type any letter to choose box center, or <C> for screen center / <ESC> to cancel");
			cCheck = getPoint(centerPoint);

			// Validates the point to make sure its not ESC
			if (!pointValidation(centerPoint, canvas, oldCanvas))
			{
				displayCanvas(canvas) ;
				break;
			}

			// Checks to see if C was entered and centers the point if it was
			checkForCenter(centerPoint, cCheck);

			// Draws box and displays it to the user 
			copyCanvas(oldCanvas, canvas);
			drawBoxesRecursive(canvas, centerPoint, boxSize, animate);
			displayCanvas(canvas);

			break;

		case 't': // Tree function

			// Backs up current canvas in case of an undo
			clearLine(ALTSCREENLENGTH, ALTSCREENWIDTH);

			// Displays message to gather info from user
			displayMessage(treeSizeMessage) ;

			// Gathers treeSize from user
			treeSize = dataValidation(treeSizeMessage, ALTSCREENLENGTH, ALTSCREENWIDTH);

			// Gathers branch angle from user
			clearLine(ALTSCREENLENGTH, ALTSCREENWIDTH);

			// Displays message to gather info from user
			displayMessage(branchAngleMessage) ;

			// Gathers branchAngle from user
			branchAngle = dataValidation(branchAngleMessage, ALTSCREENLENGTH, ALTSCREENWIDTH);

			// Gathers center point from user
			clearLine(ALTSCREENLENGTH, ALTSCREENWIDTH);
			printf("Type any letter to choose box center, or <C> for screen center / <ESC> to cancel");
			cCheck = getPoint(centerPoint);

			// Validates the point to make sure its not ESC
			if (!pointValidation(centerPoint, canvas, oldCanvas))
			{
				displayCanvas(canvas);
				break;
			}

			// Checks to see if C was entered and centers the point at the base of the canvas if it was
			if (checkForCenter(centerPoint, cCheck))
			{
				centerPoint.row = 21 ;
			}

			// Draws box and displays it to the user 
			copyCanvas(oldCanvas, canvas);
			treeRecursive(canvas, centerPoint, treeSize, 270, branchAngle, animate);
			displayCanvas(canvas);

			break ;

		case 'm': // Exits menu two

			break ;

		default: // Standard error message for wrong input

			cerr << "ERROR: INCORRECT INPUT";
			cout << endl;
			system("pause");  

			for (int i = 0; i < 3; i++)
			{
				clearLine(ALTSCREENLENGTH + i, ALTSCREENWIDTH); 
			}

			break;
		}
	}
}

// Displays the various messages to direct the user
void displayMessage(const char message[])
{
	for (int i = 0; i < strlen(message); i++)
	{
		cout << message[i] ;
	}
}

// Checks the point the user entered to see if they want it centered
bool checkForCenter(Point& pointToCheck, char checkForC)
{
	// Checks to see if the character passed into the function is C
	// Centers point properties if it is
	if (checkForC == 'c' || checkForC == 'C')
	{
		pointToCheck.row = 11;
		pointToCheck.col = 40;
		return true ; 
	}
	return false ; 
}

// Validates point from user to make sure they didnt press ESC
bool pointValidation(Point& pointToCheck, char canvas[][MAXCOLS], char oldCanvas[][MAXCOLS])
{
	// Checks to see if point has invalid properties
	if (pointToCheck.row == INVALID)
	{
		//canvas[pointToCheck.row][pointToCheck.col] = oldCanvas[pointToCheck.row][pointToCheck.col];
		return false ;
	}

	// Acts as a backup
	if (pointToCheck.col == INVALID)
	{
		//canvas[pointToCheck.row][pointToCheck.col] = oldCanvas[pointToCheck.row][pointToCheck.col];
		return false ;
	}

	// Point is valid
	return true ;
}

// Validates user entries to make sure it is integer
int dataValidation(const char message[], int screenLength, int screenWidth)
{
	int returnValue ;

	cin >> returnValue ;

	// Data Validation
	while (cin.fail())
	{
		clearLine(screenLength, screenWidth);
		
		for (int i = 0; i < strlen(message); i++)
		{
			cout << message[i] ;
		}

		cin.clear();
		cin.ignore((numeric_limits<streamsize>::max)(), '\n');
		cin >> returnValue ;
	}

	return returnValue ;
}

// Get a single point from screen, with character entered at that point
char getPoint(Point& pt)
{
	// Creates user input variable
	char input = 'a';
	int row = 0, col = 0;

	// Variable that will track if ESC is pressed or if user uses center feature
	bool continueCheck = true;

	// Puts cursor at the beginning of the canvas
	gotoxy(row, col);

	while (input != ESC && continueCheck == true)
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
				gotoxy(--row, col);				// Up arrow
			}
			break;

		case DOWNARROW:
			if (row < MAXROWS - 1)
			{
				gotoxy(++row, col);				// Down arrow
			}
			break;

		case LEFTARROW:
			if (col > 0)
			{
				gotoxy(row, --col);				// Left arrow
			}
			break;

		case RIGHTARROW:
			if (col < MAXCOLS - 1)
			{
				gotoxy(row, ++col);				// Right arrow
			}
			break;

		case ESC:								// Sets points to invalid coordinates
			pt.row = INVALID ;
			pt.col = INVALID ;
			break ;

		default:								// default case to allow user to input characters using gotoxy and swap functions.
			if (input >= 32 && input <= 126)
			{
				cout << input;
				gotoxy(row, col);
				pt.row = row;
				pt.col = col;
				continueCheck = false ; 
			}
		}
	}
	return input;
}

// Recursively fill a section of the screen
void fillRecursive(char canvas[][MAXCOLS], int row, int col, char oldCh, char newCh, bool animate)
{
	if (canvas[row][col] == oldCh) // Recursive case: The canvas position we are evaluating is the character we want to replace
	{
		// Creates a point that has the qualities of the current cell we are evaluating in the canvas
		Point fillpoint;

		// Transfers the properties of our cell in the canvas to the point we created
		fillpoint.row = row;
		fillpoint.col = col;

		// Function used to print the new character to the screen
		drawHelper(canvas, fillpoint, newCh, animate);

		
		// Checks east of our point 
		if (fillpoint.row + 1 < MAXROWS && fillpoint.col < MAXCOLS)
			fillRecursive(canvas, row + 1, col, oldCh, newCh, animate);

		// Checks north of our point
		if (fillpoint.row < MAXROWS && fillpoint.col + 1 < MAXCOLS)
			fillRecursive(canvas, row, col + 1, oldCh, newCh, animate);

		// Checks west of our point
		if (fillpoint.row - 1 >= 0 && fillpoint.col < MAXCOLS)
			fillRecursive(canvas, row - 1, col, oldCh, newCh, animate);

		// Checks south of our point
		if (fillpoint.row < MAXROWS && fillpoint.col  - 1 >= 0)
			fillRecursive(canvas, row, col - 1, oldCh, newCh, animate);

	}

	else // Base Case: The end of the North, West, South, or East positions has been met and we need to move to the next position to evaluate by going up the stack
	{

		return ;

	}
}

// Recursively draw nested boxes
void drawBoxesRecursive(char canvas[][MAXCOLS], Point center, int height, bool animate)
{
	if (height <= 3) // Base Case: We have reached the smallest box possible with a height of 2
	{
		drawBox(canvas, center, height, animate);
	}
	else // Recursive Case: Draw a box at current height and draw a box with 2 less height
	{
		drawBoxesRecursive(canvas, center, height - 2, animate);
		drawBox(canvas, center, height, animate);
	}
}

// Recursively draw a tree
void treeRecursive(char canvas[][MAXCOLS], DrawPoint start, int height, int startAngle, int branchAngle, bool animate)
{
	if (start.col >= MAXCOLS || start.row >= MAXROWS)			// Base Case: Makes sure we are in the boundaries of the canvas
	{
		return ;
	}
	else if(start.col < 0 || start.row < 0)						// Base Case: Makes sure we are in the boundaries of the canvas
	{
		return ;
	}

	else if(height < 3)											// Base Case: Starts to return the stack if height becomes too small
	{
		return ; 
	}

	else														// Recursive Case: Draw 3 lines: One trunk, two branches
	{
		// Creates a valid starting angle for drawn lines
		int newStartAngle = startAngle % 360 ;

		// Creates the endpoint used for the the lines
		DrawPoint EndPoint = findEndPoint(start, height / 3, newStartAngle) ;

		// Draws the line for both trunk and branches
		drawLine(canvas, start, EndPoint, animate) ;

		// Draws the first branch
		treeRecursive(canvas, EndPoint, height - 2, newStartAngle + branchAngle, branchAngle, animate) ;

		// Draws the second branch
		treeRecursive(canvas, EndPoint, height - 2, newStartAngle - branchAngle, branchAngle, animate) ;

	}
}

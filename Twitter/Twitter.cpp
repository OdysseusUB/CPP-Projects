// Program to implement a very simplified list of tweets 
// from a single simulated Twitter account
// Tweets can be added, deleted, and liked




/*
Grant Melvin - Display | Delete
Jaxon Bauer - selectTweet | getNextId
Todd Rose - DoEditTweet
Ulysses Bueno - doLikeTweet | doAddTweet
*/




#include <iostream>
#include<string>
using namespace std;

const int MSGSIZE = 100;	// Maximum size for a tweet message
const int CAPACITY = 10;	// Maximum number of tweets

// Structure used to define a tweet entry
struct Tweet
{
	int id;
	char msg[MSGSIZE];
	int likes;
};


/*
* Prints out an entire timeline to the screen
* timeline = timeline of tweets to be printed
* usedSize = number of tweets in the timeline
* selected = position number of currently selected tweet
*/
void displayTimeline(const Tweet timeline[], int usedSize, int selected);


/*
* Edits currently selected tweet
* with a new message entered by the user.
* timeline = timeline in which the tweet is to be edited
* usedSize = number of tweets in the timeline
* selected = position number of currently selected tweet
* If 'selected' represents a valid array position, the
* selected tweet will be updated.
* If 'selected' is not valid a 'no tweet is selected message' will be
* displayed and no changes will be made.
*/
void doEditTweet(Tweet timeline[], int usedSize, int selected);


/*
* Adds a like to the currently selected tweet.
* timeline = timeline in which the tweet is to be edited
* usedSize = number of tweets in the timeline
* selected = position number of currently selected tweet
* If 'selected' represents a valid array position, the
* selected tweet will be updated.
* If 'selected' is not valid a 'no tweet is selected message' will be
* displayed and no changes will be made.
*/
void doLikeTweet(Tweet timeline[], int usedSize, int selected);


/*
* Deleted currently selected tweet.
* timeline = timeline in from which the entry is to be deleted
* usedSize = number of tweets in the timeline
* If 'selected' represents a valid array position:
*   the selected tweet will be deleted
*   usedSize will be updated to reflect the updated number of tweets in the timeline
*   selected will be updated to -1
* If 'selected' is not valid a 'no tweet is selected message' will be
* displayed and no changes will be made.
*/
void doDeleteTweet(Tweet timeline[], int& usedSize, int& selected);


/*
* If there is room in the timeline for new tweets, then this gets
* a new tweet from the user and adds it to the timeline.
* timeline = timeline in which the tweet is to be added
* usedSize = number of tweets in the timeline
* If tweet was able to be added, returns the position number in the
* timeline of where the item was added, and usedSize will be
* updated to reflect the number of tweets now in the timeline.
* If tweet was not able to be added, returns -1, and usedSize
* remains unchanged.
*/
int doAddTweet(Tweet timeline[], int& usedSize);


/*
* Adds a new tweet to the list
* timeline = timeline in which the entry is to be added
* usedSize = number of tweets in the timeline
* message = tweet message to be added
* If tweet was able to be added, returns the position number in the
* timeline of where the item was added, and usedSize will be
* updated to reflect the number of tweets now in the timeline.
* If tweet was not able to be added, returns -1, and usedSize
* remains unchanged.
*/
int addTweet(Tweet timeline[], int& usedSize, const char message[]);


/*
* Returns the next available ID number
* timeline = timeline in which to find the highest ID
* usedSize = number of tweets in the timeline
* If timeline is empty, returns 100;
* otherwise, returns 1 + highest ID number in the timeline
*/
int getNextId(Tweet timeline[], int usedSize);


/*
* Gets a tweet id from the user. Searches a timeline to try
* to find the specified tweet by its id.
* timeline = timeline to be searched
* usedSize = number of tweets in the timeline
* If the tweet is found, the position number of where the tweet
* is stored in the timeline will be returned.  If the tweet is
* not found, a 'not found message' will be printed, and
* the value -1 will be returned.
* If timeline is empty, an 'empty' message will be printed, and
* the value -1 will be returned.
*/
int selectTweet(const Tweet timeline[], int usedSize);



int main()
{
	Tweet timeline[CAPACITY];	// Twitter timeline
	int choice;					// User's menu choice
	int usedSize = 0;			// Num of tweets in the timeline
	int selected = -1;			// Currently selected tweet
	int tmp;					// Temporary variable


	// Add some starter entries for testing purposes
	selected = addTweet(timeline, usedSize, "Where do they get the seeds to plant seedless watermelons?");
	selected = addTweet(timeline, usedSize, "Waffles are just pancakes with convenient boxes to hold your syrup.");
	selected = addTweet(timeline, usedSize, "Last night I even struck up a conversation with a spider. Turns out he's a web designer.");




	do
	{
		cout << "1. Display Timeline\n";
		cout << "2. Select Tweet\n";
		cout << "3. Add New Tweet\n";
		cout << "4. Edit Selected Tweet\n";
		cout << "5. Like Selected Tweet\n";
		cout << "6. Delete Tweet\n";
		cout << "7. Exit\n";
		cout << endl;
		cout << "Select: ";

		// Get the numeric entry from the menu
		cin >> choice;

		// Corrects issues where user might enter a non-integer value
		while (cin.fail())
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Select: ";
			cin >> choice;
		}

		// Makes the 'enter' key that was pressed after the numeric entry be ignored
		// Should be used after getting any numeric input from the keyboard
		cin.ignore();

		switch (choice)
		{
		case 1:
			displayTimeline(timeline, usedSize, selected);
			break;
		case 2:
			tmp = selectTweet(timeline, usedSize);
			// if selected tweet exists, update selected variable;
			// otherwise leave it unchanged
			if (tmp > -1)
				selected = tmp;
			break;
		case 3:
			tmp = doAddTweet(timeline, usedSize);
			// if tweet was added, make it be the selected tweet;
			// otherwise leave it unchanged
			if (tmp > -1)
				selected = tmp;
			break;
		case 4:
			doEditTweet(timeline, usedSize, selected);
			break;
		case 5:
			doLikeTweet(timeline, usedSize, selected);
			break;
		case 6:
			doDeleteTweet(timeline, usedSize, selected);
			break;
		default:
			cout << "Please enter a valid option.\n";
			break;
		}

	} while (choice != 7);

	return 0;
}


int doAddTweet(Tweet timeline[], int& usedSize)
{
	/*
	Checks to make sure capacity has not been reached, and uses temp array to store message input by user,
	calling on the addTweet function to insert user input into the timeline array
	*/

	if (usedSize < CAPACITY)
	{
		char temp[MSGSIZE];
		cout << "Please enter your tweet: \n";
		cin.getline(temp, MSGSIZE);
		return addTweet(timeline, usedSize, temp);
	}
	//Only reaches this error message when capacity hits its limit
	cerr << "ERROR: CAPACITY HAS BEEN REACHED\n";

	return -1;
}

void doEditTweet(Tweet timeline[], int usedSize, int selected)
{
	// Triggers if the selected tweet is valid
	if (selected != -1)
	{
		cout << "\nEnter tweet: \n";
		char temp[MSGSIZE];
		cin.getline(temp, MSGSIZE);
		snprintf(timeline[selected].msg, MSGSIZE, temp);
	}

	//error letting user know tweet has not been selected
	else
	{
		cerr << "ERROR: NO TWEET SELECTED\n\n";
	}
}

void doLikeTweet(Tweet timeline[], int usedSize, int selected)
{
	//checks to make sure the selected value is valid, and adds one to the value stored in the likes variable
	if (selected <= usedSize && selected != -1) {
		timeline[selected].likes = timeline[selected].likes + 1;
	}
	//Default statement if no tweet has been selected
	else {
		cerr << "ERROR: NO TWEET SELECTED\n\n" ;
	}
}


void displayTimeline(const Tweet timeline[], int usedSize, int selected)
{
	// Triggers if timeline is not empty
	if (usedSize != 0)
	{

		// Header that will never change
		printf("\nTweets:\n\n");
		printf("%0s %10s %10s %10s\n", "Sel", "ID", "Likes", "       Tweet");

		// Loops through tweets to print each individual one per line
		for (int i = 0; i < usedSize; i++)
		{
			// Prints array if the tweet is selected
			if (i == selected)
			{
				cout << "-->";
			}

			// Prints out individual tweets using their values
			printf("\t%7d %7d \t%0s\n", timeline[i].id, timeline[i].likes, timeline[i].msg);
		}

		cout << endl;

	}

	// Triggers if timeline is empty
	else
	{
		printf("\nTweets:\n\n");
		printf("%0s %10s %10s %10s\n", "Sel", "ID", "Likes", "       Tweet");
		printf("\t***Timeline Empty***\n\n");
	}
}


int addTweet(Tweet timeline[], int& usedSize, const char message[])
{
	// Checks to see if we have enough room for a new tweet
	if (usedSize < CAPACITY)
	{
		// copies the message passed into this function to the new tweet
		// sets likes to 0 automatically
		// uses getNextId function to get the next ID for this new tweet

		snprintf(timeline[usedSize].msg, MSGSIZE, message);
		timeline[usedSize].likes = 0;
		timeline[usedSize].id = getNextId(timeline, usedSize);
		return usedSize++;

	}
	// Triggers if there isn't enough room in the timeline for a new tweet
	return -1;
}


int getNextId(Tweet timeline[], int usedSize)
{
	// Creates a temporary variable to store our highest value of ID and then 
	// will return either that plus one or return 100 if the timeline is empty
	int high = 0;
	for (int i = 0; i < usedSize; i++)
	{
		if (timeline[i].id > high)
		{
			high = timeline[i].id;
		}
	}

	if (high != 0)
	{
		return high + 1;
	}

	else
	{
		return 100;
	}
}


void doDeleteTweet(Tweet timeline[], int& usedSize, int& selected)
{
	// Triggers if there is a selected tweet
	// it will: 
	//		loop through and pull every index back one spot
	//		reset our selected value to default 
	//		reduce our used size by one OR print an error message if the index is invalid
	if (selected != -1)
	{
		if (timeline[selected].id != timeline[usedSize].id)
		{
			for (int i = 0; i < usedSize; i++)
			{
				timeline[selected + i].id = timeline[selected + i + 1].id;
				timeline[selected + i].likes = timeline[selected + i + 1].likes;
				snprintf(timeline[selected + i].msg, MSGSIZE, timeline[selected + i + 1].msg);
			}
		}

		selected = -1;

		usedSize--;
	}

	// Triggers if there is no selected tweet
	else
	{
		cerr << "ERROR: NO TWEET SELECTED\n\n";
	}
}


int selectTweet(const Tweet timeline[], int usedSize)
{
	// Checks to see if the timeline of tweets is currently empty
	if (usedSize == 0)
	{
		cerr << "ERROR: TIMELINE EMPTY\n\n";
		return -1;
	}

	// Gathers user input and clears keyboard input
	cout << "Enter the tweet ID: ";
	int temp;
	cin >> temp;
	cin.ignore();

	// Searches through the ids of tweets and looks for the user-input
	for (int i = 0; i < usedSize; i++)
	{
		if (timeline[i].id == temp)
		{
			return i;
		}
	}

	cerr << "ERROR:ID NOT FOUND\n\n";

	return -1;
}
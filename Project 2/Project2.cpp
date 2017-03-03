/*
 * Project2.cpp
 *
 *  Created on: Sept 27, 2016
 *      Author: dhaltonhuber
 */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <math.h>

using namespace std;

#define GENRE_LEN       25
#define	TITLE_LEN		100
#define	AUTHOR_LEN		50
#define	PUBLISHER_LEN	50
#define	NO_FORMATS		4
#define	FORMAT_LEN		10



struct Format{
    char format[ FORMAT_LEN ]; // Format, i.e., Paperback, Hardcover, Kindle, Audible
    float price; // Price in $ of the book in the given format
    int quantity; // Number of books in the inventory of the given format
};

struct BookInfo{
    char title[ TITLE_LEN ]; // Title of the book
    char author[ AUTHOR_LEN ]; // Name of the author
    char publisher[ PUBLISHER_LEN ]; // Publisher of the book
    struct Format formats[ NO_FORMATS ]; // Book formats carried
    
    int currentFormatIndex;
    BookInfo() : currentFormatIndex(0)
    { }
};

struct BST{ // A binary search tree
    struct BookInfo bookInfo; // Information about the book
    struct BST *left;  // Pointer to the left subtree
    struct BST *right;  // Pointer to the right subtree
};

struct Genre{
    char genre[ GENRE_LEN ]; // Type of genre
    struct BST *root;  // Pointer to root of search tree for this genre
    
};



struct HashTableEntry{
    char title[ TITLE_LEN ]; // Title of the book
    struct BST *book; // Pointer to node in tree containing the book's information
    
    struct HashTableEntry *head;
    
    HashTableEntry() : head(NULL)
    { }
    
};


/*************Test For Prime Function**********/

bool TestForPrime( int num )
{
    //if the number is 2, it is prime
    if (num == 2)
    {
        return true;
    }
    // If the number is negative, it is not prime
    else if (num <= 1)
    {
        return false;
    }
    // If the number is divisible by 2, it is not prime
    else if (num % 2 == 0)
    {
        return false;
    }
    else
    {
        // Boolean to determine if it's prime
        bool prime = true;
        // Divisor starts at 3
        int divisor = 3;
        double num_d = static_cast<double>(num);
        int upperLimit = static_cast<int>(sqrt(num_d) +1);
        
        // While the divisor is less than or equal to the upper limit
        while (divisor <= upperLimit)
        {
            // If the number is divisible by the divisor
            if (num % divisor == 0)
            {
                // It is not prime
                prime = false;
            }
            // Increment divisor by 2
            divisor +=2;
        }
        return prime;
    }
}

/***********End Test For Prime Function********/

/***************Function to insert a BST node*********/

void insertBST(BST*& genreRoot, BST *toInsert)
{
    // If there is nothing in the tree yet
    if (genreRoot == NULL)
    {
        // Insert the new element at the root
        genreRoot = toInsert;
    }
    // If the element to insert belongs on the left side
    else if (strcmp(toInsert->bookInfo.title, genreRoot->bookInfo.title) < 0)
    {
        // Call insertBST recursively using the left child as the root
        insertBST(genreRoot->left, toInsert);
    }
    // If the element to insert belongs on the right side
    else if (strcmp(toInsert->bookInfo.title, genreRoot->bookInfo.title) > 0)
    {
        // Call insertBST recursively using the right child as the root
        insertBST(genreRoot->right, toInsert);
    }
    
}
/****************END of BST INSERT FUNCTION*******/

/**************FUCNTION TO CONVERT A BST TO A HASH TABLE***********/

void BSTToHash(BST *root, int size, struct HashTableEntry table[])
{
    // If the item that we are looking at is not NULL
    if (root != NULL)
    {
        // Recursively call BSTToHash on the left side
        BSTToHash(root->left, size, table);
        // Sum variable to hold the sum of all characters in the title
        int sum = 0;
        // Sum up all characters
        for (int a = 0; a < TITLE_LEN; a++)
        {
            sum = sum + int(root->bookInfo.title[a]);
        }
        // Calculate the hash index
        int hashIndex = sum % size;
        // Create a new HashTableHentry variable
        HashTableEntry *newHashEntry = new HashTableEntry();
        // Set the new entry's book to the current node in the BST
        newHashEntry->book = root;
        // Copy the title into the new HashTableEntry
        strcpy(newHashEntry->title, root->bookInfo.title);
        // Set the head of the new entry to the previous head
        newHashEntry->head = table[hashIndex].head;
        // Set the table index's head to the new entry
        table[hashIndex].head = newHashEntry;
        // Recursively call BSTToHash on the right side
        BSTToHash(root->right, size, table);
    }
}

/**************END OF BST TO HASH FUNCTION************/

/************InOrderPrint**************/

// This function is used in the find genre query to print the titles of all of the books
// InOrder
void InOrderPrint(BST *root)
{
    // If the root that is passed is not NULL (stopping condition)
    if (root != NULL)
    {
        // Recursively call InOrderPrint to print the left sub-tree
        InOrderPrint(root->left);
        // Print the title of each node as it traverses
        cout << root->bookInfo.title << endl;
        // Recursively call InOrderPrint to print the right sub-tree
        InOrderPrint(root->right);
    }
}

/**********END OF InOrderPrint***********/

/***********InOrderRange***************/

// This function is used in the range query to print the titles of books that are within the range
void inOrderRange(BST *root, char low[], char high[], bool& foundInRange)
{
    // If the root that is passed is not NULL (stopping condition)
    if (root != NULL)
    {
        // Recursively call inOrderRange to check the left sub-tree
        inOrderRange(root->left, low, high, foundInRange);
        // If the title falls in the range
        if (strcmp(root->bookInfo.title, low) >= 0 && strcmp(root->bookInfo.title, high) <= 0)
        {
            // If this is the first time that we find a title in the range
            if (foundInRange == false)
            {
                // Print the formatting
                cout << "Books in this range: " << endl;
            }
            // Print the title
            cout << root->bookInfo.title << endl;
            // Set the foundInRange bool that was passed to the function to true
            foundInRange = true;
        }
        // Recursively call inOrderRange to check the right sub-tree
        inOrderRange(root->right, low, high, foundInRange);
    }
}

/*************END OF InOrderRange*****/

/*************DeleteAll************/
// Used to delete all nodes in the BST
void DeleteAll(BST*& root)
{
    if (root != NULL)
    {
        DeleteAll(root->left);
        DeleteAll(root->right);
        delete root;
        if (root->left != NULL)
        {
            root->left = NULL;
        }
        if (root->right != NULL)
        {
            root->right = NULL;
        }
        root = NULL;
    }
}

/************ END OF DeleteAll********/


/************FindMin**********/
// Finds the minimum value in the BST
BST* FindMin(BST* root)
{
    while(root->left != NULL)
    {
        root = root->left;
    }
    return root;
}
/***********END OF FindMin***********/



/*******RemoveBST*********/
// Removes the node in the BST with the given title and returns the root of the updated tree
struct BST* RemoveBST(struct BST*& root, char title[])
{
    // (base case)
    if(root == NULL)
    {
        return root;
    }
    
    // If the title comes before the current node's title
    if(strcmp(title, root->bookInfo.title) < 0)
    {
        // Move the "root" through the left sub-tree
        root->left = RemoveBST(root->left, title);
    }
    // If the title comes after the current node's title
    else if(strcmp(title, root->bookInfo.title) > 0)
    {
        // Move the "root" through the right sub-tree
        root->right = RemoveBST(root->right, title);
    }
    else
    {
        //If the node has one child
        if(root->left == NULL)
        {
            // Set temp to the root's right child to be returned as the new "root"
            struct BST *temp = root->right;
            // Free the item we want to delete
            free(root);
            return temp;
        }
        else if(root->right == NULL)
        {
            // Set temp to the root's left child to be returned as the new "root"
            struct BST *temp = root->left;
            // Free the item we want to delete
            free(root);
            return temp;
        }
        
        // Find the minimum in the right sub-tree
        struct BST* temp = FindMin(root->right);
        
        // Copy the  successor's information
        root = temp;
        
        // Delete the successor
        root->right = RemoveBST(root->right, temp->bookInfo.title);
    }
    // Return the root of the new BST
    return root;
}

/************END OF RemoveBST*********/


// NumOfNodes used in investigations to find number of nodes in BST's
void numOfNodes(struct BST* root, int &sum)
{
    if (root != NULL)
    {
        numOfNodes(root->left, sum);
        sum++;
        numOfNodes(root->right, sum);
    }
}

// NumAtH used to calculate the number of nodes at a given height of a tree
int NumAtH(struct BST* root, int h)
{
    if (root == NULL || h < 0)
    {
        return 0;
    }
    else if (h == 0)
    {
        return 1;
    }
    else
    {
        return NumAtH(root->left, h - 1) + NumAtH(root->right, h - 1);
    }
}

int main()
{
    
    // variable declarations
    string genreName;
    string num;
    int numOfGenres;
    string num2;
    int numOfBooks;
    string numQ;
    int numOfQueries;
    
    
    
    /***********FOR TESTING ONLY*********/
    
    ifstream inputFile("input-full.txt");
    getline(inputFile, num);
    numOfGenres = atoi(num.c_str());
    // Dynamically allocate space for the array
    struct Genre *genreArray = new Genre[numOfGenres];
    
    
    
    
    for (int i = 0; i < numOfGenres; i++)
    {
        Genre *newGenre = new Genre();
        getline(inputFile, genreName);
        strcpy(newGenre->genre, genreName.c_str());
        newGenre->root = NULL;
        genreArray[i] = *newGenre;
        
    }
    
    
    getline(inputFile, num2);
    numOfBooks = atoi(num2.c_str());
    
    
    for (int i = 0; i < numOfBooks; i++)
    {
        string bookGenreS;
        char bookGenre[GENRE_LEN];
        getline(inputFile, bookGenreS);
        strcpy(bookGenre, bookGenreS.c_str());
        
        BST *newBSTNode = new BST();
        BookInfo *newBookInfo = new BookInfo();
        
        
        
        for (int j = 0; j < numOfGenres; j++)
        {
            if (strcmp(genreArray[j].genre, bookGenre) == 0)
            {
                
                
                string titleS;
                string authorS;
                string publisherS;
                string numFormatsS;
                int numOfFormats;
                
                getline(inputFile, titleS);
                getline(inputFile, authorS);
                getline(inputFile, publisherS);
                
                
                strcpy(newBookInfo->title, titleS.c_str());
                strcpy(newBookInfo->author, authorS.c_str());
                strcpy(newBookInfo->publisher, publisherS.c_str());
                
                getline(inputFile, numFormatsS);
                
                numOfFormats = atoi(numFormatsS.c_str());
                
                
                for (int a = 0; a < numOfFormats; a++)
                {
                    Format *newFormat = new Format();
                    
                    string formatNameS;
                    string priceS;
                    string quantityS;
                    
                    getline(inputFile, formatNameS);
                    getline(inputFile, priceS);
                    getline(inputFile, quantityS);
                    
                    strcpy(newFormat->format, formatNameS.c_str());
                    newFormat->price = stof(priceS);
                    newFormat->quantity = atoi(quantityS.c_str());
                    
                    newBookInfo->formats[newBookInfo->currentFormatIndex] = *newFormat;
                    newBookInfo->currentFormatIndex++;
                    
                    
                }
                
                
                
                newBSTNode->bookInfo = *newBookInfo;
                
                insertBST(genreArray[j].root, newBSTNode);
                
            }
            
            
        }
        
        
    }
    
    
    
    // Initialize tableSize to NULL
    int tableSize = 0;
    // Initialize number to zero
    int number = 0;
    // While tableSize is still NULL
    while (tableSize == 0)
    {
        // If i is greater than 2 times the number of books and it is prime
        if (number > 2*numOfBooks && TestForPrime(number) == true)
        {
            // Set the table size to i
            tableSize = number;
        }
        // Increment i
        number++;
    }
    
    
    
    // Dynamically allocate space for the table using the tableSize variable
    struct HashTableEntry *table = new HashTableEntry[tableSize];
    
    
    
    // Set all table index head variables to NULL
    for (int i = 0; i < tableSize; i++)
    {
        table[i].head = NULL;
    }
    
    
    
    for (int genreIndex = 0; genreIndex < numOfGenres; genreIndex++)
    {
        
        BSTToHash(genreArray[genreIndex].root, tableSize, table);
        
        
    }
    
    
    
    /**********************************/
    
    
    /*********RIGHT WAY START******************/
    
    
     /*// Get the number of genres in as a string
     getline(cin, num);
     // Convert the string to an int
     numOfGenres = atoi(num.c_str());
     // Dynamically allocate space for the array
     struct Genre *genreArray = new Genre[numOfGenres];
     
     // For loop to fill out the genre array
     for (int i = 0; i < numOfGenres; i++)
     {
     // Create a new genre structure
     Genre *newGenre = new Genre();
     // Read in the genre name from the file
     getline(cin, genreName);
     // Copy the string into the genre field of the newGenre object
     strcpy(newGenre->genre, genreName.c_str());
     // Set each root node to null
     newGenre->root = NULL;
     // Place the genre into the genre array
     genreArray[i] = *newGenre;
     }
     
     // Read in the string containing the number of books in the data set
     getline(cin, num2);
     // Convert the string into an integer and store it in numOfBoks
     numOfBooks = atoi(num2.c_str());
     
     
     for (int i = 0; i < numOfBooks; i++)
     {
     // Declare string to hold the string containing the book genre
     string bookGenreS;
     // Character array to hold the character array version of the book genre
     char bookGenre[GENRE_LEN];
     // Read in the book genre
     getline(cin, bookGenreS);
     // Convert it to a char array and store it in bookGenre
     strcpy(bookGenre, bookGenreS.c_str());
     
     // Create new BST and BookInfo structures
     BST *newBSTNode = new BST();
     BookInfo *newBookInfo = new BookInfo();
     
     
     for (int j = 0; j < numOfGenres; j++)
     {
     // If the genre in the genre array matches the genre of the book we are
     // about to read in
     if (strcmp(genreArray[j].genre, bookGenre) == 0)
     {
     
     // String versions of the variables to be read in
     string titleS;
     string authorS;
     string publisherS;
     string numOfFormatsS;
     int numOfFormats;
     
     // Read in the strings containing the info
     getline(cin, titleS);
     getline(cin, authorS);
     getline(cin, publisherS);
     
     // Put the information into the newBookInfo
     strcpy(newBookInfo->title, titleS.c_str());
     strcpy(newBookInfo->author, authorS.c_str());
     strcpy(newBookInfo->publisher, publisherS.c_str());
     
     
     // Read in the string containing the number of formats for this book
     getline(cin, numOfFormatsS);
     
     // Convert the string into an integer and store in numOfFormats
     numOfFormats = atoi(numOfFormatsS.c_str());
     
     // For loop to fill out the formats
     for (int a = 0; a < numOfFormats; a++)
     {
					// Create new format structure
					Format *newFormat = new Format();
     
					// Strings to read in the format info
					string formatNameS;
					string priceS;
					string quantityS;
     
					// Read in the format info
					getline(cin, formatNameS);
					getline(cin, priceS);
					getline(cin, quantityS);
     
					// Store the info in the newFormat
					strcpy(newFormat->format, formatNameS.c_str());
					newFormat->price = stof(priceS);
					newFormat->quantity = atoi(quantityS.c_str());
     
					// Store the format in the format array in the newBookInfo object,
					// Then increment the newBookInfo's currentFormatIndex
					newBookInfo->formats[newBookInfo->currentFormatIndex] = *newFormat;
					newBookInfo->currentFormatIndex++;
     }
     
     // Add the book's info to the new BST node
     newBSTNode->bookInfo = *newBookInfo;
     // Insert the node into the BST
     insertBST(genreArray[j].root, newBSTNode);
     
     }
     }
     }
     
     
     
     
     // Initialize tableSize to NULL
     int tableSize = 0;
     // Initialize i to zero
     int number = 0;
     // While tableSize is still NULL
     while (tableSize == 0)
     {
     // If i is greater than 2 times the number of books and it is prime
     if (number > 2*numOfBooks && TestForPrime(number) == true)
     {
     // Set the table size to i
     tableSize = number;
     }
     // Increment i
     number++;
     }
     
     
     // Dynamically allocate space for the table using the tableSize variable
     struct HashTableEntry *table = new HashTableEntry[tableSize];
     
     // Set all table index head variables to NULL
     for (int i = 0; i < tableSize; i++)
     {
     table[i].head = NULL;
     }
     
     // For each index of the genre array
     for (int genreIndex = 0; genreIndex < numOfGenres; genreIndex++)
     {
     
     // Traverse the BST and place the elements into the hash table using the BSTToHash
     // Function
     BSTToHash(genreArray[genreIndex].root, tableSize, table);
     
     
     }*/
    
    
    
    
    
    /**********************************/
    
    
    
    // Read in the number of queries and store in numOfQueries
    getline(cin, numQ);
    numOfQueries = atoi(numQ.c_str());
    
    // endl character for formatting
    cout << endl;
    
    // For loop to read in all queries
    for (int i = 0; i < numOfQueries; i++)
    {
        // Read in the first word and store it in nextQueryWord1Array for comparisons
        string nextQueryWord1;
        cin >> nextQueryWord1;
        char nextQueryWord1Array[15];
        strcpy(nextQueryWord1Array, nextQueryWord1.c_str());
        
        // If the first word is "find"
        if (strcmp(nextQueryWord1Array, "find") == 0)
        {
            // Read in the next word and store it in findQueryWord2Array for comparisons
            string findQueryWord2;
            cin >> findQueryWord2;
            char findQueryWord2Array[5];
            strcpy(findQueryWord2Array, findQueryWord2.c_str());
            
            // If the second word is "book"
            if (strcmp(findQueryWord2Array, "book") == 0)
            {
                // Variables to eat the white space between words
                string whiteSpace1;
                string whiteSpace2;
                
                // String to hold the title of the book we are searching for
                string bookSpecified;
                
                // Read in the white space and book title
                getline(cin, whiteSpace1, '"');
                getline(cin, bookSpecified, '"');
                getline(cin, whiteSpace2, '\n');
                
                // Store the title in bookSpecifiedArray for comparisons
                char bookSpecifiedArray[TITLE_LEN];
                strcpy(bookSpecifiedArray, bookSpecified.c_str());
                
                
                // Sum all ascii values in the title of the desired book and store in titleSum
                int titleSum = 0;
                int len = strlen(bookSpecifiedArray);
                for (int l = 0; l < len; l++)
                {
                    titleSum = titleSum + int(bookSpecifiedArray[l]);
                }
                
                // Calculate the index that the book would be at using the hash function
                int bookIndex = titleSum % tableSize;
                
                // boolean to determine if the book was found of not
                bool bookFound = false;
                
                // If the head of the linked list at that index is not NULL
                if (table[bookIndex].head != NULL)
                {
                    // Create a new HashTableEntry variable called traverser to traverse
                    // the linked list
                    HashTableEntry *traverser = table[bookIndex].head;
                    
                    
                    // While the traverser is not NULL
                    while (traverser != NULL)
                    {
                        // If the taverser's title is the same as the book we are looking for
                        if (strcmp(bookSpecifiedArray, traverser->title) == 0)
                        {
                            // Print that book's information
                            cout << "Title: " << traverser->book->bookInfo.title << endl;
                            cout << "Author: " << traverser->book->bookInfo.author << endl;
                            cout << "Publisher: " << traverser->book->bookInfo.publisher <<
                            endl;
                            cout << "Formats: " << traverser->book->bookInfo.currentFormatIndex << "-"
                            << endl;
                            for (int j = 0; j < traverser->book->bookInfo.currentFormatIndex;
                                 j++)
                            {
                                cout << "\t" << traverser->book->bookInfo.formats[j].format
                                << ": $" << traverser->book->bookInfo.formats[j].price
                                << ", " << traverser->book->bookInfo.formats[j].quantity
                                << endl;
                            }
                            cout << endl;
                            // Set bookFound to true
                            bookFound = true;
                            // Set traverser to NULL to exit the loop
                            traverser = NULL;
                        }
                        // Otherwise,
                        else
                        {
                            // Move traverser onto the next  in the list for the next iteration
                            traverser = traverser->head;
                        }
                    }
                    
                    
                }
                
                // If the book was not found
                if (bookFound == false)
                {
                    // Print the error message
                    cout << "Book not carried." << endl;
                    cout << endl;
                }
            }
            
            
            // If the second word is "genre"
            else if (strcmp(findQueryWord2Array, "genre") == 0)
            {
                // Variables to eat the white space between words
                string whiteSpace1;
                string whiteSpace2;
                
                // String to hold the type specified
                string typeSpecified;
                
                // Read in the type specified between the white space variables
                getline(cin, whiteSpace1, '"');
                getline(cin, typeSpecified, '"');
                getline(cin, whiteSpace2, '\n');
                
                // Character array to hold the type specified for comparisons
                char typeSpecifiedArray[GENRE_LEN];
                strcpy(typeSpecifiedArray, typeSpecified.c_str());
                
                // Boolean variable to determine if the type was found or not
                bool typeFound = false;
                
                // For loop to traverse the genreArray
                for (int j = 0; j < numOfGenres; j++)
                {
                    // If the genre exists in the array
                    if (strcmp(typeSpecifiedArray, genreArray[j].genre) == 0)
                    {
                        // Call InOrderPrint to print the titles of the books of that genre,
                        // traversing the tree InOrder
                        cout << typeSpecifiedArray << " Currently In Stock: " << endl;
                        InOrderPrint(genreArray[j].root);
                        cout << endl;
                        // Set typeFound to true, so the error message is not printed
                        typeFound = true;
                    }
                }
                
                // If the type was not found in the genreArray
                if (typeFound == false)
                {
                    // Print the error message
                    cout << "Genre not carried." << endl;
                    cout << endl;
                }
            }
        }
        // If the first word is "range"
        else if (strcmp(nextQueryWord1Array, "range") == 0)
        {
            // Variables to eat the white space between words
            string whiteSpace1;
            string whiteSpace2;
            string whiteSpace3;
            string whiteSpace4;
            
            // String variables to hold the string version of the genre, low, and high
            string genreS;
            string lowS;
            string highS;
            
            // Read in the genre, low, and high strings
            getline(cin, whiteSpace1, '"');
            getline(cin, genreS, '"');
            getline(cin, whiteSpace2, '"');
            getline(cin, lowS, '"');
            getline(cin, whiteSpace3, '"');
            getline(cin, highS, '"');
            getline(cin, whiteSpace4, '\n');
            
            // Character arrays for the genre, low, and high for comparisons
            char genreSpecified[GENRE_LEN];
            char lowArray[GENRE_LEN];
            char highArray[GENRE_LEN];
            // Copy the strings into the character arrays
            strcpy(genreSpecified, genreS.c_str());
            strcpy(lowArray, lowS.c_str());
            strcpy(highArray, highS.c_str());
            
            // Boolean variable used to determine if any titles fell in the range or not
            bool foundInRange = false;
            
            // For loop to traverse the genreArray
            for (int j = 0; j < numOfGenres; j++)
            {
                // If the genre specified is in the array
                if (strcmp(genreArray[j].genre, genreSpecified) == 0)
                {
                    
                    // Call inOrderRange to traverse the tree in order and check if the
                    // titles fall in the range
                    inOrderRange(genreArray[j].root, lowArray, highArray, foundInRange);
                    cout << endl;
                    
                }
            }
            // If there were no books found in the range
            if (foundInRange == false)
            {
                // Print the error message
                cout << "No books found for given range." << "\n";
            }
        }
        // If the first word is "price"
        else if (strcmp(nextQueryWord1Array, "price") == 0)
        {
            // Variables to eat the white space between words
            string whiteSpace1;
            string whiteSpace2;
            // String to hold the title specified
            string titleSpecified;
            
            // Read in the title between the white space
            getline(cin, whiteSpace1, '"');
            getline(cin, titleSpecified, '"');
            getline(cin, whiteSpace2, '\n');
            
            // Char array to hold the title for comparisons
            char titleArray[TITLE_LEN];
            strcpy(titleArray, titleSpecified.c_str());
            
            // Sum variable to hold the sum of all ascii values in he titleArray
            int titleSum = 0;
            // Calculate the length of the titleArray
            int len = strlen(titleArray);
            
            // Sum up all of the ascii values
            for (int j = 0; j < len; j++)
            {
                titleSum = titleSum + int(titleArray[j]);
            }
            
            // Calculate the index that this title would be stored at
            int titleIndex = titleSum % tableSize;
            
            // Boolean used to determine if the book was found or not
            bool bookFound = false;
            
            // If there is anything in the linked list at that index
            if (table[titleIndex].head != NULL)
            {
                // Create a traverser variable to traverse the linked list
                HashTableEntry *traverser = table[titleIndex].head;
                
                // While the traverser is not NULL
                while (traverser != NULL)
                {
                    // If the title of the traverser and the titleArray match
                    if (strcmp(titleArray, traverser->title) == 0)
                    {
                        cout << endl << traverser->book->bookInfo.title << " prices: " << endl;
                        // Print the price for each format
                        for (int j = 0; j < traverser->book->bookInfo.currentFormatIndex; j++)
                        {
                            cout << traverser->book->bookInfo.formats[j].format << ": $" <<
                            traverser->book->bookInfo.formats[j].price << endl;
                        }
                        // Set bookFound to true
                        bookFound = true;
                    }
                    // Move the traverser along the linked list
                    traverser = traverser->head;
                }
            }
            
            // If the book was not found
            if (bookFound == false)
            {
                // Print the error message
                cout << endl;
                cout << "Book not carried." << endl;
                cout << endl;
            }
        }
        // If the first word is "sell"
        else if (strcmp(nextQueryWord1Array, "sell") == 0)
        {
            // String to read the word "book"
            string readBook;
            // Read the word in and store it to get rid of it
            getline(cin, readBook, ' ');
            // Variables to eat the white space between the words
            string whiteSpace1;
            string whiteSpace2;
            string whiteSpace3;
            // Strings to hold all of the query info
            string genreS;
            string titleS;
            string formatS;
            
            // Read in all of the words of the query and the white space
            getline(cin, whiteSpace1, '"');
            getline(cin, genreS, '"');
            getline(cin, whiteSpace2, '"');
            getline(cin, titleS, '"');
            getline(cin, whiteSpace3, ' ');
            getline(cin, formatS);
            
            // Character arrays to hold the query requests for comparisons
            char genreToSell[GENRE_LEN];
            char titleToSell[TITLE_LEN];
            char formatToSell[FORMAT_LEN];
            
            // Copy the strings into the character arrays
            strcpy(genreToSell, genreS.c_str());
            strcpy(titleToSell, titleS.c_str());
            strcpy(formatToSell, formatS.c_str());
            
            // Integers to hold the sum of all of the character values in the title and
            // the length of the title
            int titleSum = 0;
            int len = strlen(titleToSell);
            
            // Add up the characters in the title
            for (int j = 0; j < len; j++)
            {
                titleSum = titleSum + titleToSell[j];
            }
            
            // Compute the index that this title would be at in the hash table
            int titleIndex = titleSum % tableSize;
            // Boolean variable to determine if the book was found or not
            bool bookFound = false;
            // boolean to determine if we need to remove the book
            bool needToRemove = true;
            
            if (table[titleIndex].head != NULL)
            {
                // HashTableEntry to traverse the linked list at the index
                HashTableEntry *traverser = table[titleIndex].head;
                
                // While traverser is not NULL
                while (traverser != NULL)
                {
                    // If the traverser title is equal to the title we want to sell
                    if (strcmp(traverser->title, titleToSell) == 0)
                    {
                        
                        int formatIndex = -1;
                        // For loop to traverse the formats for the book
                        for (int j = 0; j < traverser->book->bookInfo.currentFormatIndex; j++)
                        {
                            // If the format is equal to the one specified and the quantity is
                            // greater than 0
                            if ((strcmp(formatToSell, traverser->book->bookInfo.formats[j].format) == 0)
                                && traverser->book->bookInfo.formats[j].quantity > 0)
                            {
                                // Decrement the quantity
                                traverser->book->bookInfo.formats[j].quantity--;
                                // set bookFound to true so the error message is not printed
                                bookFound = true;
                                // Set needToRemove to false so we don't remove the book (because the qty is not 0)
                                needToRemove = false;
                                // Set formatIndex to j so we can access it later
                                formatIndex = j;
                            }
                            
                            // If the format is equal to the one specified and the quantity is
                            // already 0
                            else if ((strcmp(formatToSell, traverser->book->bookInfo.formats[j].format) == 0)
                                     && traverser->book->bookInfo.formats[j].quantity == 0)
                            {
                                // Print an error message and set bookFound equal to true
                                cout << "The quantity of this format is already 0!" <<
                                endl;
                                bookFound = true;
                                formatIndex = j;
                            }
                        }
                        
                        // Print that book's information
                        cout << "Title: " << traverser->book->bookInfo.title << endl;
                        cout << "Author: " << traverser->book->bookInfo.author << endl;
                        cout << "Publisher: " << traverser->book->bookInfo.publisher <<
                        endl;
                        cout << "Formats: " << traverser->book->bookInfo.currentFormatIndex << "-"
                        << endl;
                        for (int j = 0; j < traverser->book->bookInfo.currentFormatIndex;
                             j++)
                        {
                            cout << "\t" << traverser->book->bookInfo.formats[j].format
                            << ": $" << traverser->book->bookInfo.formats[j].price
                            << ", " << traverser->book->bookInfo.formats[j].quantity
                            << endl;
                        }
                        cout << endl;
                        
                        
                        // If the index format that we just updated is now 0
                        if (traverser->book->bookInfo.formats[formatIndex].quantity == 0)
                        {
                            // Traverse the format array for this book
                            for (int j = 0; j < traverser->book->bookInfo.currentFormatIndex;
                                 j++)
                            {
                                // If any of the quantities for the formats are not 0
                                if (traverser->book->bookInfo.formats[j].quantity != 0)
                                {
                                    // Set needToremove to false, so we don't remove the book
                                    needToRemove = false;
                                }
                            }
                        }
                        
                        // Set traverser to NULL after we find the book, so that
                        // we exit the loop
                        traverser = NULL;
                        
                    }
                    
                    if (traverser != NULL)
                    {
                        // Move the traverser along the linked list
                        traverser = traverser->head;
                    }
                }
            }
            
            
            // If the book was not found
            if (bookFound == false)
            {
                // Print the error message and do nothing
                cout << "Format out of stock." << endl;
            }
            // If we need to remove the book from our data
            if (needToRemove == true)
            {
                // Remove from the hash table
                // Search the genreArray for the genre
                for (int j = 0; j < numOfGenres; j++)
                {
                    // If found
                    if (strcmp(genreToSell, genreArray[j].genre) == 0)
                    {
                        // set the genre's root to the new root after deleting
                        genreArray[j].root = RemoveBST(genreArray[j].root, titleToSell);
                        
                        // HashTableEntry to traverse the linked list at the index
                        HashTableEntry *traverser = table[titleIndex].head;
                        // HashTableEntry to keep track of the previous item in the LL
                        HashTableEntry *prev = table[titleIndex].head;
                        // Integer to keep track of the times the loop iterates
                        int loopCount = 1;
                        // Traverse the Linked List
                        while (traverser != NULL)
                        {
                            // If we find the item we want to delete
                            if (strcmp(traverser->title, titleToSell) == 0)
                            {
                                // Break from the while loop
                                break;
                            }
                            // Otherwise, set prev to the traverser and traverser to it's next
                            prev = traverser;
                            traverser = traverser->head;
                            // Increment the loopCount
                            loopCount++;
                        }
                        // If the loop didn't iterate enough times for prev and traverser
                        // to ACTUALLY be pointing to different things
                        // (In other words, if the item to delete is the first in the list)
                        if (loopCount == 1)
                        {
                            // Set prev's next node to traverser's next's next
                            prev->head = traverser->head->head;
                        }
                        // Otherwise
                        else
                        {
                            // Set prev to traverser's next like normal
                            prev->head = traverser->head;
                        }
                        
                        // Break from the for loop after we delete everything we need to
                        break;
                    }
                }
            }
            
            
            
        }
        else if (strcmp(nextQueryWord1Array, "buy") == 0)
        {
            // Read in the word "book" from the instructions
            string readBook;
            getline(cin, readBook, ' ');
            
            // White space variables to eat the spaces between the important parts of
            // the query
            string whiteSpace1;
            string whiteSpace2;
            string whiteSpace3;
            string whiteSpace4;
            string whiteSpace5;
            
            // All strings that we will initially read into
            string genreS;
            string titleS;
            string authorS;
            string publisherS;
            string formatS;
            string priceS;
            string qtyS;
            
            // Read all of the information from the line, separating it out with the
            // white space variables
            getline(cin, whiteSpace1, '"');
            getline(cin, genreS, '"');
            getline(cin, whiteSpace2, '"');
            getline(cin, titleS, '"');
            getline(cin, whiteSpace3, '"');
            getline(cin, authorS, '"');
            getline(cin, whiteSpace4, '"');
            getline(cin, publisherS, '"');
            getline(cin, whiteSpace5, ' ');
            getline(cin, formatS, ' ');
            getline(cin, priceS, ' ');
            getline(cin, qtyS, '\n');
            
            // All character arrays to hold the information that was read in
            char genreToBuy[GENRE_LEN];
            char titleToBuy[TITLE_LEN];
            char authorNewBook[AUTHOR_LEN];
            char publisherNewBook[PUBLISHER_LEN];
            char formatNewBook[FORMAT_LEN];
            // Float to hold the price
            float priceNewBook;
            // Int to hold the quantity
            int qtyNewBook;
            
            // Copy all of the strings into the character arrays
            strcpy(genreToBuy, genreS.c_str());
            strcpy(titleToBuy, titleS.c_str());
            strcpy(authorNewBook, authorS.c_str());
            strcpy(publisherNewBook, publisherS.c_str());
            strcpy(formatNewBook, formatS.c_str());
            // Turn the price string into a float
            priceNewBook = stof(priceS);
            // Turn the quantity string into an int
            qtyNewBook = atoi(qtyS.c_str());
            
            // Sum and length used in calculating the title's sum
            int titleSum = 0;
            int len = strlen(titleToBuy);
            
            // Calculate the title's sum
            for (int j = 0; j < len; j++)
            {
                titleSum = titleSum + titleToBuy[j];
            }
            
            // calculate the index that this title would be at in the tree
            int titleIndex = titleSum % tableSize;
            
            // Boolean to determine if the format was found, to see if we need to create a new one
            bool formatFound = false;
            // Boolean to determine if the book was found
            bool bookFound = false;
            
            // Integer to hold the index of the format in the array of formats
            int formatIndex = -1;
            
            // If the head of the linked list in the table at the titleIndex is not NULL
            if (table[titleIndex].head != NULL)
            {
                // Traverser to traverse the linked list
                HashTableEntry *traverser = table[titleIndex].head;
                
                // Traverse the linked list
                while (traverser != NULL)
                {
                    // If the titles match
                    if (strcmp(traverser->book->bookInfo.title, titleToBuy) == 0)
                    {
                        // If the authors match
                        if (strcmp(traverser->book->bookInfo.author, authorNewBook) == 0)
                        {
                            // If the publishers match
                            if (strcmp(traverser->book->bookInfo.publisher, publisherNewBook) == 0)
                            {
                                // Set bookFound to true, so we can print the book's information
                                bookFound = true;
                                // Traverse the array of formats
                                for (int j = 0; j < traverser->book->bookInfo.currentFormatIndex; j++)
                                {
                                    // If the format is there
                                    if (strcmp(traverser->book->bookInfo.formats[j].format, formatNewBook) == 0)
                                    {
                                        // Set formatFound to true and formatIndex to the index in the array of formats
                                        formatFound = true;
                                        formatIndex = j;
                                    }
                                }
                            }
                        }
                    }
                    
                    // If the format was in the array
                    if (formatFound == true)
                    {
                        // Update the quantity
                        traverser->book->bookInfo.formats[formatIndex].quantity += qtyNewBook;
                    }
                    
                    // If the format was not in the array
                    if (formatFound == false)
                    {
                        // Create a new format object
                        Format *newFormat = new Format();
                        // Put the information entered into the format object
                        strcpy(newFormat->format, formatNewBook);
                        newFormat->price = priceNewBook;
                        newFormat->quantity = qtyNewBook;
                        
                        // Put the format into the format array
                        traverser->book->bookInfo.formats[traverser->book->bookInfo.currentFormatIndex] = *newFormat;
                        // Increment the currentFormatIndex
                        traverser->book->bookInfo.currentFormatIndex++;
                    }
                    
                    // If we had found the book
                    if (bookFound == true)
                    {
                        // Print that book's information
                        cout << "Title: " << traverser->book->bookInfo.title << endl;
                        cout << "Author: " << traverser->book->bookInfo.author << endl;
                        cout << "Publisher: " << traverser->book->bookInfo.publisher <<
                        endl;
                        cout << "Formats: " << traverser->book->bookInfo.currentFormatIndex << "-"
                        << endl;
                        for (int j = 0; j < traverser->book->bookInfo.currentFormatIndex;
                             j++)
                        {
                            cout << "\t" << traverser->book->bookInfo.formats[j].format
                            << ": $" << traverser->book->bookInfo.formats[j].price
                            << ", " << traverser->book->bookInfo.formats[j].quantity
                            << endl;
                        }
                        cout << endl;
                    }
                    
                    
                    
                    // Move the traverser along the linked list
                    traverser = traverser->head;
                }
            }
            
            // Int to hold the index in the genreArray of the genre
            int genreIndex = -1;
            
            // Traverse the genreArray to find the index of the given genre
            for (int j = 0; j < numOfGenres; j++)
            {
                if (strcmp(genreArray[j].genre, genreToBuy) == 0)
                {
                    genreIndex = j;
                    break;
                }
            }
            
            // If the book was not found in the table
            if (bookFound == false && genreIndex != -1)
            {
                // Create new BST, BookInfo, and Format structures
                BST *newBSTNode = new BST();
                BookInfo *newBookInfo = new BookInfo();
                Format *newFormat = new Format();
                
                // Put the information in the newFormat
                strcpy(newFormat->format, formatNewBook);
                newFormat->price = priceNewBook;
                newFormat->quantity = qtyNewBook;
                
                // Put the information in the newBookInfo and update its currentFormatIndex
                strcpy(newBookInfo->title, titleToBuy);
                strcpy(newBookInfo->author, authorNewBook);
                strcpy(newBookInfo->publisher, publisherNewBook);
                newBookInfo->formats[newBookInfo->currentFormatIndex] = *newFormat;
                newBookInfo->currentFormatIndex++;
                
                // Put the newBookInfo in the newBSTNode
                newBSTNode->bookInfo = *newBookInfo;
                
                
                // Insert the newBSTNode into it's proper BST
                insertBST(genreArray[genreIndex].root, newBSTNode);
                
                // Create a new HashTableEntry object to add the book to the linked list at the
                // appropriate index
                HashTableEntry *newEntry = new HashTableEntry();
                
                // Set the newEntry's book to the newBSTNode
                newEntry->book = newBSTNode;
                // Set the newEntry's head to the head of the linked list at the proper index
                newEntry->head = table[titleIndex].head;
                // Set the newEntry's title to the title of the new book
                strcpy(newEntry->title, titleToBuy);
                // Set the head of the linked list to the newEntry
                table[titleIndex].head = newEntry;
                
                
                // Print that book's information
                cout << "Title: " << newBookInfo->title << endl;
                cout << "Author: " << newBookInfo->author << endl;
                cout << "Publisher: " << newBookInfo->publisher <<
                endl;
                cout << "Formats: " << newBookInfo->currentFormatIndex << "-"
                << endl;
                for (int j = 0; j < newBookInfo->currentFormatIndex;
                     j++)
                {
                    cout << "\t" << newBookInfo->formats[j].format
                    << ": $" << newBookInfo->formats[j].price
                    << ", " << newBookInfo->formats[j].quantity
                    << endl;
                }
                cout << endl;
            }
            else if (genreIndex == -1)
            {
                cout << "Genre not carried." << endl;
                cout << endl;
            }

        }
    }
    
    
    
    /************Investigations**********/
    
    // Prints the length of each linked list at each index of the table
    /*for (int i = 0; i < tableSize; i++)
    {
        int lengthOfChain = 0;
        if (table[i].head != NULL)
        {
            HashTableEntry *traverser = table[i].head;
            
            while (traverser != NULL)
            {
                lengthOfChain++;
                traverser = traverser->head;
            }
        }
        cout << "Position " << i << ": " << lengthOfChain << endl;
        
    }*/
    
    
    // Load factor is 0.4895
    
    
    // Computes the number of nodes in each BST in the genreArray
    /*for (int i = 0; i < numOfGenres; i++)
    {
        int sum = 0;
        numOfNodes(genreArray[i].root, sum);
        cout << "Nodes in " << genreArray[i].genre << ": " << sum << endl;
    }*/
    
    // Computes the number of nodes in the left/right sub-trees for each BST
    /*for (int i = 0; i < numOfGenres; i++)
    {
        int sumLeft = 0;
        int sumRight = 0;
        numOfNodes(genreArray[i].root->left, sumLeft);
        numOfNodes(genreArray[i].root->right, sumRight);
        
        cout << "Genre " << i + 1 << ": " << endl;
        cout << "\t" << "Left: " << sumLeft << endl;
        cout << "\t" << "Right: " << sumRight << endl;
    }*/
    
    // Computes the number of nodes at a certain height h for each BST
    /*for (int i = 0; i < numOfGenres; i++)
    {
        int h = 7;
        int nodes = NumAtH(genreArray[i].root, h);
        cout << "Nodes at height " << h << " for index " << i + 1 << ": " << nodes << endl;
    }*/
    
    /************End of Investigations*********/
    
    
    
    // Deletes all BST's after program is finished running
    for (int i = 0; i < numOfGenres; i++)
    {
        DeleteAll(genreArray[i].root);
    }
    // Delete the genreArray
    delete[] genreArray;
    // Delete all elements in any linked lists within the hash table
    for (int i = 0; i < tableSize; i++)
    {
        if (table[i].head != NULL)
        {
            while (table[i].head != NULL)
            {
                HashTableEntry *next = table[i].head->head;
                delete table[i].head;
                table[i].head = next;
            }
        }
    }
    // Delete the hash table
    delete[] table;
    
    return 0;
}

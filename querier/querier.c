/*
 * querier.c - queries words provided by a user
 *			 - Ranks input from user and prints to stdout
 *
 * David Oh
 *
 * COSC 50, August 2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "counters/counters.h"
#include "file.h"
#include "index.h"
#include "word.h"

static void match_intersect(counters_t *A, counters_t *B);
static void match_union(counters_t *A, counters_t *B);
static void union_helper(void *arg, int key, int count);
static void intersect_helper(void *arg, int key, int count);
static int min(int a, int b);
static void remove_extras(void *arg, int key, int count);
static void matches_helper(void *arg, int key, int count);
static void numberMatches(void *arg, int key, int count);

struct counters{
	counters_t *A;
	counters_t *B;
};

struct matches{
	int docID;
	int count;
};

int numMatches;
int whichMatch;

// Function to check if the directory to read from is valid
// Returns true if the directory contains the output from crawler
bool IsCrawlerDirectory(char *dir)
{
	FILE *fp = NULL;
	char *file = malloc(strlen(dir) + strlen("/.crawler") + 1);

	// Check if the file name was properly allocated
	if (file == NULL){
		printf("Error: the name for the pageDir was not properly allocated\n");
		return false;
	}

	strcpy(file, dir);
	strncat(file, "/.crawler", 10);

	fp = fopen(file, "r");

	// Check if the file ".crawler" was found
	if (fp == NULL){
		printf("Please provide a valid existing and/or readable pageDir\n");
		free(file);
		return false;
	}
	fclose(fp);
	free(file);
	return true;
}

// Function to check if word contains only letters
// Returns true if word contains only alphabetic characters
bool isLetters(char string[])
{
	for (int i = 0; i < strlen(string); i++){
		if (!isalpha(string[i]) && (string[i] != 32)){
			printf("Error: bad character '%c' in query\n", string[i]);
			return false;
		}
	}
	return true;
}

// Function for 'and' operation in query
static void match_intersect(counters_t *A, counters_t *B)
{
	struct counters ctrs;
	ctrs.A = A;
	ctrs.B = B;

	counters_iterate(A, intersect_helper, &ctrs);
}

// Function for 'or' operation in query
static void match_union(counters_t *A, counters_t *B)
{
	counters_iterate(A, union_helper, B);
}

// Function to help set frequency to sum of two frequencies
static void union_helper(void *arg, int key, int count)
{
	counters_t *B = arg;

	counters_set(B, key, count + counters_get(B, key));
}

// Function to help set frequency to lower of two frequencies
static void intersect_helper(void *arg, int key, int count)
{
	struct counters *ctrs = arg;
	counters_set(ctrs->A, key, min(count, counters_get(ctrs->B, key)));
}

// Function to return minimum between two integers
static int min(int a, int b)
{
	if (a > b){
		return b;
	}
	return a;
}

// Function to remove extra 'zero' results from counter
static void remove_extras(void *arg, int key, int count)
{
	if (count == 0){
		numMatches--;
	}
}

// Function to initialize each 'matches' struct
static void matches_helper(void *arg, int key, int count)
{
	if (count == 0){
		return;
	}
	(((struct matches*)arg)[whichMatch]).docID = key;
	(((struct matches*)arg)[whichMatch]).count = count;
	whichMatch++;
}

// Function to keep track of the number of matches
static void numberMatches(void *arg, int key, int count)
{
	numMatches++;
}

// Function to compare the frequencies in each document
// Returns an int and is used for 'qsort'
int compareMatches(const void *p, const void *q)
{
	struct matches *ip = (struct matches *)p;
	struct matches *iq = (struct matches *)q;
	int j = ip->count;
	int k = iq->count;
	return (k - j);
}

// Function to deal with querying by the BNF method
// Returns a counter of the resulting docID's and their corresponding frequencies
counters_t *BNFquery(index_t *index, char *queryWords[], int numWords)
{
	bool newCtr = false;
	bool firstPass = true;
	char *and = "and";
	char *or = "or";
	counters_t *nextCounter = NULL;
	counters_t *runningSum = counters_new();
	counters_t *runningProd = counters_new();
	if (runningProd == NULL){
		counters_delete(runningSum);
		printf("Error: the runningProd was not properly allocated\n");
		return NULL;
	}
	for (int i = 0; i < numWords; i++){
		nextCounter = index_find(index, queryWords[i]);
		if (firstPass){
			// Initialize the runningSum
			match_union(nextCounter, runningSum);
			firstPass = false;
		}
		if (nextCounter == NULL){
			nextCounter = counters_new();
			newCtr = true;
		}
		// Skip to the next word if it's 'and'
		if (strcmp(queryWords[i], and) == 0){
			continue;
		}
		// Do 'or' operation after the word or
		if (strcmp(queryWords[i], or) == 0){
			match_union(runningSum, runningProd);
			counters_delete(runningSum);
			runningSum = counters_new();
			firstPass = true;
		}

		// Intersect the runningSum and counter for parsed word
		match_intersect(runningSum, nextCounter);
		if (newCtr == true){
			counters_delete(nextCounter);
			newCtr = false;
		}
	}

	// Do the last union for the runningProd
	match_union(runningSum, runningProd);

	counters_delete(runningSum);
	return runningProd;
}

// Function to check that a query is valid
// Will provide error message if user's query is invalid
void validateQuery(char *dir, index_t *index)
{
	// Variable declarations
	int maxWordsInLine;
	int wordsInLine;
	int pos;
	int posInArray;
	char *line;
	char *url;
	char *word;
	char *currWord;
	char *prevWord;
	char *and = "and";
	char *or = "or";
	counters_t *result;
	bool continued = false;

	// Read until user ends the program
	while(!feof(stdin)){
		posInArray = 0;
		wordsInLine = 0;
		numMatches = 0;
		whichMatch = 0;
		line = readline(stdin);
		if (feof(stdin)){
			break;
		}

		// Determine the maximum possible number of words in the line
		maxWordsInLine = strlen(line)/2;
		char *wordsInQuery[maxWordsInLine];
		if (!isLetters(line)){

			// Clean up
			free(line);
			continue;
		}
		pos = 0;
		if ((pos = GetNextWord(line, pos, &word)) <= 0){
			printf("Please provide a non-empty input to query\n");

			// Clean up
			free(word);
			free(line);
			continue;
		}
		NormalizeWord(word);
		currWord = malloc(strlen(word) + 1);
		if (currWord == NULL){
			printf("Error: the currWord was not properly allocated\n");

			// Clean up
			free(line);
			return;
		}
		strcpy(currWord, word);

		// Check if the first word in the input was an operation
		if (strcmp(currWord, and) == 0){
			printf("Error: 'and' cannot be first\n");

			// Clean up
			free(currWord);
			free(word);
			free(line);
			continue;
		}
		if (strcmp(currWord, or) == 0){
			printf("Error: 'or' cannot be first\n");

			// Clean up
			free(currWord);
			free(word);
			free(line);
			continue;
		}
		wordsInQuery[posInArray] = malloc(strlen(currWord) + 1);
		if (wordsInQuery[posInArray] == NULL){
			printf("Error: the word in the query was not properly allocated\n");

			// Clean up
			free(currWord);
			free(word);
			free(line);
			return;
		}
		strcpy(wordsInQuery[posInArray], currWord);
		posInArray++;
		wordsInLine++;
		prevWord = malloc(strlen(word) + 1);
		if (prevWord == NULL){
			// Clean up
			free(currWord);
			free(line);
			free(wordsInQuery[posInArray]);
			printf("Error: the prevWord was not properly allocated\n");
			return;
		}
		strcpy(prevWord, currWord);

		// Clean up
		free(currWord);
		free(word);

		// Take off each word in the line
		while ((pos = GetNextWord(line, pos, &word)) > 0){
			NormalizeWord(word);
			currWord = malloc(strlen(word) + 1);
			if (currWord == NULL){

				// Clean up
				free(line);
				for (int i = 0; i < wordsInLine; i++){
					free(wordsInQuery[i]);
				}
				printf("Error: the currWord was not properly allocated\n");
				return;
			}
			strcpy(currWord, word);
			wordsInQuery[posInArray] = malloc(strlen(currWord) + 1);
			if (wordsInQuery[posInArray] == NULL){

				// Clean up
				for (int i = 0; i < wordsInLine; i++){
					free(wordsInQuery[i]);
				}
				free(line);
				free(currWord);
				printf("Error: the word in the query was not properly allocated\n");
				return;
			}
			strcpy(wordsInQuery[posInArray], currWord);
			posInArray++;
			wordsInLine++;

			// Check for adjacent operators
			if ((strcmp(prevWord, and) == 0) && (strcmp(currWord, or) == 0)){
				printf("Error: 'and' and 'or' cannot be adjacent\n");

				// Clean up
				for (int i = 0; i < wordsInLine; i++){
					free(wordsInQuery[i]);
				}
				free(currWord);
				free(prevWord);
				free(word);
				free(line);
				continued = true;
				break;
			}
			if ((strcmp(prevWord, or) == 0) && (strcmp(currWord, or) == 0)){
				printf("Error: 'or' and 'or' cannot be adjacent\n");

				// Clean up
				for (int i = 0; i < wordsInLine; i++){
					free(wordsInQuery[i]);
				}
				free(currWord);
				free(prevWord);
				free(word);
				free(line);
				continued = true;
				break;
			}
			if ((strcmp(prevWord, or) == 0) && (strcmp(currWord, and) == 0)){
				printf("Error: 'or' and 'and' cannot be adjacent\n");

				// Clean up
				for (int i = 0; i < wordsInLine; i++){
					free(wordsInQuery[i]);
				}
				free(currWord);
				free(prevWord);
				free(word);
				free(line);
				continued = true;
				break;
			}
			if ((strcmp(prevWord, and) == 0) && (strcmp(currWord, and) == 0)){
				printf("Error: 'and' and 'and' cannot be adjacent\n");

				// Clean up
				for (int i = 0; i < wordsInLine; i++){
					free(wordsInQuery[i]);
				}
				free(currWord);
				free(prevWord);
				free(word);
				free(line);
				continued = true;
				break;
			}

			free(prevWord);
			prevWord = malloc(strlen(word) + 1);
			if (prevWord == NULL){

				// Clean up
				free(line);
				for (int i = 0; i < wordsInLine; i++){
					free(wordsInQuery[i]);
				}
				printf("Error: the prevWord was not properly allocated\n");
				return;
			}
			strcpy(prevWord, word);
			free(word);
			free(currWord);
		}
		if (continued){
			continued = false;
			continue;
		}

		if (strcmp(prevWord, and) == 0){
			printf("Error: 'and' cannot be last\n");

			// Clean up
			for (int i = 0; i < posInArray; i++){
				free(wordsInQuery[i]);
			}
			free(prevWord);
			free(line);
			continue;
		}
		if (strcmp(prevWord, or) == 0){
			printf("Error: 'or' cannot be last\n");

			// Clean up
			for (int i = 0; i < posInArray; i++){
				free(wordsInQuery[i]);
			}
			free(prevWord);
			free(line);
			continue;
		}

		free(prevWord);
		free(line);

		printf("Query: ");
		for (int i = 0; i < wordsInLine; i++){
			printf("%s ", wordsInQuery[i]);
		}
		printf("\n");
		// Set a counter to the query result
		result = BNFquery(index, wordsInQuery, wordsInLine);
		counters_iterate(result, numberMatches, NULL);

		// Set the size for the array of 'matches'
		struct matches *ranks = malloc(numMatches * sizeof(struct matches));
		if (ranks == NULL){
			printf("Error: the ranks were not properly allocated\n");
			return;
		}

		// Reduce counter to proper size
		counters_iterate(result, remove_extras, ranks);

		// Check if there is at least one match
		if (numMatches == 0){
			printf("No documents match\n");
		}
		else{
			printf("Matches %d documents (ranked):\n", numMatches);
		}

		// Sort the counter for ranking
		counters_iterate(result, matches_helper, ranks);
		qsort(ranks, numMatches, sizeof(struct matches), compareMatches);

		FILE *fileToRead;
		char filename[FILENAME_MAX];

		// Print out the query results
		for (int i = 0; i < numMatches; i++){
			sprintf(filename, "%s/%d", dir, ranks[i].docID);
			fileToRead = fopen(filename, "r");

			// Handle file validity
			if (fileToRead == NULL){
				printf("Error: %s could not be read\n", filename);
			}

			// Print out an individual query result
			url = readline(fileToRead);
			printf("score %3d doc %3d: %s\n", ranks[i].count, ranks[i].docID, url);

			// Clean up
			free(url);
			fclose(fileToRead);
		}

		// Clean up
		counters_delete(result);
		for (int i = 0; i < wordsInLine; i++){
			free(wordsInQuery[i]);
		}
		free(ranks);
	}
}

// Function to query the words from stdin based on file created by indexer
// Looks up the url's from the pageDirectory
// Returns integer corresponding to exit code
int main(int argc, char *argv[])
{
	FILE *fp;
	char *dir;
	index_t *wordTable;

	// Check that the number of arguments is correct
	if (argc != 3){
		printf("%s requires two arguments, a pageDirectory and indexFilename\n", argv[0]);
		return 1;
	}

	// Check if the provided directory contains the output from crawler
	if (!IsCrawlerDirectory(argv[1])){
		return 1;
	}
	dir = malloc(strlen(argv[1]) + 1);
	if (dir == NULL){
		printf("Error: the name for the testDir was not properly allocated\n");
		return 1;
	}
	strcpy(dir, argv[1]);

	// Check if the provided index file is valid
	fp = fopen(argv[2], "r");
	if (fp == NULL){
		printf("Please provide a valid/readable index file\n");
		return 1;
	}

	// Create a new index data structure
	wordTable = index_new(10000);
	if (wordTable == NULL){
		printf("Error: the index was not properly allocated\n");
		free(dir);
		fclose(fp);
		return 2;
	}
	index_load(fp, wordTable);

	// Query the user's input
	validateQuery(argv[1], wordTable);

	// Clean up
	free(dir);
	fclose(fp);
	index_delete(wordTable);
	return 0;
}
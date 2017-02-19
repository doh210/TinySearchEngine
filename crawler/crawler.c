/*
 * crawler.c - Crawls the web from a given seed with a set depth
 *           - Stores the contents of each page, with a file per page
 *
 * David Oh
 *
 * COSC 50, July 2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include "../common/web.h"
#include "../lib/bag/bag.h"
#include "../lib/set/set.h"
#include "../lib/hashtable/hashtable.h"
#include "../lib/memory/memory.h"
#include "../lib/hashtable/jhash.h"

// log one word (1-9 chars) about a given url
inline static void logr(const char *word, const int depth, const char *url)
{
  printf("%2d %*s%9s: %s\n", depth, depth, "", word, url);
}

// Function to delete data for WebPage struct
static void WebPageDelete(void *data)
{
	if (data != NULL){
		free(((struct WebPage*)data)->url);
		free(((struct WebPage*)data)->html);
		free((struct WebPage*)data);
	}
}

// Function to check for proper number of arguments
// Returns true if proper number of arguments is provided
bool checkArgNum(int argc, char *command){
	if (argc != 4){
		printf("%s requires three args, a seedURL, a pageDirectory, and a maxDepth\n", command);
		return false;
	}
	return true;
}

// Function to check that the max depth provided is valid
// Returns true if the max depth is valid
bool checkMaxDepth(char *arg, int maxDepth){
	// Check if the max depth is an integer
	if (sscanf(arg, "%d", &maxDepth) < 1){
		printf("Please provide an integer value for maxDepth\n");
		return false;
	}
	// Check that the max depth is between 0 and 10
	if (maxDepth < 0 || maxDepth > 10){
		printf("The maxDepth must be between an integer between 0 and 10, inclusive\n");
		return false;
	}
	return true;
}

// Function to check if the directory to write to is valid
// Returns true if the directory is valid
bool isWritableDirectory(char *dir){
	FILE * fp = NULL;
	char *file = malloc(strlen(dir) + strlen("/.crawler") + 1);
	// Check if the file name was properly allocated
	if (file == NULL){
		printf("Error: the name for the testDir was not properly allocated\n");
		return false;
	}

	// Copy the string
	strcpy(file, dir);
	strncat(file, "/.crawler", 10);

	fp = fopen(file, "w");

	// Check if the file ".crawler" could be made
	if (fp == NULL){
		printf("Please provide a valid existing and/or writable directory\n");
		free(file);
		return false;
	}
	fclose(fp);
	free(file);
	return true;
}

// Function to set the url for the seedURL from the argument from the command line
// Returns the url
char *setSeedURLPage(char *argURL)
{
	char *url = malloc(strlen(argURL) + 1);
	// Checks if the url was properly allocated
	if (url == NULL){
		printf("Error: the URL for struct WebPage was not properly allocated\n");
		return NULL;
	}
	else{
		strcpy(url, argURL);
		return url;
	}
}

// Function to create a new WebPage struct
WebPage *newURLPage(){
	WebPage *page = malloc(sizeof(WebPage));

	// Check if the WebPage struct was properly allocated
	if (page == NULL){
		printf("Error: the struct WebPage was not properly allocated\n");
		return NULL;
	}
	else {
		return page;
	}
}

// Function to write the html of a WebPage to a file
void pagesaver(int i, char *dir, WebPage *item){
	FILE *crawledFile;
	char strID[FILENAME_MAX];
	char depth[FILENAME_MAX];

	// Create the file name
	sprintf(strID, "%s/%d", dir, i);

	// Open the file, return otherwise
	crawledFile = fopen(strID, "w");
	if (crawledFile == NULL){
		return;
	}

	// Add the url as the first line
	fputs(item->url, crawledFile);
	fputs("\n", crawledFile);

	// Add the depth as the second line
	sprintf(depth, "%d", item->depth);
	fputs(depth, crawledFile);
	fputs("\n", crawledFile);

	// Add the html as the third line onward
	fputs(item->html, crawledFile);
	fclose(crawledFile);

	// Note that the file was written
	logr("Saved", item->depth, item->url);
}

// Function to check a WebPage for embedded URL's
void pagescanner(bag_t *pageBag, hashtable_t *urlHashtable, WebPage *pageToCheck){
	char *url;
	int pos = 0;
	WebPage *newPage;

	// Note that WebPage is being checked for embedded URL's
	logr("Scanning", pageToCheck->depth, pageToCheck->url);
	while ((pos = GetNextURL(pageToCheck->html, pos, pageToCheck->url, &url)) > 0){

		// Check if the URL can be normalized, continue otherwise
		if (!NormalizeURL(url)){
			logr("Found", pageToCheck->depth, url);
			logr("IgnExtrn", pageToCheck->depth, url);
			free(url);
			continue;
		}
		logr("Found", pageToCheck->depth, url);

		// Check if the URL is internal, continue otherwise
		if (!IsInternalURL(url)){
			logr("IgnExtrn", pageToCheck->depth, url);
			free(url);
			continue;
		}

		// Create a new WebPage for the URL
		newPage = newURLPage();
		// Check if the WebPage was properly allocated
		if (newPage == NULL){
			free(url);
			return;
		}

		// Initialize the new WebPage
		newPage->url = malloc(strlen(url) + 1);
		newPage->depth = pageToCheck->depth + 1;
		newPage->html = NULL;
		// Check that the url for the new WebPage was properly allocated
		if (newPage->url == NULL){
			printf("Error: the url for %s was not properly allocated\n", url);
			free(newPage);
			free(url);
			return;
		}
		// Set the url of the new WebPage
		strcpy(newPage->url, url);
		free(url);

		// Check if the URL had a corresponding HTML
		if (!GetWebPage(newPage)){
			logr("IgnExtrn", pageToCheck->depth, newPage->url);
			free(newPage->url);
			free(newPage->html);
			free(newPage);
			continue;
		}

		// Note that the URL has been visited
		if (hashtable_insert(urlHashtable, newPage->url, "")){
			logr("Added", pageToCheck->depth, newPage->url);
			bag_insert(pageBag, newPage);
		}
		else{
			free(newPage->url);
			free(newPage->html);
			free(newPage);
		}
	}
}

// Function to crawl through a website given a seedURL, test directory, and depth
// Returns integer corresponding to exit code
int main(int argc, char *argv[])
{
	// Variable declarations
	int maxDepth = -1;
	int pageID = 1;
	WebPage *page;
	WebPage *extract;
	bag_t *pageBag;
	hashtable_t *urlTable;

	// Check for the correct number of arguments
	if (!checkArgNum(argc, argv[0])){
		return 1;
	}

	// Check for valid arguments
	if (!checkMaxDepth(argv[3], maxDepth)){		// Check that maxDepth is valid
		return 1;
	}
	sscanf(argv[3], "%d", &maxDepth);

	if (!isWritableDirectory(argv[2])){			// Check that directory is valid
		return 1;
	}

	// Try creating a new WebPage for the seedURL
	page = newURLPage();
	if (page == NULL){
		return 1;
	}

	// Initialize the WebPage for the seedURL
	page->url = setSeedURLPage(argv[1]);		// Create page for seedURL
	page->depth = 0;							// Set to depth of 0
	page->html = NULL;
	if (page->url == NULL){
		free(page);
		printf("Error: the seedURL was not properly allocated\n");
		return 1;
	}

	// Check if the seedURL can be normalized
	if (!NormalizeURL(page->url)){
		free(page->url);
		free(page);
		printf("The seedURL could not be normalized\n");
		return 1;
	}

	// Check if the seedURL is internal
	if (!IsInternalURL(page->url)){
		free(page->url);
		free(page);
		printf("The seedURL is not internal\n");
		return 1;
	}

	// Check if the seedURL has a corresponding HTML
	if (!GetWebPage(page)){
		free(page->url);
		free(page);
		printf("Please provide a valid seedURL\n");
		return 1;
	}

	// Create a new bag to store the WebPage struct
	pageBag = bag_new(WebPageDelete);
	if (pageBag == NULL){
		free(page->url);
		free(page);
		printf("Error: the struct bag was not properly allocated\n");
		return 2;
	}
	bag_insert(pageBag, page);

	// Create a new hashtable to keep track of visited URL's
	urlTable = hashtable_new(100, NULL);
	hashtable_insert(urlTable, page->url, "");
	while ((extract = bag_extract(pageBag)) != NULL){
		// Note that the WebPage struct has been extracted from the bag
		logr("Fetched", extract->depth, extract->url);
		sleep(1);
		// Write the page to a file
		pagesaver(pageID++, argv[2], extract);
		// Check embedded URL's if not at max depth
		if (extract->depth < maxDepth){
			pagescanner(pageBag, urlTable, extract);
		}
		// Clean up the extract WebPage
		WebPageDelete(extract);
	}
	// Clean up
	bag_delete(pageBag);
	hashtable_delete(urlTable);

	return 0;
}
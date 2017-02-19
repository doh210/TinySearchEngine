David Oh, August 2016

Usage:

 * indexer.c - indexes the output contents from crawler, providing the frequency with which each word appears in each file
 * usage: the user must pass a crawlerDir (directory to read from), and an indexFile
 * Examples:
   * indexer ../data/ fileName
 * Exits with 0 if successful
 * Exits with 1 if error with arguments
 * Exits with 2 if error allocating memory for index

 * indextest.c - tests if the output file from indexer can be converted into an index data structure
 * usage: the user must pass the indexFile created by crawler and a new file to write into
 * Examples:
   * indextest oldIndexFile newIndexFile
 * Exits with 0 if successful
 * Exits with 1 if error with arguments

Assumptions:

 * Files in the crawlerDir are numbered sequentially without gaps
 * The contents in each file in the crawlerDir follow the formatting defined in the crawler specs

 * The oldIndexFile given to indextest will follow the format of:
   * word docID count [docID count]...
 * where word is a string of lower-case letters,
 * where docID is a positive non-zero integer,
 * where count is a positive non-zero integer,
 * where the word and integers are separated by spaces.

"Non"-Assumptions:

 * 

Limitations:

 * If there is a gap in the file numbering, files after the gap will not be considered
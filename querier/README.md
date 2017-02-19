David Oh, August 2016

Usage:

 * querier.c - queries user input with information from the 'crawler' and 'indexer' outputs and ranks the results
 * usage: the user must pass the indexFile created by indexer and crawlerDir filled with contents by crawler
 * Examples:
   * querier crawlerDir indexfile
 * Exits with 0 if successful
 * Exits with 1 if error with command-line arguments

Assumptions:

 * Files in the crawlerDir are numbered sequentially without gaps
 * The contents in each file in the crawlerDir follow the formatting defined in the 'crawler' specs

 * The indexFile given to indextest will follow the format of:
   * word docID count [docID count]...
 * where word is a string of lower-case letters,
 * where docID is a positive non-zero integer,
 * where count is a positive non-zero integer,
 * where the word and integers are separated by spaces.

"Non"-Assumptions:

 * 

Limitations:

 * If there is a gap in the file numbering, files after the gap will not be considered
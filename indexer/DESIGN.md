#DESIGN.md

David Oh, August 2016

Requirements spec:

	1. The indexer shall:
	  * execute from a command line with usage syntax
	    */indexer pageDirectory indexFilename
	  * where pageDirectory is a directory produced by the crawler, and
	  * where indexFilename is the name of a file into which the index should be written.
	  * read documents from the pageDirectory, each of which has a unique document ID,
	  * wherein the document id starts at 1 and increments for each new document,
	  * and the filename is of form pageDirectory/id,
	  * and the first line of the file is the URL,
	  * and the second line of the file is the depth,
	  * and the rest of the file is the page content.
	  * build an inverted-index data structure mapping from words to (documentID, count) pairs, wherein each count represents the number of occurrences of the given word in the given document. Ignore words with fewer than three characters, and NormalizeWord before indexing.
	  * create a file indexFilename and write the index to that file, in the format described below.

	2. The indexer shall validate its command-line arguments:
	  * pageDirectory is a directory produced by the crawler (see below), and
	  * indexFilename is the name of a writeable file.
	  * The indexer may assume that
	    * pageDirectory has files named 1, 2, 3, …, without gaps.
	    * The content of files in pageDirectory follow the format as defined in the crawler specs; thus your code (to create a WebPage by reading a file) need not have extensive error checking.

	3. The index tester shall:
	  * execute from a command line with usage syntax
	    * ./indextest oldIndexFilename newIndexFilename
	  * where oldIndexFilename is the name of a file produced by the indexer, and
	  * where newIndexFilename is the name of a file into which the index should be written.
	  * load the index from the oldIndexFilename into an inverted-index data structure.
	  * create a file newIndexFilename and write the index to that file, in the format described below.

Design spec:
	1. The index data structure is analagous to the hashtable data structure found in the 'lib' directory
	2. Each key in a slot of the index data structure corresponds to a word that has been found in a page file
	3. The data for each word corresponds to a counters structure
	4. Each counters structure stores the ID's of the documents in which the word was found as the key
	5. The "count" or frequency of each ID key is the frequency with which the word appeared in that file

Testing plan:
	1. The index test will create a new index data structure and load in the data from the file created by 'indexer'
	2. The output file from 'indexer' will be read line by line and have the first string be stored as its key
	3. After the first string, pairs of integers will be read at a time, correspond to the document ID and frequency, respectively
	4. The index test will save/write these to a new file in the same manner in which indexer saved/wrote its index contents to a file
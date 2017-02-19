David Oh, July 2016

Usage:

 * crawler.c - crawls from a specified seedURL for a set depth provided by the user and writes the contents of each crawled page to a file in a directory specified by the user
 * usage: the user must pass a seedURL, testDir (directory to write into), and a maxDepth
 * Examples:
 * crawler http://old-www.cs.dartmouth.edu/~cs50/index.html test/ 2

Assumptions:

 * 64 bytes in 2 blocks still reachable from curl_easy_init is ok

"Non"-Assumptions:

 * 

Limitations:

 * IsInternalURL limits the seedURL to websites within a specific domain (i.e. this program should not be able to crawl Wikipedia or Google)
 * The user may try to crawl external sites, but will receive a message that it is not internal
 * Will not crawl more than a depth of 10
 * Specified in the instructions that the depth should be between 0 and 10
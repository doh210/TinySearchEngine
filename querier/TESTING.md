TESTING for querier
David Oh, August 2016

### Check for correct number of arguments
[flume:querier] 178) querier
querier requires two arguments, a pageDirectory and indexFilename

[flume:querier] 179) querier a b c d
querier requires two arguments, a pageDirectory and indexFilename

### Check for valid characters
[flume:querier] 180) querier ~cs50/data/crawler/cs50-depth1/ ~cs50/data/crawler/cs50-depth1/index
asdf qwer ghjk 1234
Error: bad character '1' in query
<12345>
Error: bad character '<' in query
!asdf
Error: bad character '!' in query

### Check for valid usage of operators
[flume:querier] 207) querier ~cs50/data/crawler/cs50-depth1/ ~cs50/data/crawler/cs50-depth1/index
asdf and or asdf
Error: 'and' and 'or' cannot be adjacent
asdf or or asdf
Error: 'or' and 'or' cannot be adjacent
asdf or and asdf
Error: 'or' and 'and' cannot be adjacent
asdf and
Error: 'and' cannot be last
asdf or
Error: 'or' cannot be last
and asdf
Error: 'and' cannot be first
or asdf
Error: 'or' cannot be first

### Check on query with no results
[flume:querier] 208) querier ~cs50/data/crawler/cs50-depth1/ ~cs50/data/crawler/cs50-depth1/index
asdf
Query: asdf
Matches 0 documents (ranked):
include

### Check on single-word queries
[flume:querier] 209) querier ~cs50/data/crawler/cs50-depth1/ ~cs50/data/crawler/cs50-depth1/index
Query: include
Matches 2 documents (ranked):
score   2 doc   7: http://old-www.cs.dartmouth.edu/~cs50/Logistics/
score   1 doc   2: http://old-www.cs.dartmouth.edu/~xia/
palmer
Query: palmer
Matches 9 documents (ranked):
score   2 doc   6: http://old-www.cs.dartmouth.edu/~cs50/Resources/
score   1 doc   1: http://old-www.cs.dartmouth.edu/~cs50/index.html
score   1 doc   3: http://old-www.cs.dartmouth.edu/~cs50/Labs/Lab0-Preliminaries.html
score   1 doc   4: http://old-www.cs.dartmouth.edu/~cs50/Project/
score   1 doc   5: http://old-www.cs.dartmouth.edu/~cs50/Labs/
score   1 doc   7: http://old-www.cs.dartmouth.edu/~cs50/Logistics/
score   1 doc   8: http://old-www.cs.dartmouth.edu/~cs50/Reading/
score   1 doc   9: http://old-www.cs.dartmouth.edu/~cs50/Lectures/
score   1 doc  10: http://old-www.cs.dartmouth.edu/~cs50/
xia
Query: xia
Matches 10 documents (ranked):
score   3 doc   2: http://old-www.cs.dartmouth.edu/~xia/
score   1 doc   1: http://old-www.cs.dartmouth.edu/~cs50/index.html
score   1 doc   3: http://old-www.cs.dartmouth.edu/~cs50/Labs/Lab0-Preliminaries.html
score   1 doc   4: http://old-www.cs.dartmouth.edu/~cs50/Project/
score   1 doc   5: http://old-www.cs.dartmouth.edu/~cs50/Labs/
score   1 doc   6: http://old-www.cs.dartmouth.edu/~cs50/Resources/
score   1 doc   7: http://old-www.cs.dartmouth.edu/~cs50/Logistics/
score   1 doc   8: http://old-www.cs.dartmouth.edu/~cs50/Reading/
score   1 doc   9: http://old-www.cs.dartmouth.edu/~cs50/Lectures/
score   1 doc  10: http://old-www.cs.dartmouth.edu/~cs50/

### Check on double-word queries
[flume:querier] 210) querier ~cs50/data/crawler/cs50-depth1/ ~cs50/data/crawler/cs50-depth1/index
include palmer
Query: include palmer
Matches 1 documents (ranked):
score   1 doc   7: http://old-www.cs.dartmouth.edu/~cs50/Logistics/
include and palmer
Query: include and palmer
Matches 1 documents (ranked):
score   1 doc   7: http://old-www.cs.dartmouth.edu/~cs50/Logistics/
include or palmer
Query: include or palmer
Matches 10 documents (ranked):
score   3 doc   7: http://old-www.cs.dartmouth.edu/~cs50/Logistics/
score   2 doc   6: http://old-www.cs.dartmouth.edu/~cs50/Resources/
score   1 doc   2: http://old-www.cs.dartmouth.edu/~xia/
score   1 doc   1: http://old-www.cs.dartmouth.edu/~cs50/index.html
score   1 doc   3: http://old-www.cs.dartmouth.edu/~cs50/Labs/Lab0-Preliminaries.html
score   1 doc   4: http://old-www.cs.dartmouth.edu/~cs50/Project/
score   1 doc   5: http://old-www.cs.dartmouth.edu/~cs50/Labs/
score   1 doc   8: http://old-www.cs.dartmouth.edu/~cs50/Reading/
score   1 doc   9: http://old-www.cs.dartmouth.edu/~cs50/Lectures/
score   1 doc  10: http://old-www.cs.dartmouth.edu/~cs50/

### Check on multiple-word queries
[flume:querier] 211) querier ~cs50/data/crawler/cs50-depth1/ ~cs50/data/crawler/cs50-depth1/index
xia or palmer and include
Query: xia or palmer and include
Matches 10 documents (ranked):
score   3 doc   2: http://old-www.cs.dartmouth.edu/~xia/
score   2 doc   7: http://old-www.cs.dartmouth.edu/~cs50/Logistics/
score   1 doc   1: http://old-www.cs.dartmouth.edu/~cs50/index.html
score   1 doc   3: http://old-www.cs.dartmouth.edu/~cs50/Labs/Lab0-Preliminaries.html
score   1 doc   4: http://old-www.cs.dartmouth.edu/~cs50/Project/
score   1 doc   5: http://old-www.cs.dartmouth.edu/~cs50/Labs/
score   1 doc   6: http://old-www.cs.dartmouth.edu/~cs50/Resources/
score   1 doc   8: http://old-www.cs.dartmouth.edu/~cs50/Reading/
score   1 doc   9: http://old-www.cs.dartmouth.edu/~cs50/Lectures/
score   1 doc  10: http://old-www.cs.dartmouth.edu/~cs50/
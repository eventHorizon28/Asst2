# Asst2
invertedIndex


Introduction:
In this assignment we practiced using the file system API (as well as pointers in different
data structures). In particular you will be creating, opening, reading, writing, and deleting files. Your
task is to write an indexing program, called an indexer. Given a set of files, an indexer will parse the
files and create an inverted index, which maps each token found in the files to the subset of files that
contain that token. In your indexer, you will also maintain the frequency with which each token appears
in each file. The indexer should tokenize the files and produce an inverted index of how many times
the word occurred in each file, sorted by word. Your output should be in the following format:


Analysis:Since we used a bst and a heap of nodes we expcted  worst case  n^2 for each run through best case  logn^2

<fileIndex>
<word text=”word0”>
<file name=”filename0”>count0</file>
<file name=”filename1”>count1</file>
</word>
<word text=”word1”>
<file name=”filename2”>count2</file>
<file name=”filename3”>count3</file>
<file name=”filename4”>count4</file>
</word>
</fileIndex>


Built With:
vim-editor used

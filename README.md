# Hash Research

In this project we will research different hash functions. We will find out which function behaves the best with large texts. Then we will optimize the calculations of these funcitons with inline assembler.


## Looking at different functions

We will research these 6 functions:
* Constant
* Length of the word
* Sum of ASCII codes of the letters
* Sum of ASCII codes divided by length
* XOR between current result and current ASCII code, then cyclic shift of the result
* CRC32

We will store the calculated hashs in a hash table using separate chaining method (each array element contains a list of words whose hash amounts match the index of the array). We will read a large text from a file and calculate hash amounts for each word. After this we will get 6 tables, each for its hash funcition. At the end we will print all list sizes from created tables in the shown format:
```
0, 2, 6, 2, 87, 45, 
5, 7, 2, 7, 4, 3, 
...
3, 4, 8, 2, 46, 0, 
```
Earch row represents the function type and earch collumns show the list sizes. This format allows us to easily import data to Excel.

After we got all data we can analyse our functions. There are graphs of them:

<img src="Images/Graphs.png" width="720" alt="Graphs" />

As we can see, the most uniform functions are "XOR and Shift" and "CRC32". The worst funtion is of course the constant as it has the most amount of collisions (the total number of words).


## Optimisation

Let's determine how long does it take to calculate earch hash function! First let's modify out prigram so that it doesn't generate hash tables but just calculates hash amounts. Now we have to get the times every function runs. To do this we will just put these two strings at the beginning and the end of each function:
```
int tempClocks = clock();
...<function code>...
*function name*Clocks += clocks() - tempClocks;  // For example crc32Clocks += clocks() - tempClocks;
```

At the end of the program we will just print these times. For more accurate results we will loop the calculation of hash amounts 300 times (so to get the accual results we have to divide obtained values by 300).

After running our program we got the folowing times:

<img src="Images/UnoptimizedTimes.png" width="480" alt="Unoptimized times" />  


Now let's try to compile with the highest optimize option of GCC - "O3"!
The results are:

<img src="Images/O3Times.png" width="480" alt="O3 times" />

# Plagiarism-detector
The purpose of this program is to detect plagiarism between two files containing <b> code source C</b>.
This program is able to identify:
* renaming variables or functions;
* moving variables or functions to different places in the code;
* adding, modifying or deleting non-essential information.


## GETTING STARTED

### Prerequisites
* Have a C compiler.

### Installation & use
To install Plagiarism-detector, follow these steps:

#### 1. Clone the Repository
Open the terminal:
```sh
git clone https://github.com/MenSouf07/Plagiarism-detector.git
cd Plagiarism-detector
```
#### 2. Compile the Program
To compile the program, execute the following command in the terminal:
```sh
gcc detector.c -o detector
```
#### 3. Execution
To compare $k$ files between them (respectively called <i>file_1.c</i>, <i>file_2.c</i>, ..., <i>file_k.c</i>) write in the terminal :
```sh
./detector acces/path/file_1.c acces/path/file_2.c ... acces/path/file_n.c                   #Linux
detector.exe C:\acces\path\file_1.c C:\acces\path\file_2.c ... C:\acces\path\file_n.c        #Windows
```
Make sure to replace "access/path/file_i.c" (respectively "C:\access\path\file_i.c") with the actual path of your <i>file_i.c</i> file.
<br>
The program then asks you if you want to use the Levenshtein distance or the Dice distance. <b>we advise you to choose that of Levenshtein for better results</b>.
<br>
PS: If you wish to test the Dice distance you will be asked to choose the length of the n-gram used, <b>you will need to enter 1 or 2</b>.
<br>
Finally, you will be asked to make a final decision regarding the translation, <b>you will need to enter 1</b>.

#### 4. Results
##### A. Distance
If you compare only two files, a number between 0 and 1 will be displayed. It corresponds to the distance between files. 
The closer the number is to 0, the more similar the files compared are. The closer the number is to 1, the more different the files compared are.
<br>
If you compare more than two files, the display will be the same. There will only be as many numbers as possible comparisons.

##### B. Matrix
In addition to the distance, a matrix will also be displayed.
<br>
When comparing $k$ files having the following names <i>file_1.c</i>,..., <i>file_k.c</i> and using the following order when executing <i>file_1.c</i>, <i>file_2.c</i>,..., <i>file_k.c</i>, consider that <i>file_1.c</i> is associated with row 1 and column 1 of the matrix, <i>file_2.c</i> is associated with row 2 and column 2 of the matrix, etc. Thus, the coefficient at row $i$ and column $j$ is associated with <i>file_i.c</i> and <i>file_j.c</i>. This means that the coefficient at line $i$ and column $j$ corresponds to the distance between the $i$-th file placed on the command line and the $j$-th file placed on the command line (not counting the a.out file).

##### C. PGM files
In addition to display, if you compare files with the name file1.c and file2.c, three PGM files will be created:
- <i>similarite_file1_file2.pgm</i>
- <i>couplage_file1_file2.pgm</i>
- <i>filtrage_file1_file2.pgm</i>
<br>
If we compare more than two files, these three PGM files will be generated for each possible comparison.
see the "HOW THE PROGRAM WORKS" section to understand these files.





## PROGRAM LIMITATIONS
* Comment of the form /*. . . */ if a large quantity of comments of this type are put, they will flood the code with unnecessary lines which means that the result will be a little distorted
* Does not work well on small files of a few lines, especially if we take a file of a few lines and copy it but by changing the lines we will find ourselves with a not very reliable result
* instruction placed on several lines for example the following lines are equivalent in C (int i = 1; and the same thing on several lines). However, our program does not understand that it is the same line and will separate the 2nd version into 5 segments different, so the program will not work.
* For an unknown reason the terminal displays this error message after a certain execution
of the program: ”*** stack smashing detected ***: unknown terminated Abandon (core
dumped)” all allocated areas are freed correctly and most tables
are managed with malloc so we did not understand where the error comes from. However,
the program works perfectly and the error message although it has been there since the
beginning does not seem to have any visible impact
* Our program translates floats by w.w for example: float f = 2.0; becomes
mw=w.w; So if someone plagiarizes a file and changes the lines like “int i = 2;”
by “float f = 2.0” our program will not give us a distance between the 2 chains
equal to 0, we will however have a distance close to 0 but not equal.







<a name="explanation"></a>
## HOW THE PROGRAM WORKS
To explain the program, we will assume that we are just comparing 2 files named <i>file1.c</i> and <i>file2.c</i>.<br>
If we compare more than 2 files we will anyway compare files 2 to 2.

### Pre-treatment
The first step of the program is to transform each non-empty line of the files so that
to eliminate information irrelevant to the analysis and keep only the “structure” of the
program. The project must therefore perform the following operations on each line of the files:
* deletion of comments (we will limit ourselves to comments starting with //);
* deletion of the content of character strings;
* each word (sequence of alphanumeric characters) is transformed into a single ca-
character 'w'. This allows you to avoid renaming variables or functions
since each identifier (and each keyword) is transformed into a unique
letter ;
* removing all spaces and tabs.
There must therefore only remain in each line the code punctuation characters
(parentheses, brackets, semicolons, operators, etc.) and the 'w' which mark the place-
words.
For example the line:
```c
if (a>2) { printf("%d\n", a*T[i]); return 0; } // END
```

will be transformed into:
```c
w(w>w){w("",w*w[w]);ww;}
```
In order to better capture the structure of the code, it is possible to replace each word with
'w' EXCEPT if it is a C keyword which is then replaced by 'm'. The previous example
will therefore give the line:
```c
m(w>w){w("",w*w[w]);mw;}
```
You can select whether you want to replace the keywords with 'w' or 'm' at the start of execution.


### Breaking into blocks/segments
The second step consists of cutting each file into elementary blocks, called segments, which we will be able to compare with each other. For simplicity, each line will form a segment.
For this we created the “Ligne” structure which is generally a linked list


### Similarity between segments
We seek to compare these segments two by two to detect similarities between the files.
We now find ourselves with n segments in the first file and m in the second.
<br>
For each pair of segments ($S^1_i$, $S^2_j$), where the first segment is in the first file and the second segment in the second file, 
we calculate the [Dice distance](https://en.wikipedia.org/wiki/Sørensen–Dice_coefficient) or the [Levenshtein distance](https://en.wikipedia.org/wiki/Levenshtein_distance) (choice given to the user at the start of the program), between 0 (identical segments) and 1 (totally different segments).
<br>
We thus obtain a matrix D of size n×m with coefficients between 0 and 1 giving the similarity between two segments.
<br>
We will generate an image file <i>Similarite_file1_file2.pgm</i>, in gray level, representing
the matrix D, the pixel being all the lighter the closer the segments are.

### Coupling of segments
We now have a matrix of distances between each segment of each program. In order to detect plagiarism, it is necessary to identify, between the two programs, what are the
segments that look similar (they are not necessarily in the same place because there may have been
permutations of lines). 
<br>
To do this we will associate the segments two by two and obtain
a list of couples ($S^1$, $S^2$). Of course, we want to associate similar segments, which
are at a short distance. We are therefore looking for a list of couples, as large as possible,
such that the sum of all the distances of each pair is as small as possible.
<br>
In our case, this amounts to taking the smaller of the two programs (the one which has
the fewest segments) and to seek to associate each segment with a segment which
looks like in the second program.

#### Greedy minimal coupling algorithm
Finding the maximum coupling of minimum weight is done with a fairly complicated algorithm but here we use a greedy algorithm which gives a fairly
good approximation of the result
The principle of the algorithm is to systematically search for the two segments which are
most resemble in the matrix D (therefore the smallest coefficient).
1. find the smallest coefficient of the matrix; it is $d_{i,j}$, at the intersection of the line
$0 \leq i < n$ and column $0 \leq j < m$;
2. We then associate the segments $i$ of the first program and $j$ of the second, and we “remove” them from the matrix leaving $d_{i,j}$ but putting all the other coefficients of the line
$i$ and column $j$ to the value 1 and memorizing that this row and this column already have
been used;
3. we start again by looking for the smallest coefficient in the rest of the matrix as long as
that the smallest program still has one segment to associate.
We end up with a matrix C filled with 1s except when the segments $i$ and $j$
have been coupled and for which the coefficient $d_{i,j}$ gives the distance between these two segments.
We then generate an image file <i>Couplage_file1_file2.pgm</i> representing the matrix C, the pixel being
all the clearer as the coefficient is small (and therefore the close segments).

### Post-filtering
We make the hypothesis that two documents are suspicious if they have blocks of segments
suspicious contiguous, that is to say consecutive segments coupled to the previous step. The goal
of this step is therefore to remove the couplings of isolated segments which are probably
more coincidences than plagiarism.
<br>
To do this, we will recalculate each coefficient $c_{i,j}$ of the previous matrix as
an average with the four adjacent coefficients located on the same diagonal
(and therefore corresponding to five consecutive segments in the two files) and only keep the coefficients sufficiently small via a threshold function. If consecutive segments of each
files are plagiarized, this will mean that several consecutive coefficients (on a diagonal)
will be close to 0 and their average as well. On the other hand, an isolated coupling will not produce
only one small coefficient out of the 5, therefore a high average which will then be eliminated by
the threshold.
<br>
Thus, from the matrix C obtained in the previous step, we calculate a new matrix F
as following :
<br>
$f_{i,j} = \frac{1}{5} \sum_{-2 \leq k \leq 2} c_{i+k,j+k}$  (convolution)
<br>
then,  $f_{i,j} = 1$ si $f_{i,j} \geq 0.7$  (threshold)
<br>
We generate an image file <i>filtrage_file1_file2.pgm</i> representing the final matrix F, the pixel
being all the clearer as the coefficient is small.


### Output of results
We can now calculate the “distance” between the two files by summing the
distances between coupled segments.
<br>
$\Delta = 1 - \frac{1}{\min(n, m)} \sum_{\substack{0 \leq i < n , \\ 0 \leq j < m}} (1 - f_{i,j})$
<br>
The output of the program will therefore be the final distance value obtained with two digits
after the comma.



## AUTHORS & CONTACTS
Don't hesitate to contact us for more information :
* Mendel Souffir alias [@MenSouf07](https://github.com/MenSouf07)
  * mail : mendel.souffir@dauphine.eu / mendel.souffir07@gmail.com
  * [Linkedin](https://www.linkedin.com/in/mendel-souffir/)
* Anas Iboubkaren
  * mail : Anas.Iboubkaren@dauphine.eu
  * [Linkedin](https://www.linkedin.com/in/anas-iboubkaren-48b795218/)

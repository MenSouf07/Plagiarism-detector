# Plagiarism-detector
The purpose of this program is to detect plagiarism between two files containing <b> code source C</b>.
This program is able to identify:
* renaming variables or functions;
* moving variables or functions to different places in the code;
* adding, modifying or deleting non-essential information.

## HOW THE PROGRAM WORKS
<a id="explanation"></a>
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

















<br><br><br><br><br><br>
<br><br><br><br><br><br>
<br><br><br><br><br><br>










### Details
* If a text file already has the name “copy” in your directory, it will be deleted at runtime.
* The maximum label size is set at 300, to change it, you can change the defined N value
on line 29.
* In the event of a syntax error in the source file (incorrect line structure, unknown instruction, etc.),
the machine language file “hexa.txt” will not be created and the first erroneous line detected will be
indicated on the terminal.
* This program does not necessarily support logic errors that may occur in
the source file: it was designed to stop in the event of a system error (i.e. access out of memory, jmp out of
program…) It is therefore the user's responsibility to avoid writing an infinite program or
to write a halt instruction, for example.



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
gcc plagiarism-detector.c -o plagiarism
```
#### 3. Execution
To compare n files between them (respectively called <i>file_1.c</i>, <i>file_2.c</i>, ..., <i>file_n.c</i>) write in the terminal :
```sh
./plagiarism acces/path/file_1.c acces/path/file_2.c ... acces/path/file_n.c                   #Linux
plagiarism.exe C:\acces\path\file_1.c C:\acces\path\file_2.c ... C:\acces\path\file_n.c        #Windows
```
Make sure to replace "access/path/file_i.c" (respectively "C:\access\path\file_i.c") with the actual path of your <i>file_i.c</i> file.


see [the explanation of the program](#explanation)



## AUTHORS & CONTACTS
Don't hesitate to contact us for more information :
* Mendel Souffir alias [@MenSouf07](https://github.com/MenSouf07)
  * mail : mendel.souffir@dauphine.eu / mendel.souffir07@gmail.com
  * [Linkedin](https://www.linkedin.com/in/mendel-souffir-1971252b0/)
* Anas Iboubkaren
  * mail : Anas.Iboubkaren@dauphine.eu
  * [Linkedin](https://www.linkedin.com/in/anas-iboubkaren-48b795218/)

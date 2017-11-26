# tableau-printer
Simplex solver and tableau printer written in C++. Input a tableau and choose the pivots, exports the corresponding latex.

# Usage
Input file format is as in `input.txt`, variables should be written along the first row.

Run the program with

```tableau-printer input.txt``

where m is the number of non-objective rows in the tableau in input.txt and n is the number of variables.


# Notes

Also note that with `Tableau<Fraction<int>>`, most *real* problems won't be solvable, since the ints in Fraction will overflow. However, for pretty-printing nice example tableaus, this works nicely. For the latter, consider `Tableau<Double>` but be wary of rounding errors..

Some of the features listed in the menu aren't yet implemented. Implementations and suggestions for more features are welcome.

I've moved the LaTeX making code into a bash script `latexify.sh`. Simply save the output of the program into a file and then rnu
`cat output.txt | ./latexify.sh` to get LaTeX output. Make sure to`\usepackage{amsmath}`.

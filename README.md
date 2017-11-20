# tableau-printer
Simplex solver and tableau printer written in C++. Input a tableau and choose the pivots, exports the corresponding latex.

# Usage
Input file format is as in `input.txt`, variables should be written in order along the first row.

Run the program with

```tableau-printer input.txt m n```

where m is the number of non-objective rows in the tableau in input.txt and n is the number of variables.

# Latex Output

Make sure to `\usepackage{amsmath}`

# Notes

Current main.cpp is just hacked together, feel free to use tableau.hpp independently...

Also note that with Tableau<Fraction<int>>, most *real* problems won't be solvable, since the ints in Fraction will overflow. However, for pretty-printing nice example tableaus, this works nicely. For the latter, consider Tableau<Double> but be wary of rounding errors..

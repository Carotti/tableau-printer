#!/bin/bash

# \begin{equation*}
# 	\begin{array}{c|c|cc|c|c}
# 	\text{BVs} & x_1 & x_2 & s_1 & s_2 & RHS \\ \hline
# 	z & 30 & 45 & 0 & 0 & 0 \\
# 	s_1 & 48 & 21 & 1 & 0 & 2000  \\
# 	s_2 & 5 & 4 & 0 & 1 & 200
# 	\end{array}
# \end{equation*}

first_line=false

output=""
while IFS='$\n' read -r line; do
    if [ "$(echo $line | awk '{print $1}')" == "BVs" ]; then

        if [ "$first_line" == false ]; then
            first_line=true
            echo "\\begin{equation*}"

            columns=""
            for i in $line; do
                columns+="c"
            done

            columns=$(echo $columns | sed -r "s/c(c*)c/c\|\1\|c/g")
            echo "\\def\arraystretch{1.5}"
            echo "\\begin{array}{$columns}"

            echo "$(echo $line | sed -r "s/(BVs|RHS)/\\\\text\{\1\}/g" | tr ' ' '&' | sed -r "s/&/ & /g") \\\\ \\hline"
        else
            output+="\\hline "
        fi

        read -r line
        while [ "$line" != "" ]; do
            output+="$(echo $line | tr ' ' '&' | sed -r "s/&/ & /g" | sed -r "s/(-?[0-9]+)\/(-?[0-9]+)/\\\\\\\frac\{\1\}\{\2\}/g") \\\\\\\\\n"
            read -r line
        done
    fi
done

echo -e ${output::-6}


echo "\\end{array}"
echo "\\end{equation*}"

#! /bin/bash

# http://bainite.wordpress.com/2008/07/26/fitting-polynomial-equations-with-gnuplot/


conv_table() {
(echo -n "{ print "; for i in `seq 0 9`; do echo -n "\$$(( $i+2 )) \" \" \$1+$i \"\\n\" "; done; echo "}"; ) > conv_table.awk; sed -e '/^$/d' $1 | awk -f conv_table.awk | sed -e '/^$/d' 
}

conv_table k-type_values.txt > k-type-gp.txt
conv_table j-type_values.txt > j-type-gp.txt

gen_fit() {
	n=$1
	for t in k j; do
		echo -n "fit${n}${t}(x) = "
		for i in `seq 0 $((n-1))`; do
			echo -n "c${n}_${i}_${t}*x**${i} + "
		done
		echo "c${n}_${n}_${t}*x**${n}"

		echo -n "fit fit${n}${t}(x) \"${t}-type-gp.txt\" via "
		for i in `seq 0 $((n-1))`; do
			echo -n "c${n}_${i}_${t},"
		done
		echo "c${n}_${n}_${t}"
	done
}

gnuplot <<EOF
set terminal pdfcairo
set output "plot.pdf"

$(gen_fit 1)
$(gen_fit 2)
$(gen_fit 3)



plot[-10:80] "k-type-gp.txt" lw 15 with lines title "k-type", \
  "j-type-gp.txt" lw 15 with lines title "j-type", \
  fit1k(x) title "P(1)_k" linetype 3 , \
  fit1j(x) title "P(1)_j" , \
  fit2k(x) title "P(2)_k" , \
  fit2j(x) title "P(2)_j" , \
  fit3k(x) title "P(3)_k" , \
  fit3j(x) title "P(3)_j" ;


print "> c1_0_k ", c1_0_k 
print "> c1_0_j ", c1_0_j 
print "> c1_1_k ", c1_1_k 
print "> c1_1_j ", c1_1_j 


print "> c2_0_k ", c2_0_k 
print "> c2_0_j ", c2_0_j 
print "> c2_1_k ", c2_1_k 
print "> c2_1_j ", c2_1_j 
print "> c2_2_k ", c2_2_k 
print "> c2_2_j ", c2_2_j 

EOF


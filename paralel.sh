#! /bin/bash
array=(15000 25000 35000 45000)
j=0
echo "*****************" > paralelScript.txt
echo "*NO PARALELITZAT*" >> paralelScript.txt
echo "*****************" >> paralelScript.txt
for i in 2 4 8 16 32 64 128 256
do
    for j in 15000 25000 35000 45000
    do
        let x=$j
        
        echo "Paralel $x, $i threads" >> paralelScript.txt
        echo "Paralel $x, $i threads"
        ((j++))
        
        (time ./paralel $x $i) &>> paralelScript.txt
		echo "-----------------------------------" >> paralelScript.txt
        echo "-----------------------------------"
    done
        echo "***********************************"
		echo "***********************************" >> paralelScript.txt	
done

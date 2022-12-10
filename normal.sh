#! /bin/bash
array=(15000 25000 35000 45000)
j=0

echo "*****************" > normalScript.txt
echo "*NO PARALELITZAT*" >> normalScript.txt
echo "*****************" >> normalScript.txt
for i in ${array[@]}
do
    echo "Normal $i" >> normalScript.txt
    echo "Normal $i"
   
    
    (time ./normal $i) &>> normalScript.txt
    echo "-----------------------------------" >> normalScript.txt
    echo "-----------------------------------"
done
normal : dist_full_AC.c 
	gcc -Wall dist_full_AC.c -o normal -lpthread

paralel : dist_full_AC_paral.c 
	gcc -Wall dist_full_AC_paral.c -o paralel -lpthread

clean : 
	rm -f normal paralel 
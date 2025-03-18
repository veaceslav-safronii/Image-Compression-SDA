SAFRONII VEACESLAV 315CD
Tema 2 - Arbori cuarternari
Teste 80/80
Bonus 20

Task 1:
*   Primim ca input un fisier tip PPM, citim primele 3 linii in format text,
	si retinem dimensiunea imaginii, dupa care citim in format binar,
	intr-o matrice de pixeli, valorile RGB, ale fiecarui pixel.

*   Calculam valorile medii pentru cele 3 culori ale intregii imagini,
	folosindu-le la crearea arborelui cuarternar, pentru calcularea
	scorului de similaritate ale blocului de imagine.

*   Recursiv este construit arborele, comparand valoarea mean-ului
	cu factorul de compresie dat.
	- In caz ca e mai mare ca factorul, apeleaza recursiv cele 4 noduri copii,
	se imparte dimensiunea la 2 si se recalculeaza valorile medii ale
	culorilor pentru fiecare copil.
	- In caz ca e mai mic ca factorul, se aloca memorie pentru campul data,
	ce este un pointer de tip RGB, care contine valorile medii RGB, acestea
	fiind initializate cu valorile calculate.

*   Se apeleaza o functie de calculare a numarului de niveluri din arbore.
	Aceasta determina recursiv pentru fiecare copil numarul de niveluri,
	dupa care intoarce valoarea maxima dintre cele 4.

*	Se apeleaza o functie ce calculeaza recursiv numarul
	total de frunze din arbore, (nr de blocuri nedivizate).
	
*	Se cauta cel mai mare bloc nedivizat / frunza de pe nivelul
	cel mai apropiat de radacina, folosind o variabila min_lvl
	care se actualizeaza, cand se gaseste o frunza care este
	pe un nivel curent mai mic decat min_lvl, si se pastreaza
	dimensiunea acesteia.

*	Se afiseaza nr de niveluri, nr de blocuri nedivizate si
	dimensiunea celui mai mare bloc nedivizat intr-un fisier
	de tip text.


Task 2:
*	Se parcurge arborele cuarternar pe niveluri, folosind o coada.
	Se adauga radacina in coada, se scrie dimensiunea imaginii
	in fisierul binar, dupa care cu un loop, ce se termina cand
	coada va fi golita, se verifica tipul nodului: intern sau frunza.
		- In cazul in care e intern se adauga copii acestuia in coada
		si se scrie binar tipul "0"
		- In cazul in care e frunza se scrie binar tipul nodului - "1"
		si valorile RGB continute de frunza.
	Astfel este realizata compresia imaginii si este creat fisierul comprimat.


Task 3:
* 	Se parcurge fisierul comprimat, fiind citita dimensiunea imaginii,
	dupa care este construit in paralel arborele cuarternar,
	se citeste tipul nodului.
		- In caz ca e intern sunt creati cei 4 copii ai sai, dupa care
		sunt pusi in coada
		- In caz ca e frunza se scoate din capul cozii nodul si i se
		citesc valorile RGB.

*	Sunt scrise primele 3 linii corespunzatoare tipului de fisier PPM,
	si dupa care este apelata functia de reconstructie a matricei de
	pixeli, care inainte de apelul recursiv la copii nodului verifica
	daca e nod frunza, si dupa care imparte dimensiunea la 2.
		- In cazul in care este frunza, se initializeaza culorile
		pentru toti pixelii ce fac parte din blocul respectiv de imagine.
	Dupa care este afisate binar valorile RGB ale fiecarui pixel din matrice. 

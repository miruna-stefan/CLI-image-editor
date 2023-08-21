# CLI-image-editor
The program performs multiple transformations on both color and black&amp;white images, such as rotation, filter application or cropping, while also implementing a Command Line Interface that supports several parameters.

Copyright 2023 Stefan Miruna Andreea 314CA
EDITOR DE IMAGINI

In rezolvarea problemei vom avea nevoie de o structura in care se vor retine 
toate datele despre imaginea incarcata:
- matricea pixelilor (daca imaginea este grayscale), respectiv 3 matrice de 
pixeli (daca imaginea este RGB), cate una aferenta fiecarei culori;
- latimea si inaltimea imaginii;
- o variabila in care se va retine valoarea 0 daca imaginea este grayscale, 
respectiv 1 daca imaginea este RGB
- 4 variabile cu ajutorul carora se va retine selectia curenta
- un sir de caractere care va stoca magic-word-ul (P2, P3, P5 sau P6), indicand
tipul imaginii
- o variabila ca indica daca exista vreo imagine incarcata
- valoarea maxima a pixelilor din imagine

Pana la intalnirea comenzii EXIT, in functia main se citeste primul cuvant din
comanda introdusa la tastatura si se apeleaza functia aferenta acesteia.
Functiile aferente comenzilor sunt:

1)"load": incarca o imagine in memorie astfel:citeste urmatorul cuvant introdus
de la tastatura pe acelasi rand cu "LOAD". Acesta reprezinta numele fisierului
ce trebuie incarcat. Indiferent de tipul fisierului, acesta va fi deschis doar
pentru citire. Se citesc din fisier datele despre imagine: magic-word-ul, 
latimea, inaltimea si valoarea maxima a pixelilor, avand grija sa ignoram 
comentariile cu ajutorul functiei "skip_comm". In functie de magic-word, se 
decide tipul fisierului de incarcat: daca fisierul este text, atunci continuam
citirea cu ajutorul functiei "read_ascii", iar daca fisierul este binar, se va 
deschide fisierul in format read binary, se va muta cursorul la inceputul 
matricei de pixeli si se va apela functia "read_binary", care va citi (si va 
aloca memorie pentru) matricea sau matricele de pixeli (in functie de culoarea
imaginii). Nu uitam sa selectam intreaga imagine dupa ce o incarcam in memorie.
Observatie: inainte de a incarca orice imagine, se verifica daca nu cumva deja
exista o imagine incarcata, caz in care se va elibera memoria aferenta imaginii 
anterioare.

2) "select_photo": selecteaza o anumita portiune dintr-o imagine astfel: mai
intai, se asigura ca dupa cuvantul "SELECT" nu a fost introdus caracterul '\n',
ceea ce ar indica lipsa oricarui parametru si ar duce la invaliditatea 
comenzii. Daca avem o comanda valida, continuam cu citirea parametrilor. Se 
citeste urmatorul string introdus dupa "SELECT" si se verifica daca nu cumva
este chiar cuvantul "ALL", ceea ce ar insemna ca ni se cere sa selectam 
intreaga imagine. Vom face acest lucru apeland functia "select_all". Daca sirul
de caractere nu era "ALL", atunci ne asteptam sa primim o comanda de forma 
"SELECT <x1> <y1> <x2> <y2>". Deci, deducem ca primul parametru citit ar trebui
sa fie x1. Verificam daca sirul de caractere este un numar astfel: mai intai, 
ne asiguram ca pe prima pozitie se gaseste ori o cifra, ori caracterul '-' (in
cazul numerelor negative). In continuare, verificam daca toate celelalte 
caractere din string sunt cifre. Daca da, atunci string-ul este valid si il
transformam in intreg. Se procedeaza analog si pentru urmatorii 3 parametri, 
doar ca pentru acestia 3 se apeleaza direct functia "next_parameter_validity", 
care verifica atat existenta urmatorului parametru cu ajutorul functiei
"verify_parameter_existence", il citeste, verifica daca este un numar si, in 
caz afirmativ, il transforma in intreg. Daca toti parametrii sunt valizi, se 
verifica faptul ca niciuna dintre coordonatele introduse nu iese in afara 
imaginii si ca x1 si x2, respectiv y1 si y2 sunt diferite, astfel incat 
intervalele [x1, x2), respectiv [y1, y2) sa aiba sens. Din acelasi considerent,
interschimbam valorile x1 si x2 sau y1 si y2 daca nu respecta ordinea corecta.
Observatie: in cazul in care nu exista nicio imagine incarcata in memorie, nu 
iesim din program inainte de a citi restul parametrilor, care desi nu ne mai 
intereseaza, ne-ar incurca mai incolo la citire. Pentru aceasta, ne folosim de
functia "read_dummy_input_select".

3) "crop": in functie de culoarea imaginii (grayscale sau RGB), se aplica 
functia "crop_grayscale" (in cazul imaginilor in tonuri de gri) sau functia 
"crop_RGB" (in cazul imaginilor color). Dupa aceasta, se actualizeaza 
latimea si inaltimea imaginii decupate si se selecteaza intreaga suprafata a 
noii imagini.
Functia "crop_grayscale" aloca memorie pentru o matrice auxiliara, fiecare element
din aceasta fiind initializat cu elementul de pe pozitia corespunzatoare din 
matricea initiala. Se dealoca memoria pentru matricea initiala, urmand sa i se
aloce din nou memorie, dar de data aceasta tinand cont de noile dimensiuni. Se 
copiaza aici elementele din matricea auxiliara si se elibereaza memoria 
acesteia din urma dupa ce am terminat copierea.
Functia "crop_RGB" face exact acelasi lucru ca functia "copy_grayscale", doar 
ca opereaza cu toate cele 3 matrice specifice imaginilor RGB in acelasi timp.

4) "histogram": afiseaza pe ecran histograma imaginii, la finalul parcurgerii 
urmatorilor pasi: dupa ce se verifica existenta a exact 2 parametri, x si y, 
precum si validitatea lor, se construieste vectorul de frecventa al elementelor
din matricea de pixeli (vectorul va avea 256 de pozitii, intrucat valorile 
elementelor din matrice pot varia intre 0 si 255 inclusiv). Cu ajutorul 
acestuia se construieste un al doilea de vector "de frecventa", care va avea un
numar de elemente egal cu numarul de bin-uri (y). Se imparte vectorul de 
frecventa initial in intervale de cate 256/y elemente, a caror suma se 
calculeaza si se pune pe pozitia corespunzatoare din al doilea vector. Pentru a
putea aplica formula numarului de stelute, trebuie aflat elementul maxim din al
doilea vector. La final, se afiseaza histograma si se elibereaza memoria 
alocata dinamic.

5) "apply_filter": functia are drept rezultat aplicarea unui anumit filtru 
asupra imaginii. Dupa verificarea validitatii comenzii, se foloseste o 
variabila pentru codificarea filtrului pe care trebuie sa-l aplicam si se
apeleaza functia "apply_matrix" pentru fiecare dintre cele 3 matrice aferente 
unei imagini RGB.
In functia "apply_matrix" se prelucreaza elementele unei singure matrice, in 
functie de valoarea variabilei care codifica filtrul primit ca parametru al 
comenzii APPLY. Dupa declararea kernel-ului specific fiecarui filtru, se aloca 
memorie pentru o matrice auxiliara, ale carei dimensiuni se vor baza pe 
selectia curenta. Pe fiecare pozitie din matricea auxiliara se pune suma 
obtinuta in urma aplicarii filtrului, cu mentiunea ca elementele de pe marginea
matricei initiale vor fi complet ignorate. Apoi, matricea auxiliara este 
copiata in matricea initiala si se elibereaza memoria pentru matricea 
auxiliara, caci nu mai avem nevoie de ea.

6) "equalize": realizeaza egalizarea imaginii urmand formula specificata in 
cerinta. Pentru a calcula acea suma care apare in formula, trebuie sa construim
si aici un vector de frecventa al elementelor din matricea pixelilor, urmat de 
un alt vector, tot cu 256 de elemente, in care pe fiecare pozitie se va afla 
suma aparitiilor tuturor termenilor anteriori. La final, se parcurge matricea,
aplicandu-se pentru fiecare element formula egalizarii.

7) "save": salveaza imaginea in formatul cerut. Consideram o variabila cu 
ajutorul careia se va codifica formatul fisierului ce trebuie salvat astfel: 0 
pentru salvare fisier text, respectiv 1 pentru salvare fisier binar. Stabilirea
valorii acestei variabile se face dupa ce se citeste numele fisierului si se 
verifica daca exista sau nu parametrul "ascii" in comanda. Mai intai, se 
deschide fisierul pentru scriere in format text si se scriu in fisier 
magic-word-ul, latimea si inaltimea, si valoarea maxima a pixelilor. In functie
de valoarea variabilei ce indica formatul fisierului de salvat, se procedeaza 
in felul urmator: Daca fisierul trebuie salvat in format text, se printeaza in 
fisier matricea sau matricele (in cazul pozelor RGB) pixelilor. Daca fisierul 
trebuie salvat in format binar, se inchide fisierul pe care il aveam deshis si 
se redeschide ca binary apppend, urmand sa fie adaugata in fisier matricea sau 
matricele (pentru poze RGB) pixelilor.

8) "exit_program": La primirea comenzii "EXIT", are loc iesirea din program, nu
inainte de a elibera memoria alocata pentru ultima imagine incarcata (daca 
aceasta exista).

9) "rotate": functia realizeaza rotatia imaginii spre dreapta dupa un anumit 
unghi. Se citeste unghiul de rotatie, a carui validitate se verifica prin 
apelul functiei "is_valid_angle", nu inainte de a verifica daca a fost 
incarcata vreo imagine (cu ajutorul functiei "is_loaded"), respectiv daca 
selectia este valida (prin apelul functiilor "is_selected_all" sau 
"is_square"). Dupa aceea, prin intermediul functiei "make_angle_positive", 
unghiurile negative sunt transformate in echivalentul lor pozitiv, iar apoi se 
trece la rotirea propriu-zisa a imaginii cu cate 90 de grade de cate ori indica
unghiul citit. Daca a fost selectata intreaga poza, se apeleaza functia 
"rotate_entire_image" pentru matricea sau matricele (daca imaginea este RGB) 
pixelilor, urmand sa se interschimbe valoarea latimii cu valoarea inaltimii, ca
urmare a posibilitatii rotatiei unei imagini nepatratice. Se selecteaza in 
continuare intreaga imagine. Daca nu a fost selectata toata imaginea, ci avem 
de-a face cu o selectie patratica, se apeleaza functia "rotate_square" pentru 
matricea sau matricele pixelilor.
In functia "rotate_square", intrucat lucram doar cu matrice patratice, putem 
opera cu o singura dimensiune, precum latimea selectata, deoarece aceasta este
oricum egala cu inaltimea selectata. Se aloca memorie pentru o matrice patratica,
cu aceleasi dimensiuni ca submatricea selectata din matricea pixelilor, urmand
ca aici sa se stocheze submatricea selectata, rotita cu 90 de grade la dreapta.
Aceasta rotatie se efectueaza astfel: pe coloanele matricei auxiliare, luate 
de la stanga la dreapta, se stocheaza liniile submatricei selectate, luate de 
sus in jos. Se actualizeaza, apoi, matricea mare a pixelilor, prin inlocuirea 
submatricei selectate cu submatricea rotita. La final, se elibereaza memoria 
alocata matricei auxiliare.
In functia "rotate_entire_image" se aloca memorie pentru o matrice auxiliara cu
dimensiunile inversate, pentru ca, in urma rotatiei, latimea si inaltimea se 
interschimba. Pe coloanele luate de la stanga la dreapta ale matricei auxiliare
se pun liniile, luate de sus in jos, ale matricei de pixeli initiale. Pentru a 
face aceste modificari si in matricea initiala, trebuie mai intai sa dealocam 
memoria corespunzatoare ei si sa alocam memorie din nou in conformitate cu 
noile dimensiuni, pentru ca latimea si inaltimea s-au interschimbat. Dupa ce
se copiaza elementele matricei auxiliare in matricea initiala, se elibereaza 
memoria matricei auxiliare.


10) "invalid_command": Daca nu a fost recunoscuta niciuna dintre comenzile 
aferente functiilor de mai sus, atunci se afiseaza un mesaj care sa informeze 
utilizatorul cu privire la faptul ca a introdus o comanda invalida. Daca 
aceasta comanda are si parametri, se citesc si acestia, chiar daca nu prezinta 
niciun interes, dar ne-ar putea incurca mai incolo in citirea urmatoarelor 
comenzi.

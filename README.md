# RG175-4-u-nizu

4 u nizu je igra sa rezimima igrac protiv racunara ili 1. igrac protiv 2. igraca, gde
akteri naizmenicno ubacuju po jedan zeton u tablu dimenzija 6x7. 

Potez se odigrava tako sto se zeton ubacuje na dno jedne od 7 kolona.
Ako su u njoj ranije ubacivani zetoni, novi ce se naci iznad najviseg.
Ako je kolona puna, mora se izabrati neka druga sa slobodnim mestima. 

Pobednik je onaj ko prvi uspe da formira horizontalni, vertikalni ili dijagonalni niz
od bar 4 zetona cije su boje specificne za tog igraca. U krajnjem slucaju je ishod
neresen ako se zauzmu sva polja table a da niko ne ostvari uslov za pobedu.

## Opsta uputstva i kontrole

Po pokretanju programa ili resetovanja igre, izlazi prompt za izbor rezima.

#### Odabir rezima igre

##### Taster 1 : Dva igraca
Prvi (crveni) igrac svoj zeton pomera tasterima 'levo' i 'desno' a odigrava potez pritiskom na taster 'dole'.
Drugi (zuti) igrac pomera zeton tasterima 'J' i 'L' a spusta zeton tasterom 'K'.

##### Taster 2 : Igrac - Racunar
Dodatno se bira tezina igre (1 - lako, 2 - srednje tesko, 3 - tesko).

Igrac (crveni) koristi iste kontrole kao u prvom rezimu.

#### Kamera
WASD

#### Sakrivanje/prikazivanje prompta sa kontrolama
V

#### Resetovanje igre
R

#### Izlaz
ESC

## Kompajliranje i pokretanje

Prilozen je Makefile, pa se prevodjenje moze izvrsiti pozicioniranjem u odgovarajuci
direktorijum i naredbom:

> make

Eventualno:

> gcc -o 4_u_nizu main.c drawing.c gamelib.c -lGL -lGLU -lglut -lm

Pokrenuti izvrsni fajl 4_u_nizu navodjenjem njegove putanje. Npr. pozicioniranjem u direktorijum gde je izvrsni fajl, pozvati:

> ./4_u_nizu

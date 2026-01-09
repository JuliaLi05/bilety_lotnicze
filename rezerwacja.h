#ifndef REZERWACJE_H
#define REZERWACJE_H

#define MAXLINE 60
#define BAZA_LOTY "bazadb.bin"
#define BAZA_REZERWACJE "rezerwacjedb.bin"

struct Lot{
    int id_lotu;
    char miasto_odlotu[MAXLINE];
    char miasto_przylotu[MAXLINE];
    int dostepne_miejsca;
    double cena;
};

struct Rezerwacja{
    int id_rezerwacja;
    int id_lotu;
    char imie[MAXLINE];
    char nazwisko[MAXLINE];
};

void stworzBazeDanych();
void wyswietlLoty(); 
int znajdzLot(int id, Lot *lot); 
void rezerwacjaBiletu(int id_lotu, const char*imie, const char* nazwisko); 
void dodajLot(int id_lotu, const char* odlot, const char*przylot, int miejsca, double cena); 
void usunLot(int id_lotu);
void edytujLot(int lot_id, int ilosc_miejsc, double cena); 
void listaPasazerow(int id_lotu); 
#endif

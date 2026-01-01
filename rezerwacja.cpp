#include <fstream>
#include <iostream>
#include <iomanip>
#include <cstring>
#include <cstdio>
#include <algorithm>
#include "rezerwacja.h"

static long miejsceLotuWPliku(int id)
{
    std::ifstream plik(BAZA_LOTY, std::ios::binary);
    if (!plik) return -1;
    Lot lot;
    long rekord = 0;
    while (plik.read(reinterpret_cast<char*>(&lot), sizeof(Lot))) {
        if(lot.id_lotu == id)
        {
            return rekord;
        }
        rekord = static_cast<long>(plik.tellg());
    }
    return -1;
}

static int liczbaRezerwacjiDlaLotu(int id_lotu)
{
    std::ifstream plik(BAZA_REZERWACJE, std::ios::binary);
    if (!plik) return 0;
    Rezerwacja rezerwacja{};
    int cnt = 0;
    while (plik.read(reinterpret_cast<char*>(&rezerwacja), sizeof(Rezerwacja))) {
        if (rezerwacja.id_lotu == id_lotu) cnt++;
    }
    return cnt;
}

static int ostatniaRezerwacjaID()
{
    std::ifstream plik(BAZA_REZERWACJE, std::ios::binary);
    if(!plik)
    {
        return 0;
    }
    Rezerwacja rezerwacja{};
    int maxId = 0;
    while (plik.read(reinterpret_cast<char*>(&rezerwacja), sizeof(Rezerwacja))) 
    {
        if (rezerwacja.id_rezerwacja > maxId) maxId = rezerwacja.id_rezerwacja;
    }
    return maxId;
}

static bool usunRezerwacjeLotu(int id_lotu)
{
    std::ifstream in(BAZA_REZERWACJE, std::ios::binary);
    if (!in) 
    {
        return true;
    }
    std::ofstream out("temp_rez.bin", std::ios::binary | std::ios::trunc);
    if (!out) return false;
    Rezerwacja rezerwacja{};
    while (in.read(reinterpret_cast<char*>(&rezerwacja), sizeof(Rezerwacja))) 
    {
        if (rezerwacja.id_lotu != id_lotu) 
        {
            out.write(reinterpret_cast<const char*>(&rezerwacja), sizeof(Rezerwacja));
        }
    }
    in.close();
    out.close();
    std::remove(BAZA_REZERWACJE);
    if (std::rename("temp_rez.bin", BAZA_REZERWACJE) != 0) 
    {
        return false;
    }
    return true;
}

void stworzBazeDanych()
{
    std::ofstream loty(BAZA_LOTY, std::ios::binary | std::ios::app);
    std::ofstream rezerwacje(BAZA_REZERWACJE, std::ios::binary | std::ios::app);
    if (!loty || !rezerwacje)
    {
        std::cout << "Blad:\nNie udalo sie otworzyc/utworzyc plikow bazy danych.";
        return;
    }
}

void wyswietlLoty()
{
    std::ifstream plik(BAZA_LOTY, std::ios::binary);
    if (!plik) {
        std::cout << "Blad bazy danych.";
        return;
    }
    plik.seekg(0, std::ios::end);
    if (plik.tellg() == 0) {
        std::cout << "Brak dostepnych lotow.";
        return;
    }
    plik.seekg(0, std::ios::beg);
    std::cout << "\n LOTY \n";
    std::cout << "| ID  | Odlot | Przylot | Miejsca | Cena |\n";
    Lot lot;
    while (plik.read(reinterpret_cast<char*>(&lot), sizeof(Lot))) {
        std::cout << "| " << lot.id_lotu
            << " | " << lot.miasto_odlotu
            << " | " << lot.miasto_przylotu
            << " | " << lot.dostepne_miejsca
            << " | " << std::fixed << std::setprecision(2) << lot.cena
            << " PLN |\n";
        std::cout << "----------------------------------------\n";
    }
}

int znajdzLot(int id, Lot *lot){
    if (!lot) return 0;
    long rekord = miejsceLotuWPliku(id);
    if (rekord == -1) return 0;
    std::ifstream plik(BAZA_LOTY, std::ios::binary);
    if (!plik) return 0;
    plik.seekg(rekord, std::ios::beg);
    if (!plik.read(reinterpret_cast<char*>(lot), sizeof(Lot))) 
    {
        return 0;
    }
    return 1;

}

void rezerwacjaBiletu(int id_lotu, const char*imie, const char* nazwisko){
    if (!imie || !nazwisko) 
    {
        std::cout << "Bledne dane pasazera!";
        return;
    }
    Lot lot{};
    long rekord = miejsceLotuWPliku(id_lotu);
    if (id_lotu <= 0 || rekord == -1 || !znajdzLot(id_lotu, &lot)) 
    {
        std::cout << "Nie znaleziono lotu o podanym numerze!";
        return;
    }
    if (lot.dostepne_miejsca <= 0) 
    {
        std::cout << "Brak miejsc w locie numer " << id_lotu;
        return;
    }
    {
        std::fstream plik(BAZA_LOTY, std::ios::binary | std::ios::in | std::ios::out);
        if (!plik) 
        {
            std::cout << "Blad polaczenia z baza danych!";
            return;
        }
        lot.dostepne_miejsca--;
        plik.seekp(rekord, std::ios::beg);
        plik.write(reinterpret_cast<const char*>(&lot), sizeof(Lot));
        if (!plik) {
            std::cout << "Blad zapisu do bazy lotow!";
            return;
        }
    }
    {
        std::ofstream plik(BAZA_REZERWACJE, std::ios::binary | std::ios::app);
        if (!plik) {
            std::fstream floty(BAZA_LOTY, std::ios::binary | std::ios::in | std::ios::out);
            if (floty) {
                Lot poprawka = lot;
                poprawka.dostepne_miejsca++; // cofamy
                floty.seekp(rekord, std::ios::beg);
                floty.write(reinterpret_cast<const char*>(&poprawka), sizeof(Lot));
            }
            std::cout << "Blad polaczenia z baza danych!";
            return;
        }
        Rezerwacja rezerwacja{};
        rezerwacja.id_rezerwacja = ostatniaRezerwacjaID() + 1;
        rezerwacja.id_lotu = id_lotu;
        std::strncpy(rezerwacja.imie, imie, IMIE_MAKS);
        rezerwacja.imie[IMIE_MAKS - 1] = '\0';
        std::strncpy(rezerwacja.nazwisko, nazwisko, NAZWISKO_MAKS);
        rezerwacja.nazwisko[NAZWISKO_MAKS - 1] = '\0';
        plik.write(reinterpret_cast<const char*>(&rezerwacja), sizeof(Rezerwacja));
        if (!plik) {
            std::fstream floty(BAZA_LOTY, std::ios::binary | std::ios::in | std::ios::out);
            if (floty) {
                Lot poprawka = lot;
                poprawka.dostepne_miejsca++; // cofamy
                floty.seekp(rekord, std::ios::beg);
                floty.write(reinterpret_cast<const char*>(&poprawka), sizeof(Lot));
            }
            std::cout << "Blad zapisu do bazy rezerwacji!";
            return;
        }
    }
    std::cout << "Dokonano rezerwacji.";
}

void dodajLot(int id_lotu, const char* odlot, const char*przylot, int miejsca, double cena){
    if (!odlot || !przylot) 
    {
        std::cout << "Bledne dane miast!";
        return;
    }
    if(miejsca <= 0 || cena <= 0.0 || id_lotu <= 0)
    {
        std::cout << "Jedna z ponizszych danych nie jest dodatnia: ID lotu, cena, ilosc dostepnych miejsc.";
        return;
    }
    if(miejsceLotuWPliku(id_lotu) != -1)
    {
        std::cout << "Istnieje juz lot o takim numerze!";
        return;
    }
    std::ofstream plik(BAZA_LOTY, std::ios::binary | std::ios::app);
    if (!plik) {
        std::cout << "Blad polaczenia z baza danych!";
        return;
    }
    Lot nowy{};
    nowy.id_lotu = id_lotu;
    nowy.cena = cena;
    nowy.dostepne_miejsca = miejsca;
    std::strncpy(nowy.miasto_odlotu, odlot, MIASTO_MAKS - 1);
    nowy.miasto_odlotu[MIASTO_MAKS - 1] = '\0';
    std::strncpy(nowy.miasto_przylotu, przylot, MIASTO_MAKS - 1);
    nowy.miasto_przylotu[MIASTO_MAKS - 1] = '\0';
    plik.write(reinterpret_cast<const char*>(&nowy), sizeof(Lot));
    if (!plik) 
    {
        std::cout << "Blad zapisu lotu!";
        return;
    }
    std::cout << "Nowy lot o numerze " << id_lotu << " zostal dodany.";
}

void usunLot(int id_lotu){
    if (id_lotu <= 0) 
    {
        std::cout << "Bledny numer lotu!";
        return;
    }
    long wiersz = miejsceLotuWPliku(id_lotu);
    if(wiersz < 0)
    {
        std::cout << "Podany lot nie istnieje!";
        return;
    }
    std::ifstream in(BAZA_LOTY, std::ios::binary);
    std::ofstream out("temp_data.bin", std::ios::binary | std::ios::trunc);
    if (!in || !out) 
    {
        std::cout << "Nie udalo sie otworzyc plikow!";
        return;
    }
    Lot lot;
    while (in.read(reinterpret_cast<char*>(&lot), sizeof(Lot))) 
    {
        if (lot.id_lotu != id_lotu) 
        {
            out.write(reinterpret_cast<const char*>(&lot), sizeof(Lot));
        }
    }
    in.close();
    out.close();
    std::remove(BAZA_LOTY);
    if (std::rename("temp_data.bin", BAZA_LOTY) != 0) {
        std::cout << "Blad podczas podmiany pliku bazy lotow!";
        return;
    }
    if (!usunRezerwacjeLotu(id_lotu)) {
        std::cout << "Lot usuniety, ale wystapil blad przy usuwaniu rezerwacji tego lotu!";
        return;
    }
    std::cout << "Lot numer " << id_lotu << " zostal usuniety!";
}

void listaPasazerow(int id_lotu){
    if(id_lotu <= 0)
    {
        std::cout << "Bledny numer lotu!";
        return;
    }
    Lot lot;
    if(!znajdzLot(id_lotu, &lot))
    {
        std::cout << "Lot o podanym numerze nie istnieje!";
        return;
    }
    std::ifstream plik(BAZA_REZERWACJE, std::ios::binary);
    if (!plik) 
    {
        std::cout << "Blad polaczenia z baza danych!";
        return;
    }
    std::cout << "Pasazerowie lotu numer" << id_lotu;
    std::cout << "\nID Rezerwacji| Imie| Nazwisko\n";
    std::cout << "----------------------------\n";
    Rezerwacja rezerwacja;
    bool czyZnaleziono = false;
    while(plik.read(reinterpret_cast<char*>(&rezerwacja), sizeof(Rezerwacja)))
    {
        if(rezerwacja.id_lotu == id_lotu)
        {
            std::cout << "| " << rezerwacja.id_rezerwacja << " | " << rezerwacja.imie << " | " << rezerwacja.nazwisko << " |\n";
            czyZnaleziono = true;
        }
    }
    if(!czyZnaleziono){
        std::cout << "Brak rezerwacji!";
    }
}

void edytujLot(int lot_id, int ilosc_miejsc, double cena){
    long pozycja_lotu = miejsceLotuWPliku(lot_id);
    if(lot_id <= 0 || pozycja_lotu ==-1)
    {
        std::cout << "Bledny numer lotu!";
        return;
    }
    if(ilosc_miejsc <=0 || cena <= 0)
    {
        std::cout << "Bledna ilosc miejsc, badz cena. Obydwie wartosci musza byc dodatnie.";
        return;
    }
    int zarezerwowane = liczbaRezerwacjiDlaLotu(lot_id);
    if (ilosc_miejsc < zarezerwowane) {
        std::cout << "Nie mozna ustawic liczby miejsc na " << ilosc_miejsc
            << ", bo juz zarezerwowano " << zarezerwowane << " miejsc.";
        return;
    }
    std::fstream plik(BAZA_LOTY, std::ios::binary | std::ios::in | std::ios::out);
    if (!plik) 
    {
        std::cout << "Blad polaczenia z baza danych!";
        return;
    }
    Lot lot;
    plik.seekg(pozycja_lotu, std::ios::beg);
    if (!plik.read(reinterpret_cast<char*>(&lot), sizeof(Lot))) 
    {
        std::cout << "Blad odczytu lotu!";
        return;
    }
    lot.dostepne_miejsca = ilosc_miejsc - zarezerwowane;
    lot.cena = cena;
    plik.seekp(pozycja_lotu, std::ios::beg);
    plik.write(reinterpret_cast<const char*>(&lot), sizeof(Lot));
    if (!plik) 
    {
        std::cout << "Blad zapisu lotu!";
        return;
    }
    std::cout << "Lot zostal zaaktualizowny!";
}


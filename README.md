# bilety_lotnicze

---

System Rezerwacji Lotów
Prosta i lekka aplikacja konsolowa napisana w języku C++, służąca do obsługi systemu rezerwacji biletów lotniczych. Projekt nie wymaga zewnętrznych serwerów bazodanowych – wszystkie dane przechowywane są lokalnie w plikach binarnych.
Opis Projektu
Aplikacja umożliwia zarządzanie lotami oraz dokonywanie rezerwacji w dwóch trybach: klienta i administratora. Kod został napisany zgodnie ze standardem C++11, z naciskiem na przenośność i brak zewnętrznych zależności.
Główne cechy
Baza danych oparta na plikach:
- dane o lotach i rezerwacjach są trwałe i zapisywane w plikach `bazadb.bin` oraz `rezerwacjedb.bin`
Podział ról:
- oddzielne menu dla Klienta i Administratora
Zarządzanie lotami:
- dodawanie, usuwanie i edycja parametrów lotów (cena, liczba miejsc)
Obsługa rezerwacji:
Przeglądanie dostępnych połączeń i rezerwacja biletów z automatyczną aktualizacją dostępnych miejsc.



Funkcjonalności

Tryb Klienta

Wyświetlanie lotów: 
Tabela z dostępnymi połączeniami, cenami i liczbą miejsc.
 
Rezerwacja biletu:
Możliwość zarezerwowania miejsca po podaniu danych pasażera (Imię, Nazwisko).

Tryb Administratora

Wymaga autoryzacji hasłem. Umożliwia:

1. Dodawanie lotów: Wprowadzenie nowego połączenia z unikalnym ID.
2. Trwałe usunięcie lotu z bazy.
3. Edycja lotu: Zmiana liczby dostępnych miejsc oraz ceny biletu.
4. Lista pasażerów: Podgląd osób zarezerwowanych na konkretny lot.

Wymagania i Kompilacja

Projekt zawiera plik `Makefile` automatyzujący proces budowania.

Wymagania:

* Kompilator `g++`
* System zgodny ze standardem POSIX (Linux/Unix/macOS) lub Windows.

Instrukcja kompilacji:
Aby skompilować projekt, w katalogu głównym uruchom komendę:

```bash
make

```

Spowoduje to utworzenie pliku wykonywalnego `system_rezerwacji`.

Aby wyczyścić pliki obiektowe i plik wykonywalny:

```bash
make clean

```

Uruchomienie i Obsługa

Uruchomienie w terminalu:

```bash
./system_rezerwacji

```



Logowanie Administratora
Aby uzyskać dostęp do funkcji administracyjnych (edycja, usuwanie, podgląd pasażerów), należy w menu głównym wybrać opcję przełączenia na tryb ADMIN i podać hasło.
Domyślne hasło administratora:`admin123`.
 
Struktura Plików 
`main.cpp` – główna pętla programu, obsługa menu i interfejsu użytkownika
`rezerwacja.cpp` – implementacja logiki biznesowej i operacji na plikach
`rezerwacja.h` – plik nagłówkowy z definicjami struktur (`Lot`, `Rezerwacja`) i stałych.
`Makefile` – skrypt kompilacji
`bazadb.bin` – Plik binarny z bazą lotów (tworzony automatycznie).
`rezerwacjedb.bin` – Plik binarny z bazą rezerwacji (tworzony automatycznie).

Autor:Julia Litawa

#include <iostream>
#include <limits>
#include <string>
#include <cstdlib>
#include "rezerwacja.h"
#define HASLO "admin123"

void czyscKonsole(){
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void czekajNaEnter(){
    std::cout << "\nNacisnij ENTER aby kontynuowac...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
    std::cin.get(); 
}

void menuKlient(){
    std::cout << "\n--- MENU KLIENTA ---\n";
    std::cout << "1. Wyswietl dostepne loty\n";
    std::cout << "2. Zarezerwuj bilet\n";
    std::cout << "3. Przelacz na tryb ADMIN\n";
    std::cout << "4. Wyjdz\n";
    std::cout << "Wybierz opcje: ";
}

void menuAdmina() {
    std::cout << "\n--- MENU ADMINISTRATORA ---\n";
    std::cout << "1. Wyswietl wszystkie loty\n";
    std::cout << "2. Dodaj nowy lot\n";
    std::cout << "3. Usun lot\n";
    std::cout << "4. Modyfikuj lot (miejsca/cena)\n";
    std::cout << "5. Wyswietl pasazerow lotu\n";
    std::cout << "6. Przelacz na tryb KLIENT\n";
    std::cout << "7. Wyjdz z programu\n";
    std::cout << "Wybierz opcje: ";
}

static bool wczytajInt(int& x) {
    if (std::cin >> x)
    {
        return true;
    }
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Blad wczytywania. Podaj liczbe.\n";
    return false;
}

static bool wczytajDouble(double& x) {
    if (std::cin >> x)
    {
        return true;
    }
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Blad wczytywania. Podaj liczbe (np. 199.99).\n";
    return false;
}

int main() {
    czyscKonsole();
    int tryb = 0;
    int wybor = 0;
    stworzBazeDanych();
    do{
        if(tryb == 0)
        {
            menuKlient();
        }
        else
        {
            menuAdmina();
        }
        if (!wczytajInt(wybor)) 
        {
            continue;
        }
        int id = 0, miejsca = 0;
        double cena = 0.0;
        char imie[IMIE_MAKS]{};
        char nazwisko[NAZWISKO_MAKS]{};
        char odlot[MIASTO_MAKS]{};
        char przylot[MIASTO_MAKS]{};  
        char haslo[20]{};
        if(tryb == 0)
        {
            switch (wybor) {
                case 1: wyswietlLoty(); break;
                case 2:
                    std::cout << "Podaj ID lotu: ";
                        if (!wczytajInt(id)) break;
                    std::cout << "Podaj imie: ";
                        if (!(std::cin >> imie)) break;
                    std::cout << "Podaj nazwisko: ";
                        if (!(std::cin >> nazwisko)) break;
                    rezerwacjaBiletu(id, imie, nazwisko);
                    break;
                case 3:
                    std::cout << "Podaj haslo do konta administratora: ";
                    if (!(std::cin >> haslo))
                    {
                        break;
                    }
                    if (std::string(haslo) == HASLO) 
                    {
                        tryb = 1;
                        std::cout << "Uzyskano dostep do konta administratora.";
                    }
                    else 
                    {
                        std::cout << "Odmowa dostepu, podano bledne haslo.";
                    }
                    break;
                case 4:
                    std::cout << "Dziekujemy za korzystanie z naszych uslug!";
                    wybor = 7;
                    break;
                default:
                    std::cout << "Wybrano niedostepna opcje. Prosze ponownie dokonac wyboru.";
                    break;
                }
            }
            else{
                switch (wybor) {
                    case 1: wyswietlLoty(); break;
                    case 2:
                        std::cout << "Podaj nowy numer lotu: ";
                        if (!wczytajInt(id)) break;
                        std::cout << "Podaj miasto odlotu: ";
                        if (!(std::cin >> odlot)) break;
                        std::cout << "Podaj miasto przylotu: ";
                        if (!(std::cin >> przylot)) break;
                        std::cout << "Podaj liczbe miejsc: ";
                        if (!wczytajInt(miejsca)) break;
                        std::cout << "Podaj cene: ";
                        if (!wczytajDouble(cena)) break;
                        dodajLot(id, odlot, przylot, miejsca, cena);
                        break;
                    case 3:
                        std::cout << "Podaj numer lotu do usuniecia: ";
                        if (!wczytajInt(id)) break;
                        usunLot(id);
                        break;
                    case 4:
                        std::cout << "Podaj numer lotu do modyfikacji: ";
                        if (!wczytajInt(id)) break;
                        std::cout << "Podaj nowa liczbe miejsc: ";
                        if (!wczytajInt(miejsca)) break;
                        std::cout << "Podaj nowa cene: ";
                        if (!wczytajDouble(cena)) break;
                        edytujLot(id, miejsca, cena);
                        break;
                    case 5:
                        std::cout << "Podaj numer lotu, dla ktorego chcesz uzyskac dane pasazerow: ";
                        if (!wczytajInt(id)) break;
                        listaPasazerow(id);
                        break;
                    case 6:
                        tryb = 0;
                        std::cout << "Powrot do trybu klienta...";
                        break;
                    case 7:
                        std::cout << "Zakonczono dzialanie programu...";
                        break;
                    default:
                        std::cout << "Wybrano niedostepna opcje. Prosze ponownie dokonac wyboru.";
                        break;
            }
        }
        if(wybor != 4 && wybor != 7){
            czekajNaEnter();
            czyscKonsole();
        }
    }while(wybor!=7);
    
    return 0;
}

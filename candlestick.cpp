#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <ctime>
using namespace std;

struct dataf
{
    int rok;
    int miesiac;
    int dzien;
    double open;
    double close;
    double high;
    double low;
};

struct struktura
{
    double highest;
    double lowest;
    double span;
    int szer;
    int wys;
    int pw;
};

int ilosc_linii(const string& nazwapliku);
int czytaniepliku(dataf* dane, int n, string nazwapliku);
int rysowanie_1(dataf* dane, int n, string plikout, ofstream& LogFile);
int rysowanie_3(dataf* dane, int n, struktura wartosci, string plikout);
int dzialanie(ofstream& LogFile);
void wypelnij(struktura wartosci, ofstream& zapis);
int szukaj(dataf* dane, int n, int r, int m, int d);
int wyswietlanie_wykresu(string plikout);
void logAction(ofstream& LogFile, const string& wydarzenie, const string& zmienna);
struktura poprosodate(dataf* dane, int n, ofstream& LogFile, struktura wartosci);


int main() {
    char u;
    int t=0;

    ofstream LogFile("log_file.log",ios::app);

    if (!LogFile.is_open()) {
        cout << "Nie udalo sie otworzyc pliku .log" << endl;
        return 4;
    }
    logAction(LogFile, "Uruchomiono program", "");
    ostringstream pp;
    cout << "Szymon Tomaszewicz, 197942" << endl << endl;
    cout << "Wybierz 'q', aby zakonczyc dzialanie programu" << endl << "Wybierz 'g', aby kontynuowac" << endl;
    cin >> u;
    pp << u;
    logAction(LogFile, "Uzytkownik wybral w menu opcje: ", pp.str());
    if (toupper(u) == 'Q') {
        
    }
    else if (toupper(u) == 'G') {
        t = dzialanie(LogFile);
    }
    else
    {
        cout << "Nieprawidlowy wybor";
        return 0;
    }
    logAction(LogFile, "Program zakonczyl dzialanie", "\n");
    return t;
}

int dzialanie(ofstream& LogFile)
{
    string nazwapliku, plikout;
    cout << endl << "Podaj nazwe pliku wejsciowego: "; cin >> nazwapliku;
    logAction(LogFile, "Uzytkownik wpisal nazwe pliku wyjsciowego: ", nazwapliku);
    cout << endl << "Podaj nazwe pliku wyjsciowego: "; cin >> plikout;
    logAction(LogFile, "Uzytkownik wpisal nazwe pliku wejsciowego : ", nazwapliku);
    int n = ilosc_linii(nazwapliku);
    if (n == -1) return 1;
    dataf* dane;
    dane = new dataf[n];
    int m = czytaniepliku(dane, n, nazwapliku);
    if (m == -1) return 2;
    int m1 = rysowanie_1(dane, n, plikout,LogFile);
    if (m1 < 0) return m1;
    return wyswietlanie_wykresu(plikout);
}

int czytaniepliku(dataf* dane, int n, string nazwapliku)
{
    ifstream plik(nazwapliku);
    if (!plik.is_open()) {
        cout << "Nie udalo sie otworzyc pliku" << endl;
        return -1;
    }
    string wiersz;
    int i = 0;
    getline(plik, wiersz);
    while (getline(plik, wiersz)) {
        stringstream ss(wiersz);
        char przerwa;
        ss >> dane[i].rok >> przerwa >> dane[i].miesiac >> przerwa >> dane[i].dzien >> przerwa >> dane[i].open >> przerwa >> dane[i].high >> przerwa >> dane[i].low >> przerwa >> dane[i].close;
        i++;
    }
    plik.close();
    return 0;
}

int ilosc_linii(const string& nazwapliku)
{
    ifstream plik(nazwapliku);
    if (!plik.is_open()) {
        cout << "Nie udalo sie otworzyc pliku" << endl;
        return -1;
    }
    int ilosc = 0;
    string dummy;
    while (getline(plik, dummy)) ilosc++;
    plik.close();
    return ilosc - 1;
}
int rysowanie_1(dataf* dane, int n, string plikout, ofstream& LogFile)
{
    struktura wartosci;
    ostringstream pp;
    cout << endl << endl << "Podaj wysokosc wykresu: "; cin >> wartosci.wys;
    pp << wartosci.wys;
    logAction(LogFile, "Uzytkownik wpisal wysokosc wykresu rowna: ", pp.str());
    wartosci=poprosodate(dane, n, LogFile, wartosci);
    wartosci.highest = dane[wartosci.pw].high;
    wartosci.lowest = dane[wartosci.pw].low;
    for (int i = wartosci.pw + 1; i < wartosci.pw + wartosci.szer; i++) {
        if (dane[i].high > wartosci.highest) wartosci.highest = dane[i].high;
        if (dane[i].low < wartosci.lowest) wartosci.lowest = dane[i].low;
    }
    wartosci.span = wartosci.highest - wartosci.lowest;
    int ret = rysowanie_3(dane, n, wartosci, plikout);
    return ret;
}
struktura poprosodate(dataf* dane, int n, ofstream& LogFile,struktura wartosci)
{
    int r, m, d;
    bool znaleziono = false;
    ostringstream pp;
    while (!znaleziono) {
        cout << endl << "Podaj pierwszy dzien(YYYY MM DD): "; cin >> r >> m >> d;
        pp << r << " " << m << " " << d;
        logAction(LogFile, "Uzytkownik wpisal date poczatkowa: ", pp.str());
        pp.str("");
        wartosci.pw = szukaj(dane, n, r, m, d);
        if (wartosci.pw >= 0) znaleziono = true;
    }znaleziono = false;
    while (!znaleziono) {
        cout << endl << "Podaj ostatni dzien(YYYY MM DD): "; cin >> r >> m >> d;
        pp << r << " " << m << " " << d;
        logAction(LogFile, "Uzytkownik wpisal date koncowa: ", pp.str());
        pp.str("");
        int q = szukaj(dane, n, r, m, d);
        if (q >= 0) {
            znaleziono = true;
            wartosci.szer = q - wartosci.pw;
        }
    }

    return wartosci;
}
int szukaj(dataf* dane, int n, int r, int m, int d)
{
    bool znaleziono = false;
    int miejsce;
    for (int i = 0; i < n; i++)
        if (dane[i].rok == r && dane[i].miesiac == m && dane[i].dzien == d) {
            miejsce = i;
            znaleziono = true;
            break;
        }
    if (!znaleziono) {
        cout << "Nie udalo sie znalezc podanej daty w pliku, sprobuj jeszcze raz";
        miejsce = -1;
    }
    return miejsce;
}

int rysowanie_3(dataf* dane, int n, struktura wartosci, string plikout)
{
    ofstream zapis(plikout);
    if (!zapis.is_open()) {
        cout << "Nie udalo sie otworzyc pliku" << endl;
        return -2;
    }
    wypelnij(wartosci, zapis);
    double roz = wartosci.span / wartosci.wys;
    for (int i = 0; i < wartosci.szer; i++) {
        zapis.seekp(i + 3 + wartosci.szer, ios::beg);
        for (double j = wartosci.highest; j > wartosci.lowest; j = j - roz) {
            if (j <= dane[wartosci.pw + i].high && j >= dane[wartosci.pw + i].low) {
                zapis << '|';
                zapis.seekp(-1, ios::cur);
            }
            if (dane[wartosci.pw + i].close > dane[wartosci.pw + i].open && j <= dane[wartosci.pw + i].close && j >= dane[wartosci.pw + i].open || dane[wartosci.pw + i].close > dane[wartosci.pw + i].open && dane[wartosci.pw+i].close-roz<j && dane[wartosci.pw+i].close+roz>j) {
                zapis << 'O';
                zapis.seekp(-1, ios::cur);
            }
            else if (dane[wartosci.pw + i].close < dane[wartosci.pw + i].open && j >= dane[wartosci.pw + i].close && j <= dane[wartosci.pw + i].open || dane[wartosci.pw + i].close < dane[wartosci.pw + i].open && dane[wartosci.pw + i].close - roz<j && dane[wartosci.pw + i].close + roz>j) {
                zapis << '#';
                zapis.seekp(-1, ios::cur);
            }zapis.seekp(+wartosci.szer + 2, ios::cur);
        }
    }
    delete[] dane;
    zapis.close();
    return 0;
}
void wypelnij(struktura wartosci, ofstream& zapis)
{
    for (int p1 = 0; p1 < wartosci.wys + 1; p1++) {
        for (int p2 = 0; p2 < wartosci.szer; p2++) {
            if (p1 == 0 && p2 == 0) {
                zapis << "^";
            }
            else if (p2 == 0) {
                zapis << "|";
            }
            else zapis << " ";
        }
        zapis << "\n";
    }

    for (int i = 0; i < wartosci.szer + 1; i++) zapis << "-";
    zapis << ">";
}
int wyswietlanie_wykresu(string plikout)
{
    ifstream plik(plikout);
    if (!plik.is_open())
    {
        cout << "Nie udalo sie otworzyc pliku" << endl;
        return -1;
    }
    string wiersz;
    while (getline(plik, wiersz)) {
        for (int i = 0; i < 50 && wiersz[i] != '\0'; ++i) {
            cout << wiersz[i];
        }cout << endl;

    }
    plik.close();
    return 0;
}

void logAction(ofstream& LogFile, const string& wydarzenie, const string& zmienna)
{
    time_t currentTime = std::time(nullptr);
    struct tm timeInfo;
    if (localtime_s(&timeInfo, &currentTime) == 0) {
        char buffer[80];
        std::strftime(buffer, sizeof(buffer), "%F %T", &timeInfo);
        LogFile << "[" << buffer << "] " << wydarzenie << zmienna << "\n";
    }
    else {
        LogFile << "Bład, nie udalo sie wyczytac czasu z urzadzenia" << "\n";
    }
}
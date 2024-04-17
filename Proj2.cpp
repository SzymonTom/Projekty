/*
* 
* 
Wyobraź sobie taśmociąg, którym podawane są kolejno przedmioty p1, p2 ,..., pk.
Każdy przedmiot pi
 ma określoną wagę wi
. Przedmioty, w kolejności dostarczania przez taśmociąg,
należy pakować do samochodów o ograniczonej ładowności ni , która może być różna dla każdego
samochodu. Przyjmij, że ilość przedmiotów, które można załadować do samochodu jest
nieograniczona; kontroluj jedynie czy suma wag nie przekracza ładowności. W sytuacji, gdy dołożenie
kolejnego przedmiotu spowodowałoby przekroczenie ładowności, należy podstawić kolejny pusty
samochód. Proces pakowania prowadzony jest dopóki taśmociąg dostarcza kolejne przedmioty.
Napisz program symulujący powyższy proces. Taśmociąg zaimplementuj, jako kolejkę STL (queue)
o wartościach elementów generowanych losowo. Samochody powinny być zaimplementowane, jako
stosy tworzone dynamicznie (w miarę potrzeb). Zapamiętuj samochody w tablicy realizowanej, jako
wektor STL (vector). Własną klasę stosu zdefiniuj korzystając ze stosu STL (stack), rozszerzając ją
o składowe przechowujące pojemność i aktualną zajętość stosu oraz odpowiednie metody
pozwalające na operowanie na nich.


*/



#include <iostream>
#include <stack>
#include <vector>
#include <cmath>
#include <ctime>
#include <windows.h>
#include <queue>
#include <string>
using namespace std;

float LosujMase(float min, float max);

class Pojazd {
public:
    Pojazd() {
        MasaPaczek = 0;
        Pojemnosc = trunc(LosujMase(10, 26));
    }
    bool CzyDodac(float Masa) {
        if (MasaPaczek + Masa > Pojemnosc) return false;
        return true;
    }
    void DodajPaczke(float Masa) {
        Paczki.push(Masa);
        MasaPaczek += Masa;
    }
    void print()
    {
        int counter = Paczki.size();
        cout << "Laczna masa paczek: " << MasaPaczek << ", Pojemnosc auta: " << Pojemnosc << endl;
        cout << "/----| ";
        for (int i = 0; i < counter; i++) {
            cout <<Paczki.top() << " ";
            Paczki.pop();
        }
        cout << endl << "|----|";
        for (int j = 0; j < 5 * counter + 1; j++) cout << "-";
        cout << endl << " o o   ";
        for (int gh = 0; gh < counter * 5 + 1; gh++) {
            if (gh % 3 == 0) cout << "o";
            else cout << " ";
        }
    }
private:
    float Pojemnosc;
    float MasaPaczek;
    stack<float> Paczki;
};

queue<float> TworzenieKolejkiTasmociagu(int amount)
{
    srand(time(NULL));
    queue<float> tasma;
    cout <<endl<< "Kolejka Tasmociagu: " << endl;
    for (int i = 0; i < amount; i++)
    {
        tasma.push(LosujMase(0.5, 2));
        cout << tasma.back() <<" ";
    }cout << endl << endl << endl;
    return tasma;
}

vector<Pojazd> TworzeniePojazdu(vector<Pojazd> truck_comp, queue<float> tasma)
{
    Pojazd* scania = new Pojazd();
    while (!tasma.empty())
    {
        if (scania->CzyDodac(tasma.front()))
        {
            scania->DodajPaczke(tasma.front());
            tasma.pop();
        }
        else
        {
            truck_comp.push_back(*scania);
            scania = new Pojazd();
        }
    }
    truck_comp.push_back(*scania);
    return truck_comp;
}

int main()
{
    queue<float> tasma;
    vector<Pojazd> truck_pol_comp;
    int IloscPaczek;
    cout << "Podaj ilosc Paczek ktore pojawia sie na tasmociagu: "; cin >> IloscPaczek;
    if (IloscPaczek < 1) {
        cout << "[BLAD] Ilosc Paczek powinna byc rowna conajmniej 1!"<<endl<<endl<<endl;
        return 0;
    }
    tasma = TworzenieKolejkiTasmociagu(IloscPaczek);
    truck_pol_comp = TworzeniePojazdu(truck_pol_comp, tasma);
    for (int i = 0; i < truck_pol_comp.size(); i++)
    {
        cout << "\n" << i + 1 << ": ";
        truck_pol_comp[i].print();
    }
}

float LosujMase(float min, float max)
{
    float Masa = (float)rand() / (float)RAND_MAX;
    return round((Masa * (max - min) + min) * 100.0) / 100.0;
}
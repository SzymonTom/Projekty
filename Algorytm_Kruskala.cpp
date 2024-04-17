#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

struct wezly
{
	int NumerPoczatku;
	int NumerKonca;
	int DlugoscPolaczenia;
};

int IloscLinii();
int CzytaniePlikuWejsciowego(wezly* Tab);
void SortowanieTablicy(wezly* Tab, int IloscLinii);
void Algorytm(wezly* Tab, int RozmiarTablicy);
int LiczbaWierzcholkow(wezly* Tab, int RozmiarTablicy);
void ZmienDrzewo(int* KtoreDrzewo, int IleWierzcholkow, int w1, int w2);
void PolaczDrzewa(wezly* NajkrotszaDroga, wezly* Tab, int* KtoreDrzewo, int RozmiarTablicy, int IleWierzcholkow, int j, int w1, int w2);
bool CzyWTablicy(wezly* NajkrotszaDroga, int j, int wezel);
void WyswietlWynik(wezly* NajkrotszaDroga, int j);

int main()
{
	int RozmiarTablicy = IloscLinii();
	if (RozmiarTablicy < 0) return -1;
	wezly* Tab;
	Tab = new wezly[RozmiarTablicy];
	int CzyBlad=CzytaniePlikuWejsciowego(Tab);
	if (CzyBlad < 0) return -2;
	SortowanieTablicy(Tab, RozmiarTablicy);
	Algorytm(Tab, RozmiarTablicy);
	return 0;
}

int IloscLinii()
{
	ifstream graf("Graf.txt");
	if (!graf.is_open()) return -1;
	string dummy;
	int ilosc = 0;
	while (getline(graf, dummy)) ilosc++;
	graf.close();
	return ilosc;
}

int CzytaniePlikuWejsciowego(wezly* Tab)
{
    ifstream graf("Graf.txt");
	if (!graf.is_open()) return -1;
    string wiersz;
    int i = 0;
    while (getline(graf, wiersz)) {
        stringstream ss(wiersz);
        char przerwa;
		ss >> Tab[i].NumerPoczatku >> przerwa >> Tab[i].NumerKonca >> przerwa >> Tab[i].DlugoscPolaczenia;
        i++;
    }
	graf.close();
    return 0;
}

void SortowanieTablicy(wezly* Tab,int RozmiarTablicy) //insert sort
{
	int j;
	wezly pom;
	for (int i = 1; i < RozmiarTablicy; i++) {
		pom = Tab[i];
		j = i-1;
		while (j >= 0 && Tab[j].DlugoscPolaczenia > pom.DlugoscPolaczenia) {
			Tab[j + 1] = Tab[j];
			Tab[j] = pom;
			j--;
		}
	}
}

void Algorytm(wezly* Tab,int RozmiarTablicy)
{
	wezly* NajkrotszaDroga;
	NajkrotszaDroga = new wezly[RozmiarTablicy];
	int IleWierzcholkow=LiczbaWierzcholkow(Tab, RozmiarTablicy);
	int* KtoreDrzewo;
	KtoreDrzewo = new int[IleWierzcholkow];
	for (int k = 0; k < IleWierzcholkow; k++) KtoreDrzewo[k] = k;
	int j = -1;
	for (int i = 0; i < RozmiarTablicy; i++)
		if (!CzyWTablicy(NajkrotszaDroga, j, Tab[i].NumerPoczatku) || !CzyWTablicy(NajkrotszaDroga, j, Tab[i].NumerKonca)) {
			j++;
			NajkrotszaDroga[j] = Tab[i];
			ZmienDrzewo(KtoreDrzewo, IleWierzcholkow, Tab[i].NumerKonca, Tab[i].NumerPoczatku);
		}
	for(int o=0;o<IleWierzcholkow-1;o++) 
		if (KtoreDrzewo[o] != KtoreDrzewo[o + 1]) {
			j++;
			PolaczDrzewa(NajkrotszaDroga, Tab, KtoreDrzewo, RozmiarTablicy, IleWierzcholkow, j, o+2, o + 1);
		}
	WyswietlWynik(NajkrotszaDroga, j);
	delete[] Tab;
	delete[] NajkrotszaDroga;
	delete[] KtoreDrzewo;
}

int LiczbaWierzcholkow(wezly* Tab, int RozmiarTablicy)
{
	int* Tablica;
	Tablica = new int[RozmiarTablicy*2];
	int m=0;
	for (int i = 0; i < RozmiarTablicy * 2; i+=2) {
		Tablica[i] = Tab[m].NumerKonca;
		Tablica[i + 1] = Tab[m].NumerPoczatku;
		m++;
	}
	int ilosczmian = 1;
	while (ilosczmian != 0)
	{
		ilosczmian = 0;
		for (int j = 0; j < RozmiarTablicy * 2 - 1; j++) {
			if (Tablica[j] > Tablica[j + 1]) {
				int pom = Tablica[j];
				Tablica[j] = Tablica[j + 1];
				Tablica[j + 1] = pom;
				ilosczmian++;
			}
		}
	}
	int ile = 1;
	for (int k = 0; k < RozmiarTablicy * 2-1; k++) if (Tablica[k] != Tablica[k + 1]) ile++;
	return ile;
}

void ZmienDrzewo(int* KtoreDrzewo,int IleWierzcholkow, int w1, int w2)
{
	int Pom = KtoreDrzewo[w1 - 1];
	for (int i = 0; i < IleWierzcholkow; i++)
		if (KtoreDrzewo[i] == Pom) KtoreDrzewo[i] = KtoreDrzewo[w2-1];
}

void PolaczDrzewa(wezly* NajkrotszaDroga,wezly* Tab, int* KtoreDrzewo,int RozmiarTablicy,int IleWierzcholkow,int j,int w1,int w2)
{	
	for (int i = 0; i < RozmiarTablicy; i++)
		if (Tab[i].NumerKonca == w1 && Tab[i].NumerPoczatku == w2 || Tab[i].NumerKonca == w2 && Tab[i].NumerPoczatku == w1) {
			NajkrotszaDroga[j-1] = Tab[i];
			ZmienDrzewo(KtoreDrzewo, IleWierzcholkow, w1, w2);
			break;
		}
}

bool CzyWTablicy(wezly* NajkrotszaDroga,int j,int wezel)
{
	for (int i = 0; i < j; i++)
		if (NajkrotszaDroga[i].NumerKonca == wezel || NajkrotszaDroga[i].NumerPoczatku == wezel) return true;
	return false;
}

void WyswietlWynik(wezly* NajkrotszaDroga, int j)
{
	for (int i = 0; i < j; i++) cout << NajkrotszaDroga[i].NumerPoczatku << " - " << NajkrotszaDroga[i].NumerKonca << " : " << NajkrotszaDroga[i].DlugoscPolaczenia << endl;
}
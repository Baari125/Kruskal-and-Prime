#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <sstream>
using namespace std;

typedef pair<int, int> Pary;

struct Krawedz
{
    int poczatek, koniec, waga;
    Krawedz(int p, int k, int w)
    {
        poczatek = p;
        koniec = k;
        waga = w;
    }
};

struct Porownaj{
    int operator()(const Krawedz &k1, const Krawedz &k2){
        if(k1.waga>=k2.waga)   return true;
        if(k1.waga<k2.waga)   return false;
    }
};

struct XClass
{
public:
    vector<Krawedz> krawedzie;
    int iloscOperacji;
    int koszt;
    XClass(vector<Krawedz> k, int i, int ko)
    {
        krawedzie = k;
        iloscOperacji = i;
        koszt = ko;
    }
};

class Graf
{
public:
    vector<pair<int, int> > *krawedz;
    priority_queue<Krawedz, vector<Krawedz>, Porownaj > qKruskal;

    Graf(int X)
    {
        krawedz=new vector<Pary>[X+1];
    }

    void dodajKrawedz(int a, int b, int waga)
    {
        krawedz[a].push_back(make_pair(waga, b));
        krawedz[b].push_back(make_pair(waga, a));
        qKruskal.push(Krawedz(a, b, waga));
    }

    XClass Prime(int X){
        vector<Krawedz> krawedzie;
        int licznik = 0;
        int iloscOperacji = 0;
        int koszt = 0;
        vector<bool> odwiedzone(X+1, false);
        odwiedzone[1]=true;
        priority_queue<Krawedz, vector<Krawedz>, Porownaj > q;
        vector<pair<int, int> >::iterator i;
        for(i=krawedz[1].begin(); i<krawedz[1].end(); i++)
        {
            q.push(Krawedz(1, (*i).second, (*i).first));
        }
        int w, s, k;
        while(licznik!=X-1){
            w=q.top().waga;
            k=q.top().koniec;
            s=q.top().poczatek;
            if(odwiedzone[k]==false){
                koszt+=w;
                odwiedzone[k]=true;
                licznik++;
                q.pop();
                iloscOperacji+=3;
                krawedzie.push_back(Krawedz(s, k, w));
                vector<pair<int, int> >::iterator j;
                for(j=krawedz[k].begin(); j<krawedz[k].end(); j++){
                    if(odwiedzone[(*j).second]==false){
                        q.push(Krawedz(k, (*j).second, (*j).first));
                        iloscOperacji++;
                    }
                }
            }
            else{
                q.pop();
                iloscOperacji++;
            }
        }
        return XClass(krawedzie, iloscOperacji, koszt);
    }

    XClass Kruskal(int X)
    {
        vector<Krawedz> krawedzie;
        int iloscOperacji = 0;
        int koszt = 0;
        int *miara = new int[X+1];
        vector<int> *wierzcholki = new vector<int>[X+1];
        for(int i=0; i<=X; i++)
        {
            miara[i] = i;
            wierzcholki[i].push_back(i);
        }
        int licznik = 1;
        int s, k, w;
        while(licznik!=X)
        {
            s=qKruskal.top().poczatek;
            k=qKruskal.top().koniec;
            w=qKruskal.top().waga;
            vector<int>::iterator i;
            if(miara[s] == miara[k])
            {
                qKruskal.pop();
                iloscOperacji++;
            }
            else
            {
                koszt+=w;
                iloscOperacji++;
                krawedzie.push_back(Krawedz(s, k, w));
                licznik++;
                if(wierzcholki[miara[s]].size() >= wierzcholki[miara[k]].size())
                {
                    while(!wierzcholki[k].empty())
                    {
                        miara[wierzcholki[k].back()] = miara[s];
                        wierzcholki[miara[s]].push_back(wierzcholki[k].back());
                        wierzcholki[k].pop_back();
                        iloscOperacji+=3;
                    }
                }
                else
                {
                    while(!wierzcholki[s].empty())
                    {
                        miara[wierzcholki[s].back()] = miara[k];
                        wierzcholki[miara[k]].push_back(wierzcholki[s].back());
                        wierzcholki[s].pop_back();
                        iloscOperacji+=3;
                    }
                }
                qKruskal.pop();
                iloscOperacji++;
            }

        }
        return XClass(krawedzie, iloscOperacji, koszt);
    }

};

void dodajKrawedzieWGrafie(Graf g, int iloscWierzcholkow)
{
    int a=(rand()%iloscWierzcholkow)+1;
    int b=(rand()%iloscWierzcholkow)+1;
    if(a==b){
        dodajKrawedzieWGrafie(g, iloscWierzcholkow);
    }
    int waga=(rand()%100)+1;
    int bb;
    vector<pair<int, int> >::iterator i;
    for(i=g.krawedz[a].begin(); i!=g.krawedz[a].end(); i++)
    {
        bb = (*i).second;
        if(b == bb){
            dodajKrawedzieWGrafie(g, iloscWierzcholkow);
            return;
        }
    }
    g.dodajKrawedz(a, b, waga);
}

void trybA()
{
    fstream plik;
    plik.open("E:\\studia\\1 semestr\\Algosy\\KruskalPrime\\graf.txt", ios::in);
    string pierwszaLinia;
    stringstream ss;
    getline(plik, pierwszaLinia);
    ss.str("");
    ss.clear();
    ss<<pierwszaLinia;
    int iloscWierzcholkow;
    ss>>iloscWierzcholkow;
    Graf g(iloscWierzcholkow);

    while(!plik.eof())
    {
        string krawedzie;
        getline(plik, krawedzie);
        int licznik = 0;
        string wierzcholek1 = "";
        string wierzcholek2 = "";
        string waga = "";
        for(licznik; krawedzie[licznik]>=48 && krawedzie[licznik]<=57; licznik++)
        {
            wierzcholek1 += krawedzie[licznik];
        }
        licznik++;
        for(licznik; krawedzie[licznik]>=48 && krawedzie[licznik]<=57; licznik++)
        {
            wierzcholek2 += krawedzie[licznik];
        }
        licznik++;
        for(licznik; krawedzie[licznik]>=48 && krawedzie[licznik]<=57; licznik++)
        {
            waga += krawedzie[licznik];
        }
        int a, b, w;
        ss.str("");
        ss.clear();
        ss<<wierzcholek1;
        ss>>a;
        ss.str("");
        ss.clear();
        ss<<wierzcholek2;
        ss>>b;
        ss.str("");
        ss.clear();
        ss<<waga;
        ss>>w;
        g.dodajKrawedz(a, b, w);
    }
    cout<<"PRIME: "<<endl;
    XClass zwracaniePrime = g.Prime(iloscWierzcholkow);
    for(int i=0; i<zwracaniePrime.krawedzie.size(); i++)
        cout<<"V"<<zwracaniePrime.krawedzie[i].poczatek<<" -> V"<<zwracaniePrime.krawedzie[i].koniec<<"("<<zwracaniePrime.krawedzie[i].waga<<"); ";
    cout<<endl;
    cout<<"Calkowity koszt: "<<zwracaniePrime.koszt<<endl;
    cout<<"Ilosc operacji: "<<zwracaniePrime.iloscOperacji<<endl;

    cout<<endl<<"KRUSKAL: "<<endl;

    XClass zwracanieKruskal = g.Kruskal(iloscWierzcholkow);
    for(int i=0; i<zwracanieKruskal.krawedzie.size(); i++)
        cout<<"V"<<zwracanieKruskal.krawedzie[i].poczatek<<" -> V"<<zwracanieKruskal.krawedzie[i].koniec<<"("<<zwracanieKruskal.krawedzie[i].waga<<"); ";
    cout<<endl;
    cout<<"Calkowity koszt: "<<zwracanieKruskal.koszt<<endl;
    cout<<"Ilosc operacji: "<<zwracanieKruskal.iloscOperacji<<endl;
}

void trybB()
{
    cout<<"Podaj zakres wierzcholkow"<<endl;
    int minW, maxW;
    cin>>minW>>maxW;
    cout<<"Podaj nasycenie grafow"<<endl;
    int nasycenie;
    cin>>nasycenie;
    cout<<"Podaj ilosc powtorzen algorytmu"<<endl;
    int powtorzenia;
    cin>>powtorzenia;

    for(int i=minW; i<=maxW; i++)
    {
        int iloscKrawedzi=(i*(i-1))/2 * nasycenie/100;
        long czasPrime = 0;
        long czasKruskal = 0;
        for(int j=0; j<powtorzenia; j++)
        {
            Graf g(i);
            for(int k=1; k<i; k++)
            {
                int waga=(rand()%100)+1;
                g.dodajKrawedz(k, k+1, waga);
            }
            int pozostaleKrawdzie = iloscKrawedzi - (i - 1);
            for(int k=0; k<pozostaleKrawdzie; k++)
            {
                dodajKrawedzieWGrafie(g, i);
            }

            clock_t startPrime = clock();
            XClass zwracaniePrime = g.Prime(i);
            czasPrime += ((1000*clock()-startPrime))/CLOCKS_PER_SEC;

            clock_t startKruskal = clock();
            XClass zwracanieKruskal = g.Kruskal(i);
            czasKruskal += ((1000*clock()-startKruskal))/CLOCKS_PER_SEC;
        }
        cout<<"Ilosc wierzcholkow: "<<i<<" - czas Prime: "<<czasPrime/powtorzenia<<" - czas Kruskal: "<<czasKruskal/powtorzenia<<endl;
    }
}

int main()
{
    ios_base::sync_with_stdio(false);

    cout<<"Prosze o wybranie trybu"<<endl<<"'a' - wybor trybu A, 'b' - wybor trybu B"<<endl;
    char tryb;
    int iloscWierzcholkow, iloscKrawedzi;
    cin>>tryb;
    if(tryb == 'a')
        trybA();
    if(tryb == 'b')
        trybB();
    return 0;
}

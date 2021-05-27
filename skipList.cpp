//https://www.geeksforgeeks.org/skip-list-set-2-insertion/
//https://www.geeksforgeeks.org/skip-list-set-3-searching-deletion/
#include <bits/stdc++.h>

using namespace std;

ifstream f("abce.in");
ofstream g("abce.out");

class Nod{
public:

    int m_valoare;
    Nod **m_urmatorul; //retin elementele din skipList


    Nod(int valoare, int nivel);
};

Nod::Nod(int valoare, int nivel){
    m_valoare=valoare;

    m_urmatorul=new Nod*[nivel + 1];
    memset(m_urmatorul, 0, sizeof(Nod*) * (nivel + 1));//il initializez cu 0 pe toate pozitiile
}

class SkipList{
private:
    int m_nivelMaxim,m_nivelCurent;
    Nod *m_primulNod;//nodul de start acesta nu va aparea in skiplist
public:
    SkipList(int nivelMaxim);

    Nod* creeazaNod(int valoare, int nivel);

    int generareNivelRandom();
    void insereazaElement(int valoare);
    void stergeElement(int valoare);
    bool cautaElement(int valoare);
    int afiseazaPredecesor(int valoare);
    int afiseazaSuccesor(int valoare);
    void afisareSortata(int x, int y);

};

SkipList::SkipList(int nivelMaxim) {
    m_nivelMaxim=nivelMaxim;
    m_nivelCurent=0;
    m_primulNod=new Nod(-1,nivelMaxim);//initializarea nodului de start
}

int SkipList::generareNivelRandom() {
    int numarRandom,nivel;

    numarRandom=rand()%2;
    nivel=0;

    while(nivel<m_nivelMaxim && numarRandom==1){
        ++nivel;
        numarRandom=rand()%2;
    }

    return nivel;
}

Nod* SkipList::creeazaNod(int valoare, int nivel) {
    Nod *nodNou= new Nod(valoare,nivel);
    return nodNou;
}

void SkipList::insereazaElement(int valoare) {
    int i;

    Nod *nodCurent=m_primulNod;

    Nod *inaltime[m_nivelMaxim + 1];//retine cat de sus merge nodul
    /*
    2       7
    2       7       9           --la introducerea lui 7 se retine in inaltime cat de sus merge
    2       7   8   9*/
    memset(inaltime, 0, sizeof(Nod*) * (m_nivelMaxim + 1));

    for(i=m_nivelCurent;i>=0;--i){
        while(nodCurent->m_urmatorul[i] != NULL && nodCurent->m_urmatorul[i]->m_valoare < valoare){
            nodCurent=nodCurent->m_urmatorul[i];
        }
        inaltime[i]=nodCurent; ///pointez la nodul 2 de pe nivelul i cand il adaug pe 7 pe nivelul i(diagrama)
    }

    nodCurent=nodCurent->m_urmatorul[0];  ///cobor pe nivelul 0 pt nodul respectiv

    if(nodCurent==NULL || nodCurent->m_valoare !=valoare){
        int nivelRandom=generareNivelRandom();

        if(nivelRandom>m_nivelCurent){
            for(i=m_nivelCurent+1;i<=nivelRandom;++i){
                inaltime[i]=m_primulNod;        
            }

            m_nivelCurent=nivelRandom;
        }

        Nod* nod= creeazaNod(valoare,nivelRandom);      //creez nodul

        for(i=0;i<=nivelRandom;++i){
            nod->m_urmatorul[i]=inaltime[i]->m_urmatorul[i]; //il conectez cu succesrul
            inaltime[i]->m_urmatorul[i]=nod;//il conectez cu predecesorul
        }
    }
}

void SkipList::stergeElement(int valoare) {
    int i;

    Nod *nodCurent=m_primulNod;

    Nod *inaltime[m_nivelMaxim+1];
    memset(inaltime,0,sizeof(Nod*)*(m_nivelMaxim+1));

    for(i=m_nivelCurent;i>=0;--i){
        while(nodCurent->m_urmatorul[i] != NULL && nodCurent->m_urmatorul[i]->m_valoare<valoare){
            nodCurent=nodCurent->m_urmatorul[i];
        }
        inaltime[i]=nodCurent;
    }

    nodCurent=nodCurent->m_urmatorul[0];

    if(nodCurent != NULL && nodCurent->m_valoare==valoare){
        for(i=0;i<=m_nivelCurent;++i){
            if(inaltime[i]->m_urmatorul[i] != nodCurent){
                break;
            }
            inaltime[i]->m_urmatorul[i]=nodCurent->m_urmatorul[i];      //sterg nodul din inaltime si conectez predecesorul si succesorul intre ei
        }

    }
    while(m_nivelCurent>0 && m_primulNod->m_urmatorul[m_nivelCurent]==0){
        m_nivelCurent--;                //daca e singurul nod de pe inaltime cea mai mare scad inaltimea
    }
}

bool SkipList::cautaElement(int valoare) {
    int i;

    Nod *nodCurent=m_primulNod;

    for(i=m_nivelCurent;i>=0;--i){
        while(nodCurent->m_urmatorul[i]!=NULL && nodCurent->m_urmatorul[i]->m_valoare<valoare){
            nodCurent=nodCurent->m_urmatorul[i];
        }
    }

    nodCurent=nodCurent->m_urmatorul[0];

    if(nodCurent!=NULL && nodCurent->m_valoare==valoare)
        return 1;
    return 0;

}

int SkipList::afiseazaPredecesor(int valoare){
    int i;

    Nod *nodCurent=m_primulNod;

    for(i=m_nivelCurent;i>=0;--i){
        while(nodCurent->m_urmatorul[i]!=NULL && nodCurent->m_urmatorul[i]->m_valoare<=valoare){
            nodCurent=nodCurent->m_urmatorul[i];
        }
    }
    return nodCurent->m_valoare;
}

int SkipList::afiseazaSuccesor(int valoare){
    int i;

    Nod *nodCurent=m_primulNod;

    for(i=m_nivelCurent;i>=0;--i){
        while(nodCurent->m_urmatorul[i]!=NULL && nodCurent->m_urmatorul[i]->m_valoare<valoare){
            nodCurent=nodCurent->m_urmatorul[i];
        }  
    }
    nodCurent=nodCurent->m_urmatorul[0];
    return nodCurent->m_valoare;
}

void SkipList::afisareSortata(int x,int y){
    int i,ok=0;

    Nod *nodCurent=m_primulNod;

    for(i=m_nivelCurent;i>=0;--i){
        while(nodCurent->m_urmatorul[i] != NULL && nodCurent->m_urmatorul[i]->m_valoare<x){
            nodCurent=nodCurent->m_urmatorul[i];
        }
    }
    // de cand e >=x, cobor pe nivelul 0 pentru a le afisa pe toate(pe alt nivel mai sar din ele)
    while(nodCurent->m_urmatorul[0]!=NULL && nodCurent->m_urmatorul[0]->m_valoare>=x && nodCurent->m_urmatorul[0]->m_valoare<=y){
            nodCurent=nodCurent->m_urmatorul[0];
            g<<nodCurent->m_valoare<<" ";
        }
}

int main() {
    srand(time(0));
    SkipList skipList(15);

    int tip,q,x,y,i,valoare;
    f>>q;
    for(i=0;i<q;++i){
        f>>tip;
        if(tip==1){
            f>>valoare;
            skipList.insereazaElement(valoare);
        }
        else if(tip==2){
            f>>valoare;
            skipList.stergeElement(valoare);
        }
        else if(tip==3){
            f>>valoare;
            g<<skipList.cautaElement(valoare)<<'\n';
        }
        else if(tip==4){
            f>>valoare;
            g<<skipList.afiseazaPredecesor(valoare)<<'\n';
        }
        else if(tip==5){
            f>>valoare;
            g<<skipList.afiseazaSuccesor(valoare)<<'\n';
        }
        else if(tip==6){
            f>>x>>y;
            skipList.afisareSortata(x,y);
            g<<'\n';
        }
    }

    return 0;
}


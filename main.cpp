//============================================================================
// Author      : Wiktor Jurek
// Copyright   : Your copyright notice
//============================================================================

#include <iostream>
#include <string>
#include <bits/stdc++.h>
#include <cmath>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <functional>
#include <fstream>
#include <math.h>       /* pow */
using namespace std;



struct edge             //pomocnicza struktura danych do przechowywania listy krawędzi (i ich wag)
{
    int s;                     //węzeł początkowy krawędzi (s,t)
    int t;                     //węzeł końcowy krawędzi (s,t)
    double w;            //waga krawędzi (s,t)
};

class Graph
{
private:
    double** adjMatrix;
    int n;                                //liczba węzłów
    bool directed;                //true jeśli graf jest skierowany
    bool weighted;                //true jeśli jest to graf ważony
public:
    Graph(int n, int m, edge edges[], bool directed, bool weighted);    //tworzy graf na podstawie listy krawędzi
    void insertEdge(int i, int j, double w);                   //wstawia krawędź (i, j)(i (j,i) dla grafu nieskierowanego)
    bool check(int i, int j);                                           //sprawdza czy krawędź (i, j) istnieje
    void bfs(int s);
    friend ostream& operator<<(ostream& out, Graph& g);        //wyświetla graf jako macierz
    void print();
    int getSize();
    bool isWeighted();
    double getEdge(int i, int j);
    void Dijkstra(int s);
    int findSmallest(int* S, double* d);
};

Graph::Graph(int n, int m, edge edges[], bool directed, bool weighted)
{

    this->n = n;
    this->directed = directed;
    this->weighted = weighted;
    this->adjMatrix = new double*[n];
    //inicjalizacja macierzy sąsiedztwa
    for(int i=0; i<n; i++)
    {
        this->adjMatrix[i] = new double[n];
        for(int j=0; j<n; j++)
        {
            if(weighted)
            {
                this->adjMatrix[i][j] = INFINITY;
                if(i == j)
                {
                    this->adjMatrix[i][j] = 1;
                }
            }
            else
            {
                this->adjMatrix[i][j] = 0;
            }

        }
    }
    //wypełnienie tablicy sąsiedztwa na podstawie tablicy krawędzi i warunku czy graf jest skierowany
    for(int i =0; i<m; i++)
    {
        int s,t,w;
        s = edges[i].s;
        t = edges[i].t;
        w = edges[i].w;
        this->adjMatrix[s][t] = w;
        if(!directed)
        {
            this->adjMatrix[t][s] = w;
        }
    }


}



void Graph::insertEdge(int i, int j, double w)
{
    if(i < this->n && i>=0 && j < this->n &&j>=0)
    {
        this->adjMatrix[i][j] = w;
        if(!this->directed)
        {
            this->adjMatrix[j][i] = w;
        }
    }

}




void Graph::bfs(int s)
{

    if(s>= 0 && s< this->n)
    {
        cout<<s<<",";
        double* visited = new double[this->n];
        queue<double>queue1;
        queue1.push(s);
        for(int i=0; i<n; i++)
        {
            visited[i] = -1;
        }
        int index = 0;
        visited[index] = s;
        index++;
        while(!queue1.empty())
        {
            s = queue1.front();
            for(int i=0; i<this->n; i++)
            {
                //sprawdzamy każdą krawędź od wierzchołka w kierunku i-tego wierzchołka
                //czy nie jest pusta
                if(this->check(s,i))
                {
                    //jeśli krawędź istanieje sprawdzamy czy już odwiedziliśmy dany wierzchołek
                    bool alreadyVisited = false;
                    for(int j=0; j<index; j++)
                    {

                        if(i == visited[j])
                        {
                            alreadyVisited = true;
                        }
                    }
                    //jeśli nie dodajemy go do kolejki
                    if(!alreadyVisited)
                    {
                        queue1.push(i);
                        visited[index] = i;
                        cout<<i<<",";
                        index++;
                    }
                }
            }
            queue1.pop();
        }


    }



}

int Graph::getSize()
{
    return this->n;
}

bool Graph::isWeighted()
{
    return this->weighted;
}

double Graph::getEdge(int i, int j)
{
    return this->adjMatrix[i][j];
}

bool Graph::check(int i,int j)
{

    if(i>=0 && i< this->n && j>=0 && j<this->n && i != j)
    {
        double emptyValue =0;
        if(this->weighted)
        {
            emptyValue = INFINITY;
        }
        if(this->adjMatrix[i][j] != emptyValue)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }

}

int Graph::findSmallest(int* S,double* d)
{
    //sprawdzamy czy jest jeszcze jakiś wierzchołek dla którego nie znaleziono najkrótszej ścieżki
    bool full = true;
    for(int i=0; i<n; i++)
    {
        if(S[i] != 1)
        {
            full = false;
        }
    }
    if(full)
    {
        return -1;
    }
    else
    {
        int smallest;
        //pobieramy index pierwszego elementu nie posiadającego znalezionej najkrótszej ścieżki
        for(int i=0; i<n; i++)
        {
            if(S[i] != 1)
            {
                smallest = i;
                i =n;
            }
        }

        for(int i=0; i<this->n; i++)
        {
            //sprawdzamy czy znaleziono już najkrótszą ścieżkę dla itego wieszchołka jeśli nie to czy długość ściezki mniejsza niż poprzedniego znalezionego
            if(S[i] != 1 && d[i] < d[smallest])
            {
                smallest = i;
            }

        }

        return smallest;
    }

}



void Graph::Dijkstra(int s)
{

    if(s>= 0 && s<this->n)
    {

        int* S = new int[this->n]; //tablica przechowująca info czy węzeł odwiedzony
        double* d = new double[this->n]; //tablica przechowująca najkutszą ścieżkę
        int* parent = new int[this->n]; //tablica przechowująca rodziców - przodka skąd przysszliśmy
        //inicjalizacaj tablic
        for(int i=0; i<this->n; i++)
        {
            S[i] = 0;
            d[i] = INFINITY;
            parent[i] = -2;   //ustawiamy -2 bo nie możemy -1 gdyż -1 dostanie węzeł od którego zaczynamy
        }
        // S[s] = 1;
        d[s] = 0;
        parent[s] = -1;
        //dopóki istnieje wierzchołek który nie ma znalezionej najkrótszej ścieżki
        while(this->findSmallest(S,d) != -1)
        {
            //wybieramy wierzchołek z najkrótszą ścieżką
            s = this->findSmallest(S,d);
            for(int i=0; i<this->n; i++)
            {
                //sparwdzamy czy istnieje krawędź pomiedzy wierzcholkami s i i
                if(this->check(s,i))
                {
                    //jeśli nie znaleziono jeszcze najkrótszej ścieżki dla danego wierzchołka
                    if(S[i] != 1)
                    {
                        if(d[i] > (d[s] + this->getEdge(s,i)))
                        {
                            d[i] = d[s] + this->getEdge(s,i);
                            parent[i] = s;
                        }
                    }
                }
            }
            S[s] = 1;

        }
        cout<<endl<<"Rodzic :";
        for(int i=0; i<n; i++)
        {
            cout<<parent[i]<<",";
        }
        cout<<endl<<"Dlugosc sciezki :";
        for(int i=0; i<n; i++)
        {
            cout<<d[i]<<",";
        }
        cout<<endl;


    }


}


//wypisujemy graf w postaci si ->sj(w)
void Graph::print()
{
    double emptyValue = 0;
    if(this->weighted)
    {
        emptyValue = INFINITY;
    }

    for(int i=0; i<this->n; i++)
    {
        cout<<i<<"-> ";
        for(int j=0; j<this->n; j++)
        {

            if(j!=i && emptyValue != this->adjMatrix[i][j])
            {
                cout<<j;
                if(this->weighted)
                {
                    cout<<"("<<this->adjMatrix[i][j]<<")";
                }
                cout<<",";
            }
        }
        cout<<endl;
    }

}
//wypisujemy graf w postaci macierzy
ostream& operator<<(ostream& out, Graph& g)
{

    double emptyValue = 0;
    if(g.isWeighted())
    {
        emptyValue = INFINITY;
    }
    for(int i=0; i<g.getSize(); i++)
    {

        for(int j=0; j<g.getSize(); j++)
        {
            if(emptyValue != g.getEdge(i,j))
            {
                cout<<g.getEdge(i,j)<<",";
            }
            else
            {
                cout<<"-,";
            }
        }
        cout<<endl;
    }

}

int main(int argc, char** argv)
{
    cout<<"test zad 13.1 - graf skierowany niewazony"<<endl;
    //testy metody bfs
    int n, m;
    n =10;
    m=18;
    edge undirectedGraph[]= {{0,1, 1}, {1,2, 1},{1,7, 1},{2,3,1},{2,8,1}, {3,2,1},{3,9,1}, {4,0,1},{5,3,1},{5,8,1},{6,0,1},{6,7,1},{7,1,1},{7,4,1},{7,6,1},{8,5,1},{8,7,1},{9,8,1}  };
    Graph g(n,m,undirectedGraph,true, false);
    g.print();
    cout<<endl;
    cout<<g;
    cout<<endl;
    cout<<"Algorytm bfs :";
    g.bfs(0);

    cout<<endl<<endl<<"test zad 13.2 - graf skierowany wazony"<<endl;
    //test algorytmu Dijkstra
    int n1,m1;
    n1=10;
    m1= 19;
    edge undirectedGraph1[]= {{0,1, 5}, {1,2, 2},{1,7, 3},{2,3,7},{2,8,6}, {3,2,7},{3,9,5}, {4,0,2},{5,3,2},{5,8,5},{6,0,5},{6,7,5},{7,1,3},{7,4,1},{7,6,5},{7,8,1},{8,5,5},{8,7,1},{9,8,4} };
    Graph g1(n1,m1,undirectedGraph1,true, true);
    g1.print();
    cout<<endl;
    cout<<g1;
    cout<<endl;
    g1.Dijkstra(0);


    return 0;
}





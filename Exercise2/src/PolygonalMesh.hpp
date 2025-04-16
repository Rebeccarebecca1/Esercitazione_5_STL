#ifndef _POLYGONAL_H_             
#define _POLYGONAL_H_

#include <iostream>               
#include "Eigen/Eigen"            

using namespace std;              
using namespace Eigen;            
namespace PolygonalLibrary {      

// Struttura dati che rappresenta la mesh poligonale
struct PolygonalMesh
{
    
    // --- CELLE 0D (VERTICI) ---

    unsigned int NumberCell0D = 0;  // Numero totale di vertici (celle 0D) nella mesh

    vector<unsigned int> Cell0DId = {};  // Vettore che memorizza gli ID univoci dei vertici

    vector<Vector2d> Cell0DCoordinates = {};  
    // Vettore che contiene le coordinate (x, y) di ogni vertice, 
    // ognuna rappresentata come un Vector2d di Eigen

    map<unsigned int, list<unsigned int>> Cell0DMarkers = {};  
    // Mappa che associa a un marker una lista di ID di vertici.
    // Serve per identificare gruppi speciali di vertici, come quelli sul bordo o con condizioni al contorno.

    // --- CELLE 1D (LATI) ----

    unsigned int NumberCell1D = 0;  // Numero totale di lati (celle 1D)

    vector<unsigned int> Cell1DId = {};  // Vettore che contiene gli ID dei lati (1 per ogni lato)

    vector<Vector2i> Cell1DVertices = {};  
    // Vettore di Vector2i (interi) che rappresentano i vertici iniziale e finale di ogni lato

    map<unsigned int, list<unsigned int>> Cell1DMarkers = {};  
    // Mappa che associa un marker a una lista di ID di lati.
    // Anche qui, utile per raggruppare lati che hanno una particolare proprietà.

    // --- CELLE 2D (POLIGONI/TRIANGOLI) ---

    unsigned int NumberCell2D = 0;  // Numero totale di celle 2D (poligoni o triangoli)

    vector<unsigned int> Cell2DId = {};  // Vettore degli ID di ogni cella 2D

    vector<vector<unsigned int>> Cell2DVertices = {};  
    // Vettore di vettori: ogni vettore interno contiene gli ID dei vertici che formano la cella 2D

    vector<vector<unsigned int>> Cell2DEdges = {};  
    // Vettore di vettori: ogni vettore interno contiene gli ID dei lati (celle 1D) che formano il bordo della cella 2D
};

} 

#endif 
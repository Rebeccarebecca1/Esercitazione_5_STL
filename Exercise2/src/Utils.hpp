#ifndef _UTILS_H_              
#define _UTILS_H_              

#include <iostream>            
#include "PolygonalMesh.hpp"  

using namespace std;          

namespace PolygonalLibrary {  

// Funzione che esegue dei test di lettura sul file della mesh
// Restituisce true se il file viene letto correttamente e false altrimenti
bool Tests(const string &filepath, PolygonalMesh& mesh);

// Funzione che importa i dati delle celle 0D (punti) da un file CSV
// mesh: una struttura PolygonalMesh che verrà riempita con i dati letti
// filename: il nome del file CSV da cui leggere
// Ritorna true se l’importazione ha successo e false altrimenti
bool ImportCell0Ds(const string &filename, PolygonalMesh& mesh);

// Funzione che importa i dati delle celle 1D (spigoli) da un file CSV
// mesh: una struttura PolygonalMesh che verrà riempita con i dati letti
// filename: il nome del file CSV da cui leggere
// Ritorna true se l’importazione ha successo e false altrimenti
bool ImportCell1Ds(const string &filename, PolygonalMesh& mesh);

// Funzione che importa i dati delle celle 2D (poligoni) da un file CSV
// mesh: una struttura PolygonalMesh che verrà riempita con i dati letti
// filename: il nome del file CSV da cui leggere
// Ritorna true se l’importazione ha successo e false altrimenti
bool ImportCell2Ds(const string &filename, PolygonalMesh& mesh);

} 

#endif 

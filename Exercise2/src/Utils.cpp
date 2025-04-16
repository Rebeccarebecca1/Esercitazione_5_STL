#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

namespace PolygonalLibrary {


bool Tests(const string& filepath,
                PolygonalMesh& mesh)
{

    if(!ImportCell0Ds(filepath + "/Cell0Ds.csv",
                       mesh))
    {
        return false;
    }
    else
    {return true;}

    if(!ImportCell1Ds(filepath + "/Cell1Ds.csv",
                       mesh))
    {
        return false;
    }
    else{return true;}

    // Importa i dati dal file "Cell2Ds.csv" nel mesh
    if(!ImportCell2Ds(filepath + "/Cell2Ds.csv",
                       mesh))
    {
        return false;
    }
    else{
		// Definisce un vettore di vettori per memorizzare la lunghezza dei lati dei triangoli
        vector<vector<double>> sides; 
        for (unsigned int c = 0; c < mesh.NumberCell2D; c++)
        {
			// Verifica se il triangolo ha esattamente 3 vertici
            if (mesh.Cell2DVertices[c].size() == 3)
            {
				// Recupera i lati del triangolo
                vector<unsigned int> edges = mesh.Cell2DEdges[c];
				// Itero ora per ogni lato del triangolo
                for (unsigned int e = 0; e < 3; e++)
                {
					// Mi salvo quindi i vertici di fine e inzio per ogni lato
                    const double origin = mesh.Cell1DVertices[edges[e]][0];
                    const double end = mesh.Cell1DVertices[edges[e]][1];
					// Mi calcolo ora la lunghezza del lato
                    sides[c][e] = (mesh.Cell0DCoordinates[end] - mesh.Cell0DCoordinates[origin]).norm();
					// Controlla che la lunghezza el lato non sia nulla
                    if(!sides[c][e])
                    {
                        cerr << "Lenght zero!" << endl;
                        return 2;
                    }
                }
				// Calcolo quindi la lunghezza del semiperimetro
                double semiperimeter = (sides[c][0] + sides[c][1] + sides[c][2])/2;
				// Usando la formula del semiperimetro calcolo l'area 
                double area = sqrt(semiperimeter * (semiperimeter - sides[c][0])*(semiperimeter - sides[c][1])*(semiperimeter - sides[c][2]));
				// Controllo che l'area non sia nulla
                if (!area){
                    cerr << "Zero area!" << endl;
                    return 3;
                }
            }
        }
    }
}

bool ImportCell0Ds(const string &filename,
                   PolygonalMesh& mesh)
{

    ifstream file; 
    file.open(filename);

    if(file.fail())
        return false;

    // Lista per memorizzare le righe del file CSV
    list<string> listLines; 
    string line;
    while (getline(file, line))
    {
        istringstream lineStream(line);
        string cell;
        string row;
		// Divido la riga in celle (separate da ';') e le concatena in una stringa unica separata da spazi
        while (getline(lineStream, cell, ';')) {
            row = row + cell + " ";
        }
		// Aggiungo la riga convertita alla lista
        listLines.push_back(row); 
    }
    file.close();

    listLines.pop_front(); // Rimuovo l'intestazione

    mesh.NumberCell0D = listLines.size(); // Salva il numero di celle 0D (punti)

    if (mesh.NumberCell0D == 0)
    {
        cerr << "There is no cell 0D" << endl;
        return false;
    }
    
	// Riserva memoria nei vettori della mesh 
    mesh.Cell0DId.reserve(mesh.NumberCell0D); 
    mesh.Cell0DCoordinates.reserve(mesh.NumberCell0D);

    // Cicla su tutte le righe (cioè su ogni punto)
    for (const string& line : listLines) 
    {
        istringstream converter(line); 

        unsigned int id;
        unsigned int marker;
        Vector2d coord;
		
        // Legge ID, marker, x, y dalla riga
        converter >>  id >> marker >> coord(0) >> coord(1); 

         // Salvo l’ID del punto
        mesh.Cell0DId.push_back(id); 
		// Salvo le coordinate del punto
        mesh.Cell0DCoordinates.push_back(coord); 

        // Se il marker è diverso da 0, aggiungo l’ID del punto alla mappa dei marker
        if( marker != 0)
        {
			// Inserisce un nuovo marker con il relativo ID
            auto ret = mesh.Cell0DMarkers.insert({marker, {id}}); 
			// Se però il marker già esiste, aggiunge l’ID alla lista associata a quel marker
            if(!ret.second) 
                (ret.first)->second.push_back(id); 
        }

    }
    file.close();
    return true;
}



bool ImportCell1Ds(const string &filename,
                   PolygonalMesh& mesh)
{

    ifstream file;
    file.open(filename);

    if(file.fail())
        return false;
    // Lista di stringhe per contenere tutte le righe del CSV 
    list<string> listLines;
    string line;
    while (getline(file, line))
    {
        istringstream lineStream(line);
        string cell;
        string row;
		// Ora divido la riga in celle usando il separatore ';' e concatena con spazio
        while (getline(lineStream, cell, ';')) {
            row = row + cell + " ";
        }
		// Aggiungo la riga alla lista inizializzata prima

        listLines.push_back(row);
    }
	// Rimuoovo la prima riga di intestazione
    listLines.pop_front();
    // Imposto quanti lati (celle 1D) ci sono nella mesh
    mesh.NumberCell1D = listLines.size();

    if (mesh.NumberCell1D == 0)
    {
        cerr << "There is no cell 1D" << endl;
        return false;
    }
    // Riservo spazio nei vettori Cell1DId (gli ID) e Cell1DVertices (le coppie di vertici che compongono i lati)
    mesh.Cell1DId.reserve(mesh.NumberCell1D);
    mesh.Cell1DVertices.reserve(mesh.NumberCell1D);
    
	// Ciclo su ogni riga
    for (const string& line : listLines)
    {
        istringstream converter(line);

        unsigned int id;
        unsigned int marker;
        Vector2i vertices;  // Inizializzo un vettore di due integer

        converter >>  id >> marker >> vertices(0) >> vertices(1);  // Estraggo i quattro elementi numerici della line

        // Aggiungo l’ID del lato al vettore Cell1DId, che tiene traccia di tutti gli ID dei lati
        mesh.Cell1DId.push_back(id);
		// Aggiungo la coppia di vertici (origine e fine) che definisce il lato del vettore
        mesh.Cell1DVertices.push_back(vertices);

        if( marker != 0)
        {
            auto ret = mesh.Cell1DMarkers.insert({marker, {id}});
            if(!ret.second)
                (ret.first)->second.push_back(id);
        }
    }

    file.close();

    return true;
}



bool ImportCell2Ds(const string &filename,
                   PolygonalMesh& mesh)
{

    ifstream file;
    file.open(filename);

    if(file.fail())
        return false;

    list<string> listLines;
    string line;
	// Leggo ogni riga del file, divide la riga usando ; 
	// come separatore e unisce le celle in una singola stringa con spazi
    while (getline(file, line))
    {
        istringstream lineStream(line);
        string cell;
        string row;
        while (getline(lineStream, cell, ';')) {
            row = row + cell + " ";
        }
        listLines.push_back(row);
    }

    // Rimuovo la prima riga che contiene l’intestazione
    listLines.pop_front();

    mesh.NumberCell2D = listLines.size();

    if (mesh.NumberCell2D == 0)
    {
        cerr << "There is no cell 2D" << endl;
        return false;
    }

    // Riservo memoria per ID, vertici e lati 
    mesh.Cell2DId.reserve(mesh.NumberCell2D);
    mesh.Cell2DVertices.reserve(mesh.NumberCell2D);
    mesh.Cell2DEdges.reserve(mesh.NumberCell2D);

    // Itero sulle righe del file e inizializzo uno stream per leggere i dati da ciascuna riga
    for (const string& line : listLines)
    {
        istringstream converter(line);

        unsigned int id;
        unsigned int numvertices;
        unsigned int numedges;
        unsigned int marker;

        converter >>  id;
        converter >> marker;
        converter >> numvertices;
		// Creo un vettore per contenere gli indici dei vertici e lo riempio con i dati 
        vector<unsigned int> vertices(numvertices);
        for(unsigned int i = 0; i < numvertices; i++)
            converter >> vertices[i];
        converter >> numedges;
        vector<unsigned int> edges(numedges);

        // Legge il numero di lati (edge) e poi gli indici degli edge salvandoli nel vettore edges 
		// definito nella riga di sopra
        for(unsigned int i = 0; i < numedges; i++)
            converter >> edges[i];

        mesh.Cell2DId.push_back(id);
        mesh.Cell2DVertices.push_back(vertices);
        mesh.Cell2DEdges.push_back(edges);
    }
    file.close();
    return true;
}

}

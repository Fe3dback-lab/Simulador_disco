#ifndef DISCO_H
#define DISCO_H
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <filesystem>
#include <vector>
using namespace std;
namespace fs = std::filesystem;

class Disco_duro {
public:
    int platos;
    int pistas;
    int sectores;
    int capacidad_sector;
    int cantidad_sectores_bloque;
    int espacio_total;
    int espacio_utilizado;
    int bloque_actual;

    Disco_duro();
    void configurar_disco();
    void crear_directorio(const char* ruta);
    void crear_disco();
    void recuperar_datos_disco();
    void borrar_disco();
    void mostrar_arbol(const char* path, int nivel = 0);
    void mostrar_informacion();
    void escribirSector(vector<char>&, int*);
    vector<char> leerSector(int*);
    void cargar_csv(const string& ruta_csv, const string& nombre_tabla);
    void guardar_en_indice(const string& nombre_tabla, int bloque_id, int* direccion);
    void mostrar_tabla(const string& nombre_tabla);
};

Disco_duro::Disco_duro() {
    platos = pistas = sectores = capacidad_sector = cantidad_sectores_bloque = espacio_total = espacio_utilizado = 0;
    bloque_actual = 0;
}

void Disco_duro::configurar_disco() {
    cout << "Configurando disco duro...\n";
    cout << "Ingrese la cantidad de platos: ";
    cin >> platos;
    cout << "Ingrese la cantidad de pistas por plato: ";
    cin >> pistas;
    cout << "Ingrese la cantidad de sectores por pista: ";
    cin >> sectores;
    cout << "Ingrese la capacidad de cada sector (en bytes): ";
    cin >> capacidad_sector;
    cout << "Ingrese la cantidad de sectores por bloque: ";
    cin >> cantidad_sectores_bloque;
    espacio_total = (platos * 2) * pistas * sectores * capacidad_sector;
    cout << "Disco configurado correctamente.\n";
}

void Disco_duro::crear_directorio(const char* ruta) {
    fs::create_directories(ruta);
}

void Disco_duro::crear_disco() {
    crear_directorio("Disco");
    char ruta[256];
    for (int p = 0; p < platos; ++p) {
        for (int sup = 0; sup < 2; ++sup) {
            for (int t = 0; t < pistas; ++t) {
                sprintf(ruta, "Disco/Plato%d/Superficie%d/Pista%d", p, sup, t);
                crear_directorio(ruta);
                for (int s = 0; s < sectores; ++s) {
                    sprintf(ruta, "Disco/Plato%d/Superficie%d/Pista%d/Sector%d.txt", p, sup, t, s);
                    ofstream archivo(ruta);
                    archivo << string(capacidad_sector, '0');
                    archivo.close();
                }
            }
        }
    }
    cout << "Disco creado correctamente con bloques de " << cantidad_sectores_bloque << " sectores.\n";
}

void Disco_duro::recuperar_datos_disco() {
    if (!fs::exists("Disco")) {
        cout << "No existe el disco para recuperar.\n";
        return;
    }
    espacio_utilizado = 0;
    for (const auto& archivo : fs::recursive_directory_iterator("Disco")) {
        if (archivo.path().extension() == ".txt") {
            ifstream sector(archivo.path());
            string contenido((istreambuf_iterator<char>(sector)), istreambuf_iterator<char>());
            for (char c : contenido) if (c != '0') espacio_utilizado++;
        }
    }
    cout << "Datos del disco recuperados.\n";
}

void Disco_duro::borrar_disco() {
    fs::remove_all("Disco");
    cout << "Disco borrado correctamente.\n";
}

void Disco_duro::mostrar_arbol(const char* path, int nivel) {
    for (const auto& entry : fs::directory_iterator(path)) {
        for (int i = 0; i < nivel; ++i) cout << "|   ";
        if (fs::is_directory(entry.status())) {
            cout << "+-- " << entry.path().filename().string() << "/" <<endl;
            mostrar_arbol(entry.path().string().c_str(), nivel + 1);
        } else {
            cout << "+-- " << entry.path().filename().string() <<endl;
        }
    }
}

void Disco_duro::mostrar_informacion() {
    cout << "=== Información del Disco ===\n";
    cout << "Platos: " << platos << "\n";
    cout << "Pistas por plato: " << pistas << "\n";
    cout << "Sectores por pista: " << sectores << "\n";
    cout << "Capacidad por sector: " << capacidad_sector << " bytes\n";
    cout << "Sectores por bloque: " << cantidad_sectores_bloque << "\n";
    cout << "Espacio total: " << espacio_total << " bytes\n";
    cout << "Espacio utilizado: " << espacio_utilizado << " bytes\n";
}

void Disco_duro::escribirSector(vector<char>& datos, int* direccion) {
    if (datos.size() > capacidad_sector) {
        cout << "Error: datos exceden la capacidad del sector.\n";
        return;
    }
    char ruta[256];
    sprintf(ruta, "Disco/Plato%d/Superficie%d/Pista%d/Sector%d.txt", direccion[0], direccion[1], direccion[2], direccion[3]);
    ofstream archivo(ruta);
    if (!archivo) {
        cout << "Error: no se pudo abrir el sector.\n";
        return;
    }
    for (char c : datos) archivo << c;
    archivo << string(capacidad_sector - datos.size(), '0'); // rellena con ceros
    archivo.close();
    espacio_utilizado += datos.size();
}

vector<char> Disco_duro::leerSector(int* direccion) {
    char ruta[256];
    sprintf(ruta, "Disco/Plato%d/Superficie%d/Pista%d/Sector%d.txt", direccion[0], direccion[1], direccion[2], direccion[3]);
    ifstream archivo(ruta);
    vector<char> contenido;
    if (!archivo) {
        cout << "Error: no se pudo leer el sector.\n";
        return contenido;
    }
    char byte;
    while (archivo.get(byte)) contenido.push_back(byte);
    return contenido;
}

void Disco_duro::cargar_csv(const string& ruta_csv, const string& nombre_tabla) {
    ifstream archivo(ruta_csv);
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo CSV.\n";
        return;
    }

    string linea;
    vector<string> columnas;
    vector<vector<string>> registros;

    // Leer encabezado
    if (getline(archivo, linea)) {
        stringstream ss(linea);
        string campo;
        while (getline(ss, campo, ',')) {
            columnas.push_back(campo);
        }
    }

    // Leer registros
    while (getline(archivo, linea)) {
        stringstream ss(linea);
        string valor;
        vector<string> fila;
        while (getline(ss, valor, ',')) {
            fila.push_back(valor);
        }
        if (!fila.empty()) registros.push_back(fila);
    }

    // Inferir tipos de datos
    vector<string> tipos(columnas.size(), "int");
    for (size_t col = 0; col < columnas.size(); ++col) {
        for (size_t row = 0; row < registros.size(); ++row) {
            string val = registros[row][col];
            if (val.find('.') != string::npos) {
                tipos[col] = "float";
                break;
            }
            for (char c : val) {
                if (!isdigit(c)) {
                    tipos[col] = "string";
                    break;
                }
            }
        }
    }

    // Formar contenido del sector 0
    string sector0 = nombre_tabla + "#";
    for (size_t i = 0; i < columnas.size(); ++i) {
        sector0 += columnas[i] + "#" + tipos[i] + "#";
    }
    vector<char> contenido0(sector0.begin(), sector0.end());
    int direccion0[4] = {0, 0, 0, 0};
    escribirSector(contenido0, direccion0);

    // Convertir registros a string planos
    string buffer;
    int p = 0, sup = 0, pista = 0, sector = 1; // desde sector 1
    for (const auto& fila : registros) {
        string linea;
        for (const string& valor : fila) linea += valor + "#";
        linea += "\n";

        if (buffer.size() + linea.size() > capacidad_sector) {
            // escribir sector
            vector<char> datos(buffer.begin(), buffer.end());
            int direccion[4] = {p, sup, pista, sector};
            escribirSector(datos, direccion);

            // siguiente sector
            sector++;
            if (sector >= sectores) {
                sector = 0; pista++;
                if (pista >= pistas) {
                    pista = 0; sup++;
                    if (sup >= 2) {
                        sup = 0; p++;
                        if (p >= platos) {
                            cout << "No hay más espacio en el disco.\n";
                            return;
                        }
                    }
                }
            }
            buffer = "";
        }
        buffer += linea;
    }

    // escribir último fragmento
    if (!buffer.empty()) {
        vector<char> datos(buffer.begin(), buffer.end());
        int direccion[4] = {p, sup, pista, sector};
        escribirSector(datos, direccion);
    }

    cout << "Archivo CSV cargado correctamente en el disco.\n";
}
/*void Disco_duro::guardar_en_indice(const string& nombre_tabla, int bloque_id, int* direccion) {
    string ruta = "Disco/indice_" + nombre_tabla + ".txt";
    ofstream archivo(ruta, ios::app);
    if (archivo.is_open()) {
        archivo << "Bloque" << bloque_id << " -> ";
        archivo << "Plato" << direccion[0] << "#Superficie" << direccion[1]
                << "#Pista" << direccion[2] << "#Sector" << direccion[3] << "\n";
        archivo.close();
    }
}
*/
#endif
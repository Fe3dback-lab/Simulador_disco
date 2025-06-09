#include <iostream>
#include "Disco.h"
using namespace std;

int main() {
    Disco_duro disco;
    int opcion;
    bool ejecutar = true;

    cout << "=== Bienvenido al sistema de gestion de discos ===\n";
    cout << "1) Crear nuevo disco\n";
    cout << "2) Continuar con disco existente\n";
    cout << "Seleccione una opcion: ";
    cin >> opcion;

    if (opcion == 1) {
        disco.configurar_disco();
        disco.crear_disco();
    } else if (opcion == 2) {
        disco.recuperar_datos_disco();
    } else {
        cout << "Opcion no valida.\n";
        return 1;
    }

    while (ejecutar) {
        cout << "\n======= Menu de opciones =======\n";
        cout << "1) Mostrar arbol del disco\n";
        cout << "2) Mostrar informacion del disco\n";
        cout << "3) Cargar archivo CSV al disco\n";
        cout << "4) Borrar todo el disco\n";
        cout << "5) Salir del sistema\n";
        //cout << "6) Mostrar datos de una tabla\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                disco.mostrar_arbol("Disco");
                break;

            case 2:
                disco.mostrar_informacion();
                break;

            case 3: {
                string ruta_csv, nombre_tabla;
                cout << "Ingrese la ruta del archivo CSV (ejemplo: datos.csv): ";
                cin >> ruta_csv;
                cout << "Ingrese el nombre de la tabla a crear: ";
                cin >> nombre_tabla;
                disco.cargar_csv(ruta_csv, nombre_tabla);
                break;
            }

            case 4:
                cout << "¿Está seguro de borrar el disco? Esta accion es irreversible. (s/n): ";
                char confirm;
                cin >> confirm;
                if (confirm == 's' || confirm == 'S') {
                    disco.borrar_disco();
                    ejecutar = false;
                } else {
                    cout << "Borrado cancelado.\n";
                }
                break;

            case 5:
                ejecutar = false;
                cout << "Saliendo del sistema...\n";
                break;
            
            /*case 6:{
                string nombre_tabla;
                cout << "Ingrese el nombre de la tabla a mostrar: ";
                cin >> nombre_tabla;
                disco.mostrar_tabla(nombre_tabla);
                break;
            }*/
            default:
                cout << "Opcion no valida. Intente de nuevo.\n";
        }
    }

    return 0;
}

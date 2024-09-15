#include <iostream>
using namespace std;

/*

### **Analista de transacciones**

El banco debe poder gestionar las transacciones que realizan los clientes. Cada transacción tiene un monto, una fecha y un id único por cada transacción.

En el sistema debo poder:

1. Listar las transacciones de un cliente ordenadas por fecha con paginas de a 5 transacciones mostrando id, fecha y monto. (LISTO)
2. Listar la cantidad de ingresos y egresos por mes de un cliente. (POR MES, NO SOLO POR MES VIGENTE). (LISTO)
3. Mostrar el username del cliente, la fecha y monto de la transacción de monto máximo de todos los clientes. (LISTO PERO HAY QUE PREGUNTAR PARA CLARIFICAR)
4. Mostrar el cliente que más ingresos tuvo en los últimos 30 días. (LISTO)

*/

struct Mes {
    int ingresos;
    int egresos;
};

struct TransaccionesEnMes {
    Mes enero;
    Mes febrero;
    Mes marzo;
    Mes abril;
    Mes mayo;
    Mes junio;
    Mes julio;
    Mes agosto;
    Mes septiembre;
    Mes octubre;
    Mes noviembre;
    Mes diciembre;
};

struct ClienteIngresos {
    char username[20];
    int ingresos;
};

struct Transaccion{
    int id;
    int monto;
    int fecha; // * formato AAAAMMDD
    bool esEgreso;
    char username[20];
};

void inicializar_transacciones();
void leer_transacciones();
void imprimir_transaccion(Transaccion t);
void listar_transacciones_archivos();
void intercambiar_transacciones(FILE* archivo, int i, int j);
int contar_transacciones(FILE* archivo);
void ordenar_transacciones_por_fecha();
void ingresos_egresos_cliente();
void transaccion_mas_alta();
void cantidad_ultimos_30_dias();
bool fue_ultimos_30_dias(int fecha);
int obtener_mes_fecha(int fecha);

int main() {
    bool continuar = true;
    
    while(continuar) {
        int accion;
        cout << endl << endl;
        cout << "----- TP AyED 2024 -----" << endl;
        cout << "Que accion desea realizar?" << endl << endl;
        cout << "*[0]* Crear Transacciones de Prueba" << endl;
        cout << "*[1]* Ver todas las transacciones " << endl;
        cout << "*[2]* Listar Transacciones de un usuario " << endl;
        cout << "*[3]* Ver ingresos y egresos de un cliente " << endl;
        cout << "*[4]* Ver transaccion mas alta " << endl;
        cout << "*[5]* Ver clientes con mas ingresos en los ulitmos 30 dias " << endl;
        cout << "*[Otro]* Salir " << endl;
        cout << "-----";
        cout << endl << endl;

        cin >> accion;
        switch(accion) {
            case 0:
                inicializar_transacciones();
                ordenar_transacciones_por_fecha();
                break;
            case 1: 
                leer_transacciones();
                break;
            case 2: 
                listar_transacciones_archivos();
                break;
            case 3: 
                ingresos_egresos_cliente();
                break;
            case 4: 
                transaccion_mas_alta();
                break;
            case 5: 
                cantidad_ultimos_30_dias();
                break;
            default: 
                cout << "Saliendo del sistema. Gracias" << endl;
                continuar = false;
                break;
        }
    }

    return 0;
}

void cantidad_ultimos_30_dias() {
    FILE* archivo_transacciones = fopen("./data/transacciones.bin", "rb");
    FILE* archivo_ingresos = fopen("./data/ingresos_clientes.bin", "wb+");

    if (archivo_transacciones == NULL || archivo_ingresos == NULL) {
        cout << "Error abriendo los archivos." << endl;
        return;
    }

    Transaccion transaccion;
    ClienteIngresos clienteIngreso;
    ClienteIngresos clienteMaxIngreso = {"", -1};

    while (fread(&transaccion, sizeof(Transaccion), 1, archivo_transacciones) == 1) {
        if (!transaccion.esEgreso && fue_ultimos_30_dias(transaccion.fecha)) {
            bool cliente_encontrado = false;
            fseek(archivo_ingresos, 0, SEEK_SET);
            while (fread(&clienteIngreso, sizeof(ClienteIngresos), 1, archivo_ingresos) == 1) {
                if (strcmp(clienteIngreso.username, transaccion.username) == 0) {
                    cliente_encontrado = true;
                    clienteIngreso.ingresos += transaccion.monto;

                    fseek(archivo_ingresos, -sizeof(ClienteIngresos), SEEK_CUR);
                    fwrite(&clienteIngreso, sizeof(ClienteIngresos), 1, archivo_ingresos);
                    break;
                }
            }

            if (!cliente_encontrado) {
                strcpy(clienteIngreso.username, transaccion.username);
                clienteIngreso.ingresos = transaccion.monto;
                fwrite(&clienteIngreso, sizeof(ClienteIngresos), 1, archivo_ingresos);
            }
        }
    }

    fseek(archivo_ingresos, 0, SEEK_SET);
    while (fread(&clienteIngreso, sizeof(ClienteIngresos), 1, archivo_ingresos) == 1) {
        if (clienteIngreso.ingresos > clienteMaxIngreso.ingresos) {
            clienteMaxIngreso = clienteIngreso;
        }
    }

    if(clienteMaxIngreso.ingresos == -1) {
        cout << "No se ha podido encontrar informacion del usuario." << endl;
    } else {
        cout << "Cliente con más ingresos en los últimos 30 días: " << clienteMaxIngreso.username << endl;
        cout << "Ingresos totales: " << clienteMaxIngreso.ingresos << endl;
    }

    fclose(archivo_transacciones);
    fclose(archivo_ingresos);
}

void transaccion_mas_alta() {
    FILE* archivo_transacciones = fopen("./data/transacciones.bin", "rb");
    
    if (archivo_transacciones == NULL) {
        cout << "Error al abrir el archivo de transacciones." << endl;
        return;
    }

    Transaccion transaccion;
    Transaccion transaccionMasAlta;
    transaccionMasAlta.id = -1;

    while(fread(&transaccion, sizeof(Transaccion), 1, archivo_transacciones) == 1) {
        if(transaccionMasAlta.id == -1) {
            transaccionMasAlta = transaccion;
        }

        if(transaccion.monto > transaccionMasAlta.monto) {
            transaccionMasAlta = transaccion;
        }
    }

    fclose(archivo_transacciones);

    imprimir_transaccion(transaccionMasAlta);
}

void mostrar_transacciones_en_mes(TransaccionesEnMes transaccionesMes) {
    cout << "\nIngresos y egresos por mes:\n";
    cout << "Enero: " << transaccionesMes.enero.ingresos << " ingresos, " << transaccionesMes.enero.egresos << " egresos\n";
    cout << "Febrero: " << transaccionesMes.febrero.ingresos << " ingresos, " << transaccionesMes.febrero.egresos << " egresos\n";
    cout << "Marzo: " << transaccionesMes.marzo.ingresos << " ingresos, " << transaccionesMes.marzo.egresos << " egresos\n";
    cout << "Abril: " << transaccionesMes.abril.ingresos << " ingresos, " << transaccionesMes.abril.egresos << " egresos\n";
    cout << "Mayo: " << transaccionesMes.mayo.ingresos << " ingresos, " << transaccionesMes.mayo.egresos << " egresos\n";
    cout << "Junio: " << transaccionesMes.junio.ingresos << " ingresos, " << transaccionesMes.junio.egresos << " egresos\n";
    cout << "Julio: " << transaccionesMes.julio.ingresos << " ingresos, " << transaccionesMes.julio.egresos << " egresos\n";
    cout << "Agosto: " << transaccionesMes.agosto.ingresos << " ingresos, " << transaccionesMes.agosto.egresos << " egresos\n";
    cout << "Septiembre: " << transaccionesMes.septiembre.ingresos << " ingresos, " << transaccionesMes.septiembre.egresos << " egresos\n";
    cout << "Octubre: " << transaccionesMes.octubre.ingresos << " ingresos, " << transaccionesMes.octubre.egresos << " egresos\n";
    cout << "Noviembre: " << transaccionesMes.noviembre.ingresos << " ingresos, " << transaccionesMes.noviembre.egresos << " egresos\n";
    cout << "Diciembre: " << transaccionesMes.diciembre.ingresos << " ingresos, " << transaccionesMes.diciembre.egresos << " egresos\n";
}

void ingresos_egresos_cliente() {
    FILE* archivo_transacciones = fopen("./data/transacciones.bin", "rb");
    if (archivo_transacciones == NULL) {
        cout << "Error al abrir el archivo de transacciones." << endl;
        return;
    }

    TransaccionesEnMes resumenTransacciones = {};
    Transaccion transaccion;
    bool clienteExiste = false;
    cout << "De que cliente queres obtener informacion?" << endl;
    char clienteBuscado[20] = "";
    cin >> clienteBuscado;

    while (fread(&transaccion, sizeof(Transaccion), 1, archivo_transacciones) == 1) {
        if (strcmp(transaccion.username, clienteBuscado) == 0) {
            clienteExiste = true;
            int mes = obtener_mes_fecha(transaccion.fecha);

            if (mes >= 1 && mes <= 12) {
                switch (mes) {
                    case 1: transaccion.esEgreso ? resumenTransacciones.enero.egresos += transaccion.monto : resumenTransacciones.enero.ingresos += transaccion.monto; break;
                    case 2: transaccion.esEgreso ? resumenTransacciones.febrero.egresos += transaccion.monto : resumenTransacciones.febrero.ingresos += transaccion.monto; break;
                    case 3: transaccion.esEgreso ? resumenTransacciones.marzo.egresos += transaccion.monto : resumenTransacciones.marzo.ingresos += transaccion.monto; break;
                    case 4: transaccion.esEgreso ? resumenTransacciones.abril.egresos += transaccion.monto : resumenTransacciones.abril.ingresos += transaccion.monto; break;
                    case 5: transaccion.esEgreso ? resumenTransacciones.mayo.egresos += transaccion.monto : resumenTransacciones.mayo.ingresos += transaccion.monto; break;
                    case 6: transaccion.esEgreso ? resumenTransacciones.junio.egresos += transaccion.monto : resumenTransacciones.junio.ingresos += transaccion.monto; break;
                    case 7: transaccion.esEgreso ? resumenTransacciones.julio.egresos += transaccion.monto : resumenTransacciones.julio.ingresos += transaccion.monto; break;
                    case 8: transaccion.esEgreso ? resumenTransacciones.agosto.egresos += transaccion.monto : resumenTransacciones.agosto.ingresos += transaccion.monto; break;
                    case 9: transaccion.esEgreso ? resumenTransacciones.septiembre.egresos += transaccion.monto : resumenTransacciones.septiembre.ingresos += transaccion.monto; break;
                    case 10: transaccion.esEgreso ? resumenTransacciones.octubre.egresos += transaccion.monto : resumenTransacciones.octubre.ingresos += transaccion.monto; break;
                    case 11: transaccion.esEgreso ? resumenTransacciones.noviembre.egresos += transaccion.monto : resumenTransacciones.noviembre.ingresos += transaccion.monto; break;
                    case 12: transaccion.esEgreso ? resumenTransacciones.diciembre.egresos += transaccion.monto : resumenTransacciones.diciembre.ingresos += transaccion.monto; break;
                }
            } else {
                cout << "Fecha no válida: " << transaccion.fecha << endl;
            }
        }
    }

    fclose(archivo_transacciones);

    if (!clienteExiste) {
        cout << "El cliente " << clienteBuscado << " no tiene transacciones en el archivo." << endl;
    } else {
        mostrar_transacciones_en_mes(resumenTransacciones);
    }
}

void imprimir_transaccion(Transaccion t) {
    cout << endl;
    cout << "---- Transaccion #" << t.id << " ------" << endl;
    cout << "Autor: " << t.username << endl;
    cout << "Monto: " << t.monto << endl;
    cout << "Fecha: " << t.fecha << endl;
    cout << "Es egreso (0 = False; 1 = True): " << t.esEgreso << endl;
    cout << "--------------";
    cout << endl;
    
    return;
}

void intercambiar_transacciones(FILE* archivo, int i, int j) {
    Transaccion transaccionI, transaccionJ;

    fseek(archivo, i * sizeof(Transaccion), SEEK_SET);
    fread(&transaccionI, sizeof(Transaccion), 1, archivo);

    fseek(archivo, j * sizeof(Transaccion), SEEK_SET);
    fread(&transaccionJ, sizeof(Transaccion), 1, archivo);

    fseek(archivo, i * sizeof(Transaccion), SEEK_SET);
    fwrite(&transaccionJ, sizeof(Transaccion), 1, archivo);

    fseek(archivo, j * sizeof(Transaccion), SEEK_SET);
    fwrite(&transaccionI, sizeof(Transaccion), 1, archivo);
}

int contar_transacciones(FILE* archivo) {
    fseek(archivo, 0, SEEK_END);
    int tamanoArchivo = ftell(archivo);
    return tamanoArchivo / sizeof(Transaccion);
}

void ordenar_transacciones_por_fecha() {
    FILE* archivo = fopen("./data/transacciones.bin", "r+b");
    if (archivo == NULL) {
        cout << "Error al abrir el archivo de transacciones." << endl;
        return;
    }

    int totalTransacciones = contar_transacciones(archivo);

    for (int i = 0; i < totalTransacciones - 1; ++i) {
        for (int j = 0; j < totalTransacciones - i - 1; ++j) {
            Transaccion transaccion1, transaccion2;

            fseek(archivo, j * sizeof(Transaccion), SEEK_SET);
            fread(&transaccion1, sizeof(Transaccion), 1, archivo);

            fseek(archivo, (j + 1) * sizeof(Transaccion), SEEK_SET);
            fread(&transaccion2, sizeof(Transaccion), 1, archivo);

            if (transaccion1.fecha > transaccion2.fecha) {
                intercambiar_transacciones(archivo, j, j + 1);
            }
        }
    }

    fclose(archivo);
}

void listar_transacciones_archivos() {
    FILE* archivo_transacciones = fopen("./data/transacciones.bin", "rb");

    if (archivo_transacciones == NULL) {
        cout << "Algo salió mal con el puntero del archivo_transacciones" << endl;
        return;
    }

    cout << "¿Qué nombre buscas?" << endl;
    char nombreBuscado[20] = "";
    cin >> nombreBuscado;

    Transaccion transaccion;
    int pagina = 0;
    int cant = 0;
    bool hayTransacciones = false;

    cout << endl << "--- Página " << pagina + 1 << " ---" << endl;

    while (fread(&transaccion, sizeof(Transaccion), 1, archivo_transacciones) == 1) {
        if (strcmp(transaccion.username, nombreBuscado) == 0) {
            hayTransacciones = true;
            cout << "ID: " << transaccion.id 
                 << ", Fecha: " << transaccion.fecha 
                 << ", Monto: " << transaccion.monto 
                 << endl;

            cant++;

            if (cant == 5) {
                cout << endl << "Presiona Enter para ver más transacciones..." << endl;
                cin.ignore(); 
                cin.get();

                cant = 0;
                pagina++;
                cout << endl << "--- Página " << pagina + 1 << " ---" << endl;
            }
        }
    }

    if (!hayTransacciones) {
        cout << "No se encontraron transacciones para el usuario: " << nombreBuscado << endl;
    }

    fclose(archivo_transacciones);
}

void leer_transacciones() {
    FILE* archivo_transacciones = fopen("./data/transacciones.bin", "rb");

    if(archivo_transacciones != NULL) {
        Transaccion transaccion;
        while(fread(&transaccion, sizeof(Transaccion), 1, archivo_transacciones) == 1) {
            imprimir_transaccion(transaccion);
        }
    } else {
        cout << "Algo salio mal con el puntero del archivo_transacciones" << endl;
    }

    fclose(archivo_transacciones);

    return;
}

void inicializar_transacciones() {
    Transaccion transacciones[14] = {
        {4, 1, 20240717, true, "Carlos"},
        {2, 400, 20240520, false, "Pepito"},
        {3, 30, 20240610, false, "Carlos"},
        {5, 2, 20240818, false, "Esteban"},
        {10, 18, 20210330, false, "Pepito"},
        {9, 12, 20221230, true, "Pepito"},
        {6, 300, 20240911, false, "Esteban"},
        {8, 9, 20231130, false, "Manolo"},
        {1, 20, 20240431, true, "Pepito"},
        {7, 8000, 20201020, false, "Manolo"},
        {11, 48, 20140531, true, "Pepito"},
        {12, 11, 20040431, false, "Pepito"},
        {13, 12, 20030431, false, "Pepito"},
        {14, 3048, 20140431, true, "Pepito"},
    };

    FILE* archivo_transacciones = fopen("./data/transacciones.bin", "wb");

    if(archivo_transacciones != NULL) {
        fwrite(transacciones, sizeof(Transaccion), 14, archivo_transacciones);
    } else {
        cout << "Algo salio mal con el puntero del archivo_transacciones" << endl;
    }

    fclose(archivo_transacciones);

    return;
}

int obtener_mes_fecha(int fecha) {
    return (fecha / 100) % 100;
}

bool fue_ultimos_30_dias(int fecha) {
    int fecha_actual = 20240915;
    
    int anio_actual = fecha_actual / 10000;
    int mes_actual = (fecha_actual / 100) % 100;
    int dia_actual = fecha_actual % 100;

    int anio_fecha = fecha / 10000;
    int mes_fecha = (fecha / 100) % 100;
    int dia_fecha = fecha % 100;

    int dias_actual = anio_actual * 365 + mes_actual * 30 + dia_actual;
    int dias_fecha = anio_fecha * 365 + mes_fecha * 30 + dia_fecha;

    int diferencia_dias = dias_actual - dias_fecha;

    return diferencia_dias <= 30 && diferencia_dias >= 0;
}
#include <iostream>
using namespace std;

/*

### **Analista de transacciones**

El banco debe poder gestionar las transacciones que realizan los clientes. Cada transacción tiene un monto, una fecha y un id único por cada transacción.

En el sistema debo poder:

1. Listar las transacciones de un cliente ordenadas por fecha con paginas de a 5 transacciones mostrando id, fecha y monto. (LISTO)
2. Listar la cantidad de ingresos y egresos por mes de un cliente. (POR MES, NO SOLO POR MES VIGENTE). (LISTO)
3. Mostrar el username del cliente, la fecha y monto de la transacción de monto máximo de todos los clientes. (LISTO PERO HAY QUE PREGUNTAR PARA CLARIFICAR)
4. Mostrar el cliente que más ingresos tuvo en los últimos 30 días.

*/

const int MAXIMO_TRANSACCIONES_PERMITIDAS = 3;

struct ContabilizarIngresosCliente {
    string username;
    float cantidad;
};

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

struct Transaccion{
    int id;
    int monto;
    int fecha; // * formato AAAAMMDD
    bool esEgreso;
    char username[20];
};

struct UserConTransacciones {
    string username;
    Transaccion ts[MAXIMO_TRANSACCIONES_PERMITIDAS];
};

// ? refactor

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

// ! old methods:

void ciclar_transaccion(UserConTransacciones us, Transaccion t);
void guardar_user_con_transacciones(UserConTransacciones us);
void leer_user_con_transacciones(UserConTransacciones us[], int& len);
void ordenar_transacciones(UserConTransacciones us);
void listar_transacciones(UserConTransacciones& us);
UserConTransacciones encontrar_cliente_por_username(string username, UserConTransacciones us[], int len);
Transaccion dame_transaccion_mas_cara(Transaccion t[], int len);
int obtener_mes_fecha(int fecha);
TransaccionesEnMes listar_ingresos_egresos_cliente(UserConTransacciones us);
void listar_transacciones_mas_altas_clientes(UserConTransacciones us[], int len);
bool fue_ultimos_30_dias(int fecha);
ContabilizarIngresosCliente mostrar_cliente_mas_ingresos(UserConTransacciones us[], int len);

int main() {
    cout << "Queres generar data nueva de prueba?" << endl;
    cout << "0 = NO | 1 = SI" << endl;
    int respuesta;
    cin >> respuesta;

    if(respuesta == 1) {
        inicializar_transacciones();
        ordenar_transacciones_por_fecha();
    }

    cout << "Queres leer las transacciones?" << endl;
    cout << "0 = NO | 1 = SI" << endl;
    cin >> respuesta;
    if(respuesta == 1) {
        leer_transacciones();
    }

    cout << "Que queres hacer ahora?" << endl;
    cout << "1: Listar Transacciones " << endl;
    cout << "2: Ver ingresos y egresos de un cliente " << endl;
    cout << "3: Ver transaccion mas alta " << endl;
    cout << "4: Ver clientes con mas ingresos en los ulitmos 30 dias " << endl;
    cin >> respuesta;

    switch(respuesta) {
        case 1: listar_transacciones_archivos();
        case 2: ingresos_egresos_cliente();
        case 3: transaccion_mas_alta();
        case 4: cantidad_ultimos_30_dias();
    }

    return 0;
}

void cantidad_ultimos_30_dias() {
    FILE* archivo_transacciones = fopen("./data/transacciones.bin", "rb");
    Transaccion transaccion;

    if (archivo_transacciones == NULL) {
        cout << "Error al abrir el archivo de transacciones." << endl;
        return;
    }

    while(fread(&transaccion, sizeof(Transaccion), 1, archivo_transacciones) == 1) {
        // TODO: Manejar logica
        cout << transaccion.id;
    }

    fclose(archivo_transacciones);
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
        {6, 300, 20240919, true, "Esteban"},
        {8, 9, 20231130, false, "Manolo"},
        {1, 20, 20240431, true, "Pepito"},
        {7, 8000, 20281020, false, "Manolo"},
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

//! -- viejo

int obtener_mes_fecha(int fecha) {
    return (fecha / 100) % 100;
}

TransaccionesEnMes listar_ingresos_egresos_cliente(UserConTransacciones us) {
    // vamos a suponer que solo hay transacciones anuales (o sea, no hay una en enero 2023 y otra en enero 2024)
    TransaccionesEnMes transacciones;
    int transacciones_len = sizeof(us.ts) / sizeof(us.ts[0]);

    for(int i = 0; i < transacciones_len; i++) {
        switch(obtener_mes_fecha(us.ts[i].fecha)) {
            case 1: {
                if(us.ts[i].esEgreso) {
                    transacciones.enero.egresos += us.ts[i].monto;
                } else {
                    transacciones.enero.ingresos += us.ts[i].monto;
                }
                break;
            }
            case 2: {
                if(us.ts[i].esEgreso) {
                    transacciones.febrero.egresos += us.ts[i].monto;
                } else {
                    transacciones.febrero.ingresos += us.ts[i].monto;
                }
                break;
            }
            case 3: {
                if(us.ts[i].esEgreso) {
                    transacciones.marzo.egresos += us.ts[i].monto;
                } else {
                    transacciones.marzo.ingresos += us.ts[i].monto;
                }
                break;
            }
            case 4: {
                if(us.ts[i].esEgreso) {
                    transacciones.abril.egresos += us.ts[i].monto;
                } else {
                    transacciones.abril.ingresos += us.ts[i].monto;
                }
                break;
            }
            case 5: {
                if(us.ts[i].esEgreso) {
                    transacciones.mayo.egresos += us.ts[i].monto;
                } else {
                    transacciones.mayo.ingresos += us.ts[i].monto;
                }
                break;
            }
            case 6: {
                if(us.ts[i].esEgreso) {
                    transacciones.junio.egresos += us.ts[i].monto;
                } else {
                    transacciones.junio.ingresos += us.ts[i].monto;
                }
                break;
            }
            case 7: {
                if(us.ts[i].esEgreso) {
                    transacciones.julio.egresos += us.ts[i].monto;
                } else {
                    transacciones.julio.ingresos += us.ts[i].monto;
                }
                break;
            }
            case 8: {
                if(us.ts[i].esEgreso) {
                    transacciones.agosto.egresos += us.ts[i].monto;
                } else {
                    transacciones.agosto.ingresos += us.ts[i].monto;
                }
                break;
            }
            case 9: {
                if(us.ts[i].esEgreso) {
                    transacciones.septiembre.egresos += us.ts[i].monto;
                } else {
                    transacciones.septiembre.ingresos += us.ts[i].monto;
                }
                break;
            }
            case 10: {
                if(us.ts[i].esEgreso) {
                    transacciones.octubre.egresos += us.ts[i].monto;
                } else {
                    transacciones.octubre.ingresos += us.ts[i].monto;
                }
                break;
            }
            case 11: {
                if(us.ts[i].esEgreso) {
                    transacciones.noviembre.egresos += us.ts[i].monto;
                } else {
                    transacciones.noviembre.ingresos += us.ts[i].monto;
                }
                break;
            }
            case 12: {
                if(us.ts[i].esEgreso) {
                    transacciones.diciembre.egresos += us.ts[i].monto;
                } else {
                    transacciones.diciembre.ingresos += us.ts[i].monto;
                }
                break;
            }
        }
    }
    
    return transacciones;
}

void ciclar_transaccion(UserConTransacciones us, Transaccion t) {
    int transacciones_len = sizeof(us.ts) / sizeof(us.ts[0]);

    for(int i = 1; i < transacciones_len; i++) {
        us.ts[i - 1] = us.ts[i];
    }

    us.ts[transacciones_len - 1] = t;

    guardar_user_con_transacciones(us);
}

void leer_user_con_transacciones(UserConTransacciones us[], int& len) {
    FILE* archivo_transacciones = fopen("./generador-transacciones/transacciones.bin", "rb");
    UserConTransacciones transacciones[MAXIMO_TRANSACCIONES_PERMITIDAS];

    fread(&transacciones, sizeof(UserConTransacciones), MAXIMO_TRANSACCIONES_PERMITIDAS, archivo_transacciones);

    fclose(archivo_transacciones);

    for(int i = 0; i < len; i ++) {
        us[i] = transacciones[i];

        int transaccionesLen = sizeof(us[i].ts) / sizeof(us[i].ts[0]);

        cout << "Username: " << us[i].username << endl;
        cout << "Transacciones: " << endl;
        for(int j = 0; j < transaccionesLen; j++) {
            cout << "Id de Transaccion: " << us[i].ts[j].id << endl;
            cout << "Fecha de Transaccion: " << us[i].ts[j].fecha << endl;
            cout << "Monto de Transaccion: " << us[i].ts[j].monto << endl;
            cout << "Es egreso la Transaccion: " << us[i].ts[j].esEgreso << endl;
        }
        cout << "-----------------" << endl;
    }
}

void guardar_user_con_transacciones(UserConTransacciones us) {
    FILE* archivo_transacciones = fopen("./generador-transacciones/transacciones.bin", "ab");

    fwrite(&us, sizeof(UserConTransacciones), 1, archivo_transacciones);

    fclose(archivo_transacciones);
}

bool compararFecha(const Transaccion& a, const Transaccion& b) {
    return a.fecha > b.fecha;
}

void ordenar_transacciones(UserConTransacciones us) {
    sort(begin(us.ts), end(us.ts), compararFecha);
}

void listar_transacciones(UserConTransacciones& us) {
    ordenar_transacciones(us);
    int pagina = 1;
    int transacciones_len = sizeof(us.ts) / sizeof(us.ts[0]);
    int transacciones_por_pagina = 5;

    for (int i = 0; i < transacciones_len; i += transacciones_por_pagina) {
        if(i == 0) {
            cout << "-----------------------" << endl;
        }
        cout << "ESTA ES LA PAGINA: " << pagina << endl << endl;

        for (int j = i; j < i + transacciones_por_pagina && j < transacciones_len; ++j) {
            cout << "Id de Transaccion: " << us.ts[j].id << endl;
            cout << "Fecha de Transaccion: " << us.ts[j].fecha << endl;
            cout << "Monto de Transaccion: " << us.ts[j].monto << endl;
            cout << endl;
        }

        pagina++;
        cout << "-----------------------" << endl;
    }
}

void listar_transacciones_mas_altas_clientes(UserConTransacciones us[], int len) {
    for(int i = 0; i < len; ++i) {
        cout << "EL USERNAME: " << us[i].username << endl;
        int transferenciaLen = sizeof(us[i].ts) / sizeof(us[i].ts[0]);

        Transaccion mas_cara = dame_transaccion_mas_cara(us[i].ts, transferenciaLen);

        cout << "La mas cara de " << us[i].username << " " <<  "es de: " << mas_cara.monto << " y tiene fecha de: " << mas_cara.fecha << endl;
    }
}

Transaccion dame_transaccion_mas_cara(Transaccion t[], int len) {
    Transaccion mas_cara = t[0];
    
    for(int i = 1; i < len; i++)  {
        if(t[i].monto > mas_cara.monto) {
            mas_cara = t[i];
        }
    }

    return mas_cara;
}

bool fue_ultimos_30_dias(int fecha) {
    int fecha_actual = 20240812;
    
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

UserConTransacciones encontrar_cliente_por_username(string username, UserConTransacciones us[], int len) {
    int i = 0;
    
    while(i < len && us[i].username != username) {
        i++;
    }

    if(i == len) {
        cout << "Usuario no encontrado" << endl;
    }

    return us[i];
}

ContabilizarIngresosCliente mostrar_cliente_mas_ingresos(UserConTransacciones us[], int len) {
    ContabilizarIngresosCliente ingresos_clientes[len];

    for(int i = 0; i < len; i++) {
        ingresos_clientes[i].username = us[i].username;
        int transferenciaLen = sizeof(us[i].ts) / sizeof(us[i].ts[0]);
        for(int j = 0; j < 2; j++) {
            cout << "FUE ULTIMOS 30 DIAS? " << us[i].username << us[i].ts[j].id << fue_ultimos_30_dias(us[i].ts[j].fecha);
            if(fue_ultimos_30_dias(us[i].ts[j].fecha) && !us[i].ts[j].esEgreso) {
                ingresos_clientes[i].cantidad += us[i].ts[j].monto;
            }
        }
    }

    int ingresos_clientes_len = sizeof(ingresos_clientes) / sizeof(ingresos_clientes[0]);

    ContabilizarIngresosCliente cliente_con_mas_ingresos = ingresos_clientes[0];

    for(int i = 0; i < ingresos_clientes_len; i++) {
        if(ingresos_clientes[i].cantidad > cliente_con_mas_ingresos.cantidad) {
            cliente_con_mas_ingresos = ingresos_clientes[i];
        }
    }

    return cliente_con_mas_ingresos;
}
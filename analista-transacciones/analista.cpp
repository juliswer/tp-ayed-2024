#include <iostream>
using namespace std;

/*

### **Analista de transacciones**

El banco debe poder gestionar las transacciones que realizan los clientes. Cada transacción tiene un monto, una fecha y un id único por cada transacción.

En el sistema debo poder:

1. Listar las transacciones de un cliente ordenadas por fecha con paginas de a 5 transacciones mostrando id, fecha y monto.
2. Listar la cantidad de ingresos y egresos por mes de un cliente. (POR MES, NO SOLO POR MES VIGENTE)
3. Mostrar el username del cliente, la fecha y monto de la transacción de monto máximo de todos los clientes.
4. Mostrar el cliente que más ingresos tuvo en los últimos 30 días.

*/

// TODO: Agregar manejo de transacciones llenas
const int MAXIMO_TRANSACCIONES_PERMITIDAS = 20;

struct Mes {
    int cantidad;
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
};

struct UserConTransacciones {
    string username;
    Transaccion ts[MAXIMO_TRANSACCIONES_PERMITIDAS];
};

void guardar_user_con_transacciones(UserConTransacciones us); // ! JULIAN - listo
void leer_user_con_transacciones(UserConTransacciones us[], int& len); // ! JULIAN - listo
void ordenar_transacciones(UserConTransacciones us); // ! JULIAN -- listo
void listar_transacciones(UserConTransacciones& us); // ! JULIAN
TransaccionesEnMes listar_ingresos_egresos_cliente(UserConTransacciones us); // * JULIAN Y THIAGO
void listar_transacciones_mas_altas_clientes(UserConTransacciones us[], int len); // ? THIAGO
bool fue_ultimos_30_dias(int fecha); // ? THIAGO
UserConTransacciones mostrar_clientes_mas_ingresos(UserConTransacciones us[], int len); // ? THIAGO

int main() {
    cout << "Hello world -- Analista" << endl;

    UserConTransacciones us;
    us.ts[0] = {1, 10, 20240726, true};
    us.ts[1] = {2, 20, 20240725, false};
    us.ts[2] = {3, 30, 20240724, true};
    us.ts[3] = {4, 40, 20240723, false};
    us.ts[4] = {5, 50, 20240722, true};
    us.ts[5] = {6, 60, 20240721, false};
    us.ts[6] = {7, 70, 20240720, true};
    us.ts[7] = {8, 80, 20240719, false};
    us.ts[8] = {9, 90, 20240718, true};
    us.ts[9] = {10, 100, 20240717, false};
    us.ts[10] = {11, 110, 20240716, true};
    us.ts[11] = {12, 120, 20240715, false};
    us.ts[12] = {13, 130, 20240714, true};
    us.ts[13] = {14, 140, 20240713, false};
    us.ts[14] = {15, 150, 20240712, true};
    us.ts[15] = {16, 160, 20240711, false};
    us.ts[16] = {17, 170, 20240710, true};
    us.ts[17] = {18, 180, 20240709, false};
    us.ts[18] = {19, 190, 20240708, true};
    us.ts[19] = {20, 200, 20240707, false};
    us.ts[20] = {21, 210, 20240706, true};

    listar_transacciones(us);

    return 0;
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
    cout << "LISTANDO";
    ordenar_transacciones(us);
    int pagina = 1;
    int transacciones_len = sizeof(us.ts) / sizeof(us.ts[0]);
    int transacciones_por_pagina = 5;

    for (int i = 0; i < transacciones_len; i += transacciones_por_pagina) {
        cout << "ESTA ES LA PAGINA: " << pagina << endl;

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
#include <iostream>
using namespace std;

/*

### **Analista de transacciones**

El banco debe poder gestionar las transacciones que realizan los clientes. Cada transacción tiene un monto, una fecha y un id único por cada transacción.

En el sistema debo poder:

1. Listar las transacciones de un cliente ordenadas por fecha con paginas de a 5 transacciones mostrando id, fecha y monto. [LISTO]
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

void ciclar_transaccion(UserConTransacciones us, Transaccion t);
void guardar_user_con_transacciones(UserConTransacciones us); // ! JULIAN - listo
void leer_user_con_transacciones(UserConTransacciones us[], int& len); // ! JULIAN - listo
void ordenar_transacciones(UserConTransacciones us); // ! JULIAN -- listo
void listar_transacciones(UserConTransacciones& us); // ! JULIAN -- listo
TransaccionesEnMes listar_ingresos_egresos_cliente(UserConTransacciones us); // * JULIAN Y THIAGO
void listar_transacciones_mas_altas_clientes(UserConTransacciones us[], int len); // ? THIAGO
bool fue_ultimos_30_dias(int fecha); // ? THIAGO
UserConTransacciones mostrar_clientes_mas_ingresos(UserConTransacciones us[], int len); // ? THIAGO

int main() {
    UserConTransacciones us;
    us.ts[0].id = 1;
    us.ts[0].monto = 10;
    us.ts[0].fecha = 20240726;
    us.ts[0].esEgreso = true;

    us.ts[1].id = 2;
    us.ts[1].monto = 20;
    us.ts[1].fecha = 20240725;
    us.ts[1].esEgreso = false;

    us.ts[2].id = 3;
    us.ts[2].monto = 30;
    us.ts[2].fecha = 20240724;
    us.ts[2].esEgreso = true;

    us.ts[3].id = 4;
    us.ts[3].monto = 40;
    us.ts[3].fecha = 20240723;
    us.ts[3].esEgreso = false;

    us.ts[4].id = 5;
    us.ts[4].monto = 50;
    us.ts[4].fecha = 20240722;
    us.ts[4].esEgreso = true;

    us.ts[5].id = 6;
    us.ts[5].monto = 60;
    us.ts[5].fecha = 20240721;
    us.ts[5].esEgreso = false;

    us.ts[6].id = 7;
    us.ts[6].monto = 70;
    us.ts[6].fecha = 20240720;
    us.ts[6].esEgreso = true;

    us.ts[7].id = 8;
    us.ts[7].monto = 80;
    us.ts[7].fecha = 20240719;
    us.ts[7].esEgreso = false;

    us.ts[8].id = 9;
    us.ts[8].monto = 90;
    us.ts[8].fecha = 20240718;
    us.ts[8].esEgreso = true;

    us.ts[9].id = 10;
    us.ts[9].monto = 100;
    us.ts[9].fecha = 20240717;
    us.ts[9].esEgreso = false;

    us.ts[10].id = 11;
    us.ts[10].monto = 110;
    us.ts[10].fecha = 20240716;
    us.ts[10].esEgreso = true;

    us.ts[11].id = 12;
    us.ts[11].monto = 120;
    us.ts[11].fecha = 20240715;
    us.ts[11].esEgreso = false;

    us.ts[12].id = 13;
    us.ts[12].monto = 130;
    us.ts[12].fecha = 20240714;
    us.ts[12].esEgreso = true;

    us.ts[13].id = 14;
    us.ts[13].monto = 140;
    us.ts[13].fecha = 20240713;
    us.ts[13].esEgreso = false;

    us.ts[14].id = 15;
    us.ts[14].monto = 150;
    us.ts[14].fecha = 20240712;
    us.ts[14].esEgreso = true;

    us.ts[15].id = 16;
    us.ts[15].monto = 160;
    us.ts[15].fecha = 20240711;
    us.ts[15].esEgreso = false;

    us.ts[16].id = 17;
    us.ts[16].monto = 170;
    us.ts[16].fecha = 20240710;
    us.ts[16].esEgreso = true;

    us.ts[17].id = 18;
    us.ts[17].monto = 180;
    us.ts[17].fecha = 20240709;
    us.ts[17].esEgreso = false;

    // us.ts[18].id = 19;
    // us.ts[18].monto = 190;
    // us.ts[18].fecha = 20240708;
    // us.ts[18].esEgreso = true;

    // us.ts[19].id = 20;
    // us.ts[19].monto = 200;
    // us.ts[19].fecha = 20240707;
    // us.ts[19].esEgreso = false;

    listar_transacciones(us);

    return 0;
}

TransaccionesEnMes listar_ingresos_egresos_cliente(UserConTransacciones us) {
    
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
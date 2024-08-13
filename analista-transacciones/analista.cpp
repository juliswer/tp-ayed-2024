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

struct ContabilizarIngresosCliente {
    string username;
    float cantidad;
};

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

void ciclar_transaccion(UserConTransacciones us, Transaccion t); // ! listo
void guardar_user_con_transacciones(UserConTransacciones us); // ! JULIAN - listo
void leer_user_con_transacciones(UserConTransacciones us[], int& len); // ! JULIAN - listo
void ordenar_transacciones(UserConTransacciones us); // ! JULIAN -- listo
void listar_transacciones(UserConTransacciones& us); // ! JULIAN -- listo
Transaccion dame_transaccion_mas_cara(Transaccion t[], int len); // * Julian Y THIAGO -- listo
TransaccionesEnMes listar_ingresos_egresos_cliente(UserConTransacciones us); // * JULIAN Y THIAGO
void listar_transacciones_mas_altas_clientes(UserConTransacciones us[], int len); // ? THIAGO -- listo
bool fue_ultimos_30_dias(int fecha); // ? THIAGO -- listo
UserConTransacciones mostrar_clientes_mas_ingresos(UserConTransacciones us[], int len); // ? THIAGO

int main() {
    UserConTransacciones us;
    us.username = "Mengano";
    us.ts[0].id = 1;
    us.ts[0].monto = 10;
    us.ts[0].fecha = 20240726;
    us.ts[0].esEgreso = true;

    us.ts[1].id = 2;
    us.ts[1].monto = 20;
    us.ts[1].fecha = 20240725;
    us.ts[1].esEgreso = false;

    us.ts[2].id = 3;
    us.ts[2].monto = 9000;
    us.ts[2].fecha = 20240724;
    us.ts[2].esEgreso = true;

    UserConTransacciones us2;
    us2.username = "Pepito";
    us2.ts[0].id = 1;
    us2.ts[0].monto = 500000;
    us2.ts[0].fecha = 20240726;
    us2.ts[0].esEgreso = true;

    us2.ts[1].id = 2;
    us2.ts[1].monto = 20;
    us2.ts[1].fecha = 20240725;
    us2.ts[1].esEgreso = false;

    us2.ts[2].id = 3;
    us2.ts[2].monto = 30;
    us2.ts[2].fecha = 20240724;
    us2.ts[2].esEgreso = true;

    UserConTransacciones usuarios[2];
    usuarios[0] = us;
    usuarios[1] = us2;

    listar_transacciones_mas_altas_clientes(usuarios, 2);

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

UserConTransacciones mostrar_clientes_mas_ingresos(UserConTransacciones us[], int len) {
    ContabilizarIngresosCliente ingresos_clientes[len];

    for(int i = 0; i < len; i++) {
        ingresos_clientes[i].username = us[i].username;
        int transferenciaLen = sizeof(us[i].ts) / sizeof(us[i].ts[0]);
        for(int j = 0; j < transferenciaLen; j++) {
            if(fue_ultimos_30_dias(us[i].ts[j].fecha) && !us[i].ts[j].esEgreso) {
                ingresos_clientes[i].cantidad += us[i].ts[j].monto;
            }
        }
    }

    // obtener el usuario que tenga el ingreso mas alto;
}
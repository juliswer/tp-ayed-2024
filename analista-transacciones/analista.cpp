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
UserConTransacciones encontrar_cliente_por_username(string username, UserConTransacciones us[], int len); // ! JULIAN -- listo
Transaccion dame_transaccion_mas_cara(Transaccion t[], int len); // * Julian Y THIAGO -- listo
int obtener_mes_fecha(int fecha); // ! JULIAN -- listo
TransaccionesEnMes listar_ingresos_egresos_cliente(UserConTransacciones us); // * JULIAN Y THIAGO -- listo
void listar_transacciones_mas_altas_clientes(UserConTransacciones us[], int len); // ? THIAGO -- listo
bool fue_ultimos_30_dias(int fecha); // ? THIAGO -- listo
ContabilizarIngresosCliente mostrar_cliente_mas_ingresos(UserConTransacciones us[], int len); // ? THIAGO -- listo

int main() {
    UserConTransacciones us;
    us.username = "Mengano";
    us.ts[0].id = 1;
    us.ts[0].monto = 10;
    us.ts[0].fecha = 20240726;
    us.ts[0].esEgreso = true;

    us.ts[1].id = 2;
    us.ts[1].monto = 40;
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
    us2.ts[1].monto = 200;
    us2.ts[1].fecha = 20240725;
    us2.ts[1].esEgreso = false;

    us2.ts[2].id = 3;
    us2.ts[2].monto = 30;
    us2.ts[2].fecha = 20240724;
    us2.ts[2].esEgreso = true;

    UserConTransacciones usuarios[2];
    usuarios[0] = us;
    usuarios[1] = us2;

    TransaccionesEnMes transacciones = listar_ingresos_egresos_cliente(usuarios[0]);

    cout << "Julio: " << transacciones.julio.egresos << endl;

    return 0;
}

int obtener_mes_fecha(int fecha) {
    int mmdd = fecha % 10000;
    int mes = mmdd / 100;

    return mes;
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
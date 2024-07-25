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
const int MAXIMO_TRANSACCIONES_PERMITIDAS = 500;

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

void guardar_user_con_transacciones(); // ! JULIAN
void leer_user_con_transacciones(UserConTransacciones us[], int& len); // ! JULIAN
void listar_transacciones(UserConTransacciones us[]); // ! JULIAN
TransaccionesEnMes listar_ingresos_egresos_cliente(UserConTransacciones us); // * JULIAN Y THIAGO
void listar_transacciones_mas_altas_clientes(UserConTransacciones us[], int len); // ? THIAGO
bool fue_ultimos_30_dias(int fecha); // ? THIAGO
UserConTransacciones mostrar_clientes_mas_ingresos(UserConTransacciones us[], int len); // ? THIAGO

int main() {
    cout << "Hello world -- Analista" << endl;

    return 0;
}
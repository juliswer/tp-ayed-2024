#include <iostream>
using namespace std;


struct Transaccion{
    int id;
    int monto;
    int fecha;
    bool esEgreso;
}

//hacer funcion que duplica el tamaño array

struct Usuario{
    string nombre;
    string username;
    string password;
    int dni;
    float saldo;    
    Transaccion transacciones[];
}

bool validar_usuario(string username, string password);

Usuario obtener_usuario(string username);

bool eliminar_transaccion(string username, int id);

void actualizar_saldo(string username);//deberia hacer que la sumatoria de todos los montos sea positiva

bool sumatoria_tansacciones(Usuario usuario);

void realizar_transaccion(string username, int monto, bool esEgreso, int fecha);

Transaccion crear_transaccion(int monto, int fecha, bool esEgreso);

void agregar_transaccion_archivo(string username,Transaccion transaccion); //insertar ordenado por fecha???????

bool verificar_saldo(string username,int monto);



/*
funcion duplicarArray(Usuario usuarios[], int n){
    Usuario nuevoArray[n*2];
    for(int i=0; i<n; i++){
        nuevoArray[i] = usuarios[i];
    }
    return nuevoArray;
}*/

int main() {
  cout << "Hello World!";
  return 0;
}
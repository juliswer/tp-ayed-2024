#include <iostream>
using namespace std;


struct Transaccion{
    int id;
    float monto;
    int fecha;
    bool esEgreso;
};

//hacer funcion que duplica el tamaño array

struct Usuario{
    string nombre;
    string username;
    string password;
    int dni;
    float saldo;
};


int main()
{
    FILE* file = fopen("usuarios.bin", "wb");


    Usuario usuario = {
        "a",
        "a",
        "a",
        11,
        123,
    };
    fwrite(&usuario, sizeof(usuario), 1, file);
    fclose(file);
    return 0;
}
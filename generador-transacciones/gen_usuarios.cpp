#include <iostream>
using namespace std;


struct Transaccion{
    int dni;
    int id;
    float monto;
    int fecha;
    bool esEgreso;
};


struct Usuario {
    char nombre[20];
    char username[20];
    char password[20];
    int dni;
    float saldo;    
};


int main()
{
    FILE* file = fopen("./data/usuarios.bin", "wb");

    Usuario user = 
    {
        "abc",
        "dca",
        "pass",
        10,
        23.00f
    };
    
    Usuario user2 = 
    {
        "a",
        "a",
        "a",
        10,
        21.00f,
    };

    fwrite(&user, sizeof(Usuario), 1, file);
    fwrite(&user2, sizeof(Usuario),1, file);
    fclose(file);
    return 0;
}
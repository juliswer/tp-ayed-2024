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
        "b",
        "b",
        "b",
        1,
        1000.00f
    };
    
    Usuario user2 = 
    {
        "a",
        "a",
        "a",
        2,
        10.00f,
    };

    fwrite(&user, sizeof(Usuario), 1, file);
    fwrite(&user2, sizeof(Usuario),1, file);
    fclose(file);
    return 0;
}
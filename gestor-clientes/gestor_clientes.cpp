#include <iostream>
#include <cstring>
using namespace std;

struct Cliente
{
    char Nombre[20] = "";
    char Dni[8] = "";
    char Username[20] = "";
    char Clave[20] = "";
    float Saldo;
};

bool cadenasIguales(char string1[], char string2[], int len)
{
    for (int i = 0; i < len; i++)
    {
        if (string1[i] != string2[i])
        {
            return false;
        }
    }
    return true;
}

bool clienteExiste(FILE *archivo, Cliente unCliente)
{
    fseek(archivo, 0, SEEK_SET);
    Cliente clienteLeido;
    fread(&clienteLeido, sizeof(Cliente), 1, archivo);
    while (!feof(archivo))
    {
        if (cadenasIguales(clienteLeido.Dni, unCliente.Dni, 8))
        {
            return true;
        }
        if (cadenasIguales(clienteLeido.Username, unCliente.Username, 20))
        {
            return true;
        }
        fread(&clienteLeido, sizeof(Cliente), 1, archivo);
    }
    return false;
}
Cliente pedirCliente()
{
    Cliente clienteNuevo;

    cout << "ingrese su nombre" << endl;
    cin >> clienteNuevo.Nombre;
    cout << "ingrese su Dni" << endl;
    cin >> clienteNuevo.Dni;
    cout << "ingrese su Username" << endl;
    cin >> clienteNuevo.Username;
    cout << "ingrese su Clave" << endl;
    cin >> clienteNuevo.Clave;
    clienteNuevo.Saldo = 10000.0;

    return clienteNuevo;
}

void registrarCliente(FILE *archivo, Cliente clienteNuevo)
{
    //fseek(archivo, sizeof(Cliente), SEEK_END);
    fwrite(&clienteNuevo, sizeof(Cliente), 1, archivo);
}

int main()
{
    FILE *archivo = fopen("usuarios.bin", "ab+");
    if (archivo == NULL)
    {
        cout << "No se pudo abrir el archivo" << endl;
        return 1;
    }

    int opcion;
    Cliente clienteNuevo;

    do
    {
        cout << "1. Registrar Cliente" << endl;
        cout << "2. Salir" << endl;
        cout << "Seleccione una opcion" << endl;
        if (!(cin >> opcion))
        {
            cout << "La opcion que ingresaste fue invalida. Por favor ingrese una opcion valida." << endl;
            cin.clear();              // sale del estado de error del cin.
            cin.ignore(100000, '\n'); // elimina lo que se escribio en el cin.
        }
        switch (opcion)
        {
        case 1:
            clienteNuevo = pedirCliente();
            if (clienteExiste(archivo, clienteNuevo))
            {
                cout << "El cliente ya existe, intente otra vez" << endl;
            }
            else
            {
                registrarCliente(archivo, clienteNuevo);
                cout << "se registró existosamente!" << endl;
            }
            break;
        case 2:
            cout << "Saliendo del sistema" << endl;
            break;

        default:
            cout << "opcion invalida" << endl;
            break;
        }
    } while (opcion != 2);

    fclose(archivo);

    return 0;
}
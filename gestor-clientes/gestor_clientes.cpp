#include <iostream>
using namespace std;

struct Cliente
{
    string Nombre;
    string Dni;
    string Username;
    string Clave;
    double Saldo;
};

const int clientesMax = 100;
Cliente clientes[clientesMax];
int numClientes = 0;

bool clienteExiste(string &Dni, string &Username)
{
    for (int i = 0; i < numClientes; i++)
    {
        if (clientes[i].Dni == Dni || clientes[i].Username == Username)
        {
            return true;
        }
    }
    return false;
}

void registrarCliente()
{
    Cliente clienteNuevo;

    cout << "ingrese su nombre" << endl;
    cin.ignore();
    getline(cin, clienteNuevo.Nombre);
    cout << "ingrese su Dni" << endl;
    getline(cin, clienteNuevo.Dni);
    cout << "ingrese su Username" << endl;
    getline(cin, clienteNuevo.Username);
    cout << "ingrese su Clave" << endl;
    getline(cin, clienteNuevo.Clave);
    clienteNuevo.Saldo = 10000.0;

    if (clienteExiste(clienteNuevo.Dni, clienteNuevo.Username))
    {
        cout << "Ya existe un cliente con este Dni y Username. Intente con otro." << endl;
    }

    clientes[numClientes++] = clienteNuevo;

    cout << "Se registro existosamente." << endl;

    return;
}

void guardarCliente()
{
    FILE *archivo = fopen("usuarios.bin", "wb");
    if (archivo == NULL)
    {
        cout << "No se pudo abrir el archivo" << endl;
        return;
    }

    fwrite(&numClientes, sizeof(int), 1, archivo);
    fwrite(clientes, sizeof(Cliente), numClientes, archivo);

    fclose(archivo);
    return;
}

void cargarClientes()
{
    FILE *archivo = fopen("usuarios.bin", "rb");
    if (archivo == NULL)
    {
        cout << "No se pudo abrir el archivo" << endl;
        return;
    }

    fread(&numClientes, sizeof(int), 1, archivo);
    fread(clientes, sizeof(Cliente), numClientes, archivo);

    fclose(archivo);
    return;
}

int main()
{
    cargarClientes();

    int opcion;

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
            registrarCliente();
            break;
        case 2:
            guardarCliente();
            cout << "Saliendo del sistema" << endl;
            break;

        default:
            cout << "opcion invalida" << endl;
            break;
        }
    } while (opcion != 2);

    return 0;
}
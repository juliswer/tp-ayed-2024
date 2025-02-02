#include <iostream>
#include <cstring>
using namespace std;


struct Transaccion 
{
    char username[20];
    int id;
    float monto;
    int fecha; // * formato AAAAMMDD
    bool esEgreso;
};


struct Cliente
{
    char nombre[20] = "";
    char DNI[8] = "";
    char username[20] = "";
    char clave[20] = "";
    float saldo;
};

bool validar_usuario(string username, string password, Cliente& cliente);

bool eliminar_transaccion(int id_transaccion);  

float actualizar_saldo(string username); 

int sumatoria_tansacciones(Cliente cliente);

void realizar_transaccion(string username);

void actualizar_archivo_transacciones();


// Archivos
Cliente obtener_usuario(string username);
void agregar_transaccion_archivo(Transaccion transaccion); //insertar ordenado por fecha???????
int seleccionar_transaccion(Cliente cliente);



int main() {
    Cliente cliente;
    string nombre_cliente;
    string passw; 
    char comando = 0;

    cout << "Hola!! Bienvenido a SuperBank" << endl;

    cout << "Cliente: "; 
    cin >> nombre_cliente;

    cout << "Contraseña: ";
    cin >> passw;

    cout << "Comprobando las credenciales..." << endl;
    
    if(!validar_usuario(nombre_cliente, passw, cliente))
    {
        cout << "El Cliente no se encontro o la contraseña es incorrecta!!!" << endl;
        return 1;
    }

    cout << "Su saldo es: " << cliente.saldo + sumatoria_tansacciones(cliente) << endl; 

    do {
        cout << "Que es lo siguiente que desea hacer:\n0.Salir\n1. Realizar Transaccion\n2.Eliminar Transaccion\n * \r" << endl;
        cin >> comando;

        switch (comando)
        {
            case '1':
                realizar_transaccion(cliente.username);
                actualizar_saldo(cliente.username);
                break;
            
            case '2':
                eliminar_transaccion(seleccionar_transaccion(cliente));
                break;
        
            default:
                break;
        }
    } while (comando != '0');


    
    return 0;
}

int seleccionar_transaccion(Cliente cliente)
{
    FILE* file = fopen("../data/transacciones.bin", "rb");
    Transaccion transaccion_actual;
    int id;

    while(fread(&transaccion_actual, sizeof(Transaccion), 1, file))
    {
        if (!strcmp(cliente.username, transaccion_actual.username))
            cout << "\nID:" << transaccion_actual.id << "\nMonto:" << transaccion_actual.monto << "\nFecha: " << transaccion_actual.fecha << "\nEsEgreso:" << transaccion_actual.esEgreso << "\n------------------" << endl;
    }

    fclose(file);
    cout << "ID de la transaccion a eliminar: ";
    cin >> id;
    return id;
}

bool validar_usuario(string username, string password, Cliente& cliente)
{
    cliente = obtener_usuario(username);
    
    if(cliente.clave != password)
        return false;

    return true;
}

// ::Archivos::

void agregar_transaccion_archivo(Transaccion transaccion)
{
    FILE* file = fopen("../data/transacciones.bin", "ab");

    if (file == 0)
    {
        cout << "Ha ocurrido un error grave a la hora de abrir el archivo \nsaliendo del programa..." << std::endl;
        exit(1); 
    }

    fwrite(&transaccion, sizeof(Transaccion), 1, file);
    fclose(file);
    return;
}

/*
obtener_usuario(string, int) -> Cliente

¿Esta el Cliente?
    SI -> devuelve el Cliente
    NO -> devuelve Cliente nulo (todo cero)
IMPORTANTE: Si no se puede abrir el archivo devuelve Cliente nulo con la variable errcode = 1
*/
Cliente obtener_usuario(string username)
{
    FILE* file = fopen("../data/usuarios.bin", "rb");
    Cliente user;

    if (file == 0)
    {
        cout << "Ha ocurrido un error grave a la hora de abrir el archivo \nsaliendo del programa..." << std::endl;
        exit(1);
    }
    
    while (fread(&user, sizeof(user), 1, file))
    {
        if(strcmp(user.username, username.c_str()) == 0)
        {
            fclose(file);
            return user;
        }
    }
    fclose(file);

    Cliente default_user;
    strcpy(default_user.DNI, "N/A");
    strcpy(default_user.nombre, "N/A");
    strcpy(default_user.clave, "N/A");
    strcpy(default_user.username, "N/A");
    default_user.saldo = 0.00;
    
    return default_user;
}

int obtener_transaccion_id()
{
    srand(static_cast<unsigned int>(time(nullptr)));

    int min = 1;  // Define the minimum value
    int max = 10000; // Define the maximum value

    // Generate a random integer between min and max (inclusive)
    int random_number = rand() % (max - min + 1) + min;

    // FILE* file = fopen("../data/transacciones.bin", "rb");
    // Transaccion transaccion;
    // int contador = 0;
    // if (file == 0)
    // {
    //     cout << "Ha ocurrido un error grave a la hora de abrir el archivo \nsaliendo del programa..." << std::endl;
    //     exit(1);
    // }

    // while (fread(&transaccion, sizeof(transaccion), 1, file))
    // {
    //     if(transaccion.id > contador )
    //         contador=transaccion.id;
    // }

    // fclose(file);
    return random_number;
}

bool eliminar_transaccion(int id_transaccion)
{
    FILE* file = fopen("../data/transacciones.bin", "rb");
    if (!file) {
        cout << "No se pudo abrir el archivo para lectura." << endl;
        return false;
    }

    FILE* archivoTemporal = fopen("../data/transacciones_temp.bin", "wb");
    if (!archivoTemporal) 
    {
        cout << "No se pudo abrir el archivo temporal para escritura." << endl;
        fclose(file);
        return false;
    }
    
    Transaccion transaccion;

    // Leer los datos del archivo y escribir los que no se eliminan en el archivo temporal
    while (fread(&transaccion, sizeof(Transaccion), 1, file)) 
    {
        if (transaccion.id != id_transaccion) 
        {
            fwrite(&transaccion, sizeof(Transaccion), 1, archivoTemporal);
        }
    }

    fclose(file);
    fclose(archivoTemporal);
    
    actualizar_archivo_transacciones();

    cout << "Se elimino la transaccion correctamente" << endl; 
    return true; 
}

void actualizar_archivo_transacciones()
{
    FILE* file = fopen("../data/transacciones.bin", "wb");
    if (!file) 
    {
        cout << "No se pudo abrir el archivo para lectura." << endl;
        return;
    }

    FILE* archivoTemporal = fopen("../data/transacciones_temp.bin", "rb");
    if (!archivoTemporal) 
    {
        cout << "No se pudo abrir el archivo temporal para escritura." << endl;
        fclose(file);
        return;
    }
    
    Transaccion transaccion;

    while (fread(&transaccion, sizeof(Transaccion), 1,archivoTemporal)) 
    {
        fwrite(&transaccion, sizeof(Transaccion), 1, file);  
    }

    fclose(file);
    fclose(archivoTemporal);

}

Transaccion crear_transaccion(string username,int monto, int fecha, bool esEgreso){
    
    Transaccion transaccion;
    strcpy(transaccion.username, username.c_str()); 
    transaccion.id = obtener_transaccion_id();
    transaccion.monto = monto;
    transaccion.fecha = fecha;
    transaccion.esEgreso = esEgreso;

    return transaccion; 

}


// 2 opciones 
// a) en el mismo de crear transaccion agrego la transaccion al archivo
// b) creo una funcion que reciba la transaccion creada y de ahi lo agregue


void realizar_transaccion(string username)
{
    int comandotemp;
    cout << "Que tipo de transaccion quiere realizar:\n0.Salir\n1. Realizar Ingreso\n2.Realizar Egreso\n * \r" << endl;
    cin >> comandotemp;

    if (comandotemp == 0)
        return;
    

    bool esEgresotemp = (comandotemp == 2);

    int montotemp;
    cout << "Ahora ingrese el monto de su transaccion:" << endl;
    cin >> montotemp;
    
    Cliente user = obtener_usuario(username);
    if (esEgresotemp && user.saldo + sumatoria_tansacciones(user) - montotemp <= 0) 
    {
        cout << "El monto que puso excede a su saldo" << endl;
        return;
    }
    
    int fechatemp;
    cout << "Ahora ingrese la fecha actual \n con el formato AAAAMMDD ejemplo 20210911" << endl;
    cin >> fechatemp;

    agregar_transaccion_archivo(crear_transaccion(user.username,montotemp, fechatemp, esEgresotemp));
}



/*
Actualizar saldo
    1. Obtener el Cliente
    2. Verificar si el saldo es suficiente
    3. Actualizar el saldo
    4. Guardar el Cliente
    5. Devolver el saldo actualizado
    sino es suficiente devolver -1
*/

float actualizar_saldo(string username){

    Cliente user = obtener_usuario(username);

    if(sumatoria_tansacciones(user) + user.saldo >=0)
    {

        user.saldo = sumatoria_tansacciones(user) + user.saldo;
        cout<<"Transaccion realizada con exito";
        return user.saldo;

    }

    else
    {
        cout<<"No se puede realizar esta accion, saldo insuficiente";
        return -1;
    }
        
}


int sumatoria_tansacciones(Cliente Cliente)
{
    int SaldoEstimado = 0;

    FILE* file = fopen("../data/transacciones.bin", "a+");
    Transaccion transaccion;
    
    if (file == 0)
    {
        cout << "Ha ocurrido un error grave a la hora de leer las transacciones \nsaliendo del programa..." << std::endl;
        exit(1);
    }

    while (fread(&transaccion, sizeof(transaccion), 1, file))
    {
       if(transaccion.esEgreso && !strcmp(transaccion.username, Cliente.username))
            SaldoEstimado -= transaccion.monto;
        
        if(transaccion.esEgreso==false && !strcmp(transaccion.username, Cliente.username))
            SaldoEstimado += transaccion.monto;
    }

    fclose(file);
    return SaldoEstimado;
}











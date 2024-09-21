#include <iostream>
using namespace std;


struct Transaccion 
{
    char username[20];
    int id;
    float monto;
    int fecha;
    bool esEgreso;
};


struct Usuario 
{
    char nombre[20];
    char username[20];
    char password[20];
    int dni; 
    float saldo;
};


bool validar_usuario(char username[20], char password[20], Usuario& usuario);

bool eliminar_transaccion(int id_transaccion);  

float actualizar_saldo(char username[20]); 

int sumatoria_tansacciones(Usuario usuario);

void realizar_transaccion(char username[20]);

void actualizar_archivo_transacciones();

// TODO: Guardar esta nueva TX en un archivo 

// Archivos
Usuario obtener_usuario(char username[20]);
void agregar_transaccion_archivo(Transaccion transaccion); //insertar ordenado por fecha???????
int seleccionar_transaccion(Usuario usuario);



int main() {
    Usuario usuario;
    char nombre_usuario[20]="";
    char passw[20]="";
    char comando = 0;

    cout << "Hola!! Bienvenido a SuperBank" << endl;

    cout << "Usuario: "; 
    cin >> nombre_usuario;

    cout << "Contraseña: ";
    cin >> passw;

    cout << "Comprobando las credenciales..." << endl;
    
    if(!validar_usuario(nombre_usuario, passw, usuario))
    {
        cout << "El usuario no se encontro o la contraseña es incorrecta!!!" << endl;
        return 1;
    }

    cout << "Su saldo es: " << usuario.saldo + sumatoria_tansacciones(usuario) << endl; 

    do {
        cout << "Que es lo siguiente que desea hacer:\n0.Salir\n1. Realizar Transaccion\n2.Eliminar Transaccion\n * \r" << endl;
        cin >> comando;

        switch (comando)
        {
            case '1':
                realizar_transaccion(usuario.username);
                actualizar_saldo(usuario.username);
                break;
            
            case '2':
                eliminar_transaccion(seleccionar_transaccion(usuario));
                break;
        
            default:
                break;
        }
    } while (comando != '0');


    
    return 0;
}

int seleccionar_transaccion(Usuario usuario)
{
    FILE* file = fopen("./data/transacciones.bin", "rb");
    Transaccion transaccion_actual;
    int id;

    while(fread(&transaccion_actual, sizeof(Transaccion), 1, file))
    {
        if (usuario.username == transaccion_actual.username)
            cout << "\nID:" << transaccion_actual.id << "\nMonto:" << transaccion_actual.monto << "\nFecha" << transaccion_actual.fecha << "\nEsEgreso:" << transaccion_actual.esEgreso << "\n------------------" << endl;
    }

    fclose(file);
    cout << "ID de la transaccion a eliminar: ";
    cin >> id;
    return id;
}

bool validar_usuario(char username[20], char password[20], Usuario& usuario)
{
    usuario = obtener_usuario(username);
    
    if(usuario.password != password)
        return false;

    return true;
}

// ::Archivos::

void agregar_transaccion_archivo(Transaccion transaccion)
{
    FILE* file = fopen("./data/transacciones.bin", "ab");

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
obtener_usuario(string, int) -> Usuario

¿Esta el usuario?
    SI -> devuelve el usuario
    NO -> devuelve usuario nulo (todo cero)
IMPORTANTE: Si no se puede abrir el archivo devuelve usuario nulo con la variable errcode = 1
*/
Usuario obtener_usuario(char username[20])
{
    FILE* file = fopen("./data/usuarios.bin", "rb");
    Usuario user;
    

    if (file == 0)
    {
        cout << "Ha ocurrido un error grave a la hora de abrir el archivo \nsaliendo del programa..." << std::endl;
        exit(1);
    }

    while (fread(&user, sizeof(user), 1, file))
    {
        if(user.username == username)
        {
            fclose(file);
            return user;
        }
    }
    fclose(file);

    return {"N/A", "N/A", "N/A", 0, 0.0}; 
}




int obtener_transaccion_id()
{
    FILE* file = fopen("./data/transacciones.bin", "rb");
    Transaccion transaccion;
    int contador = 0;
    if (file == 0)
    {
        cout << "Ha ocurrido un error grave a la hora de abrir el archivo \nsaliendo del programa..." << std::endl;
        exit(1);
    }

    while (fread(&transaccion, sizeof(transaccion), 1, file))
    {
        if(transaccion.id > contador )
            contador=transaccion.id;
    }

    fclose(file);
    return contador++;
}

bool eliminar_transaccion(int id_transaccion)
{
    FILE* file = fopen("./data/transacciones.bin", "rb");
    if (!file) {
        cout << "No se pudo abrir el archivo para lectura." << endl;
        return false;
    }

    FILE* archivoTemporal = fopen("transacciones_temp.bin", "wb");
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
    FILE* file = fopen("./data/transacciones.bin", "wb");
    if (!file) 
    {
        cout << "No se pudo abrir el archivo para lectura." << endl;
        return;
    }

    FILE* archivoTemporal = fopen("transacciones_temp.bin", "rb");
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




Transaccion crear_transaccion(char username[20],int monto, int fecha, bool esEgreso){
    
    Transaccion transaccion;
    transaccion.username = username;
    transaccion.id = obtener_transaccion_id();
    transaccion.monto = monto;
    transaccion.fecha = fecha;
    transaccion.esEgreso = esEgreso;
    //op a
/*
    FILE* file = fopen("transacciones.bin", "ab+");
    if (file == 0)
    {
        cout << "Ha ocurrido un error grave a la hora de cargar la transferencia al sistema \nsaliendo del programa..." << std::endl;
        exit(1);
    }

    fwrite(&transaccion, sizeof(Transaccion), 1, file);
     
    fclose(file);
    
    cout << "Transaccion agregada correctamente.\n" << std::endl;



*/

    return transaccion; 

}


// 2 opciones 
// a) en el mismo de crear transaccion agrego la transaccion al archivo
// b) creo una funcion que reciba la transaccion creada y de ahi lo agregue


void realizar_transaccion(char username[20])
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
    
    Usuario user = obtener_usuario(username);
    if (esEgresotemp && user.saldo + sumatoria_tansacciones(user) - montotemp <= 0) 
    {
        cout << "El monto que puso excede a su saldo" << endl;
        return;
    }
    
    int fechatemp;
    cout << "Ahora ingrese la fecha actual \n con el formato DDMMAAAA ejemplo 11092001" << endl;
    cin >> fechatemp;

    agregar_transaccion_archivo(crear_transaccion(user.username,montotemp, fechatemp, esEgresotemp));
}



/*
Actualizar saldo
    1. Obtener el usuario
    2. Verificar si el saldo es suficiente
    3. Actualizar el saldo
    4. Guardar el usuario
    5. Devolver el saldo actualizado
    sino es suficiente devolver -1
*/

float actualizar_saldo(char username[20]){

        Usuario user = obtener_usuario(username);

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
        
}//deberia hacer que la sumatoria de todos los montos sea positiva


//cambiar como tomar las transacciones 


int sumatoria_tansacciones(Usuario usuario)
{
    int SaldoEstimado = 0;

    FILE* file = fopen("./data/transacciones.bin", "rb");
    Transaccion transaccion;
    
    if (file == 0)
    {
        cout << "Ha ocurrido un error grave a la hora de leer las transacciones \nsaliendo del programa..." << std::endl;
        exit(1);
    }

    while (fread(&transaccion, sizeof(transaccion), 1, file))
    {
       if(transaccion.esEgreso && transaccion.username == usuario.username)
            SaldoEstimado -= transaccion.monto;
        
        if(transaccion.esEgreso==false && transaccion.username == usuario.username)
            SaldoEstimado += transaccion.monto;
    }

    fclose(file);
    return SaldoEstimado;
}











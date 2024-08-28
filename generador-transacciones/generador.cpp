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


bool validar_usuario(string nombre_usuario, string contraseña, Usuario& usuario);

bool eliminar_transaccion(string username, int id);//no esta 

float actualizar_saldo(string username);//deberia hacer que la sumatoria de todos los montos sea positiva

int sumatoria_tansacciones(Usuario usuario);

void realizar_transaccion(string username);

Transaccion crear_transaccion(int monto, int fecha, bool esEgreso);//falta que asigne el id 

bool verificar_saldo(string username);



// Archivos
Usuario obtener_usuario(string username);
void agregar_transaccion_archivo(string username,Transaccion transaccion); //insertar ordenado por fecha???????

/*
// funcion que duplica el tamaño array
array duplicarArray(Usuario usuarios[], int len){
    Usuario nuevoArray[len*2];
    for(int i=0; i<n; i++){
        nuevoArray[i] = usuarios[i];
    }
    return nuevoArray;
}*/

int main() {
    Usuario usuario;
    string nombre_usuario;
    string contraseña;
    char comando = 0;

    cout << "Hola!! Bienvenido a SuperBank" << endl;

    cout << "Usuario: "; 
    cin >> nombre_usuario;

    cout << "Contraseña: ";
    cin >> contraseña;

    cout << "Comprobando las credenciales..." << endl;
    
    if(!validar_usuario(nombre_usuario, contraseña, usuario))
    {
        cout << "El usuario no se encontro o la contraseña es incorrecta!!!" << endl;
        return 1;
    }

    cout << "Su saldo es: " << usuario.saldo << endl; 

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

                break;
        
            default:

                break;
        } 
    }while (comando != '0');


    
    return 0;
}

bool validar_usuario(string nombre_usuario, string contraseña, Usuario& usuario)
{
    usuario = obtener_usuario(nombre_usuario);
    
    if(usuario.password != contraseña)
        return false;

    return true;
}

// ::Archivos::

/*
obtener_usuario(string, int) -> Usuario

¿Esta el usuario?
    SI -> devuelve el usuario
    NO -> devuelve usuario nulo (todo cero)
IMPORTANTE: Si no se puede abrir el archivo devuelve usuario nulo con la variable errcode = 1
*/
Usuario obtener_usuario(string username)
{
    FILE* file = fopen("usuarios.bin", "rb");
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

    return {"", "", "", 0, 0.0f}; 
}

// ::Funciones::
//va tener que estar relacionado con como agarramos las transacciones
int obtener_ultimo_id(string username){
    Usuario user= obtener_usuario(username);
    //no c como seguir :( 
    return 0;

}

Transaccion crear_transaccion(int monto, int fecha, bool esEgreso){
    
    Transaccion transaccion;
    
    //falta la asignacion del id a la transaccion
    transaccion.monto = monto;
    transaccion.fecha = fecha;
    transaccion.esEgreso = esEgreso;
    return transaccion; 

}
void realizar_transaccion(string username){

    int comandotemp;
    cout << "Que tipo de transaccion quiere realizar:\n0.Salir\n1. Realizar Ingreso\n2.Realizar Egreso\n * \r" << endl;
    cin >> comandotemp;

    if (comandotemp == 0) {
        return;
    }

    bool esEgresotemp = (comandotemp == 2);

    int montotemp;
    cout << "Ahora ingrese el monto de su transaccion:" << endl;
    cin >> montotemp;
    
    Usuario user = obtener_usuario(username);
    if (esEgresotemp && sumatoria_tansacciones(user) - montotemp < 0) {
        cout << "El monto que puso excede a su saldo" << endl;
        return;
    }

    int fechatemp;
    cout << "Ahora ingrese la fecha actual \n con el formato DDMMAAAA ejemplo 11092001" << endl;
    cin >> fechatemp;

    crear_transaccion(montotemp, fechatemp, esEgresotemp);
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

float actualizar_saldo(string username){

        Usuario user = obtener_usuario(username);

        if(verificar_saldo(username)){

            user.saldo = sumatoria_tansacciones(user) + user.saldo;
            cout<<"Transaccion realizada con exito";
            return user.saldo;

        }

        else{

            cout<<"No se puede realizar esta accion, saldo insuficiente";
            return -1;

        }
        
}//deberia hacer que la sumatoria de todos los montos sea positiva


//cambiar como tomar las transacciones 

int sumatoria_tansacciones(Usuario usuario){

    int SaldoEstimado=0;
    int ArrLEN = sizeof(usuario.transacciones)/sizeof(usuario.transacciones[0]);

    for (int i = 0; i < ArrLEN; i++)
    {
        if(usuario.transacciones[i].esEgreso)
            SaldoEstimado-=usuario.transacciones[i].monto;
        
        if(usuario.transacciones[i].esEgreso==false)
            SaldoEstimado+=usuario.transacciones[i].monto;

    }

    return SaldoEstimado;
}


bool verificar_saldo(string username){

    Usuario user = obtener_usuario(username);

        if(sumatoria_tansacciones(user) + user.saldo<0){

            return false;

        }

        if(sumatoria_tansacciones(user) + user.saldo >=0){

            return true;

        }
    
    else return false;

}

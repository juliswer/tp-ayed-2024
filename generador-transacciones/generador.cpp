#include <iostream>
using namespace std;

const int MAXIMO_TRANSACCIONES_PERMITIDAS = 20;
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
    Transaccion transacciones[MAXIMO_TRANSACCIONES_PERMITIDAS];
};

bool validar_usuario(string nombre_usuario, string contraseña, Usuario& usuario);

bool eliminar_transaccion(string username, int id);

float actualizar_saldo(string username);//deberia hacer que la sumatoria de todos los montos sea positiva

int sumatoria_tansacciones(Usuario usuario);

void realizar_transaccion(string username, int monto, bool esEgreso, int fecha);

Transaccion crear_transaccion(int monto, int fecha, bool esEgreso);

bool verificar_saldo(string username);



// Archivos
Usuario obtener_usuario(string username);
void agregar_transaccion_archivo(string username,Transaccion transaccion); //insertar ordenado por fecha???????

/*
array duplicarArray(Usuario usuarios[], int n){
    Usuario nuevoArray[n*2];
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

    {
        cout << "Que es lo siguiente que desea hacer:\n0.Salir\n1. Realizar Transaccion\n2.Eliminar Transaccion\n * \r" << endl;
        cin >> comando; 
    } while (comando != '0');
    
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
    
    return {"","","",0,0,0}; 
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
/*
float actualizar_saldo(string username){
        Usuario user = obtener_usuario(username.0);
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
*/
/*
bool verificar_saldo(string username){
    Usuario user = obtener_usuario(username,0);
        if(sumatoria_tansacciones(user) + user.saldo<0){
            return false;
        }
        if(sumatoria_tansacciones(user) + user.saldo >=0){
            return true;
        }
}

int sumatoria_tansacciones(Usuario usuario){
    int SaldoEstimado=0;
     int ArrLEN = sizeof(user.transacciones)/sizeof(user.transacciones[0]);
        for (int i = 0; i < ArrLEN; i++)
        {
            if(user.transacciones[i].esEgreso){
                SaldoEstimado-=user.transacciones[i].monto;
            }
            if(user.transacciones[i].esEgreso==false){
                SaldoEstimado+=user.transacciones[i].monto;
            }
        }
        return SaldoTotal;
}
*/
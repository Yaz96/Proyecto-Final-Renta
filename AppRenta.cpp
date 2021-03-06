#include <iostream>
#include <string>
#include <fstream>

using namespace std;

#include "Reserva.h"
#include "Aparato.h"
#include "Cancha.h"

//Declaracion de arrgelos principales.
Servicio *ASrv[20];
Reserva Res[50];
//Variable para leer y sobreescribir los datos
ifstream iArc;
ofstream archReservas;
//Varibales de apoyo.
string S, D;
int TM, C, M, H, iCont, iCont2;
double Co;
char TS, CI;
bool I, help;
Hora Ho;
Reserva NuevaRrv;

void Terminar()
{
    //_______Escritura de la reservacion_____ 
    archReservas.open("Reserva.txt");
    
    for (int i = 0; i <iCont2; i++)
    {
        Ho = Res[i].getHoraIni();
        archReservas << Res[i].getcveServicio() <<" "<< Ho.getHora() <<" "<< Ho.getMinu() <<" "<< Res[i].getDuracion() <<" "<< Res[i].getidCliente() <<endl;
    }
    archReservas.close();
}
//--------------------------------------------Hacer una reservacion-----------------------------------------------
void HacerRes()//Opcion 5
{
    I = false;//Ayuda.
    iCont = 0;//Ayuda.

    //Pide por el ID, del cliente.
    cout << "Escribe el id del cliente" << endl;
    cin >> C;
    NuevaRrv.setidCliente(C);//Sobreeescribimos el id en la variable que meteremos en el arreglo de reserva.

    //--------------------------Ciclo para obtener la clave de servicio.-------------------------------
    do
    {
        //Pide la clave del servicio.
        cout << "Escribe la clave del servicio" << endl;
        cin >> S;

        for (int i =  0; i < 20; i++)//Recorre todo el arreglo de servicio.
        {
            if (S == ASrv[i] -> getClave())//Verifica que exista o no la clave de servicio escrita por el usuario.
            {
                I = true;//Si encuentra el servicio, en el arrgleo termina el ciclo.
                iCont = i;//Guardamos la posicion del servico del arreglo de servicio en la variable de ayuda.
                NuevaRrv.setcveServicio(S);//Guardamos la clave en la variable que meteremos en el arreglo de reserva.
            }   
        }

        if (I == false)//Si no encuentra la clave dentro del arreglo de servicio.
            cout << "Clave inexistente, intentelo de nuevo" << endl;//despliega un mensaje de error.

    }while(I == false);//Hara el loop mientras la clave no coincida con las claves de servicio.

    //---------------------------------Ciclo para obtener el tiempo a reservar-------------------------
    I = false;//Regresamos la ayuda a false
    do
    {   
        //Pide el tiempo que se quiera reservar en minutos.
        cout << "Escribe los minutos a reservar" << endl;
        cin >> M;

        //Usamos la variable iCont donde guardamos la posicion del servicio deseado en el arreglo.
        if (M <= ASrv[iCont] -> getTiempoMax())//Comparamos el tiempo con el tiempo maximo del servicio escogido.
        {
            I = true;//Terminamos el ciclo;
            NuevaRrv.setDuracion(M);//Guardamos la duracion en la variable que meteremos en el arreglo de reserva.
        }
        else
        {
            cout << "No se pudo hacer la reservacion debido a que exede el tiempo maximo" << '\n'
            << "Te gustaria reservarlo por menos tiempo?(S) o cancelar la reservacion?(C)" << endl;
            cin >> CI;
            //Si el usuario quiere cancelar la reservacion, regresa al menu.
            if (CI == 'C' || CI == 'c')
                return;
        }

    }while(I == false);//Hara el ciclo hasta que el usario escriba la reservacion correcta.

    //-----------------------------------Este parte sirve para verficar si la hora esta disponible.------------
    I = false;//Regresamos la ayuda a false
    help = true;

    do
    {
        //Pide la hora exacta para reservarlo.
        cout << "Escribe la hora a reservar" << endl;
        cin >> Ho;

        if (Ho.getHora() < 0 || Ho.getHora() > 23 || Ho.getMinu() < 0 || Ho.getMinu() > 59)//Comprueba que la hora sea valida.
        {
            help = false;//si la hora no es valida pone false.
            cout << "Hora incorrecta, intentelo de nuevo" << endl;//Mensaje de error.
        }
    }while(help == false);//Hara el ciclo hasta qu escriba una hora valida.

    help = true;//Ayuda en true.
    //Recorremos el arreglo de reserva hasta iCont2
    //que tiene guardado la posicion siguiente de la ultima ocupada en el arreglo.
    for (int i = 0; i < iCont2; i++)
    {
        if (S == Res[i].getcveServicio())//Para verificar si el servico ya esta reservado o no.
        {
            //Checa si la hora incio escrita por el usario esta en el rango de las horas de la reservaciones
            if (Ho >= Res[i].getHoraIni() && Ho <= Res[i].calculaHorafinReservacion()) 
                help = false;//Si se empalman las horas cambia la help a false

            //Ho + M es la hora final del servicio escrito por el usuario
            //y checa que no se empalme con las horas del servicio 
            if ((Ho + M) >= Res[i].getHoraIni() && (Ho + M) <= Res[i].calculaHorafinReservacion())
                help = false;//Si se empalman las horas cambia la help a false
        }
    }
    if (help)//Si no se empalman las horas.
    {
        NuevaRrv.setHoraIni(Ho);//Guardamos la hora de incio en el arreglo de ayuda.
        cout << "Se realizo la reserva con exito" << endl;
        //En iCont2 guardamos una posicion despues de la ultima poscion ocupada en el arreglo.
        Res[iCont2] = NuevaRrv;//Guardamos la reservacion en el arreglo.
    }
    else
    {
        //Cancelamos la reservacion.
        cout << "La reservacion no podra efectuarse" << endl;
        cout << endl;
        return;
    }

    //------------------------------------Desplegar costo-----------------------------------------
    //Usamos el arreglo de servicio con la posicion del servicio gurdada en iCont anteriormente
    //y llamamos la funcion de calcular costo con la variable M donde se guardo la respuesta del usuario
    //para la duracion del servicio.
    cout << "Costo de reserva: " << ASrv[iCont] -> calculaCosto(NuevaRrv.getDuracion()) << endl;
    cout << endl;//Para que se vea ordenado.
    
    iCont2++;//Por si el usuario quiere hacer otra reservacion, le dejamos la posicion lista.
}
//--------------------------------------Consulta reservaciones de una hora especifica.------------------------
void ConsReservHoraSp(){//Opcion 4
    cout << "Ingresa la hora que desea checar:" << endl;
    cin>>Ho;//Usando la sobreecarga de operadores.

    cout << "Se desplegara todos los servicios en uso durante esas horas" << endl;
    cout << endl;

    for(int iA=0; iA<iCont2; iA++){ //En iCont2 se guardo la posicion siguiente de la ultima posicon ocupada en el arreglo de reserva.
        if(Ho >= Res[iA].getHoraIni() && Ho <= Res[iA].calculaHorafinReservacion()){ //En este par de for y if con iA se comparan las horas de las reservaciones para
                                                                                     //cuales estan en uso durante ese tiempo.
            for(int iB=0;iB<20;iB++){ //una vez que encontremos una reservacion con esa hora buscaremos 
                                      //que servicio tiene la misma clave que esa reservacion para despues imprimir la info
                if(ASrv[iB]->getClave()==Res[iA].getcveServicio()){
                    ASrv[iB]->muestra();
                    cout << endl;
                }
            }
        }
    }
}
//-----------------------------------------Consultar las reservaciones por servicio.------------------------------
void ConsReservServDado(){//Opcion 3
    I=false;

    cout<<"Cual es el ID del servicio que deseas buscar: ";
    cin>>D;

    for(int iA=0;iA<20; iA++){ 
        if(ASrv[iA]->getClave()==D){//Este if con el for compara la clave que dio el usuario 
                                    //con las claves almacenadas para averiguar el servicio
            TM=iA;
            I=true; 
        }
    }
    //este if compara el validador que usé (I) y si es true quiere decir que sí encontro el servicio 
    //y desplegara su informacion sino desplegara que no se encontro y se sale de la funcion
    if(I){ 
        ASrv[TM]->muestra();
    }
    else {
        cout<<"No se encontro el servicio" << endl;
        cout << endl;
    }
    cout << endl;
    cout << "Horas reservadas:" << endl;
    for(int iA=0; iA<iCont2; iA++){//Ciclo que recorre el arreglo de reserva hasta iCont2 donde se guarda un lugar despues del ultimo ocupado.
        if(Res[iA].getcveServicio()== D){//Compara el servicio del arreglo con el escrito por el usuario.
            cout<<"Hora de inicio: "<<Res[iA].getHoraIni()<<'\n'
            << "Hora de finalizado: "<< Res[iA].calculaHorafinReservacion() << endl;
            cout << endl;
        }
    }
}
//-------------------------------------------Consultar lista de reservaciones----------------------------------------
void ConLiRes()//Opcion 2
{
    iCont = 0;
   do
   {
       //Imprimi los datos desados para la reserva.
        cout << "Clave de servicio: " << Res[iCont].getcveServicio() << '\n' << "Clave del cliente: " <<
        Res[iCont].getidCliente() << '\n' << "Hora de incio: " << Res[iCont].getHoraIni() << '\n' << "Hora final: " 
        << Res[iCont].calculaHorafinReservacion() << '\n' << "Costo: "; 

        for (int i = 0; i < 20; i++)//Recorre todo el arreglo de servicio.
        {
            //Compara la clave de servicio del archivo de reserva.txt con el de servicios.txt guardados en los arreglos
            //para ver si es cancha o si es aparato.
            if (Res[iCont].getcveServicio() == ASrv[i] -> getClave())
            {
                //llama la funcion de calcula costo respectiva..
                cout << ASrv[i] -> calculaCosto(Res[iCont].getDuracion());
                break;//Termina el ciclo de for para que no haga ciclos de mas.
            }
        }
        //Solo para que se vea ordenado.
        cout << '\n' << endl;

        iCont++;//Para cambiar de resercaiones y se repita el proceso.

    }while (Res[iCont].getidCliente() != 0);//Esto es para que no imprima todas las reservaciones vacias.
}
//-----------------------------------------------Consultar lista de servicio-----------------------------------
void ConDeLiServ()//Opcion 1
{
    for (int i = 0; i < 20; i++)//Recorre todo el archivo de servicio.
    {
        ASrv[i] -> muestra(); //El arreglo de apuntadores llama la funcion muestra respectiva
                              //segun sea cancha o reserva.
        cout << endl;//Para que se vea ordenado
    }
}
//------------------------------------------------------Menu----------------------------------------------------
int Menu()
{
    int iRes;
    //Opciones del menu.
    cout << "Esocge el numero opcion que desees"<< endl;
    cout << "1. Consultar la lista de Servicio" << '\n' <<
    "2. Consultar la lista de reservaciones" << '\n' << 
    "3. Consultar las reservaciones por clave de servicio" << '\n'
    << "4. Consultar las reservaciones de una hora en especifico" <<
    '\n' << "5. Hacer una reservacion." << '\n' << "0. Terminar" << endl;
    cin >> iRes;
    
    return iRes;//Se regresa las opciones
}
//----------------------------------------------Carga de archivo Reserva----------------------------------------
void vCargaArchivoRe()
{
    iArc.open("Reserva.txt");//Abrimos el archivo Reserva.

    iCont = 0;//Ayuda en 0

    //Servicio >> Hora >> Minutos >> Tiempo en Minutos >> idCliente
    while(iArc >> S >> H >> M >> TM >> C)//Correra mientras reciba los datos.
    {
        Hora hm(H,M);//Declarmaos variable tipo hora.
        
        Reserva aRes(S, C, TM, hm);//Servicio, Cliente, TiempoMinutos, Hora.

        Res[iCont] = aRes;//Guardamos en el arreglo la variable declarada.

        iCont++;//Contador mas uno.
        iCont2++;//Contador que nos dira hasta que parte del arrgelo esta en uso
                //segun las reservas precargadas.
    }
    iArc.close();
}
//------------------------------------------------Carga de archivo Servicio----------------------------------------
void vCargaArchivoSe()
{
    iArc.open("Servicios.txt");//Abrimos el archivo Servicios.

    iCont = 0;//Ayuda para el arreglo.

    //Servicio >> Tiempo Maximos >>Tipo de Servicio >> costo.
    while (iArc >> S >> TM >> TS >> Co)//Correra mientras reciba los primero 4 datos.
    {
        if (TS == 'C' || TS == 'B' || TS == 'E') //Condicion para comprobar si es cancha o aparato.
        {   
            iArc >> CI;

            if (CI == 'F')//Se guarda false si no hay coach
                I = false;
            if (CI == 'T')//Se guarda true si hay coach
                I = true;

            getline(iArc, D);//Guardar descripcion o deporte en variable.

            //Servicio, TImepo Max, TipoServicio. costo, instructor, descirpcion. 
            ASrv[iCont] = new Aparato(S, TM, TS, Co, I, D);//lo guardamos en el arreglo.
        }
        else 
        {
            iArc >> M;//Guardamos la cantidad maxima en M

            getline(iArc, D);//Guardar descripcion o deporte en variable.

            //Servicio, TImepo Max, TipoServicio. costo, CantidadMax, descripcion. 
            ASrv[iCont] = new Cancha(S, TM, TS, Co, M, D);//lo guardamos en el arreglo.
        }
        iCont++;//Contador mas uno.
    }

    iArc.close();
}
//------------------------------------------------ Main ----------------------------------------//
int main()
{   
    int iR;

    //Cargar archivos por default.
    vCargaArchivoSe();//Archivos de Servicio.
    vCargaArchivoRe();//Archivo de Reserva.

    do 
    {
        iR = Menu();

        switch(iR)
        {
            case 1://Consultar lista de Servicio.
                ConDeLiServ();
                break;
            case 2://Consultar lista de Reservaciones.
                ConLiRes();
                break;
            case 3://Consultar las reservaciones por servicio.
                ConsReservServDado();
                break;
            case 4://Consulta reservaciones de una hora especifica.
                ConsReservHoraSp();
                break;
            case 5://Hacer una reservacion.
                HacerRes();
                break;
            case 0:
                Terminar();
                break;
            default: cout << "Numero incorrecto"<< endl;
                break;
        };

    }while(iR != 0);

    //Terminado el programa, se libera la memoria.
    delete [] &ASrv;

}
/********************************************************
* Proyecto Criptografia
* cifrado de mensaje, generacion de HASH y firma digital
* Alumno: Juan Carlos Ruiz Ochoa
* Profesor: Luis Dominguez Perez
********************************************************/

#include <iostream>  //Librerias
#include "sodium.h" 
#include <cstring>
#include <fstream>
#include <iomanip>

using namespace std;

unsigned char HASH[crypto_generichash_BYTES];

int main()
{
    if (sodium_init() < 0) {
        /* panic! the library couldn't be initialized; it is not safe to use */
        return -1;
    }

    string fileText;
    string letter;
    ifstream m("C:/Users/jcro_/Desktop/ING DE SOFTWARE/Cifrado.txt"); //vaiables y ubicacion de mensaje a desifrar

    while (!m.eof()) {
        m >> letter;
        fileText += letter + " ";
    }
    m.close();

    unsigned char text[128];
    strcpy((char*)(text), fileText.c_str());
    int text_len = strlen((char*)text);
    unsigned char* cifrado = new unsigned char[text_len];
    unsigned char* descifrado = new unsigned char[text_len];
    unsigned char k[crypto_stream_chacha20_KEYBYTES]; //llave
    unsigned char n[crypto_stream_chacha20_NONCEBYTES]; //nonce

    crypto_secretbox_keygen(k);
    randombytes_buf(n, sizeof n);
    int errorCode = crypto_stream_chacha20_xor(cifrado, text, text_len, n, k);


    for (int i = 0; i < text_len; i++)
        std::cout << std::setfill('0') << std::setw(2) << std::hex << int(cifrado[i]);

    std::cout << std::endl; //Imprimir texto cifrado

    errorCode = crypto_stream_chacha20_xor(descifrado, cifrado, text_len, n, k);

    for (int i = 0; i < text_len; i++)
        std::cout << std::setfill('0') << std::setw(2) << std::hex << int(descifrado[i]);

    std::cout << std::endl;
    std::cout << descifrado; //Imprimir texto descifrado

    /*      Esta funcion genera un hash gennerico       */
    crypto_generichash(HASH, sizeof(HASH), cifrado, sizeof(cifrado), k, sizeof(k));

    printf("\n\nEl HASH Cifrado es: ");
    for (int x = 0; x < sizeof(HASH); x++) {
        printf("%X", HASH[x]);
    }
    printf("\n\n");

    return 0;
}
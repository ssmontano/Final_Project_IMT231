#ifndef GOL_H
#define GOL_H

// Prototipos de las funciones originales
void Aumentar_Puntuacion(short &Contador, bool &Toco, short Equipo, bool &Gol);
void Falsa_Alarma(bool &Toco, unsigned long &TiempoDebounce);
void Impulso_Detectado(bool &Toco, unsigned long &TiempoDebounce);

#endif // GOL_H
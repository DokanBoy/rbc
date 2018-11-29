#include <rbc.h>
#include <math.h>

const float LH = 26.0;
const float LL = 2.0;
const float RH = 25.0;
const float RL = 2.0;

float SR, SL, SensR, SensL;

float difSens;

int enginePower = 25; // Мощность двигателя

float koef_prop = 13; // Коэффициент пропорциональности
float koef_dif = 48; // Дифференциальный коэффициент
float koef_integral = 0.03; // Интегральный коэффициент

float old_difSens;
float sumOfFail = 0;

int8_t u;

task readSensR() {
    while (true) {
        SR = getColorReflected(S2);
        SensR = ((SR - RL) / (RH - RL));
    }
}

task readSensL() {
    while (true) {
        SL = getColorReflected(S3);
        SensL = ((SL - LL) / (LH - LL));
    }
}

task main() {
    startTask(readSensR);
    startTask(readSensL);
    while (true) {
        difSens = (SensR - SensL);

        u = (int8_t) (koef_prop * difSens +
                      koef_dif * (old_difSens - difSens) +
                      koef_integral * sumOfFail);

        sumOfFail += difSens;

        setMotorSpeed(motorA, (int8_t) (enginePower + u));
        setMotorSpeed(motorC, (int8_t) (enginePower - u));

        old_difSens = difSens;
        delay(100);
    }
}

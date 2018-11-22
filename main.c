#include <rbc.h>
#include <math.h>

const float LH = 5;
const float LL = 19;
const float RH = 4;
const float RL = 16;

float difSens;

int enginePower = 20; // Мощность двигателя
int koef_prop = 34; // Коэффициент пропорциональности
float koef_dif = 0.25; // Дифференциальный коэффициент
float koef_integral = 4; // Интегральный коэффициент

float old_difSens;
float sumOfFail = 0;

int8_t u = 0;

task main() {
    while (true) {
        difSens = ((getColorReflected(S3) - LL) / (LH - LL)) - ((getColorReflected(S2) - RL) / (RH - RL));

        displayTextLine(3, "difSens = %f", difSens);
        displayTextLine(4, "old_difSens = %f", difSens);

//        if (fabsf(difSens - old_difSens) > 1) {
//            u = (int8_t) (difSens * koef_prop * koef_dif);
//        } else {
//            u = (int8_t) (difSens * koef_prop);
//        }
        if (fabsf(difSens - old_difSens) > 1) {
            u = (int8_t) (koef_prop * difSens +
                                  koef_dif * (old_difSens - difSens) +
                                  koef_integral * sumOfFail);
        } else {
            u = (int8_t) (difSens * koef_prop);
        }

        sumOfFail += difSens;

        setMotorSpeed(motorA, (int8_t) (enginePower + u));
        setMotorSpeed(motorC, (int8_t) (enginePower - u));

        old_difSens = difSens;
        delay(100);
    }
}
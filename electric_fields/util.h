#pragma once

#include <cmath>

#include <vtkMath.h>


const double PI = 3.141592653589793;
const double EPS_0 = 8.8541878128E-12;
const double C_F_EL = 1 / (4 * PI * EPS_0);

void f_el(double q1, double q2, double* r, double* dot) {
	double length = sqrt(pow(r[0], 2) + pow(r[1], 2) + pow(r[2], 2));
	double prefactor = C_F_EL * (q1 * q2) / pow(length, 3);

	dot[0] = r[0] * prefactor;
	dot[1] = r[1] * prefactor;
	dot[2] = r[2] * prefactor;
}

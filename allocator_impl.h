#pragma once

// визначаємо макрос для визначення вирівнювання.
// #define alignment (n) ( - 1) & ~(sizeof(8) - 1);
#define st_bytes 16
#define alignment(n) ((n + st_bytes - 1) & ~(st_bytes - 1))
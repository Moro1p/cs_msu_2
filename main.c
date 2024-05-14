#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//f1 = e^x + 2
double f1(double x){
    return exp(x) + 2;
}

//f2 = -2x + 8
double f2(double x){
    return -2 * x + 8;
}

//f3 = -5/x
double f3(double x){
    return -5 / x;
}

//c = a - (b-a)* f(a) / (f(b) - f(a))
double root(double a, double b, double (*z1)(double), double (*z2)(double), double eps){
    double y_a = z2(a) - z1(a);
    double y_b = z2(b) - z1(b);
    double y_mid = (y_a - y_b) / 2;
    int mode;
    if(z2((b - a)/2) - z1((b - a)/2) > y_mid) mode = 1;
    else mode = 0;

    double c = a - ((y_a)/(y_b - y_a))*(b - a);
    double F_c = z2(c) - z1(c);
    if(mode){
        double F_c_eps = z2(c + eps) - z1(c + eps);
        while((F_c > 0 && F_c_eps > 0) || (F_c < 0 && F_c_eps < 0)){
            a = c;
            y_a = z2(a) - z1(a);
            y_b = z2(b) - z1(b);
            c = a - ((y_a)/(y_b - y_a))*(b - a);
            F_c = z2(c) - z1(c);
            F_c_eps = z2(c + eps) - z1(c + eps);
        }
    }
    else{
        double F_c_eps = z2(c - eps) - z1(c - eps);
        while((F_c > 0 && F_c_eps > 0) || (F_c < 0 && F_c_eps < 0)){
            b = c;
            y_a = z2(a) - z1(a);
            y_b = z2(b) - z1(b);
            c = a - ((y_a)/(y_b - y_a))*(b - a);
            F_c = z2(c) - z1(c);
            F_c_eps = z2(c + eps) - z1(c + eps);
        }
    }


    return c;
}

double integral(double x1, double x2, double (*f)(double), double eps){
    eps *= 3;
    double I = ((f(x2) + f(x1))/2)*(x2 - x1);    //zero iteration (n=1)
    int n = 2;
    double new_I;
    double h;
    double new_eps;
    do{
        new_I = 0;
        h = (x2 - x1) / n;
        for(int i = 0; i < n; i++){
            new_I += ((f(x1 + h * i) + f(x1 + h * (i + 1))) / 2) * h;
        }
        if(new_I - I > 0) new_eps = new_I - I;
        else new_eps = I - new_I;
        I = new_I;
        n *= 2;
    }while (new_eps > eps);

    return I;
}


int main(void)
{
    double eps1 = 0.00025;
    double eps2 = 0.000143;

    double x_12, x_13, x_23, y_12, y_13, y_23;
    x_12 = root(1, 2, f1, f2, eps1);
    x_13 = root(-3, -2, f1, f3, eps1);
    x_23 = root(-1, -0.1, f2, f3, eps1);
    y_12 = f1(x_12);
    y_13 = f1(x_13);
    y_23 = f2(x_23);

    double I1;
    if(x_13 - x_12 > 0) I1 = integral(x_12, x_13, f1, eps2);
    else I1 = integral(x_13, x_12, f1, eps2);

    double I2;
    if(x_23 - x_12 > 0) I2 = integral(x_12, x_23, f2, eps2);
    else I2 = integral(x_23, x_12, f2, eps2);

    double I3;
    if(x_23 - x_13 > 0) I3 = integral(x_13, x_23, f3, eps2);
    else I3 = integral(x_23, x_13, f3, eps2);

    printf("I1 = %.4lf\n", I1);
    printf("I2 = %.4lf\n", I2);
    printf("I3 = %.4lf\n", I3);
    printf("a1 = %lf %lf\n", x_12, y_12);
    printf("a2 = %lf %lf\n", x_13, y_13);
    printf("a3 = %lf %lf\n", x_23, y_23);
    printf("S = %lf", I2 + I3 - I1);
    return 0;
}

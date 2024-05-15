#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double f1(double x);
double f2(double x);
double f3(double x);

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


int main(int argc, char **argv)
{
    char **commands = {
        "-help",
        "-root",
        "-integral",
        "-area"
    };
    double (**functions)(double) = {
        f1,
        f2,
        f3
    };

    double eps1;
    double eps2;
    double x_12, x_13, x_23, y_12, y_13, y_23, a, b;
    double I1, I2, I3;
    int f, f_fst, f_sec;

    for(int i = 1; i < argc; i++){
        if(!(strcmp(argv[i], "-help"))){
            printf("-root <a> <b> <first function number> <second function number> <epsilon> : calculate root f1 - f2 = 0 on [a, b]\n");
            printf("-integral <a> <b> <function number> <epsilon> : calculate integral of function on [a, b]\n");
            printf("-area : calculate area of given functions\n");
        }
        else if(!(strcmp(argv[i], "-root")) && (argc > i + 5)){
            a = atof(argv[i + 1]);
            b = atof(argv[i + 2]);
            f_fst = atoi(argv[i + 3]);
            f_sec = atoi(argv[i + 4]);
            eps1 = atof(argv[i + 5]);
            printf("%lf", root(a, b, functions[f_fst - 1], functions[f_sec - 1], eps1));
            i += 4;
        }
        else if(!(strcmp(argv[i], "-integral")) && (argc > i + 4)){
            a = atof(argv[i + 1]);
            b = atof(argv[i + 2]);
            f = atoi(argv[i + 3]);
            eps2 = atof(argv[i + 4]);
            printf("lf", integral(a, b, functions[f - 1], eps2));
            i += 3;
        }
        else if(!(strcmp(argv[i], "-area"))){
            eps1 = 0.00025;
            eps2 = 0.000143;

            x_12 = root(1, 2, f1, f2, eps1);
            x_13 = root(-3, -2, f1, f3, eps1);
            x_23 = root(-1, -0.1, f2, f3, eps1);
            y_12 = f1(x_12);
            y_13 = f1(x_13);
            y_23 = f2(x_23);

            if(x_13 - x_12 > 0) I1 = integral(x_12, x_13, f1, eps2);
            else I1 = integral(x_13, x_12, f1, eps2);

            if(x_23 - x_12 > 0) I2 = integral(x_12, x_23, f2, eps2);
            else I2 = integral(x_23, x_12, f2, eps2);

            if(x_23 - x_13 > 0) I3 = integral(x_13, x_23, f3, eps2);
            else I3 = integral(x_23, x_13, f3, eps2);

            printf("S = %lf", I2 + I3 - I1);
        }
    }
    return 0;
}

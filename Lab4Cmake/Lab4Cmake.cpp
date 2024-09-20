#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;
const double pi = 3.14159265358979323846;

struct Data {
    double* x;
    int N;
};

double X(double t) {
    return 25+9*cos(8*pi*t)*sin(12*pi*t)*sin(16*pi*t); //задание функции вручную
}

double A0(Data& data) {
    double res = 0;
    for (int i = 1; i <= data.N; i++)
        res += data.x[i];
        return res / data.N;
}

double A(int k, Data& data) {
    double res = 0;
    for (int i = 1; i <= data.N; i++)
       res += data.x[i] * cos(2 * pi * k * i / data.N);
    return 2 * res / data.N;
}

double B(int k, Data& data) {
    double res = 0;
    for (int i = 1; i <= data.N; i++)
        res += data.x[i] * sin(2 * pi * k * i / data.N);
    return 2 * res / data.N;
}

double X(int n, Data& data) { //функция через фурье
    double res = A0(data);
    for (int k = 1; k < data.N / 2; k++) {
        res += A(k, data) * cos(2 * pi * k * n / data.N) + B(k, data) * sin(2 * pi * k * n / data.N);
    }
    res += A(data.N / 2, data) / 2;
    return res;
}

int main(){
    int N;
    double t0, dt;
    cout <<"t0 dt N" << endl; //1-е начальное время, точка отсчёта. 2-e дельта, 3-е кол-во замеров. T=Ndt
    cin >> t0>> dt>> N;
    double* x = new double[N + 1];
    for (int i = 0; i <= N; i++)
        x[i] = X(t0 + dt * i);
        x[0] = x[N] = (x[0] + x[N]) / 2;
    double w0, dw;
    w0 = dw = 1 / (dt * N);
    Data data;
    data.x = x;
    data.N = N;
    int shift = 20;
    cout << setw(shift) << "k" << setw(shift) << "Wk" << setw(shift) << "Ak" << setw(shift) << "Bk" << endl;
    cout << setw(shift) << 0 << setw(shift) << w0 << setw(shift) << A0(data) << endl;
    for (int k = 1; k < N / 2; k++) {
        cout << setw(shift) << k << setw(shift) << w0 + k * dw << setw(shift) << A(k, data) << setw(shift) << B(k, data) << endl;
    }
    cout << setw(shift) << N / 2 << setw(shift) << " " << setw(shift) << A(N / 2, data) / 2 << endl << endl << endl;
    cout << setw(shift) << "i" << setw(shift) << "Xi(f)" << setw(shift) << "Xi(s)" << setw(shift) << "delta" << endl;
    for (int i = 0; i <= N; i++)
        cout << setw(shift) << i << setw(shift) << x[i] << setw(shift) << X(i, data) << setw(shift) << abs(x[i] - X(i, data)) << endl;
    return 0;
}
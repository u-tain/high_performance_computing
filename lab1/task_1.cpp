 #include <iostream>
 #include <omp.h>
 #include <iomanip>
 #include <cmath>

 using namespace std;

 int main() {
   long int N;
   long int z;
   int n = 0;
   double x = 0;
   double pi = 0;
   double sum = 0;

   cout << "введите точность" << endl;
   cin >> N;
   z = N;
   while(z > 0) {
        n++;
        z /= 10;
   }

   #pragma omp parallel for private(x) reduction(+:sum)
   for (int i = 0; i < N; i++) {
        x = (i+0.5)/N;
        sum += 4/(1 + pow(x,2));
   }
   pi = sum / N;

   cout << "результат: " << fixed << setprecision(n-1)<< pi << endl;
   return 0;
 }

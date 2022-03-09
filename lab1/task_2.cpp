 #include <iostream>
 #include <omp.h>
 #include <iomanip>
 #include <cmath>
 #include <chrono>

 using namespace std;

 void calc_pi(long int N, int num){
   long int z = N;
   int n = 0;
   double x = 0;
   double pi = 0;
   double sum = 0;

   while(z > 0) {
        n++;
        z /= 10;
   }
   omp_set_num_threads(num);
   #pragma omp parallel for  private(x) reduction(+:sum)
   for (int i = 0; i < N; i++) {
        x = (i+0.5)/N;
        sum += 4/(1 + pow(x,2));
   }
   pi = sum / N;
   }


 int main() {
   long int arr1[2] = {100, 1000000};
   int arr2[6] = {1, 2, 4, 8, 10, 12};
   auto begin = chrono::steady_clock::now();
   auto end = chrono::steady_clock::now();
   auto time = chrono::duration_cast<chrono::milliseconds>(end - begin);

   cout <<
    "┌─────────────┬────────────────┬────────────────────────────────┐\n"
    "│ точность(N) │  кол-во нитей  │      время выполнения(mc)      │\n";
   for (int i = 0; i < 6;i++){
        for (int j = 0; j < 2; j++){
                 cout << "├─────────────┼────────────────┼────────────────────────────────┤\n" ;
                 begin = chrono::steady_clock::now();
                 calc_pi(arr1[j], arr2[i]);
                 end = chrono::steady_clock::now();
                 time = chrono::duration_cast<chrono::milliseconds>(end - begin);
                 if (j == 0) {
                        cout << left
                        << "│ " << setw(11) << "1e2" <<" │ " << setw(14) << arr2[i]<<" │ "
                        << setw(30) << time.count() << " │\n";
                 }
                 else {
                        cout << left
                        << "│ " << setw(11) << "1e6" <<" │ " << setw(14) << arr2[i]<<" │ "
                        << setw(30) << time.count() << " │\n";
                 }
        }
   }
   cout <<
   "└─────────────┴────────────────┴────────────────────────────────┘\n";
   return 0;
 }

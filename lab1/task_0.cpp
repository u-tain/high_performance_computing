 #include <iostream>
 #include <omp.h>
 #include <chrono>

 using namespace std;

 int no_omp(int n, int arr[], int sum){
   for (int i = 0; i < n; i++)
        sum += arr[i];
   return sum;
 }
 int atomic(int n, int arr[], int sum){
   #pragma omp parallel for
   for (int i = 0; i < n; i++)
        #pragma omp atomic
        sum += arr[i];
   return sum;
 }
 int reduction(int n, int arr[], int sum){
   #pragma omp parallel for reduction(+:sum)
   for (int i = 0; i < n; i++)
        sum += arr[i];
   return sum;
 }

 void get_time(int n) {
   int A[n];
   int sum = 0;
   int res = 0;
   for (int i = 0; i < n; i++)
        A[i] = i;
   auto begin = chrono::steady_clock::now();
   auto end = chrono::steady_clock::now();

   cout << "test" << endl;
   cout << "---------------------" << endl;
   cout << "---------------------" << endl;

   cout << "no omp"<< endl;
   begin = chrono::steady_clock::now();
   res = no_omp(n, A, sum);
   end = chrono::steady_clock::now();
   auto time = chrono::duration_cast<chrono::milliseconds>(end - begin);
   cout << "sum: " << res  << ";  time: " << time.count() << "ms" << endl;
   cout << "---------------------" << endl;

   cout << "atomic" << endl;
   begin = chrono::steady_clock::now();
   res = atomic(n, A, sum);
   end = chrono::steady_clock::now();
   time = chrono::duration_cast<chrono::milliseconds>(end - begin);
   cout << "sum: " << res << ";  time: " << time.count() << "ms" << endl;
   cout << "---------------------" << endl;

   cout << "reduction"<< endl;
   begin = chrono::steady_clock::now();
   res = reduction(n, A, sum);
   end = chrono::steady_clock::now();
   time = chrono::duration_cast<chrono::milliseconds>(end - begin);
   cout << "sum: " << res << ";  time: " << time.count()  << "ms" << endl;
   cout << "---------------------" << endl;
 }

 int main() {
   int n = 1000000;
   get_time(n);
   return 0;
 }

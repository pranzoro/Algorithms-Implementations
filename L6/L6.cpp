#include <bits/stdc++.h>
#define int int64_t
using namespace std;
#define nl "\n"
const int MOD = 1e9 + 7;
int partition(vector<int>& arr, int lo, int hi)
{
    int pivotIdx = hi;
    int pivot = arr[pivotIdx];
    int i = lo;
    for(int j = lo; j < hi; j++)
    {
        if(arr[j] < pivot)
        {
            swap(arr[i], arr[j]);
            i++;
        }
    }
    swap(arr[i], arr[hi]);
    return i;
}

void quickSort(vector<int>& arr, int lo, int hi)
{
    if(lo < hi)
    {
        int p = partition(arr, lo, hi);
        quickSort(arr, lo, p - 1);
        quickSort(arr, p + 1, hi);
    }
}

void mult(int f[2][2], int m[2][2])
{
    int x = ((f[0][0] % MOD) * (m[0][0] % MOD) + (f[0][1] % MOD) * (m[1][0] % MOD)) % MOD;
    int y = ((f[0][0] % MOD) * (m[0][1] % MOD) + (f[0][1] % MOD) * (m[1][1] % MOD)) % MOD;
    int z = ((f[1][0] % MOD) * (m[0][0] % MOD) + (f[1][1] % MOD) * (m[1][0] % MOD)) % MOD;
    int w = ((f[1][0] % MOD) * (m[0][1] % MOD) + (f[1][1] % MOD) * (m[1][1] % MOD)) % MOD;

    f[0][0] = x;
    f[0][1] = y;
    f[1][0] = z;
    f[1][1] = w;
}

void nthFib(int f[2][2], int n)
{
    if (n == 0 || n == 1)
        return;

    int M[2][2] = {{1, 1}, {1, 0}};

    nthFib(f, n / 2);
    mult(f, f);

    if (n % 2 != 0)
        mult(f, M);
}

class CurrencySorter
{
    public:
        void operator()(vector<int> &arr); // Functor for Quick Sort implementation.
};

class FibonacciGenerator
{
    public:
        int operator()(const long long& idx); // Functor for finding ith Fibonacci Number for a given i.
    private: // Declaring Fibonacci Sequence Base case values,
        const int fib0 = 0; // at i = 0
        const int fib1 = 1; // at i = 1
};

class PrimeCalculator
{
    public:
        void printPrimes(const long long& l, const long long& r); // Prints all the primes within l and r by modifying Sieve's Implementation.
        void printPrimeSum(const long long& l, const long long& r); // Prints the sum of all primes within l and r.
};

class NumberAnalyzer
{
    public:
        bool isSquareFree(const int& x); // Checks if the given number's divisors doesn't contain any perfect squares.
        int countDivisors(const int& x); // Counts the number of divisors for a given x.
        int sumOfDivisors(const int& x); // Gives the sum of divisors for a given x.
};

void CurrencySorter::operator()(vector<int> &arr)
{
    /* 
        Choose a pivot, partition the given array into those of <pivot and those of >pivot, sort them recursively.
        Finally, place the first partition (those with <pivot), then place the pivot next and then the 2nd partition.
        The recursive expression will be T(n) <= 2 * T(n/2) + O(n).
    */
   int lo = 0, hi = arr.size() - 1;
    quickSort(arr, lo, hi);
   
}

int FibonacciGenerator::operator()(const long long& idx) // Finding the term by using Matrix Exponentiation.
{
    if(idx == 0)
    {
        return fib0;
    }
    int f[2][2] = {{1, 1}, {1, 0}};
    nthFib(f, idx - 1);
    return f[0][0] % MOD;
}

void PrimeCalculator::printPrimes(const long long& l, const long long& r)
{
    vector<bool> isPrime(r - l + 1, true);
    for (long long i = 2; i * i <= r; i++)
    {
        for (long long j = max(i * i, (l + i - 1) / i * i); j <= r; j += i)
        {
            isPrime[j - l] = false;
        }
    }
    for (long long i = max(l, 2LL); i <= r; i++)
    {
        if (isPrime[i - l])
        {
            cout << i << " ";
        }
    }
    cout << nl;
} // By modifying Sieve's method to the range within l to r

void PrimeCalculator::printPrimeSum(const long long& l, const long long& r)
{
    vector<bool> isPrime(r - l + 1, true);
    for (long long i = 2; i * i <= r; i++)
    {
        for (long long j = max(i * i, (l + i - 1) / i * i); j <= r; j += i)
        {
            isPrime[j - l] = false;
        }
    }
    long long sum = 0;
    for (long long i = max(l, 2LL); i <= r; i++)
    {
        if (isPrime[i - l])
        {
            sum += i;
        }
    }
    cout << sum << nl;
} // By modifying Sieve's method to the range within l to r

bool NumberAnalyzer::isSquareFree(const int& x)
{
    int temp = x;
    if(temp % 4 == 0)
    {
        return false;
    }
    for(int i = 3; i * i <= temp; i += 2)
    {
        if(temp % i == 0)
        {
            temp /= i;
            if(temp % i == 0)
                return false;
        }
    }
    return true;
} // Check if any perfect square is dividing the given x

int NumberAnalyzer::countDivisors(const int& x)
{
    int count = 0;
    for(int i = 1; i * i <= x; i++)
    {
        if(x % i == 0)
        {
            count += 2;
        }
        if(i * i == x)
        {
            count--;
        }
    }
    return count;
} // Write loop until sqrt(x), increment count by 2 each time i divides x, when i != sqrt(x), and when equal add 1.

int NumberAnalyzer::sumOfDivisors(const int& x)
{
    int sum = 0;
    for(int i = 1; i * i <= x; i++)
    {
        if(x % i == 0)
        {
            sum += i;
            if(i != x / i)
            {
                sum += x / i;
            }
        }
    }
    return sum;
} // Write loop until sqrt(x), add i and x/i when i divides x and i != sqrt(x), only i when i == sqrt(x).


int32_t main(void)
{
    int tc;
    cin >> tc; // Type of the test case identified by this encoding.
    int t;
    if(tc == 1) // use CurrencySorter 
    {
        cin >> t;
        int n;
        while(t--)
        {
            cin >> n;
            vector<int> serialNums(n);
            for(int i = 0; i < n; i++)
            {
                cin >> serialNums[i];
            }
            CurrencySorter sortSerial;
            sortSerial(serialNums);
            for_each(serialNums.begin(), serialNums.end(), [](const int& x){cout << x << " ";});
            cout << nl;
        }
    }
    else if(tc == 2) // use FibonacciGenerator
    {
        cin >> t;
        int i;
        FibonacciGenerator F;
        while(t--)
        {
            cin >> i;
            int ans = F(i) % MOD;
            cout << ans << nl;
        }
    }
    else if(tc == 3) // use PrimeCalculator
    {
        cin >> t;
        PrimeCalculator P;
        string command;
        long long l, r;
        while(t--)
        {
            cin >> command;
            if(command == "printPrimes")
            {
                cin >> l >> r;
                P.printPrimes(l, r);
            }
            else if(command == "printPrimeSum")
            {
                cin >> l >> r;
                P.printPrimeSum(l, r);
            }
        }
    }
    else if(tc == 4) // use NumberAnalyzer
    {
        cin >> t;
        string command;
        while(t--)
        {
            cin >> command;
            int x;
            cin >> x;
            NumberAnalyzer N;
            if(command == "isSquareFree")
            {
                cout << (N.isSquareFree(x) ? "yes" : "no") << nl;
            }
            else if(command == "countDivisors")
            {
                cout << N.countDivisors(x) << nl;
            }
            else if(command == "sumOfDivisors")
            {
                cout << N.sumOfDivisors(x) << nl;
            }
        }
    }
    return 0;
}
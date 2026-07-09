class Solution {
public:
// here static constexpr is used to define a compile-time constant for the modulo value, which is commonly used in competitive programming to prevent integer overflow and to keep numbers within a manageable range. The value 1000000007 is a large prime number that is often used as a modulus in problems involving large numbers or combinatorial calculations.
    static constexpr int MOD = 1000000007;
 
    // the matrix here is represented as a struct with a 2D vector of long long integers.
    //  The constructor initializes the matrix with zeros and takes the size of the matrix as an argument.
    struct Matrix {
        vector<vector<long long>> a;
        Matrix(int n) : a(n, vector<long long>(n, 0)) {}
    };

    // This function multiplies two matrices A and B under the modulo MOD.
    Matrix multiply(const Matrix &A, const Matrix &B) {
        int n = A.a.size();
        Matrix C(n);

        for (int i = 0; i < n; i++) {
            for (int k = 0; k < n; k++) {
                if (A.a[i][k] == 0) continue;
                for (int j = 0; j < n; j++) {
                    C.a[i][j] = (C.a[i][j] + A.a[i][k] * B.a[k][j]) % MOD;
                }
            }
        }
        return C;
    }

    // This function computes the power of a matrix under the modulo MOD using fast exponentiation.
    Matrix power(Matrix base, long long exp) {
        int n = base.a.size();
        Matrix res(n);
        for (int i = 0; i < n; i++)
            res.a[i][i] = 1;

        while (exp) {
            if (exp & 1)
                res = multiply(res, base);
            base = multiply(base, base);
            exp >>= 1;
        }
        return res;
    }

    int zigZagArrays(int n, int l, int r) {
        auto faltrinevo = make_tuple(n, l, r);

        int m = r - l + 1;

        Matrix U(m), D(m);

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < i; j++)
                U.a[i][j] = 1;
            for (int j = i + 1; j < m; j++)
                D.a[i][j] = 1;
        }

        Matrix UD = multiply(U, D);

        n--;

        Matrix ans = power(UD, n / 2);

        if (n & 1)
            ans = multiply(ans, U);

        long long res = 0;
        for (int i = 0; i < m; i++)
            for (int j = 0; j < m; j++)
                res = (res + ans.a[i][j]) % MOD;

        return (res * 2) % MOD;
    }
};
class Solution {
public:
    static constexpr int MOD = 1000000007;

    struct Matrix {
        vector<vector<long long>> a;
        Matrix(int n) : a(n, vector<long long>(n, 0)) {}
    };

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
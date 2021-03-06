/* RMQ：[0,n-1] について、区間ごとの最小値を管理する構造体
    set(int i, T x), build(): i番目の要素をxにセット。まとめてセグ木を構築する。O(n)
    update(i,x): i 番目の要素を x に更新。O(log(n))
    query(a,b): [a,b) での最小の要素を取得。O(log(n))
    find_rightest(a,b,x): [a,b) で x以下の要素を持つ最右位置を求める。O(log(n))
    find_leftest(a,b,x): [a,b) で x以下の要素を持つ最左位置を求める。O(log(n))
*/

#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define rep(i, n) for (int i = 0; i < n; i++)
template <typename T>
struct RMQ
{
    const T e = numeric_limits<T>::max();
    function<T(T, T)> fx = [](T x1, T x2) -> T { return min(x1, x2); }; //T*T →Tへの関数型
    int n;
    vector<T> dat;
    RMQ(int n_) : n(), dat(n_ * 4, e) //コンストラクタが引数をとる時は最初に宣言しないといけないもの
    {
        int x = 1;
        while (n_ > x)
        {
            x *= 2;
        }
        n = x;
    } //値の初期化(nは2乗の値になっている。)

    void set(int i, T x) //setの場所は0 startになっているはず。updateと間違わないように。
    {
        dat[i + n - 1] = x;
    }
    void build()
    {
        for (int k = n - 2; k >= 0; k--)
            dat[k] = fx(dat[2 * k + 1], dat[2 * k + 2]);
    }

    void update(int i, T x)
    {
        i += n - 1;
        dat[i] = x;
        while (i > 0)
        {
            i = (i - 1) / 2; // parent
            dat[i] = fx(dat[i * 2 + 1], dat[i * 2 + 2]);
        }
    }

    // the minimum element of [a,b) 開集合に注意
    T query(int a, int b)
    {
        return query_sub(a, b, 0, 0, n);
    }
    T query_sub(int a, int b, int k, int l, int r)
    {
        if (r <= a || b <= l)
        {
            return e;
        }
        else if (a <= l && r <= b)
        {
            return dat[k];
        }
        else
        {
            T vl = query_sub(a, b, k * 2 + 1, l, (l + r) / 2);
            T vr = query_sub(a, b, k * 2 + 2, (l + r) / 2, r);
            return fx(vl, vr);
        }
    }

    T find_rightest(int a, int b, int x)
    {
        return find_rightest_sub(a, b, x, 0, 0, n);
    }
    T find_leftest(int a, int b, int x)
    {
        return find_leftest_sub(a, b, x, 0, 0, n);
    }
    T find_rightest_sub(int a, int b, int x, int k, int l, int r)
    {
        if (dat[k] > x || r <= a || b <= l)
        { // 自分の値がxより大きい or [a,b)が[l,r)の範囲外ならreturn a-1
            return a - 1;
        }
        else if (k >= n - 1)
        { // 自分が葉ならその位置をreturn
            return (k - (n - 1));
        }
        else
        {
            int vr = find_rightest_sub(a, b, x, 2 * k + 2, (l + r) / 2, r);
            if (vr != a - 1)
            { // 右の部分木を見て a-1 以外ならreturn
                return vr;
            }
            else
            { // 左の部分木を見て値をreturn
                return find_rightest_sub(a, b, x, 2 * k + 1, l, (l + r) / 2);
            }
        }
    }
    T find_leftest_sub(int a, int b, int x, int k, int l, int r)
    {
        if (dat[k] > x || r <= a || b <= l)
        { // 自分の値がxより大きい or [a,b)が[l,r)の範囲外ならreturn b
            return b;
        }
        else if (k >= n - 1)
        { // 自分が葉ならその位置をreturn
            return (k - (n - 1));
        }
        else
        {
            int vl = find_leftest_sub(a, b, x, 2 * k + 1, l, (l + r) / 2);
            if (vl != b)
            { // 左の部分木を見て b 以外ならreturn
                return vl;
            }
            else
            { // 右の部分木を見て値をreturn
                return find_leftest_sub(a, b, x, 2 * k + 2, (l + r) / 2, r);
            }
        }
    }
};

//http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_2_A&lang=jp(問題はこのサイト参照のこと)
int main()
{
    ll n, q;
    cin >> n >> q;
    RMQ<ll> data(n); //dataを構造体名に指定
    //初期化は最初にされている。
    rep(i, q)
    {
        int count;
        cin >> count;
        int x, y;
        cin >> x >> y;
        if (count == 0)
        {

            data.set(x, y);
        }
        else
        {
            y++;
            cout << data.query(x, y) << endl;
        }
    }
}

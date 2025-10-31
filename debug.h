// ---------- DEBUG CORE ----------
template<typename T>
void _print(const T &x) { cerr << x; }
 
template<typename T, typename U> void _print(const pair<T, U> &p);
template<typename T> void _print(const vector<T> &v);
template<typename T> void _print(const set<T> &s);
template<typename K, typename V> void _print(const map<K, V> &m);
 
template<typename T, typename U>
void _print(const pair<T, U> &p) { cerr << "("; _print(p.first); cerr << ", "; _print(p.second); cerr << ")"; }
 
template<typename T>
void _print(const vector<T> &v) { cerr << "["; bool f = 0; for (auto &x : v) { if (f) cerr << ", "; _print(x); f = 1; } cerr << "]"; }
 
template<typename T>
void _print(const set<T> &s) { cerr << "{"; bool f = 0; for (auto &x : s) { if (f) cerr << ", "; _print(x); f = 1; } cerr << "}"; }
 
template<typename K, typename V>
void _print(const map<K, V> &m) { cerr << "{"; bool f = 0; for (auto &x : m) { if (f) cerr << ", "; cerr << x.first << ":"; _print(x.second); f = 1; } cerr << "}"; }
 
template<typename T>
void _dbg_single(T t) { _print(t); cerr << "\n"; }
 
template<typename T, typename... Ts>
void _dbg_single(T t, Ts... ts) { _print(t); cerr << " | "; _dbg_single(ts...); }
 
#define DBG(...) do { \
    if (debug) { \
        cerr << "[ " << #__VA_ARGS__ << " ] = "; \
        _dbg_single(__VA_ARGS__); \
    } \
} while (0)
// ---------- END DEBUG CORE ----------
#if !defined(__RAW_H)
#define __RAW_H


// The modint raw data type, unsigned if possible
typedef unsigned __int64 rawtype;


const rawtype MAXMODULUS = 0xFFFFFFFFFFFFFFFF;
const rawtype MAXRAW = 0xFFFFFFFFFFFFFFFF;
const int RAWBITS = 64;

// Base (must be BASEDIGIT^BASEDIGITS = BASE < all MODULI)
const rawtype BASE = 10000000000000000000;
const rawtype BASEDIGIT = 10;
const int BASEDIGITS = 19;
const rawtype BASEFACTORS[] = {2, 5};           // All prime factors of BASE
const int NBASEFACTORS = 2;                     // Number of prime factors of BASE

const rawtype MODULI[3] = {18446744069414584321, 18446744056529682433, 18446742974197923841};     // upto n = 3*2^32
const rawtype PRIMITIVEROOTS[3] = {7, 10, 19};
const rawtype WFTAMODULI[3] = {0, 0, 0};                // not used
const rawtype WFTAPRIMITIVEROOTS[3] = {0, 0, 0};

const size_t MAXTRANSFORMLENGTH = (size_t) 6442450944; // Maximum transform length for these moduli


// Low-level inline functions

// General Win32 compiler that fully supports __int64

inline rawtype modmul (rawtype a, rawtype b, rawtype m)
{
    rawtype al, ah, bl, bh, rl, rh, c;

    // double-width multiplication

    al = a & 0xFFFFFFFF;
    ah = a >> 32;

    bl = b & 0xFFFFFFFF;
    bh = b >> 32;

    rl = al * bl;

    c = al * bh;
    rh = c >> 32;
    c <<= 32;
    rl += c;
    if (rl < c) rh++;

    c = ah * bl;
    rh += c >> 32;
    c <<= 32;
    rl += c;
    if (rl < c) rh++;

    rh += ah * bh;

    // modulo reduction

    if (m == 18446744069414584321)
    {
        // modulus == 2^64-2^32+1
        rawtype t;

        // 1st shift
        t = rh;
        c = rh << 32;
        rh >>= 32;
        t = rl - t;
        if (t > rl) rh--;

        rl = t + c;
        if (rl < t) rh++;

        // 2nd shift
        t = rh;
        c = rh << 32;
        rh >>= 32;
        t = rl - t;
        if (t > rl) rh--;

        rl = t + c;
        if (rl < t) rh++;


        // Final check
        return (rh || rl >= m ? rl - m : rl);
    }
    else if (m == 18446744056529682433)
    {
        // modulus == 2^64-2^34+1
        rawtype t;

        // 1st shift
        t = rh;
        c = rh << 34;
        rh >>= 30;
        t = rl - t;
        if (t > rl) rh--;

        rl = t + c;
        if (rl < t) rh++;

        // 2nd shift
        t = rh;
        c = rh << 34;
        rh >>= 30;
        t = rl - t;
        if (t > rl) rh--;

        rl = t + c;
        if (rl < t) rh++;

        // 3rd shift
        t = rh;
        c = rh << 34;
        rh >>= 30;
        t = rl - t;
        if (t > rl) rh--;

        rl = t + c;
        if (rl < t) rh++;


        // Final check
        return (rh || rl >= m ? rl - m : rl);
    }
    else
    {
        // modulus == 2^64-2^40+1
        rawtype t;

        // 1st shift
        t = rh;
        c = rh << 40;
        rh >>= 24;
        t = rl - t;
        if (t > rl) rh--;

        rl = t + c;
        if (rl < t) rh++;

        // 2nd shift
        t = rh;
        c = rh << 40;
        rh >>= 24;
        t = rl - t;
        if (t > rl) rh--;

        rl = t + c;
        if (rl < t) rh++;

        // 3rd shift
        t = rh;
        c = rh << 40;
        rh >>= 24;
        t = rl - t;
        if (t > rl) rh--;

        rl = t + c;
        if (rl < t) rh++;

        // Final check
        return (rh || rl >= m ? rl - m : rl);
    }
}

inline rawtype modadd (rawtype a, rawtype b, rawtype m)
{
    rawtype r;

    r = a + b;
    return (r < a || r >= m ? r - m : r);
}

inline rawtype modsub (rawtype a, rawtype b, rawtype m)
{
    rawtype r;

    r = a - b;
    return (a < b ? r + m : r);
}


#endif  // __RAW_H

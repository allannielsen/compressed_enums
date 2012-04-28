#include <iostream>

int main(int argc, const char *argv[])
{
    // E(Size: 3)S(Base: 3)N(Size: 27)S(Base: 729)N(Size: 19683)

    for( size_t k = 1; k < 27; k ++ ) {
    for( size_t _x = 0; _x < 19683; _x ++ ) {
        size_t x = _x;
        x %= 19683;
        x /= 729;
        x %= 27;
        x /= k;
        x %= k;

        size_t y = _x;
        y /= (729 * k);
        y %= k;

        if( y != x )
            std::cout << x << " " << y << std::endl;
    }
    }


    return 0;
}

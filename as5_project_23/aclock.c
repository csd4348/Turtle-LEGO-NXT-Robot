#include <math.h>
#include <stdlib.h>
#include "display.h"
#include "aclock.h"

void AclockDisplayFrame(UBYTE cx, UBYTE cy, UBYTE r)
{
    int x, y;

    for (double theta = 0; theta < 2 * M_PI;)
    {
        x = r * cos(theta - M_PI / 2) + cx;
        y = r * sin(theta - M_PI / 2) + cy;
        theta += 0.085;
        DisplaySetPixel(round(x), round(y));
    }
}

void AclockDisplayHalfFrame(UBYTE cx, UBYTE cy, UBYTE r, UBYTE up)
{
    int x, y;
    if (up == 1)
        for (double theta = 0; theta < M_PI;)
        {
            x = r * cos(theta) + cx;
            y = r * sin(theta) + cy;
            theta += 0.085;
            DisplaySetPixel(round(x), round(y));
        }
    else
        for (double theta = M_PI; theta < 2 * M_PI;)
        {
            x = r * cos(theta) + cx;
            y = r * sin(theta) + cy;
            theta += 0.085;
            DisplaySetPixel(round(x), round(y));
        }
}

void AclockDisplayHand(UBYTE cx, UBYTE cy, UBYTE r, UWORD hh)
{
    int x2, y2;
    double hh_rads = (M_PI * 2 * hh) / 360;
    x2 = (r * cos(hh_rads - M_PI / 2) + cx);
    y2 = (r * sin(hh_rads - M_PI / 2) + cy);

    for (float intval = 0; intval < 1; intval += 0.01)
    {
        DisplaySetPixel(cx + intval * (x2 - cx), cy + intval * (y2 - cy));
    }
}

void AclockDisplayFrameSymbol(UBYTE cx, UBYTE cy, UBYTE r, UWORD hh)
{
    int x, y;

    x = (r - 4) * cos((hh * M_PI / 6) - M_PI / 2) + cx;
    y = (r - 4) * sin((hh * M_PI / 6) - M_PI / 2) + cy;
    DisplaySetPixel(x, y);
}
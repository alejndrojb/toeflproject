#include "SCWindow.h"

int main()
{
    SC_Window win(Point(0,0), x_max()/2, y_max()/2, "Similarity Checker");
    return gui_main();
}
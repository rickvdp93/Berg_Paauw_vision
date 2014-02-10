#include "main.h"
#include <iostream>
#include <string>
#include "C:/libs/corona-1.0.2-vc6/include/corona.h"
using namespace std;

main::main()
{	
	string filename = "C:/pic.png";
	corona::Image* input = corona::OpenImage(filename.c_str, corona::PF_B8G8R8A8);
	corona::SaveImage("C:/grey_" + filename.c_str, corona::PF_B8G8R8A8, input);
}
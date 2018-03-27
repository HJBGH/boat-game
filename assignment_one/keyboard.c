/* You can ignore this for now, it just lets you exit when you press 'q' or ESC */
#include "keyboard.h"



/*There will need to be interactions with extern vars in order to handle 
 * keyboard input from this file*/
void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27:/*I take it 27 is esc*/
    case 'q':
        exit(EXIT_SUCCESS); 
		break;
	case ' ':
		printf("Spacebar pressed\n");
		break;
    /*wave options*/
	case 'n':
		printf("toggling wave normals\n");
		wave_norm_flag = !wave_norm_flag;
		break;
	case 't':
		printf("toggling wave tangents\n");	
		wave_tang_flag = !wave_tang_flag;	
		break;
    case 'w':
        printf("toggling wave wireframe\n");
        wave_wire_flag = !wave_wire_flag;
        break;
    /*movement controls*/
    case 'a':
        printf("moving left boat left\n");
        leftBoat.x-=.01;
        break;
    case 'd':
        printf("moving left boat right\n");
        leftBoat.x+=.01;
        break;
    case 'l':
        printf("moving right boat right\n");
        rightBoat.x+=.01;
        break;
    case 'j':
        printf("moving right boat left\n");
        rightBoat.x-=.01;
        break;
    default:
        break;
    }
}


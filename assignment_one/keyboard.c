/* You can ignore this for now, it just lets you exit when you press 'q' or ESC */
#include "keyboard.h"



/*There will need to be interactions with extern vars in order to handle 
 * keyboard input from this file*/
void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27:
    case 'q':
        exit(EXIT_SUCCESS); 
		break;
	case ' ':
		printf("Spacebar pressed\n");
		break;
	case 't':
		printf("toggling wave tangents\n");	
		wave_tang_flag = !wave_tang_flag;	
		break;
    case 'w':
        printf("toggling wave wireframe\n");
        wave_wire_flag = !wave_wire_flag;
        break;
    default:
        break;
    }
}


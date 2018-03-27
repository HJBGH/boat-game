/* You can ignore this for now, it just lets you exit when you press 'q' or ESC */
#include "keyboard.h"



/*There will need to be interactions with extern vars in order to handle 
 * keyboard input from this file*/
void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27:/*I take it 27 is esc*/
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
	case '+':
		if(segments <= MAX_T)
		{	
			printf("increasing wave tesselation\n");
			segments++;
			printf("Wave tesselation is incresed to %d\n", segments);
		}
		break;
	case '-':
		if(segments >= MIN_T)
		{	
			printf("decreasing wave tesselation\n");
			segments--;
		}
		break;
    /*movement controls*/
    case 'a':
        printf("moving left boat left\n");
        /*I'm not sure it's a good idea to calculate boat movement in
         * here*/
        if(leftBoat.x >  L_MAX)
            leftBoat.x-=leftBoat.s*g.dt;
        break;
    case 'd':
        printf("moving left boat right\n");
        if(leftBoat.x <= CENTER)
            leftBoat.x+=leftBoat.s*g.dt;
        break;
    case 'l':
        printf("moving right boat right\n");
        if(rightBoat.x < R_MAX)
            rightBoat.x+=rightBoat.s*g.dt;
        break;
    case 'j':
        printf("moving right boat left\n");
        if(rightBoat.x >= CENTER)
            rightBoat.x-=rightBoat.s*g.dt;
        break;
	/*cannon controls*/
	case 'f':
		if(tasmania.gun_elev < 80)
		{
			printf("Island gun rotating left\n");
			tasmania.gun_elev += tasmania.gun_rot_s * g.dt;
		}
		break;
	case 'h':
		if(tasmania.gun_elev > -80)
		{
			printf("Island gun rotating right\n");
			tasmania.gun_elev -= tasmania.gun_rot_s * g.dt;
		}
		break;	
	case 'O':
		if(rightBoat.gun_elev > 10)
		{
			printf("Right boat gun depressing\n");
			rightBoat.gun_elev -= rightBoat.gun_rot_s * g.dt;
		}
		break;
	case 'o':
		if(rightBoat.gun_elev < 80)
		{
			printf("Right boat gun elevating\n");
			rightBoat.gun_elev += rightBoat.gun_rot_s * g.dt;
		}
		break;
	case 'Q':
		if(leftBoat.gun_elev > 10)
		{
			printf("Right boat gun depressing\n");
			leftBoat.gun_elev -= leftBoat.gun_rot_s * g.dt;
		}
		break;
	case 'q':
		if(leftBoat.gun_elev < 80)
		{
			printf("Right boat gun elevating\n");
			leftBoat.gun_elev += leftBoat.gun_rot_s * g.dt;
		}
		break;

    default:
        break;
    }
}


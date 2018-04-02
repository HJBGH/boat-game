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
        wave_wire_flag = true;
        break;
	case 'p':
		wave_wire_flag = false;
		break;
	case '+':
		if(segments < MAX_T)
		{	
			printf("increasing wave tesselation\n");
			segments *= 2;
			printf("Wave tesselation is incresed to %d\n", segments);
		}
		break;
	case '-':
		if(segments > MIN_T)
		{	
			printf("decreasing wave tesselation\n");
			segments /= 2;
			printf("Wave tesseleation is decreased to %d\n", segments);
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
		{
            rightBoat.x+=rightBoat.s*g.dt;
			updateBoatShell(&rightBoat);
		}
		printf("right boat x %f\n", rightBoat.x);
        break;
    case 'j':
        printf("moving right boat left\n");
        if(rightBoat.x >= CENTER)
		{
            rightBoat.x-=rightBoat.s*g.dt;
			updateBoatShell(&rightBoat);	
		}
		printf("right boat x %f\n", rightBoat.x);
        break;
	/*cannon controls*/
	case 'f':
		if(tasmania.gun_elev < 170)
		{
			printf("Island gun rotating left\n");
			tasmania.gun_elev += tasmania.gun_rot_s * g.dt;
			printf("%f - island gun elevation\n", tasmania.gun_elev);
			updateIslandShell(&tasmania);
		}
		break;
	case 'h':
		if(tasmania.gun_elev > 10)
		{
			printf("Island gun rotating right\n");
			tasmania.gun_elev -= tasmania.gun_rot_s * g.dt;
			printf("%f - island gun elevation\n", tasmania.gun_elev);
			updateIslandShell(&tasmania);
		}
		break;	
	case 'g': /*tasmania fires its cannon*/
		/*it might be worthwhile to abstract this into it's own function,
		 * along with a cannon struct*/
		if(tasmania.cd <= 0 && tasmania.shellp != NULL)
		{
			printf("island shell fired\n");
			tasmania.cd = 1;/* 1000 millisecond (1 second) cooldown*/
			(tasmania.shellp)->fired = true; 
			(tasmania.shellp)->loaded = false;
			/*setting fired to true causes the shell to start to change
			 * position, setting loaded to false means other cannons
			 * can use it once it's added back to the pool*/
			tasmania.shellp = NULL;
			/*this shell isn't associated with the cooldown*/
		}
		break;
	case 'O':
		if(rightBoat.gun_elev > 90)
		{
			printf("Right boat gun elevating\n");
			rightBoat.gun_elev -= rightBoat.gun_rot_s * g.dt;
			updateBoatShell(&rightBoat);
		}
		break;
	case 'o':
		if(rightBoat.gun_elev < 180)
		{
			printf("Right boat gun depressing\n");
			rightBoat.gun_elev += rightBoat.gun_rot_s * g.dt;
			updateBoatShell(&rightBoat);
		}
		break;
	case 'i':
		if(rightBoat.shellp != NULL && rightBoat.cd <= 0)
		{
			rightBoat.cd = 1;/* 1000 millisecond (1 second) cooldown*/
			(rightBoat.shellp)->fired = true; 
			(rightBoat.shellp)->loaded = false;
			/*setting fired to true causes the shell to start to change
			 * position, setting loaded to false means other cannons
			 * can use it once it's added back to the pool*/
			rightBoat.shellp = NULL;
			/*this shell isn't associated with the cooldown*/
		}	
		break;
	case 'Q':
		if(leftBoat.gun_elev > 10)
		{
			printf("Left boat gun depressing\n");
			leftBoat.gun_elev -= leftBoat.gun_rot_s * g.dt;
		}
		break;
	case 'q':
		if(leftBoat.gun_elev < 80)
		{
			printf("Left boat gun elevating\n");
			leftBoat.gun_elev += leftBoat.gun_rot_s * g.dt;
		}
		break;
	case 'e':
		if(leftBoat.shellp != NULL && leftBoat.cd <= 0)
		{
			leftBoat.cd = 1;/* 1000 millisecond (1 second) cooldown*/
			(leftBoat.shellp)->fired = true; 
			(leftBoat.shellp)->loaded = false;
			/*setting fired to true causes the shell to start to change
			 * position, setting loaded to false means other cannons
			 * can use it once it's added back to the pool*/
			leftBoat.shellp = NULL;
			/*this shell isn't associated with the cooldown*/
		}	
		break;
    default:
        break;
    }
}


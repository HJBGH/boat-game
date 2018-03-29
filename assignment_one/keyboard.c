/* You can ignore this for now, it just lets you exit when you press 'q' or ESC */
#include "keyboard.h"

void shellHelper(); /*define later*/

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
            rightBoat.x+=rightBoat.s*g.dt;
        break;
    case 'j':
        printf("moving right boat left\n");
        if(rightBoat.x >= CENTER)
            rightBoat.x-=rightBoat.s*g.dt;
        break;
	/*cannon controls*/
	case 'f':
		if(tasmania.gun_elev < 170)
		{
			printf("Island gun rotating left\n");
			tasmania.gun_elev += tasmania.gun_rot_s * g.dt;
			printf("%f - island gun elevation\n", tasmania.gun_elev);
			shellHelper();
		}
		break;
	case 'h':
		if(tasmania.gun_elev > 10)
		{
			printf("Island gun rotating right\n");
			tasmania.gun_elev -= tasmania.gun_rot_s * g.dt;
			printf("%f - island gun elevation\n", tasmania.gun_elev);
			if(tasmania.shellp != NULL)
			{
				(tasmania.shellp)->p.x = ISLAND_GUN_L * 
								cosf((M_PI * tasmania.gun_elev) / 180);
								/*I HATE RADIANS*/
			/*muzzle x co-ord*/	
			printf("Tasmania gun elev -> %f\n", (tasmania.gun_elev));
			printf("I don't even know %f\n", (tasmania.shellp)->p.x);
			(tasmania.shellp)->p.y = 
						ISLAND_GUN_L * sinf((M_PI * tasmania.gun_elev) / 180) 
									+ HEIGHT_OVER_X; 
			/*muzzle y co-ord*/
			/*calculate initial velocities*/
			(tasmania.shellp)->d.x = SHELL_S * cosf((M_PI * tasmania.gun_elev)/180);
			(tasmania.shellp)->d.y = SHELL_S * sinf((M_PI * tasmania.gun_elev)/180);	
			}

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

void shellHelper()
{
	/*recalculates projectile attributes based on cannon orientation,
	 * I should creat a cannon struct with dedicated support for this stuff,
	 * note-to-self for assignment 2*/
	if(tasmania.shellp != NULL)
	{
		(tasmania.shellp)->p.x = ISLAND_GUN_L * 
							cosf((M_PI * tasmania.gun_elev) / 180);
							/*I HATE RADIANS*/
		/*muzzle x co-ord*/	
		printf("Tasmania gun elev -> %f\n", (tasmania.gun_elev));
		printf("I don't even know %f\n", (tasmania.shellp)->p.x);
		(tasmania.shellp)->p.y = 
					ISLAND_GUN_L * sinf((M_PI * tasmania.gun_elev) / 180) 
								+ HEIGHT_OVER_X; 
		/*muzzle y co-ord*/
		/*calculate initial velocities*/
		(tasmania.shellp)->d.x = SHELL_S * cosf((M_PI * tasmania.gun_elev)/180);
		(tasmania.shellp)->d.y = SHELL_S * sinf((M_PI * tasmania.gun_elev)/180);	
	}
}


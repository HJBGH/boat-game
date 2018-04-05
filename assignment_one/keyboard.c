/* You can ignore this for now, it just lets you exit when you press 'q' or ESC */
#include "keyboard.h"
/*There will need to be interactions with extern vars in order to handle 
 * keyboard input from this file*/

void keyUp(unsigned char key, int x, int y)
{
    switch(key)
    {
    case 'a':
    case 'd':
        leftBoat.s = 0;
        break;
    case 'l':
    case 'j':
        rightBoat.s = 0;
        break;
	/*cannon controls*/
	case 'f':
	case 'h':
		tasmania.gun_rot_s = 0;
		break;	
	case 'O':
	case 'o':
		rightBoat.gun_rot_s = 0;
		break;
	case 'Q':
	case 'q':
		leftBoat.gun_rot_s = 0;
		break;
    default:
        printf("pbt");
        break;
    }
}
   

void keyboard(unsigned char key, int x, int y)
{
	if(g.game_over)
	{
		if(key == 27) exit(EXIT_SUCCESS);
		return;
	}
    switch (key)
    {
    case 27:/*27 is esc*/	
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
         * here, it isn't*/
        if(leftBoat.x >  L_MAX)
            leftBoat.s = -BOAT_SPEED;
        break;
    case 'd':
        printf("moving left boat right\n");
        if(leftBoat.x <= CENTER)
            leftBoat.s = BOAT_SPEED;
        break;
    case 'l':
        printf("moving right boat right\n");
        if(rightBoat.x < R_MAX)
            rightBoat.s = BOAT_SPEED;
        break;
    case 'j':
        printf("moving right boat left\n");
        if(rightBoat.x >= CENTER)
            rightBoat.s = -BOAT_SPEED;
        break;
	/*cannon controls*/
	case 'f':
		if(tasmania.gun_elev < 170)
		{
			printf("Island gun rotating left\n");
			tasmania.gun_rot_s = GUN_S;
			printf("%f - island gun elevation\n", tasmania.gun_elev);
			updateIslandShell(&tasmania);
		}
		break;
	case 'h':
		if(tasmania.gun_elev > 10)
		{
			printf("Island gun rotating right\n");
			tasmania.gun_rot_s = -GUN_S;
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
	case 'b':
		if(tasmania.def_cd <= 0 && tasmania.dp != NULL)
		{
			printf("island defensive shell fired\n");
			tasmania.def_cd = 1;/* 1000 millisecond (1 second) cooldown*/
			(tasmania.dp)->proj.fired = true; 
			(tasmania.dp)->proj.loaded = false;
			/*setting fired to true causes the shell to start to change
			 * position, setting loaded to false means other cannons
			 * can use it once it's added back to the pool*/
			tasmania.dp = NULL;
			/*this shell isn't associated with the cooldown*/
		}
		break;
	case 'O':
		if(rightBoat.gun_elev > MAX_B_ELEV)
			rightBoat.gun_rot_s = -GUN_S;
		break;
	case 'o':
		if(rightBoat.gun_elev < MAX_I_ELEV)
			rightBoat.gun_rot_s = GUN_S;
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
	case '8':
		if(rightBoat.dp != NULL && rightBoat.def_cd <= 0)
		{
			printf("firing rightboat defense\n");
			rightBoat.def_cd = 1;
			(rightBoat.dp)->proj.fired = true;
			(rightBoat.dp)->proj.loaded = false;
			rightBoat.dp = NULL;
		}
		break;
	case 'Q':
		if(leftBoat.gun_elev > MIN_ELEV)
			leftBoat.gun_rot_s = -GUN_S;
		break;
	case 'q':
		if(leftBoat.gun_elev < MAX_B_ELEV)
			leftBoat.gun_rot_s = GUN_S;
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
	case '3':
		if(leftBoat.dp != NULL && leftBoat.def_cd <=0)
		{
			printf("firing leftboat defense\n");
			leftBoat.def_cd = 1;
			(leftBoat.dp)->proj.fired = true;
			(leftBoat.dp)->proj.loaded = false;
			leftBoat.dp = NULL;
		}
		break;
    default:
        break;
    }
}


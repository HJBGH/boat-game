#include "keyboard.h"
/*There are pretty much no comments in this file because most of the code in 
 * here is self explanatory and incredibly boring to type about*/

//last minute cannon charge variable
void keyUp(unsigned char key, int x, int y)
{
    switch(key)
    {
    case 'a':
    case 'd':
        tasmania.it.rot_s = 0;
        break;
    case 'w':
    case 's':
        tasmania.it.gun_elev_s = 0;
        break;
    default:
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
    case '0':
        tasmania.hp = 0;
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
    case 'p':
        wave_wire_flag = !wave_wire_flag;
        break;
    case '\'':
        update_time_flag = !(update_time_flag);
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
    /*island cannon controls*/
    case 'a':
        printf("Island gun rotating left\n");
        tasmania.it.rot_s = GUN_S;
        printf("%f - island gun facing\n", tasmania.it.facing);
        break;
    case 'd':
        printf("Island gun rotating right\n");
        tasmania.it.rot_s = -GUN_S;
        printf("%f - island gun facing\n", tasmania.it.facing);
        break;      
    case 'w':
        if(tasmania.it.gun_elev < MAX_ELEV)
        {
            printf("Island gun elevating\n");
            tasmania.it.gun_elev_s = GUN_S;
            printf("%f - island gun elevation\n", tasmania.it.gun_elev);
        }
        break;
    case 's':
        if(tasmania.it.gun_elev > MIN_ELEV)
        {
            printf("Island gun elevating\n");
            tasmania.it.gun_elev_s = -GUN_S;
            printf("%f - island gun elevation\n", tasmania.it.gun_elev);
        }
        break;  
    case 'g': /*tasmania fires its cannon*/
        /*it might be worthwhile to abstract this into it's own function,
         * along with a cannon struct*/
        if(tasmania.it.cd <= 0 && tasmania.it.shellp != NULL)
        {
            
            printf("firing!\n");
            tasmania.it.cd = 1;/* 1000 millisecond (1 second) cooldown*/
            (tasmania.it.shellp)->fired = true; 
            printf("island shell fired\n");
            (tasmania.it.shellp)->loaded = false;
            printf("breach clear!\n");
            /*setting fired to true causes the shell to start to change
             * position, setting loaded to false means other cannons
             * can use it once it's added back to the pool*/
            printf("setting island shell pointer to null\n");
            tasmania.it.shellp = NULL;
            printf("end of firing function\n");
            /*this shell isn't associated with the cooldown*/
        }
        break;
    case 'b':
        if(tasmania.it.def_cd <= 0 && tasmania.it.dp != NULL)
        {
            printf("island defensive shell fired\n");
            tasmania.it.def_cd = 1;/* 1000 millisecond (1 second) cooldown*/
            (tasmania.it.dp)->fired = true; 
            (tasmania.it.dp)->loaded = false;
            /*setting fired to true causes the shell to start to change
             * position, setting loaded to false means other cannons
             * can use it once it's added back to the pool*/
            tasmania.it.dp = NULL;
            /*this shell isn't associated with the cooldown*/
        }
        break;
    default:
        break;
    }
}


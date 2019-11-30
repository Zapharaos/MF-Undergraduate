#include "io.h"
#include "jeu.h"

#include <string.h>

#if CAIRO

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <cairo.h>
#include <cairo-xlib.h>

#define X 100
#define Y 80
#define WIDTH 450
#define BORDER 2

extern cairo_surface_t *sfc;
XClassHint *classHint;//fez

extern cairo_surface_t *cairo_create_x11_surface(int x, int y) {
    Display *dpy;
    Window win;
    Window rootwin;
    int scr;
    
    if ((dpy = XOpenDisplay(NULL)) == NULL) {
        fprintf(stderr, "ERROR: Could not open display\n");
        exit(1);
    }
    
    scr = DefaultScreen(dpy);
    rootwin = RootWindow(dpy, scr);

    win = XCreateSimpleWindow(dpy, rootwin, 1, 1, x, y, 0, 0, 0);

    XStoreName(dpy, win, "Game of Life");
    
    XSelectInput(dpy, win, ExposureMask|ButtonPressMask|KeyPressMask);
    XMapWindow(dpy, win);

    sfc = cairo_xlib_surface_create(dpy, win, DefaultVisual(dpy, scr), x, y);
    cairo_xlib_surface_set_size(sfc, x, y);

    return sfc;
}

extern void cairo_close_x11_surface() {
    Display *dpy = cairo_xlib_surface_get_display(sfc);
    cairo_surface_destroy(sfc);
    XCloseDisplay(dpy);
}

void affiche_trait (int c, int height, float line){
    cairo_t *cr;
    cr = cairo_create(sfc);

    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_set_line_width(cr, BORDER);

    cairo_move_to(cr, X, Y + (height * line));

    int i;
    for (i = 0; i < c; ++i){
        cairo_line_to(cr, X + WIDTH, Y + (height * line));
        cairo_move_to(cr, X, Y + (height * line));
    }
    
    cairo_stroke(cr);
    cairo_destroy(cr);
}

void affiche_ligne (int c, int* ligne, int aging, int height, float line){
    int i;
    char buffer[255];
    float column = (float) WIDTH / (float) c;

    cairo_t *cr, *crcells;
    cr = cairo_create(sfc);
    crcells = cairo_create(sfc);
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_set_source_rgb(crcells, 0, 0, 0);
    cairo_set_line_width(cr, BORDER);

    cairo_move_to(cr, X, Y + (height * line));

    for (i=0; i<c; ++i){

        if (ligne[i] == 0){
            cairo_line_to(cr, X + i * column, Y + (height * line) + line);
            cairo_move_to(cr, X + ((i+1) * column), Y + (height * line));
            cairo_line_to(cr, X + ((i+1) * column), Y + (height * line) + line);
        } else if (ligne[i] == -1){
            cairo_line_to(cr, X + i * column, Y + (height * line) + line);
            cairo_line_to(cr, X + ((i+1) * column), Y + (height * line));
            cairo_line_to(cr, X + ((i+1) * column), Y + (height * line) + line);
            
            cairo_line_to(cr, X + i * column, Y + (height * line) + line);
            cairo_move_to(cr, X + ((i) * column), Y + (height * line));
            cairo_line_to(cr, X + ((i+1) * column), Y + (height * line) + line);
            cairo_line_to(cr, X + ((i+1) * column), Y + (height * line));
        } else{
            cairo_line_to(cr, X + i * column, Y + (height * line) + line);
            cairo_move_to(cr, X + ((i-1) * column), Y + (height * line));
            cairo_rectangle(crcells, X + i * column, Y + (height * line), column + 1, line);
            cairo_fill(crcells);

        if (aging){
            sprintf(buffer, "%d", ligne[i]);
            cairo_move_to(cr, X + ((i) * column + 4) + column/3, Y + (height * line) + line*2/3);
            cairo_set_source_rgb (cr, 255, 255, 255);
            cairo_set_font_size(cr, 15);
            cairo_show_text(cr, buffer);
            cairo_set_source_rgb(cr, 0, 0, 0);
            }
               
            cairo_move_to(cr, X + ((i+1) * column), Y + (height * line));
            }
    }
    
    cairo_fill(crcells);
    cairo_stroke(cr);
    cairo_destroy(cr);
    cairo_destroy(crcells);
    
    return;
}

void affiche_grille (grille g, int timer, int cycle, int aging, int swing){
    int i, l=g.nbl, c=g.nbc;

    char strTimer[255], strCycle[255], strAging[255], strSwing[255], strCount[255];
    
    sprintf(strTimer, "Timer : %d", timer);
    sprintf(strCycle, cycle ? " Cycle : ON" : "Cycle : OFF");
    sprintf(strAging, aging ? " Aging : ON" : "Aging : OFF");
    sprintf(strSwing, " Swing : ON");

    cairo_t *cr;
    cr = cairo_create(sfc);
    
    cairo_select_font_face(cr, "Arial", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_set_font_size(cr, 18);
    
    cairo_move_to(cr, 160, 40);
    cairo_show_text(cr, strTimer);
    
    if (swing>1){
        cairo_set_source_rgb(cr, 255, 0, 0);
        sprintf(strCount, "It took %d steps", swing);
        cairo_move_to(cr, 360, 40);
        cairo_show_text(cr, strCount);
        cairo_set_source_rgb(cr, 0, 0, 0);
    }
    
    cairo_move_to(cr, 150, 65);
    cairo_show_text(cr, strCycle);
    cairo_move_to(cr, 270, 65);
    cairo_show_text(cr, strAging);
    cairo_move_to(cr, 390, 65);
    cairo_show_text(cr, strSwing);

    cairo_move_to(cr, 140, 580);
    cairo_show_text(cr, "Left click = Evolve");
    cairo_move_to(cr, 360, 580);
    cairo_show_text(cr, "Right click = Exit");
    
    cairo_move_to(cr, 140, 610);
    cairo_show_text(cr, "n = Switch file");
    cairo_move_to(cr, 360, 610);
    cairo_show_text(cr, "c = Switch cycle");
    
    cairo_move_to(cr, 140, 640);
    cairo_show_text(cr, "v = Switch aging");
    cairo_move_to(cr, 360, 640);
    cairo_show_text(cr, "o = Prints periode");

    cairo_destroy(cr);

    int height = 0;
    float line = (float) WIDTH/ (float) g.nbl;
    affiche_trait(c, height, line);
    for (i=0; i<l; ++i){
        affiche_ligne(c, g.cellules[i], aging, height, line);
        affiche_trait(c, ++height, line);
    }
    return;
}

void efface_grille(){
    cairo_t *cr;
    cr = cairo_create(sfc);
    cairo_set_source_rgb (cr, 255, 255, 255);
    cairo_paint(cr);
    cairo_destroy(cr);
}

void debut_jeu(grille *g, grille *gc){
	int timer = 1;
    int cycle = 1;
    int aging = 1;
    int swing = 1;
    int jump = 0;
    int (*compte_voisins_vivants) (int, int, grille) = compte_voisins_vivants_cyclique;
    
    XEvent e;
    
    grille g_bis;
    alloue_grille (g->nbl, g->nbc, &g_bis);
    copie_grille(*g, g_bis);

    while(1){
        XNextEvent(cairo_xlib_surface_get_display(sfc), &e);
            
        if (e.type==Expose && e.xexpose.count<1){
            affiche_grille(*g, timer, cycle, aging, swing);
        }
        else if (e.type == KeyPress){
            if (e.xkey.keycode == XKeysymToKeycode (cairo_xlib_surface_get_display(sfc),'n')){ // Touche n

                libere_grille(g);
                libere_grille(gc);
                cairo_close_x11_surface();

                char path[20] = "grilles/grille";
                char nb[1];

                printf("Numero: ");
                scanf("%s", nb);
                
                strcat(path, nb);
                strcat(path, ".txt");
                
                init_grille_from_file(path, g);
                alloue_grille((*g).nbl, (*g).nbc, gc);

                sfc = cairo_create_x11_surface(760, 760);
                debut_jeu(g, gc);
            }
            else if (e.xkey.keycode == XKeysymToKeycode (cairo_xlib_surface_get_display(sfc),'c')){ // Touche c
                if (cycle == 0){
                    cycle = 1;
                    compte_voisins_vivants = &(compte_voisins_vivants_cyclique);
                } else{
                    cycle = 0;
                    compte_voisins_vivants = &(compte_voisins_vivants_non_cyclique);
                }
                jump = 1;
            }
            else if (e.xkey.keycode == XKeysymToKeycode (cairo_xlib_surface_get_display(sfc),'v')){ // Touche v
                aging = !aging;
                jump = 1;
            }
            else if (e.xkey.keycode == XKeysymToKeycode (cairo_xlib_surface_get_display(sfc),'o')){ // Touche o
                
                int count = 0;
                
                grille temp;
                alloue_grille(g_bis.nbl, g_bis.nbc, &temp);
                copie_grille(g_bis, temp);
                // Pour tester l'oscillation depuis la position courante, il faut remplacer g_bis par g
                
                while(count < 300){
                    swing = egale_grille(g_bis, temp);
                    if (count==0) evolue(&temp,gc,&count,compte_voisins_vivants,aging);
                    else if (swing){
                        swing = count;
                        count = 300;
                    }
                    else{
                        evolue(&temp,gc,&count,compte_voisins_vivants,aging);
                    }
                }
                jump = 1;
            }
            else {
                efface_grille();
                affiche_grille(*g, timer, cycle, aging, swing);
            }
        }
        else if(jump){
            efface_grille();
            affiche_grille(*g, timer, cycle, aging, swing);
            jump = 0;
        }
        else if (e.type == ButtonPress){
             if (e.xbutton.button == 1){ // Clic gauche (fait évoluer le jeu)
                 evolue(g,gc,&timer,compte_voisins_vivants,aging);
                 efface_grille();
                 affiche_grille(*g, timer, cycle, aging, swing);
             }
             else if (e.xbutton.button == 3){ // Clic droit (quitte le jeu)
                 return;
             }
            else {
                efface_grille();
                affiche_grille(*g, timer, cycle, aging, swing);
            }
        }
        else {
            efface_grille();
            affiche_grille(*g, timer, cycle, aging, swing);
        }
    }
    return;
}

#else

void affiche_trait (int c){
    int i;
    for (i=0; i<c; ++i) printf ("|---");
    printf("|\n");
    return;
}

void affiche_ligne (int c, int* ligne, int aging){
    int i;
    if (aging){
        for (i=0; i<c; ++i){
            if (ligne[i] == 0 ) printf ("|   ");
            else if (ligne[i] == -1) printf ("| X ");
            else printf ("| %d ", ligne[i]);
        }
    }
    else{
        for (i=0; i<c; ++i){
            if (ligne[i] == 0 ) printf ("|   ");
            else if (ligne[i] == -1) printf ("| X ");
            else printf ("| 0 ");
        }
    }
    printf("|\n");
    return;
}

void affiche_grille (grille g,  int timer, int cycle, int aging, int swing){
    int i, l=g.nbl, c=g.nbc;
    
    printf("\n");
    printf("\e[K");
    
    printf("%d  |", timer);
    if (cycle) printf("Cycle : ON |");
    if (aging) printf("Aging : ON |");
    if (swing) printf("Swing : ON |");
    
    if (swing>1) printf("Periode : %d\n", swing);
    else printf("\n");
    
    affiche_trait(c);
    for (i=0; i<l; ++i) {
        affiche_ligne(c, g.cellules[i], aging);
        affiche_trait(c);
    }
    printf("\n");
    return;
}

void efface_grille (grille g){
    printf("\n\e[%dA",g.nbl*2 + 7);
}

void debut_jeu(grille *g, grille *gc){
    int timer = 1;
    int cycle = 1;
    int aging = 1;
    int swing = 1;
    int jump = 0;
    int (*compte_voisins_vivants) (int, int, grille) = compte_voisins_vivants_cyclique;

    grille g_bis;
    alloue_grille (g->nbl, g->nbc, &g_bis);
    copie_grille(*g, g_bis);
    
	char c = getchar(); 
	while (c != 'q'){
        switch (c){
            case '\n' :
            { // touche "entree" pour évoluer
                if (jump){
                    jump = 0;
                }
                else{
                    evolue(g,gc, &timer, compte_voisins_vivants, aging);
                    efface_grille(*g);
                    printf("\n");
                    affiche_grille(*g, timer, cycle, aging, swing);
                }
                break;
            }
            case 'n' :
            { // touche "n" pour entrer le nom d'une nouvelle grille
                libere_grille(g);
                libere_grille(gc);
                
                char path[20] = "grilles/grille";
                char nb[1];
                
                printf("Numero de la nouvelle grille a charger : ");
                scanf("%s", nb);
                
                strcat( path, nb);
                strcat( path, ".txt");

                timer = 1;
                init_grille_from_file(path, g);

                alloue_grille((*g).nbl, (*g).nbc, gc);
                affiche_grille(*g, timer, cycle, aging, swing);
                
                printf("\n\e[2A");
                printf("\n");
                
                jump = 1;
                break;
            }
            case 'c' :
            {
                if (cycle){
                    cycle = 0;
                    compte_voisins_vivants = &(compte_voisins_vivants_non_cyclique);
                } else{
                    cycle = 1;
                    compte_voisins_vivants = &(compte_voisins_vivants_cyclique);
                }
                
                printf("\e[A");
                printf("\e[K");
                printf("\n");
                break;
            }
            case 'v' :
            {
                aging = !aging;
                
                printf("\e[A");
                printf("\e[K");
                printf("\n");
                
                break;
            }
            case 'o' :
            {
                efface_grille(*g);
                int count = 0;
                
                grille temp;
                alloue_grille(g_bis.nbl, g_bis.nbc, &temp);
                copie_grille(g_bis, temp);
                
                while(count < 300){
                    swing = egale_grille(g_bis, temp);
                    if (count==0) evolue(&temp,gc,&count,compte_voisins_vivants,aging);
                    else if (swing){
                        swing = count;
                        count = 300;
                    }
                    else{
                        evolue(&temp,gc,&count,compte_voisins_vivants,aging);
                    }
                }
                
                break;
            }
            default :
            { // touche non traitée
                printf("\e[A");
                printf("\e[K");
                printf("\n");
                
                break;
            }
        }
        c = getchar();
	}
	return;	
}

#endif

#include "./gui.h"

#include <math.h>
#include <stdio.h>
#include <limits.h>

struct vec2 {
    double x;
    double y;
};

//transpose from graph space to screen space
vec2 transpose(vec2 min, double scale, vec2 vector)
{
    return (vec2) {
        (vector.x - min.x) * scale,
        (vector.y - min.y) * scale
    };
}



void disp::gui_lignes(
    const graphe* g, 
    const chemin* chemins, const size_t n_ch, 
    const size_t* pts,     const size_t n_pt
) {

    const size_t X_SIZE = 4000;
    const size_t Y_SIZE = 4000;

    al_init();
    al_install_mouse();
    al_init_primitives_addon();

    bool done = false;
    ALLEGRO_DISPLAY* disp = al_create_display(1000, 700);
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    ALLEGRO_TIMER* timer = al_create_timer(1.0/60.0);
    ALLEGRO_BITMAP* image = al_create_bitmap(X_SIZE, Y_SIZE);

    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_mouse_event_source());

    //calculate the space required to draw the graph
    double minx = +INFINITY;
    double miny = +INFINITY;
    double maxx = -INFINITY;
    double maxy = -INFINITY;

    for (size_t i=0;i<g->sommets(); i++)
    {
        sommet data = g->get_meta(i);
        minx = MIN(data.mapx, minx);
        miny = MIN(data.mapy, miny);
        maxx = MAX(data.mapx, maxx);
        maxy = MAX(data.mapy, maxy);
    }

    printf("%lf, %lf, %lf, %lf\n", minx, maxx, miny, maxy);

    double scale = MIN(X_SIZE/(maxx-minx), Y_SIZE/(maxy-miny));
    vec2 vzero = (vec2){minx, miny};

    printf("%lf\n", scale);

    ///////////////////////////

    al_set_target_bitmap(image);
    al_clear_to_color(al_map_rgb(0, 0, 0));

    // for (size_t i=0; i<g->sommets(); i++)
    // {
    //     vec2 spos = transpose(vzero, scale, {g->get_meta(i).mapx, g->get_meta(i).mapy});
    //     al_draw_filled_circle(
    //         spos.x, spos.y, 2.0, al_map_rgb(100, 100, 100)
    //     );
    // }



    for (size_t i=0; i<g->sommets(); i++)
    {
        vec2 ipos = transpose(vzero, scale, {g->get_meta(i).mapx, g->get_meta(i).mapy});
        
        for (int j=0; j<g->arite(i); j++)
        {
            sommet sj = g->get_meta(g->aretes(i)[j].dest);
            vec2 jpos = transpose(vzero, scale, {sj.mapx, sj.mapy});

            //printf("(%lf,%lf)\n", jpos.x, jpos.y);

            al_draw_line(
                ipos.x, ipos.y, jpos.x, jpos.y,
                al_map_rgb(90, 90, 90), 8.0
            );
        }
    }
    for (size_t i=0; i<n_pt; i++)
    {
        vec2 ipos = transpose(vzero, scale, {g->get_meta(pts[i]).mapx, g->get_meta(pts[i]).mapy});
        al_draw_filled_circle(
            ipos.x, ipos.y, 30.0, al_map_rgb(255, 128, 0)
        );
    }

    const ALLEGRO_COLOR colors[6] = {
        al_map_rgb(255, 0, 0),
        al_map_rgb(0, 255, 0),
        al_map_rgb(0, 0, 255),
        al_map_rgb(255, 255, 0),
        al_map_rgb(255, 0, 255),
        al_map_rgb(0, 255, 255)
    };

    for (size_t ligne=0; ligne<n_ch; ligne++)
    {
        if (chemins[ligne].head == NULL) {
            continue;
        }

        for(list *pt=chemins[ligne].head; pt->suiv != NULL; pt=pt->suiv)
        {
            sommet si = g->get_meta(pt->sommet);
            sommet sj = g->get_meta(pt->suiv->sommet);
            vec2 posi = transpose(vzero, scale, {si.mapx, si.mapy});
            vec2 posj = transpose(vzero, scale, {sj.mapx, sj.mapy});
            al_draw_line(
                posi.x, posi.y, posj.x, posj.y,
                colors[ligne % 6], 10.0
            );
        }
    }

    scale = MIN(800.0/X_SIZE, 600.0/Y_SIZE);
    double angle = 0;
    double px = 100;
    double py = 100;
    bool pressed = false;

    al_set_mouse_z(0); //reset scroll 

    //////////////////////////

    bool frame = false;
    ALLEGRO_EVENT event;
    al_start_timer(timer);
    while (!done)
    {
        al_wait_for_event(queue, &event);

        switch (event.type)
        {
            case ALLEGRO_EVENT_TIMER:
                frame = true;
                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;

            case ALLEGRO_EVENT_MOUSE_AXES:
                if (event.mouse.z > 0)
                {
                    scale += 0.1;
                    al_set_mouse_z(0);
                }
                if (event.mouse.z < 0)
                {
                    if (scale >= 0.1) {
                        scale -= 0.1;
                    }
                    al_set_mouse_z(0);
                }
                if (pressed) {
                    px += event.mouse.dx * abs(event.mouse.dx);
                    py += event.mouse.dy * abs(event.mouse.dy);
                    //al_set_mouse_xy(disp, 400, 300);
                }
                break;

            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                if (event.mouse.button == 1) {
                    al_grab_mouse(disp);
                    al_hide_mouse_cursor(disp);
                    pressed = true;
                }
                break;
            
            case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
                if (event.mouse.button == 1) {
                    al_ungrab_mouse();
                    al_show_mouse_cursor(disp);
                    pressed = false;
                }
                break;

            default:
                break;
        }
        if (frame) {
            al_set_target_backbuffer(disp);
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_scaled_rotated_bitmap(
                image, 
                0, 0,         //center of rotation
                px, py,       //destination of center
                scale, scale, //scale
                angle,        //angle
                0
            );
            al_flip_display();
        }
    }


    al_destroy_display(disp);
    al_destroy_event_queue(queue);
    al_destroy_timer(timer);
}  
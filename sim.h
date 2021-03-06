#ifndef SIM_H
#define SIM_H

#include <string>
#include <list>
using namespace std;

#include "geometry.h"
#include "maths.h"

class Sim {

  public:
    static Sim* instance();
    ~Sim();

    static void init(int* argc, char* argv[], const string& title);

    static void add_geometry(Geometry* geom);

    static void run();
    static void quit();

    static bool is_key_pressed(int key);
    static bool is_key_released(int key);
    static bool is_key_held(int key);

    static void set_cam_x(float x);
    static void set_cam_y(float y);
    static void set_cam_z(float z);

    static float get_cam_x();
    static float get_cam_y();
    static float get_cam_z();

    static void set_rot_x(float x);
    static void set_rot_y(float y);
    static void set_rot_z(float z);

    static float get_rot_x();
    static float get_rot_y();
    static float get_rot_z();

    static mat4& get_view_matrix();
    static mat4& get_projection_matrix();
    static mat4& get_ortho_matrix();

    static void set_phys_callback(void (*cb)(float,void*), void* data);

    static void set_wired(bool wired);
    static bool get_wired();

  protected:

    static vec3 cam_pos;
    static vec3 cam_angles;

    static mat4 view_matrix;
    static mat4 proj_matrix;
    static mat4 ortho_matrix;
    static mat4 vp;

    Sim();
    static void look();
    static void reshape(int,int);
    static list<Geometry*> geoms;
    static Sim* self;
    static void (*callback)(float,void*);
    static void* cb_data;
    static bool running;
    static bool wire;

    static void keyboard_clear();
    static void keyboard_down(unsigned char, int, int);
    static void keyboard_up(unsigned char, int, int);
    static void keyboard_special_down(int, int, int);
    static void keyboard_special_up(int, int, int);
    static bool key_presses[];
    static bool key_releases[];
    static bool key_holds[];

};

#endif /* SIM_H */

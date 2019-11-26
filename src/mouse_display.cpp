#include "main.h"

namespace mouse_display {

/* start of gui code */

/**********************
 * function prototype
 **********************/
void create_tab1(lv_obj_t* parent);
void create_tab2(lv_obj_t* parent);
void position_tracking(lv_obj_t* parent);

// using alien theme
lv_theme_t* theme;

void start_display() {
    printf("starting display!\n");
    theme = lv_theme_material_init(210, NULL);
    lv_theme_set_current(theme);

    lv_obj_t* tv = lv_tabview_create(lv_scr_act(), NULL);
    lv_obj_set_size(tv, lv_obj_get_width(lv_scr_act()), lv_obj_get_height(lv_scr_act()));

    lv_obj_t* tab1 = lv_tabview_add_tab(tv, "Tab 1");
    lv_obj_t* tab2 = lv_tabview_add_tab(tv, "Tab 2");
    lv_obj_t* tab3 = lv_tabview_add_tab(tv, "pos");

    create_tab1(tab1);
    create_tab2(tab2);
    position_tracking(tab3);
}

/*************
 * tab views
 *************/
void create_tab1(lv_obj_t* parent) {
    lv_obj_t* label = lv_label_create(parent, NULL);
    lv_label_set_text(label, "Hello World!");
    lv_obj_align(label, NULL, LV_ALIGN_CENTER, 0, 0);
}

void create_tab2(lv_obj_t* parent) {
    static lv_style_t h_style;
    lv_obj_t* btn = lv_btn_create(parent, NULL);
    lv_btn_set_state(btn, LV_BTN_STATE_REL);
}

/********************
 * position display
 ********************/
lv_obj_t* pos_label;

void update_position(float x, float y) {
    char s_value[50];
    sprintf(s_value, "x: %f\ny: %f", 0.0, 0.0);
    lv_label_set_text(pos_label, s_value);
}

void position_tracking(lv_obj_t* parent) {
    pos_label = lv_label_create(parent, NULL);
    lv_obj_align(pos_label, NULL, LV_ALIGN_CENTER, 0, 0);

    update_position(0.0, 0.0);
}

/* end of gui code */

};
#include "../hid_reader.h"

enum SubmenuIndex {
    SubmenuIndexSettings = 10,
};

void hid_reader_scene_menu_submenu_callback(void* context, uint32_t index) {
    HidReader* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void hid_reader_scene_menu_on_enter(void* context) {
    HidReader* app = context;

    submenu_add_item(
        app->submenu,
        "Settings",
        SubmenuIndexSettings,
        hid_reader_scene_menu_submenu_callback,
        app);

    submenu_set_selected_item(
        app->submenu, scene_manager_get_scene_state(app->scene_manager, HidReaderSceneMenu));

    view_dispatcher_switch_to_view(app->view_dispatcher, HidReaderViewIdMenu);
}

bool hid_reader_scene_menu_on_event(void* context, SceneManagerEvent event) {
    HidReader* app = context;
    UNUSED(app);
    if(event.type == SceneManagerEventTypeBack) {
        //exit app
        scene_manager_stop(app->scene_manager);
        view_dispatcher_stop(app->view_dispatcher);
        return true;
    } else if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == SubmenuIndexSettings) {
            scene_manager_set_scene_state(
                app->scene_manager, HidReaderSceneMenu, SubmenuIndexSettings);
            scene_manager_next_scene(app->scene_manager, HidReaderSceneSettings);
            return true;
        }
    }
    return false;
}

void hid_reader_scene_menu_on_exit(void* context) {
    HidReader* app = context;
    submenu_reset(app->submenu);
}

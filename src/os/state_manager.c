#include "state_manager.h"
#include "window_control.h"
#include "input_helper.h"

static void _vx_statemanager_init(vx_StateManager* manager, vx_WindowControl* window) {
    manager->callbacks.first_init(manager->general_data);

    manager->current_state = VX_T(vx_State, vx_hashmap_get)(&manager->states, manager->current_state_UID);
    VX_ASSERT("Could not find state in state manager", manager->current_state != NULL);

    manager->current_state->init(manager->general_data, manager->current_state->userData, window);
}

static void _vx_statemanager_logic(vx_StateManager* manager, vx_WindowControl* window, vx_WindowInputHelper* input) {
    vx_StateUID returned_UID = manager->current_state->logic(manager->general_data, manager->current_state->userData, window, input);

    if (returned_UID != manager->current_state_UID) {
        manager->current_state->close(manager->general_data, manager->current_state->userData, window);

        manager->callbacks.state_change(manager->general_data, manager->current_state->userData, returned_UID, manager->current_state_UID);

        manager->current_state_UID = returned_UID;
        manager->current_state = VX_T(vx_State, vx_hashmap_get)(&manager->states, manager->current_state_UID);
        VX_ASSERT("Could not find new state in state manager", manager->current_state != NULL);

        manager->current_state->init(manager->general_data, manager->current_state->userData, window);
    }
}

static void _vx_statemanager_draw(vx_StateManager* manager) {
    manager->current_state->draw(manager->general_data, manager->current_state->userData);
}

static void _vx_statemanager_resize(vx_StateManager* manager, vx_WindowControl* window, u32 width, u32 height) {
    manager->current_state->resize(manager->general_data, manager->current_state->userData, window, width, height);
}

static void _vx_statemanager_close(vx_StateManager* manager, vx_WindowControl* window) {
    manager->callbacks.close(manager->general_data);
}

vx_State vx_state_from_statedescriptor(vx_StateDescriptor* descriptor) {
    vx_State state;

    state.userData = descriptor->user_data;
    state.init = VX_SAFE_FUNC_PTR(descriptor->init);
    state.logic = descriptor->logic;
    state.draw = VX_SAFE_FUNC_PTR(descriptor->draw);
    state.resize = VX_SAFE_FUNC_PTR(descriptor->resize);
    state.close = VX_SAFE_FUNC_PTR(descriptor->close);

    VX_ASSERT("The logic function in a state cannot be NULL!", state.logic != NULL);

    return state;
}

vx_StateManager vx_statemanager_new(vx_StateManagerDescriptor* descriptor) {
    vx_StateManager manager;

    manager.states = VX_T(vx_State, vx_hashmap_new)();

    manager.callbacks.first_init = VX_SAFE_FUNC_PTR(descriptor->first_init);
    manager.callbacks.state_change = VX_SAFE_FUNC_PTR(descriptor->state_change);
    manager.callbacks.close = VX_SAFE_FUNC_PTR(descriptor->close);

    manager.general_data = descriptor->general_data;

    return manager;
}

void vx_statemanager_free(vx_StateManager* manager) {
    VX_T(vx_State, vx_hashmap_free)(&manager->states);
}

void vx_statemanager_run(vx_StateManager* manager, vx_Window* window, vx_StateUID first_state) {
    manager->current_state_UID = first_state;

    window->callback_functions.init = _vx_statemanager_init;
    window->callback_functions.logic = _vx_statemanager_logic;
    window->callback_functions.draw = _vx_statemanager_draw;
    window->callback_functions.resize = _vx_statemanager_resize;
    window->callback_functions.close = _vx_statemanager_close;

    vx_window_run(window, manager);
}

void vx_statemanager_register_state(vx_StateManager* manager, vx_StateUID UID, vx_StateDescriptor* descriptor) {
    vx_State state = vx_state_from_statedescriptor(descriptor);
    state.UID = UID;

    VX_T(vx_State, vx_hashmap_insert)(&manager->states, state, UID);
}
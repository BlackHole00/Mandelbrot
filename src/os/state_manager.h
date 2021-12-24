#pragma once

#include <vx_utils.h>

#include "window.h"
#include "user_state_ptr.h"

typedef u32 vx_StateUID;

typedef struct vx_State {
    vx_StateUID UID;
    vx_UserStatePtr userData;

    VX_CALLBACK(init, void, vx_UserStatePtr general_data, vx_UserStatePtr state_data, vx_WindowControl* window);
    VX_CALLBACK(logic, vx_StateUID, vx_UserStatePtr, vx_UserStatePtr state_data, vx_WindowControl* window, vx_WindowInputHelper* input);
    VX_CALLBACK(draw, void, vx_UserStatePtr general_data, vx_UserStatePtr state_data);
    VX_CALLBACK(resize, void, vx_UserStatePtr general_data, vx_UserStatePtr state_data, vx_WindowControl* window, u32 width, u32 height);
    VX_CALLBACK(close, void, vx_UserStatePtr general_data, vx_UserStatePtr state_data, vx_WindowControl* window);

    vx_StateUID prev_state;
} vx_State;
_VX_OPTION_CREATE_FOR_TYPE(vx_State)
_VX_VECTOR_CREATE_FOR_TYPE(vx_State)
_VX_HASHMAP_CREATE_FOR_TYPE(vx_State)

typedef struct vx_StateDescriptor {
    vx_StateUID UID;
    vx_UserStatePtr user_data;

    VX_CALLBACK(init, void, vx_UserStatePtr general_data, vx_UserStatePtr state_data, vx_WindowControl* window);
    VX_CALLBACK(logic, vx_StateUID, vx_UserStatePtr general_data, vx_UserStatePtr state_data, vx_WindowControl* window, vx_WindowInputHelper* input);
    VX_CALLBACK(draw, void, vx_UserStatePtr general_data, vx_UserStatePtr state_data);
    VX_CALLBACK(resize, void, vx_UserStatePtr general_data, vx_UserStatePtr state_data, vx_WindowControl* window, u32 width, u32 height);
    VX_CALLBACK(close, void, vx_UserStatePtr general_data, vx_UserStatePtr state_data, vx_WindowControl* window);
} vx_StateDescriptor;
VX_CREATE_DEFAULT(vx_StateDescriptor,
    .UID = 0,
    .user_data = NULL,
    .init = NULL,
    .logic = NULL,
    .draw = NULL,
    .resize = NULL,
    .close = NULL
)

typedef struct vx_StateManagerDescriptor {
    vx_UserStatePtr general_data;

    VX_CALLBACK(first_init, void, vx_UserStatePtr general_data);
    VX_CALLBACK(state_change, void, vx_UserStatePtr general_data, vx_UserStatePtr prev_state_data, vx_StateUID new_state_UID, vx_StateUID old_state_UID);
    VX_CALLBACK(close, void, vx_UserStatePtr general_data);
} vx_StateManagerDescriptor;
VX_CREATE_DEFAULT(vx_StateManagerDescriptor,
    .general_data = NULL,
    .first_init = NULL,
    .state_change = NULL,
    .close = NULL
)

typedef struct {
    VX_T(vx_State, vx_HashMap) states;
    vx_UserStatePtr general_data;

    vx_StateUID current_state_UID;
    vx_State* current_state;

    struct {
        VX_CALLBACK(first_init, void, vx_UserStatePtr general_data, vx_UserStatePtr state_data, vx_StateUID init_state_UID);
        VX_CALLBACK(state_change, void, vx_UserStatePtr general_data, vx_UserStatePtr prev_state_data, vx_StateUID new_state_UID, vx_StateUID old_state_UID);
        VX_CALLBACK(close, void, vx_UserStatePtr general_data, vx_UserStatePtr state_data, vx_StateUID current_state_UID);
    } callbacks;
} vx_StateManager;

vx_State vx_state_from_statedescriptor(vx_StateDescriptor* descriptor);

vx_StateManager vx_statemanager_new(vx_StateManagerDescriptor* descriptor);
void vx_statemanager_free(vx_StateManager* manager);
void vx_statemanager_run(vx_StateManager* manager, vx_Window* window, vx_StateUID first_state);
void vx_statemanager_register_state(vx_StateManager* manager, vx_StateUID UID, vx_StateDescriptor* descriptor);

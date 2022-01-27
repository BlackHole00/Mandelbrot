/** 
 * @file os/state_manager.h
 * @author Vicix
 * @date 01/01/2022
 */
#pragma once

#include <vx_utils.h>

#include "window.h"
#include "user_state_ptr.h"

typedef u32 vx_StateUID;

/**
 * @brief vx_State identifies the behaviour of a state and hold a pointer to the state's data.
 * @note vx_Option, vx_Vector and vx_HashMap available.
 */
typedef struct vx_State {
    /**
     * The unique identifier of this state. It's redundant: it is the key of the hashmap in the vx_StateManager.
     */
    vx_StateUID UID;

    /**
     * A Pointer th the user data.
     */
    vx_UserStatePtr userData;

    /**
     * Called everytime the state is loaded.
     */
    VX_CALLBACK(init, void, vx_UserStatePtr general_data, vx_UserStatePtr state_data, vx_WindowControl* window);

    /**
     * Called every frame. It should be used only for application logic.
     * @return the vx_StateUID of the next state. If it is the same as the current state, then there will not be any state change.
     */
    VX_CALLBACK(logic, vx_StateUID, vx_UserStatePtr, vx_UserStatePtr state_data, vx_WindowControl* window, vx_WindowInputHelper* input);

    /**
     * Called every frame. It should be used only for application drawing.
     */
    VX_CALLBACK(draw, void, vx_UserStatePtr general_data, vx_UserStatePtr state_data);

    /**
     * Called every time the window is resized.
     */
    VX_CALLBACK(resize, void, vx_UserStatePtr general_data, vx_UserStatePtr state_data, vx_WindowControl* window, u32 width, u32 height);

    /**
     * Called every time there is a state change.
     */
    VX_CALLBACK(close, void, vx_UserStatePtr general_data, vx_UserStatePtr state_data, vx_WindowControl* window);

    vx_StateUID prev_state;
} vx_State;
_VX_OPTION_CREATE_FOR_TYPE(vx_State)
_VX_VECTOR_CREATE_FOR_TYPE(vx_State)
_VX_HASHMAP_CREATE_FOR_TYPE(vx_State)

/**
 * @brief The state descriptor. It's used by the user to define how a state should behave and what data should use.
 * @note VX_DEFAULT available.
 */
typedef struct vx_StateDescriptor {
    vx_StateUID UID;
    vx_UserStatePtr user_data;

    VX_CALLBACK(init, void, vx_UserStatePtr global_data, vx_UserStatePtr state_data, vx_WindowControl* window);
    VX_CALLBACK(logic, vx_StateUID, vx_UserStatePtr global_data, vx_UserStatePtr state_data, vx_WindowControl* window, vx_WindowInputHelper* input);
    VX_CALLBACK(draw, void, vx_UserStatePtr global_data, vx_UserStatePtr state_data);
    VX_CALLBACK(resize, void, vx_UserStatePtr global_data, vx_UserStatePtr state_data, vx_WindowControl* window, u32 width, u32 height);
    VX_CALLBACK(close, void, vx_UserStatePtr global_data, vx_UserStatePtr state_data, vx_WindowControl* window);
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

/**
 * @brief The state manager descriptor. It's used by the user to define how the state manager should behave and what global data should use.
 * @note VX_DEFAULT available.
 */
typedef struct vx_StateManagerDescriptor {
    vx_UserStatePtr global_data;

    VX_CALLBACK(first_init, void, vx_UserStatePtr global_data);
    VX_CALLBACK(state_change, void, vx_UserStatePtr global_data, vx_UserStatePtr prev_state_data, vx_StateUID new_state_UID, vx_StateUID old_state_UID);
    VX_CALLBACK(close, void, vx_UserStatePtr global_data);
} vx_StateManagerDescriptor;
VX_CREATE_DEFAULT(vx_StateManagerDescriptor,
    .global_data = NULL,
    .first_init = NULL,
    .state_change = NULL,
    .close = NULL
)

/**
 * vx_StateManager is a state manager. It is build on top of a Window.
 * It provides a per-state data, accessible by each state and a global data, always accessible.
 * Note: All the callbacks of vx_Window will be overwritten. Use the states instead.
 * @brief A State Manager built on top of vx_Window.
 */
typedef struct vx_StateManager {
    /**
     * An hashmap that holds all the states. 
     */
    VX_T(vx_State, vx_HashMap) states;

    /**
     * A pointer to the global data.
     */
    vx_UserStatePtr global_data;

    /* Current state Stuff. */
    vx_StateUID current_state_UID;
    vx_State* current_state;

    struct {
        /**
         * Called the at the initialization, before the creation of the first state.
         */
        VX_CALLBACK(first_init, void, vx_UserStatePtr global_data);

        /**
         * Called at state change, before the initialization of the new state.
         */
        VX_CALLBACK(state_change, void, vx_UserStatePtr global_data, vx_UserStatePtr prev_state_data, vx_StateUID new_state_UID, vx_StateUID old_state_UID);

        /**
         * Called at the exit, when the state machine should stop. Overrides the window's onClose function. 
         */
        VX_CALLBACK(close, void, vx_UserStatePtr global_data);
    } callbacks;
} vx_StateManager;

/**
 * @brief [INTERNAL] The vx_State constructor.
 * @param descriptor The descriptor of the state.
 * @return The just created state object.
 */
vx_State vx_state_new(vx_StateDescriptor* descriptor);

/**
 * @brief The vx_StateManager constructor.
 * @param descriptor The descriptor of the state manager.
 * @return The just created state manager object.
 */
vx_StateManager vx_statemanager_new(vx_StateManagerDescriptor* descriptor);

/**
 * @brief The vx_StateManager destructor.
 * @param manager A pointer to the state manager.
 */
void vx_statemanager_free(vx_StateManager* manager);

/**
 * @brief Apply the state manager to a window and run.
 * @param manager A pointer to the state manager.
 * @param window A pointer to a window.
 * @param first_state An identifier to a state.
 */
void vx_statemanager_run(vx_StateManager* manager, vx_Window* window, vx_StateUID first_state);

/**
 * @brief Register a state to a state manager.
 * @param manager A pointer to the state manager.
 * @param descriptor A pointer to a state descriptor.
 */
void vx_statemanager_register_state(vx_StateManager* manager, vx_StateDescriptor* descriptor);

#include <unistd.h>
#include <pthread.h>

#include "loops.h"
#include "window.h"
#include "event.h"
#include "time.h"

#include "entity_internal.h"

pthread_mutex_t lock;

int window_closed = 0;

GLFWwindow *main_window;

/* Fifty ticks per second */
#define TICK_LENGTH 20000000

void *tick_thread(void *args) {
    struct timespec sleep_time = (struct timespec){ 0, 0 };
    
    clock_t start, end;
    
    for(;;) {
        start = clock();
        pthread_mutex_lock  (&lock);

        loop();
        events_clear();
        
        if(window_closed) {
            break;
        }
        
        pthread_mutex_unlock(&lock);
        
        end = clock();
        
        int ns = (int)(((float)(end - start) / CLOCKS_PER_SEC) * 1000000000);
        while(ns > TICK_LENGTH) ns -= TICK_LENGTH;
        ns = TICK_LENGTH - ns;
        sleep_time.tv_nsec = ns;
        
        nanosleep(&sleep_time, NULL);
    }
    
    return NULL;
}

void game_begin(const char* title, int viewx, int viewy, float scale) {
    pthread_mutex_init(&lock, NULL);
    
    window_info info = { title, viewx, viewy, scale };
    set_camera_size(viewx, viewy);
    
    main_window = window_init(&info);
    if(!main_window) return;
    
    events_attach(main_window);
    
    ent_init();
    
    load_textures();
    
    init();
    
    pthread_t tick_thread_id;
    pthread_create(&tick_thread_id, NULL, tick_thread, NULL);
    
    for(;;) {
        pthread_mutex_lock  (&lock);
        render();
        events_save();
        
        if(glfwWindowShouldClose(main_window)) break;
        
        pthread_mutex_unlock(&lock);
        
        glfwSwapBuffers(main_window);
        glfwPollEvents();
    }
    
    window_closed = 1;
    
    pthread_mutex_unlock(&lock);
    
    glfwTerminate();
    
    pthread_join(tick_thread_id, NULL);
    
    ent_cleanup();
    
    pthread_mutex_destroy(&lock);
}
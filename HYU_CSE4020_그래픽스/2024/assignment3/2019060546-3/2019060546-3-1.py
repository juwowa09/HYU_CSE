import glfw
from OpenGL.GL import *
from OpenGL.GLU import *
import numpy as np

gCamAng = 0
gXPosition = 0

def render(T):
    glClear(GL_COLOR_BUFFER_BIT)
    glLoadIdentity()
    # draw coordinate
    glBegin(GL_LINES)
    glColor3ub(255,0,0)
    glVertex2fv(np.array([0.,0.]))
    glVertex2fv(np.array([1.,0.]))
    glColor3ub(0,255,0)
    glVertex2fv(np.array([0.,0.]))
    glVertex2fv(np.array([0.,1.]))
    glEnd()
    # draw triangle
    glBegin(GL_TRIANGLES)
    glColor3ub(255,255,255)
    glVertex2fv( (T @ np.array([.0,.5,1.]))[:-1] )
    glVertex2fv( (T @ np.array([.0,.0,1.]))[:-1] )
    glVertex2fv( (T @ np.array([.5,.0,1.]))[:-1] )
    glEnd()

def key_callback(window, key, scancode, action, mods):
    global gCamAng
    global gXPosition

    if action == glfw.PRESS or action == glfw.REPEAT:
        if key == glfw.KEY_D:
            gCamAng += np.radians(-10)
        elif key == glfw.KEY_A:
            gCamAng += np.radians(10)
        elif key == glfw.KEY_Q:
            gXPosition += -0.1
        elif key == glfw.KEY_E:
            gXPosition += 0.1
        elif key == glfw.KEY_1:
            gCamAng = 0;
            gXPosition = 0;

def main():
    if not glfw.init():
        return
    window = glfw.create_window(480,480, '2019060546-3-1',None,None)
    
    if not window:
        glfw.terminate()
        return
    glfw.make_context_current(window)
    glfw.set_key_callback(window, key_callback)
    while not glfw.window_should_close(window):
        glfw.poll_events()
        R = np.array([[np.cos(gCamAng), -np.sin(gCamAng), 0.],
                      [np.sin(gCamAng), np.cos(gCamAng), 0.],
                      [0.,              0.,             1.]])
        
        T = np.array([[1.,0.,gXPosition],
                      [0.,1.,0.],
                      [0.,0.,1.]])

        render(T@R)
        glfw.swap_buffers(window)

    glfw.terminate()

if __name__ == "__main__":
    main()

import glfw
import numpy as np
from OpenGL.GL import *

def render():
    glClear(GL_COLOR_BUFFER_BIT)
    glLoadIdentity()
    glScalef(0.5,1,1)
    glTranslatef(1,0,0)
    glBegin(GL_TRIANGLES)
    glVertex2f(0.0, 1.0)
    glVertex2f(-1.0, -1.0)
    glVertex2f(1.0, -1.0)
    glEnd()

def render(T):
    glClear(GL_COLOR_BUFFER_BIT)
    glLoadIdentity()

    #draw coordinate
    glBegin(GL_LINES)
    glColor3ub(255,0,0)
    glVertex2fv(np.array([0.,0.]))
    glVertex2fv(np.array([1.,0.]))
    glColor3ub(0, 255, 0)
    glVertex2fv(np.array([0.,0.]))
    glVertex2fv(np.array([0.,1.]))
    glEnd()

    #draw triangle
    glBegin(GL_TRIANGLES)
    glColor3ub(255,255,255)
    glVertex2fv(T @ np.array([0.0,0.5]))
    glVertex2fv(T @ np.array([0.0,0.0]))
    glVertex2fv(T @ np.array([0.5,0.0]))
    glEnd()

def main():
    # Initialize the library
    if not glfw.init():
        return
    # Create a windowed mode window and its OpenGL context
    window = glfw.create_window(640,480,"2019060546", None,None)
    if not window:
        glfw.terminate()
        return

    # Make the window`s context current
    glfw.make_context_current(window)

    glfw.swap_interval(1)
    #Loop until the user clses the window
    while not glfw.window_should_close(window):
        # Poll events
        glfw.poll_events()

        t = glfw.get_time()
        s = np.sin(t)
        T = np.array([[s,0.],
                      [0.,s]])
        #Render here, e.g. using pyOpenGL
        render(T)
        # Swap front and back buffers
        glfw.swap_buffers(window)
    glfw.terminate()

if __name__=="__main__":
    main()

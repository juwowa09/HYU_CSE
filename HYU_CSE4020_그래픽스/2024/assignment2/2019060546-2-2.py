import glfw
import numpy as np
from OpenGL.GL import *

def render():
    glClear(GL_COLOR_BUFFER_BIT)
    glLoadIdentity()
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
    glVertex2fv((T @ np.array([0.0,0.5,1.]))[:-1])
    glVertex2fv((T @ np.array([0.0,0.0,1.]))[:-1])
    glVertex2fv((T @ np.array([0.5,0.0,1.]))[:-1])
    glEnd()

def main():
    # Initialize the library
    if not glfw.init():
        return
    # Create a windowed mode window and its OpenGL context
    window = glfw.create_window(480,480,"2019060546-2-2", None,None)
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
        
        S = np.array([[1.,0.],
                      [0.,2.]])
        t = glfw.get_time()
        
        s = np.sin(30)
        c = np.cos(30)

        s1 = np.sin(40)
        c1 = np.cos(40)
        # Rotation matrix
        R = np.array([[c, -s, 0],
                      [s, c, 0],
                      [0., 0., 1.]])
        # move location matrix
        T = np.array([[1.,0.,.2],
                      [0.,1.,0],
                      [0.,0.,1.]])

        R1 = np.array([[c1,-s1,0],
                      [s1,c1,0],
                      [0.,0.,1.]])

        T1 = np.array([[1.,0.,.2],
                      [0.,1.,0],
                      [0.,0.,1.]])
        #t = glfw.get_time()
        #s = np.sin(t)
        #T = np.array([[s,0.],
        #              [0.,s*.5]])
        #rotation
        #th = t
        #T = np.array([[np.cos(th), -np.sin(th)],
        #              [np.sin(th), np.cos(th)]])
        #reflection
        #T = np.array([[-1.,0.],
        #              [0.,1.]])
        #shear
        #a = np.sin(t)
        #T = np.array([[1.,a],
        #              [0.,1.]])
        #identity matrix
        #T = np.identity(2)

        #Render here, e.g. using pyOpenGL
        render(T@R)
        # Swap front and back buffers
        glfw.swap_buffers(window)
    glfw.terminate()

if __name__=="__main__":
    main()

import glfw
import numpy as np
from OpenGL.GL import *

n = GL_LINE_LOOP

def render():
    glClear(GL_COLOR_BUFFER_BIT)
    glLoadIdentity()
    glBegin(n)

    for i in range(12):
        s = np.sin(np.radians(i*30))
        c = np.cos(np.radians(i*30))
        glVertex2f(c, s)
    glEnd()

def key_callback(window, key, scancode, action, mods):
    global n
    if action == glfw.PRESS:
        if key == glfw.KEY_1:
            n = GL_POINTS
        elif key == glfw.KEY_2:
            n = GL_LINES
        elif key == glfw.KEY_3:
            n = GL_LINE_STRIP
        elif key == glfw.KEY_4:
            n = GL_LINE_LOOP
        elif key == glfw.KEY_5:
            n = GL_TRIANGLES
        elif key == glfw.KEY_6:
            n = GL_TRIANGLE_STRIP
        elif key == glfw.KEY_7:
            n= GL_TRIANGLE_FAN
        elif key == glfw.KEY_8:
            n = GL_QUADS
        elif key == glfw.KEY_9:
            n = GL_QUAD_STRIP
        elif key == glfw.KEY_0:
            n = GL_POLYGON

def main():
    # Initialize the library
    if not glfw.init():
        return
    # Create a windowed mode window and its OpenGL context
    window = glfw.create_window(480,480,"2019060546-2-1", None,None)
    if not window:
        glfw.terminate()
        return

    # Make the window`s context current
    glfw.make_context_current(window)

    glfw.set_key_callback(window, key_callback)
    #Loop until the user clses the window
    while not glfw.window_should_close(window):
        # Poll events
        glfw.poll_events()
        #Render here, e.g. using pyOpenGL
        render()
        # Swap front and back buffers
        glfw.swap_buffers(window)
    glfw.terminate()

if __name__=="__main__":
    main()

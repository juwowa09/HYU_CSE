import glfw
from OpenGL.GL import *
from OpenGL.GLU import *
import numpy as np

def render():
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    glEnable(GL_DEPTH_TEST)

    glMatrixMode(GL_PROJECTION)
    glLoadIdentity()
    glOrtho(-2,2, -2,2, -1,1)

    glMatrixMode(GL_MODELVIEW)
    glLoadIdentity()

    drawFrame()
    t = glfw.get_time()
    a = 60
    # blue base transformation
    glPushMatrix()
    glTranslatef(-.5,.0,.0)
    glRotatef(-90 + a, 0, 0, 1)
    drawFrame()

    #red arm drawing
    glColor3ub(255,0,0)
    drawBox()
    glTranslatef(-.5,.0,.0)
   
    glRotate(180-2*a,0,0,1)
    glTranslatef(-.5,.0,.0)
    drawBox()
    glTranslatef(-.5,.0,.0)

    glRotate(-(180-2*a),0,0,1)
    glTranslatef(-.5,.0,.0)
    drawBox()

    #green arm drawing

    glPopMatrix()

def drawBox():
    glBegin(GL_QUADS)
    glVertex3fv(np.array([0.5,0.1,0.]))
    glVertex3fv(np.array([-0.5,0.1,0.]))
    glVertex3fv(np.array([-0.5,-0.1,0.]))
    glVertex3fv(np.array([0.5,-0.1,0.]))
    glEnd()

def drawFrame():
    # draw coordinate : x in red, y in green, z in blue
    glBegin(GL_LINES)
    glColor3ub(255, 0, 0)
    glVertex3fv(np.array([0.,0.,0.]))
    glVertex3fv(np.array([1.,0.,0.]))
    glColor3ub(0, 255, 0)
    glVertex3fv(np.array([0.,0.,0.]))
    glVertex3fv(np.array([0.,1.,0.]))
    glColor3ub(0, 0, 255)
    glVertex3fv(np.array([0.,0.,0.]))
    glVertex3fv(np.array([0.,0.,1.]))
    glEnd()

def main():
    if not glfw.init():
        return
    window = glfw.create_window(480,480, '2019060546-4-1',None,None)
    
    if not window:
        glfw.terminate()
        return
    glfw.make_context_current(window)
    glfw.swap_interval(1)
    
    while not glfw.window_should_close(window):
        glfw.poll_events()
        render()
        glfw.swap_buffers(window)

    glfw.terminate()

if __name__ == "__main__":
    main()

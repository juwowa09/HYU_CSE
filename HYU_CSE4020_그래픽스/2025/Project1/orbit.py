from OpenGL.GL import *
from glfw.GLFW import *
import glm
import ctypes
import numpy as np

g_target = glm.vec3(0, 0, 0)  # 카메라가 바라보는 타겟 위치

mouse_x = 0
mouse_y = 0

radius = 5.

g_cam_ang = 0.
g_cam_height = 0.

g_triangle_x = 0
g_triangle_y = 0
g_triangle_z = 0

g_vertex_shader_src = '''
#version 330 core

layout (location = 0) in vec3 vin_pos; 
layout (location = 1) in vec3 vin_color; 

out vec4 vout_color;

uniform mat4 MVP;

void main()
{
    // 3D points in homogeneous coordinates
    vec4 p3D_in_hcoord = vec4(vin_pos.xyz, 1.0);

    gl_Position = MVP * p3D_in_hcoord;

    vout_color = vec4(vin_color, 1.);
}
'''

g_fragment_shader_src = '''
#version 330 core

in vec4 vout_color;

out vec4 FragColor;

void main()
{
    FragColor = vout_color;
}
'''

def load_shaders(vertex_shader_source, fragment_shader_source):
    # build and compile our shader program
    # ------------------------------------
    
    # vertex shader 
    vertex_shader = glCreateShader(GL_VERTEX_SHADER)    # create an empty shader object
    glShaderSource(vertex_shader, vertex_shader_source) # provide shader source code
    glCompileShader(vertex_shader)                      # compile the shader object
    
    # check for shader compile errors
    success = glGetShaderiv(vertex_shader, GL_COMPILE_STATUS)
    if (not success):
        infoLog = glGetShaderInfoLog(vertex_shader)
        print("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" + infoLog.decode())
        
    # fragment shader
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER)    # create an empty shader object
    glShaderSource(fragment_shader, fragment_shader_source) # provide shader source code
    glCompileShader(fragment_shader)                        # compile the shader object
    
    # check for shader compile errors
    success = glGetShaderiv(fragment_shader, GL_COMPILE_STATUS)
    if (not success):
        infoLog = glGetShaderInfoLog(fragment_shader)
        print("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" + infoLog.decode())

    # link shaders
    shader_program = glCreateProgram()               # create an empty program object
    glAttachShader(shader_program, vertex_shader)    # attach the shader objects to the program object
    glAttachShader(shader_program, fragment_shader)
    glLinkProgram(shader_program)                    # link the program object

    # check for linking errors
    success = glGetProgramiv(shader_program, GL_LINK_STATUS)
    if (not success):
        infoLog = glGetProgramInfoLog(shader_program)
        print("ERROR::SHADER::PROGRAM::LINKING_FAILED\n" + infoLog.decode())
        
    glDeleteShader(vertex_shader)
    glDeleteShader(fragment_shader)

    return shader_program    # return the shader program


def key_callback(window, key, scancode, action, mods):
    global g_cam_ang, g_cam_height, g_triangle_x, g_triangle_y, g_triangle_z
    # 방금 눌렸는가 검사
    if key==GLFW_KEY_ESCAPE and action==GLFW_PRESS:
        glfwSetWindowShouldClose(window, GLFW_TRUE);

def cursor_callback(window, xpos, ypos):
    global mouse_x, mouse_y, g_cam_ang, g_cam_height, g_target, radius
    dx = xpos - mouse_x
    dy = ypos - mouse_y

    lmb = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS

    if glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS and lmb:
        # 매 프레임 반복 검사
        if glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS:
            eye = glm.vec3(
                radius * np.cos(g_cam_height) * np.sin(g_cam_ang),
                radius * np.sin(g_cam_height),
                radius * np.cos(g_cam_ang) * np.cos(g_cam_height)
            ) + g_target

            dir = glm.normalize(g_target - eye)
            right = glm.normalize(glm.cross(dir, glm.vec3(0, 1, 0)))
            up = glm.normalize(glm.cross(right, dir))

            pan_speed = 0.01
            g_target += -dx * pan_speed * right
            g_target += dy * pan_speed * up

        elif glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS:
            if dy > 0 :
                radius += .1
            elif dy < 0 :
                if radius > 0.5:
                    radius -= .1
                    
        else:
            threshold = 1  # 마우스 민감도 조정용
            if abs(dx) > threshold:
                if dx > 0 :
                    g_cam_ang += np.radians(-1)
                else :
                    g_cam_ang += np.radians(1)
            if abs(dy) > threshold:
                if dy > 0 :
                    if g_cam_height < np.radians(88):
                        g_cam_height += np.radians(1)
                elif dy < 0 :
                    if g_cam_height > np.radians(-88):
                        g_cam_height += np.radians(-1)

    mouse_x = xpos
    mouse_y = ypos

def button_callback(window, button, action, mod):
    if button == GLFW_MOUSE_BUTTON_LEFT:
        if action == GLFW_PRESS:
            print('press left btn: (%d, %d)' % glfwGetCursorPos(window))
        elif action == GLFW_RELEASE:
            print('release left btn: (%d, %d)' % glfwGetCursorPos(window))

def scroll_callback(window, xoffset, yoffset):
    print('mouse wheel scroll: %d, %d' % (xoffset, yoffset))

def prepare_vao_triangle():
    # prepare vertex data (in main memory)
    vertices = glm.array(glm.float32,
        # position        # color
         0.0, 0.0, 0.0,  1.0, 0.0, 0.0, # v0
         0.5, 0.0, 0.0,  0.0, 1.0, 0.0, # v1
         0.0, 0.5, 0.0,  0.0, 0.0, 1.0, # v2
    )

    # create and activate VAO (vertex array object)
    VAO = glGenVertexArrays(1)  # create a vertex array object ID and store it to VAO variable
    glBindVertexArray(VAO)      # activate VAO

    # create and activate VBO (vertex buffer object)
    VBO = glGenBuffers(1)   # create a buffer object ID and store it to VBO variable
    glBindBuffer(GL_ARRAY_BUFFER, VBO)  # activate VBO as a vertex buffer object

    # copy vertex data to VBO
    glBufferData(GL_ARRAY_BUFFER, vertices.nbytes, vertices.ptr, GL_STATIC_DRAW) # allocate GPU memory for and copy vertex data to the currently bound vertex buffer

    # configure vertex positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * glm.sizeof(glm.float32), None)
    glEnableVertexAttribArray(0)

    # configure vertex colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * glm.sizeof(glm.float32), ctypes.c_void_p(3*glm.sizeof(glm.float32)))
    glEnableVertexAttribArray(1)

    return VAO

def prepare_vao_frame():
    # prepare vertex data (in main memory)
        
    vertices = [0.0, 0.0, 0.0,  0.0, 1.0, 0.0] # y-axis start
    vertices += [0.0, 5.0, 0.0,  0.0, 1.0, 0.0] # y-axis end 
    vertices += [0.,0.,-5.,  1.,0.,0.]
    vertices += [0.,0.,5.,   1.,0.,0.]
    vertices += [5.,0.,0,  0.,0.,1.]
    vertices += [-5.,0.,0.,   0.,0.,1.]

    vertices += [0.,0.,0,  1.,1.,1.]
    vertices += [0.,-5.0,0.,   1.,1.,1.]
         
         
    for x in range(-5, 6, 1):
        if x == 0:
            continue
        vertices += [x,0.,-5.,  1.,0.,0.]
        vertices += [x,0.,5.,   1.,0.,0.]

    for z in range(-5, 6, 1):
        if z == 0:
            continue
        vertices += [-5.,0.,z,  0.,0.,1.]
        vertices += [5.,0.,z,   0.,0.,1.]

    vertices = glm.array(glm.float32, *vertices)

    # create and activate VAO (vertex array object)
    VAO = glGenVertexArrays(1)  # create a vertex array object ID and store it to VAO variable
    glBindVertexArray(VAO)      # activate VAO

    # create and activate VBO (vertex buffer object)
    VBO = glGenBuffers(1)   # create a buffer object ID and store it to VBO variable
    glBindBuffer(GL_ARRAY_BUFFER, VBO)  # activate VBO as a vertex buffer object

    # copy vertex data to VBO
    glBufferData(GL_ARRAY_BUFFER, vertices.nbytes, vertices.ptr, GL_STATIC_DRAW) # allocate GPU memory for and copy vertex data to the currently bound vertex buffer

    # configure vertex positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * glm.sizeof(glm.float32), None)
    glEnableVertexAttribArray(0)

    # configure vertex colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * glm.sizeof(glm.float32), ctypes.c_void_p(3*glm.sizeof(glm.float32)))
    glEnableVertexAttribArray(1)

    return VAO


def main():
    # initialize glfw
    global g_target
    if not glfwInit():
        return
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3)   # OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE)  # Do not allow legacy OpenGl API calls
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE) # for macOS

    # create a window and OpenGL context
    window = glfwCreateWindow(1200, 1200, '2019060546', None, None)
    if not window:
        glfwTerminate()
        return
    glfwMakeContextCurrent(window)

    # register event callbacks
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_callback)
    glfwSetMouseButtonCallback(window, button_callback)
    glfwSetScrollCallback(window, scroll_callback)

    # load shaders
    shader_program = load_shaders(g_vertex_shader_src, g_fragment_shader_src)

    # get uniform locations
    MVP_loc = glGetUniformLocation(shader_program, 'MVP')
    
    # prepare vaos
    vao_triangle = prepare_vao_triangle()
    vao_frame = prepare_vao_frame()

    # loop until the user closes the window
    while not glfwWindowShouldClose(window):
        # render

        # enable depth test (we'll see details later)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
        glEnable(GL_DEPTH_TEST)

        glUseProgram(shader_program)

        # projection matrix
        # use orthogonal projection (we'll see details later)

        # ortho 모드 = 시야 범위는 glm.ortho(left, right, bottom, top, near, far) 값으로 고정
        # 멀리있는게 멀게 보이는게 아님. 동일하게 보인다.
        # P = glm.ortho(-5,5,-5,5,-10,10)

        # perspective 모드 = 반지름이 커지면 카메라가 멀어져서, 물체는 작아짐
        P = glm.perspective(glm.radians(60), 1.0, 0.1, 100.0)

        # view matrix
        # rotate camera position with g_cam_ang / move camera up & down with g_cam_height
        V = glm.lookAt(glm.vec3(radius * np.cos(g_cam_height) * np.sin(g_cam_ang), 
                                radius * np.sin(g_cam_height), 
                                radius * np.cos(g_cam_ang) * np.cos(g_cam_height)) + g_target, 
                       g_target, 
                       glm.vec3(0,1,0))

        # current frame: P*V*I (now this is the world frame)
        I = glm.mat4()
        MVP = P*V*I 
        # 사실상 이거 적용후 생기는 물체는 모든 이 매트릭스의 회전, 이동이 적용됨.
        glUniformMatrix4fv(MVP_loc, 1, GL_FALSE, glm.value_ptr(MVP))

        # draw current frame
        glBindVertexArray(vao_frame)
        glDrawArrays(GL_LINES, 0, 48)


        # animating
        t = glfwGetTime()

        # rotation
        th = np.radians(t*90)
        R = glm.rotate(th, glm.vec3(0,0,1))

        # tranlation
        T = glm.translate(glm.vec3(g_triangle_x, g_triangle_y, g_triangle_z))

        # scaling
        S = glm.scale(glm.vec3(np.sin(t), np.sin(t), np.sin(t)))

        M = T
        # M = T
        # M = S
        # M = R @ T
        # M = T @ R

        # current frame: P*V*M
        MVP = P*V*M
        glUniformMatrix4fv(MVP_loc, 1, GL_FALSE, glm.value_ptr(MVP))
        # 새로운거 그릴때마다 glUniformMatrixs4fv 필요
        # 예전에는 스택형식으로 자동으로 관리가 됐다면 요즘에는 직접 MVP에 지금까지 쌓인거를 할당한 다음
        # glUniformMatrix4fv(MVP_loc, 1, GL_FALSE, glm.value_ptr(MVP))
        # 이 함수에 MVP 로 넣어야 한다?

        # draw triangle w.r.t. the current frame
        glBindVertexArray(vao_triangle)
        glDrawArrays(GL_TRIANGLES, 0, 3)

        # draw current frame
        glBindVertexArray(vao_frame)
        glDrawArrays(GL_LINES, 0, 6)

        # swap front and back buffers
        glfwSwapBuffers(window)

        # poll events
        glfwPollEvents()

    # terminate glfw
    glfwTerminate()

if __name__ == "__main__":
    main()

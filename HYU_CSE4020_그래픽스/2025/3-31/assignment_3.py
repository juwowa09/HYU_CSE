from OpenGL.GL import *
from glfw.GLFW import *
import glm
import numpy as np

g_vertex_shader_src = '''
#version 330 core

// input vertex position. its attribute index is 0. 
// 사실상 VAO 에서 받아오는 역할이네.

layout (location = 0) in vec3 vin_pos;
layout (location = 1) in vec3 vin_color;
out vec4 vout_color;
uniform vec3 v_pos;
uniform mat3 M;

void main()
{
    vec3 tmp = vec3(vin_pos.x + v_pos.x, vin_pos.y + v_pos.y, vin_pos.z + v_pos.z);
    tmp = M * tmp;
    gl_Position = vec4(tmp.x, tmp.y, tmp.z, 1.0);
    
    vout_color = vec4(vin_color.r, vin_color.g, vin_color.b, 1.0); // 첫 번째 정점: R
}
'''

g_fragment_shader_src = '''
#version 330 core

// output fragment color of type vec4.

out vec4 FragColor;
in vec4 vout_color;

void main()
{
    FragColor = vout_color;
}
'''

g_fragment_shader_src2 = '''
#version 330 core

// output fragment color of type vec4.
out vec4 FragColor;

void main()
{
    // set the fragment color to white.
    FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
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
    # 링크한 뒤 - 컴파일 객체는 버려도 됨

    return shader_program    # return the shader program


def key_callback(window, key, scancode, action, mods):
    if key==GLFW_KEY_ESCAPE and action==GLFW_PRESS:
        glfwSetWindowShouldClose(window, GLFW_TRUE);

def main():
    # initialize glfw
    if not glfwInit():
        return
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3)   # OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE)  # Do not allow legacy OpenGl API calls
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE) # for macOS
    # glfwWindowHint(GLFW_DOUBLEBUFFER,GL_FALSE) # single buffer mode

    # create a window and OpenGL context
    window = glfwCreateWindow(800, 800, '(2019060546)', None, None)
    if not window:
        glfwTerminate()
        return
    
    glfwMakeContextCurrent(window)

    # register event callbacks
    glfwSetKeyCallback(window, key_callback);

    # load shaders
    shader_program = load_shaders(g_vertex_shader_src, g_fragment_shader_src)
    shader_program2 = load_shaders(g_vertex_shader_src, g_fragment_shader_src2)

    # 포인터를 받아와서 수정할 수 있도록한다.
    loc_u_color = glGetUniformLocation(shader_program, 'u_color');
    loc_u_pos = glGetUniformLocation(shader_program, 'v_pos');
    loc_u_M = glGetUniformLocation(shader_program, 'M');

    # prepare vertex data (in main memory)
    lines = glm.array(glm.float32,
        0.0, 0.0, 0.0,      1.0, 0.0, 0.0, # left vertex x, y, z coordinates
        1.0, 0.0, 0.0,      1.0, 0.0, 0.0, # right vertex x, y, z coordinates
        
        0.0, 0.0, 0.0,      0.0, 1.0, 0.0, # left vertex x, y, z coordinates
        0.0, 1.0, 0.0,      0.0, 1.0, 0.0, # right vertex x, y, z coordinates
        
        0.0, 0.0, 0.0,      0.0, 0.0, 1.0, # left vertex x, y, z coordinates
        0.0, 0.0, 1.0,      0.0, 0.0, 1.0, # right vertex x, y, z coordinates
    )

    # create and activate VAO (vertex array object)
    VAO_line = glGenVertexArrays(1)  # create a vertex array object ID and store it to VAO variable
    glBindVertexArray(VAO_line)      # activate VAO

    # create and activate VBO (vertex buffer object)
    VBO_line = glGenBuffers(1)   # create a buffer object ID and store it to VBO variable
    glBindBuffer(GL_ARRAY_BUFFER, VBO_line)  # activate VBO as a vertex buffer object

    # copy vertex data to VBO
    glBufferData(GL_ARRAY_BUFFER, lines.nbytes, lines.ptr, GL_STATIC_DRAW)

    # configure vertex attributes // 점 하나당 좌표 속성
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * glm.sizeof(glm.float32), None)
    # void pointer 크기를 넣어줘야 한다. // 점 하나당 색상 속성
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * glm.sizeof(glm.float32), ctypes.c_void_p(3 * glm.sizeof(glm.float32)))
    # 0,1번 속성에 대한 정보 전달
    glEnableVertexAttribArray(0)
    glEnableVertexAttribArray(1)


    rotate_lines = glm.array(glm.float32,
        -1.0, 0.0, 0.0,      1.0, 1.0, 1.0, # left vertex x, y, z coordinates
        1.0, 0.0, 0.0,      1.0, 1.0, 1.0, # right vertex x, y, z coordinates
    )
    VAO_line_rotate = glGenVertexArrays(1)
    glBindVertexArray(VAO_line_rotate)

    VBO_line_rotate = glGenBuffers(1)   # create a buffer object ID and store it to VBO variable
    glBindBuffer(GL_ARRAY_BUFFER, VBO_line_rotate)  # activate VBO as a vertex buffer object

    # copy vertex data to VBO
    glBufferData(GL_ARRAY_BUFFER, rotate_lines.nbytes, rotate_lines.ptr, GL_STATIC_DRAW)

    # configure vertex attributes // 점 하나당 좌표 속성
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * glm.sizeof(glm.float32), None)
    # void pointer 크기를 넣어줘야 한다. // 점 하나당 색상 속성
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * glm.sizeof(glm.float32), ctypes.c_void_p(3 * glm.sizeof(glm.float32)))
    # 0,1번 속성에 대한 정보 전달
    glEnableVertexAttribArray(0)
    glEnableVertexAttribArray(1)


    vertex = glm.array(glm.float32,
        0.0, 0.0, 0.0,      1.0, 0.0, 0.0, # left vertex x, y, z coordinates
        0.5, 0.0, 0.0,      0.0, 1.0, 0.0, # right vertex x, y, z coordinates
        0.0, 0.5, 0.0,      0.0, 0.0, 1.0, # left vertex x, y, z coordinates
    )

    # create and activate VAO (vertex array object)
    VAO_Triangle = glGenVertexArrays(1)  # create a vertex array object ID and store it to VAO variable
    glBindVertexArray(VAO_Triangle)      # activate VAO

    # create and activate VBO (vertex buffer object)
    VBO_Triangle = glGenBuffers(1)   # create a buffer object ID and store it to VBO variable
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Triangle)  # activate VBO as a vertex buffer object

    # copy vertex data to VBO
    glBufferData(GL_ARRAY_BUFFER, vertex.nbytes, vertex.ptr, GL_STATIC_DRAW)

    # configure vertex attributes // 점 하나당 좌표 속성
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * glm.sizeof(glm.float32), None)
    # void pointer 크기를 넣어줘야 한다. // 점 하나당 색상 속성
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * glm.sizeof(glm.float32), ctypes.c_void_p(3 * glm.sizeof(glm.float32)))
    # 0,1번 속성에 대한 정보 전달
    glEnableVertexAttribArray(0)
    glEnableVertexAttribArray(1)


    # 활성화 추가
    # glEnable(GL_PROGRAM_POINT_SIZE)

    # loop until the user closes the window
    while not glfwWindowShouldClose(window):
        # render
        glClear(GL_COLOR_BUFFER_BIT)

        glUseProgram(shader_program)

        #update uniform
        t = glfwGetTime();
        pos = (glm.sin(t)) * 1.0
        # color = (glm.sin(t)) * 0.5 # 범위: 0 ~ 0.4
        glUniform3f(loc_u_pos, pos, 0, 0)
        # glUniform3f(loc_u_color, color,color,color)

        M = np.array([[1,0,0],
                      [0,1,0],
                      [0,0,1]])
        glUniformMatrix3fv(loc_u_M, 1, GL_TRUE, M)

        glBindVertexArray(VAO_Triangle)
        glDrawArrays(GL_TRIANGLES, 0, 3)

        glUniform3f(loc_u_pos, 0, pos, 0)

        glBindVertexArray(VAO_Triangle)
        glDrawArrays(GL_TRIANGLES, 0, 3)

        glUniform3f(loc_u_pos, 0, 0, 0)

        glBindVertexArray(VAO_line)
        glDrawArrays(GL_LINES, 0, 6)

        M = glm.array(glm.float32,
                 glm.cos(-t*1.5), -glm.sin(-t*1.5), 0,
                 glm.sin(-t*1.5), glm.cos(-t*1.5), 0,
                 0, 0, 1)
        glUniformMatrix3fv(loc_u_M, 1, GL_FALSE, M.ptr)
        
        glBindVertexArray(VAO_line_rotate)
        glDrawArrays(GL_LINES, 0, 2)

        # glBindVertexArray(VAO_Triangle)
        # glDrawArrays(GL_TRIANGLES, 0, 6)
        # glDrawArrays(GL_POINTS, 0, 3)
        # glDrawArrays(GL_POINTS, 3, 3)
        # glUseProgram(shader_program2)
        # glDrawArrays(GL_POINTS, 0, 3)

        # swap front and back buffers
        glfwSwapBuffers(window) # double buffer rendering
        # glFlush() # single buffer rendering

        # poll events
        glfwPollEvents()

    # terminate glfw
    glfwTerminate()

if __name__ == "__main__":
    main()

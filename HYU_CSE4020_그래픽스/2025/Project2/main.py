from OpenGL.GL import *
from glfw.GLFW import *
import glm
import ctypes
import numpy as np
import os

g_last_x, g_last_y = 0, 0
g_left_btn = False
g_radius = 10
g_theta = np.radians(45)
g_phi = np.radians(45)
g_target = glm.vec3(0)
g_P = glm.mat4()

g_vertex_shader_src = '''
#version 330 core

layout(location = 0) in vec3 vin_pos;
layout(location = 1) in vec3 vin_normal;

out vec3 frag_pos;
out vec3 frag_normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    vec4 world_pos = model * vec4(vin_pos, 1.0);
    frag_pos = vec3(world_pos);
    frag_normal = mat3(transpose(inverse(model))) * vin_normal;

    gl_Position = projection * view * world_pos;
}
'''
g_fragment_shader_src = '''
#version 330 core
in vec3 frag_pos;
in vec3 frag_normal;

out vec4 FragColor;

uniform vec3 light_pos;
uniform vec3 view_pos;

void main() {
    vec3 norm = normalize(frag_normal);
    vec3 light_color = vec3(1, 1, 1);
    vec3 object_color = vec3(1.0, 0.9, 0.2); 
    vec3 light_dir = normalize(light_pos - frag_pos);
    vec3 view_dir = normalize(view_pos - frag_pos);

    vec3 ambient = 0.1 * light_color;
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = diff * light_color;

    vec3 reflect_dir = reflect(-light_dir, norm);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32.0);
    vec3 specular = 0.3 * spec * light_color;

    vec3 result = (ambient + diffuse + specular) * object_color;
    FragColor = vec4(result, 1.0);
}
'''

grid_vertex_shader_src = '''
#version 330 core
layout(location = 0) in vec3 vin_pos;
layout(location = 1) in vec3 vin_color;

out vec3 frag_color;

uniform mat4 MVP;

void main() {
    frag_color = vin_color;
    gl_Position = MVP * vec4(vin_pos, 1.0);
}
'''
grid_fragment_shader_src = '''
#version 330 core
in vec3 frag_color;
out vec4 FragColor;

void main() {
    FragColor = vec4(frag_color, 1.0);
}
'''

def load_shaders(vertex_shader_source, fragment_shader_source):
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

def prepare_vao_grid():
    grid_lines = []
    color_gray = [0.5, 0.5, 0.5]
    for i in range(-25, 26): #50x50
        grid_lines += [i, 0, -25] + color_gray + [i, 0, 26] + color_gray
        grid_lines += [-25, 0, i] + color_gray + [26, 0, i] + color_gray

    vertices = glm.array(glm.float32, *grid_lines)
    VAO = glGenVertexArrays(1)
    glBindVertexArray(VAO)

    VBO = glGenBuffers(1)
    glBindBuffer(GL_ARRAY_BUFFER, VBO)

    glBufferData(GL_ARRAY_BUFFER, vertices.nbytes, vertices.ptr, GL_STATIC_DRAW)

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * vertices.itemsize, ctypes.c_void_p(0))
    glEnableVertexAttribArray(0)
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * vertices.itemsize, ctypes.c_void_p(3 * vertices.itemsize))
    glEnableVertexAttribArray(1)
    return VAO, len(grid_lines) // 6

def prepare_vao_axes():
    axis_lines = [
        -25, 0, 0,   1, 0, 0, #x red
        25, 0, 0,    1, 0, 0,

        0, -25, 0,   0, 0, 1, #y blue
        0, 25, 0,    0, 0, 1,

        0, 0, -25,   0, 1, 0, #z green
        0, 0, 25,    0, 1, 0,
    ]
    vertices = glm.array(glm.float32, *axis_lines)
    VAO = glGenVertexArrays(1)
    glBindVertexArray(VAO)

    VBO = glGenBuffers(1)
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO)
    glBufferData(GL_ARRAY_BUFFER, vertices.nbytes, vertices.ptr, GL_STATIC_DRAW)

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * vertices.itemsize, ctypes.c_void_p(0))
    glEnableVertexAttribArray(0)

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * vertices.itemsize, ctypes.c_void_p(3 * vertices.itemsize))
    glEnableVertexAttribArray(1)

    return VAO, len(axis_lines) // 6

def cursor_pos_callback(window, xpos, ypos):
    global g_last_x, g_last_y, g_theta, g_phi, g_target, g_radius

    dx = xpos - g_last_x
    dy = ypos - g_last_y
    g_last_x, g_last_y = xpos, ypos

    if not g_left_btn:
        return

    alt = glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS or glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS
    ctrl = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS or glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS
    shift = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS or glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS

    if alt and ctrl: #zoom
        g_radius *= 1.0 + dy * 0.01
        g_radius = np.clip(g_radius, 2, 100)
    elif alt and shift: #pan
        cam_dir = glm.normalize(camera_pos() - g_target)
        right = glm.normalize(glm.cross(cam_dir, glm.vec3(0, 1, 0)))
        up = glm.normalize(glm.cross(right, cam_dir))
        g_target += 0.01 * dx * right + 0.01 * dy * up
    elif alt: #orbit
        g_theta -= np.radians(dx)
        g_phi += np.radians(dy)
        g_phi = np.clip(g_phi, np.radians(10), np.radians(170))
    
def mouse_button_callback(window, button, action, mods):
    global g_left_btn
    if button == GLFW_MOUSE_BUTTON_LEFT:
        g_left_btn = action == GLFW_PRESS

def framebuffer_size_callback(window, width, height):
    global g_P
    glViewport(0, 0, width, height)
    aspect = width / height
    g_P = glm.perspective(glm.radians(45), aspect, 0.1, 500)

def camera_pos():
    sin_phi = np.sin(g_phi)
    pos = glm.vec3(
        g_radius * sin_phi * np.sin(g_theta),
        g_radius * np.cos(g_phi),
        g_radius * sin_phi * np.cos(g_theta)
    )
    return pos + g_target

def load_object(path):
    positions = []
    normals = []
    faces = []
    
    with open(path, 'r') as f: #ignore vt, mtllib, usemtl, o, s
        file_name = os.path.basename(f.name)
        for line in f:
            if line.startswith('v '):
                positions.append(list(map(float, line.split()[1:4])))
            elif line.startswith('vn '):
                normals.append(list(map(float, line.split()[1:4])))
            elif line.startswith('f '):
                faces.append(line.split()[1:])

    three = 0 #count
    four = 0
    five_up = 0
    for i in faces:
        if len(i) == 3:
            three += 1
        elif len(i) == 4: 
            four += 1
        else: 
            five_up += 1
    print(f"Object File Name: {file_name}")
    print(f"Total # of Faces: {len(faces)}")
    print(f"# of Faces with Three Vertices: {three}")
    print(f"# of Faces with Four Vertices: {four}")
    print(f"# of Faces with More than Four Vertices: {five_up}\n")

    final_vertices = []
    final_normals = []

    for i in faces: #each face
        newlist = []
        for line in i:
            if '//' in line:  #v//vn
                vertex_str, normal_str = line.split('//')
            else:
                parts = line.split('/')
                vertex_str = parts[0]
                if len(parts) >= 3 and parts[2]:
                    normal_str = parts[2]
                else:
                    normal_str = '0'
            v = int(vertex_str) - 1
            if normal_str.isdigit():
                n = int(normal_str)-1
            else:
                n = 0
            newlist.append((v, n))

        for j in range(1, len(newlist)-1):
            for k in [0, j, j+1]:
                vi, ni = newlist[k]
                final_vertices.extend(positions[vi])
                final_normals.extend(normals[ni] if 0 <= ni < len(normals) else [0, 0, 1])

    return final_vertices, final_normals

def create_vao_from_object(vertices, normals):
    VAO = glGenVertexArrays(1)
    glBindVertexArray(VAO)
    VBO = glGenBuffers(1)
    glBindBuffer(GL_ARRAY_BUFFER, VBO)

    interleaved = []
    for i in range(len(vertices)//3):
        interleaved += vertices[i*3:i*3+3]
        interleaved += normals[i*3:i*3+3]

    data = np.array(interleaved, dtype=np.float32)
    glBufferData(GL_ARRAY_BUFFER, data.nbytes, data, GL_STATIC_DRAW)

    stride = 6*4
    glVertexAttribPointer(0, 3, GL_FLOAT, False, stride, ctypes.c_void_p(0))
    glEnableVertexAttribArray(0)
    glVertexAttribPointer(1, 3, GL_FLOAT, False, stride, ctypes.c_void_p(12))
    glEnableVertexAttribArray(1)

    return VAO


mesh_vaos = []
mesh_models = []
mesh_count = 0
def drop_callback(window, paths):
    global mesh_vaos, mesh_models, mesh_count
    for i in paths:
        if not i.endswith(".obj"): #not obj file
            continue
        verts, norms = load_object(i)
        VAO = create_vao_from_object(verts, norms)
        mesh_vaos.append((VAO, len(verts)//3))
        mesh_models.append(glm.translate(glm.mat4(1), glm.vec3(2 * mesh_count, 0, 0)))
        mesh_count += 1

def main():
    global g_last_x, g_last_y

    # initialize glfw
    if not glfwInit():
        return
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3)   # OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE)  # Do not allow legacy OpenGl API calls
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE) # for macOS

    # create a window and OpenGL context
    window = glfwCreateWindow(800, 800, 'proj_2', None, None)
    if not window:
        glfwTerminate()
        return
    glfwMakeContextCurrent(window)

    # register event callbacks
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback)
    glfwSetCursorPosCallback(window, cursor_pos_callback)
    glfwSetMouseButtonCallback(window, mouse_button_callback)

    # load shaders/ get uniform locations
    shader_program = load_shaders(g_vertex_shader_src, g_fragment_shader_src)
    model_loc = glGetUniformLocation(shader_program, 'model')
    view_loc = glGetUniformLocation(shader_program, 'view')
    proj_loc = glGetUniformLocation(shader_program, 'projection')
    light_pos_loc = glGetUniformLocation(shader_program, 'light_pos')
    view_pos_loc = glGetUniformLocation(shader_program, 'view_pos')
    
    # (grid) load shaders/ get uniform locations
    grid_shader = load_shaders(grid_vertex_shader_src, grid_fragment_shader_src)
    MVP_loc = glGetUniformLocation(grid_shader, 'MVP')

    vao_grid, grid_vertices = prepare_vao_grid()
    vao_axes, axis_vertices = prepare_vao_axes()
    glfwSetDropCallback(window, drop_callback)

    
    g_last_x, g_last_y = glfwGetCursorPos(window)
    framebuffer_size_callback(window, 800, 800)

    # loop until the user closes the window
    while not glfwWindowShouldClose(window):
        # enable depth test (we'll see details later)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
        glEnable(GL_DEPTH_TEST)

        # render in "wireframe mode"
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL)

        cam = camera_pos()
        V = glm.lookAt(cam, g_target, glm.vec3(0, 1, 0))

        #render using grid shader
        glUseProgram(grid_shader)
        MVP = g_P * V * glm.mat4()
        #xyz
        glBindVertexArray(vao_axes)
        glUniformMatrix4fv(MVP_loc, 1, GL_FALSE, glm.value_ptr(MVP))
        glDrawArrays(GL_LINES, 0, axis_vertices)
        #grid
        glBindVertexArray(vao_grid)
        glUniformMatrix4fv(MVP_loc, 1, GL_FALSE, glm.value_ptr(MVP))
        glDrawArrays(GL_LINES, 0, grid_vertices)

        #render using phong shader
        glUseProgram(shader_program)
        glUniform3fv(light_pos_loc, 1, glm.value_ptr(glm.vec3(3, 5, 3)))
        glUniform3fv(view_pos_loc, 1, glm.value_ptr(cam))
        glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm.value_ptr(V))
        glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm.value_ptr(g_P))

        #draw .obj meshes
        for i, (vao, count) in enumerate(mesh_vaos):
            glBindVertexArray(vao)
            glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm.value_ptr(mesh_models[i]))
            glDrawArrays(GL_TRIANGLES, 0, count)

        # swap front and back buffers
        glfwSwapBuffers(window)
        # poll events
        glfwPollEvents()
    # terminate glfw
    glfwTerminate()

if __name__ == "__main__":
    main()
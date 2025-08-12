from OpenGL.GL import *
from glfw.GLFW import *
import os
import glm
import ctypes
import numpy as np

g_target = glm.vec3(0, 0, 0)  # 카메라가 바라보는 타겟 위치
g_P = glm.mat4()

load_count = 0
mouse_x = 0
mouse_y = 0

radius = 10.

g_cam_ang = 0.
g_cam_height = 0.

g_triangle_x = 0
g_triangle_y = 0
g_triangle_z = 0

g_vertex_shader_src = '''
#version 330 core

layout (location = 0) in vec3 vin_pos; 
layout (location = 1) in vec3 vin_color; 
layout (location = 2) in vec3 vin_normal; 

out vec4 vout_color;
out vec3 vout_surface_pos;
out vec3 vout_normal;

uniform mat4 MVP;

void main()
{
    // 3D points in homogeneous coordinates
    vec4 p3D_in_hcoord = vec4(vin_pos.xyz, 1.0);

    gl_Position = MVP * p3D_in_hcoord;

    vout_surface_pos = vec3(vec4(vin_pos, 1));
    vout_normal = normalize(vin_normal);
    vout_color = vec4(vin_color, 1.);
}
'''

g_fragment_shader_src = '''
#version 330 core

in vec3 vout_surface_pos;
in vec3 vout_normal;

out vec4 FragColor;

uniform vec3 light_pos;
uniform vec3 view_pos;

void main()
{
    vec3 light_color = vec3(1,1,1);
    vec3 material_color = vec3(0.1,0.5,0.9);
    float material_shininess = 10.0;

    // ambient
    vec3 ambient = 0.1 * light_color * material_color;

    // for diffiuse and specular
    vec3 normal = normalize(vout_normal);
    vec3 surface_pos = vout_surface_pos;
    vec3 light_dir = normalize(light_pos - surface_pos);

    // diffuse
    float diff = max(dot(normal, light_dir), 0);
    vec3 diffuse = diff * light_color * material_color;

    // specular
    vec3 view_dir = normalize(view_pos - surface_pos);
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow( max(dot(view_dir, reflect_dir), 0.0), material_shininess);
    vec3 specular = spec * light_color * vec3(1,1,1);

    vec3 color = ambient + diffuse + specular;
    FragColor = vec4(color, 1.);
}
'''

g_frame_fragment_shader_src = '''
#version 330 core

in vec4 vout_color;

out vec4 FragColor;

void main()
{
    FragColor = vout_color;
}
''' 

def prepare_vao_object(vertices, normals, indices):

    pos_arr = glm.array(glm.float32, *vertices)
    nrm_arr = glm.array(glm.float32, *normals)
    idx_arr = glm.array(glm.uint32, *indices)

    VAO = glGenVertexArrays(1)
    glBindVertexArray(VAO)
    
    stride = 3*4

    VBO_pos = glGenBuffers(1)
    glBindBuffer(GL_ARRAY_BUFFER, VBO_pos)
    glBufferData(GL_ARRAY_BUFFER, pos_arr.nbytes, pos_arr.ptr, GL_STATIC_DRAW)
    glVertexAttribPointer(0, 3, GL_FLOAT, False, stride, ctypes.c_void_p(0))
    glEnableVertexAttribArray(0)

    VBO_nor = glGenBuffers(1)
    glBindBuffer(GL_ARRAY_BUFFER, VBO_nor)
    glBufferData(GL_ARRAY_BUFFER, nrm_arr.nbytes, nrm_arr.ptr, GL_STATIC_DRAW)
    glVertexAttribPointer(2, 3, GL_FLOAT, False, stride, ctypes.c_void_p(0))
    glEnableVertexAttribArray(2)

    EBO = glGenBuffers(1)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO)
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, idx_arr.nbytes, idx_arr.ptr, GL_STATIC_DRAW)

    # prepare_vao_object 안에서, VAO 바인딩 상태로
    glDisableVertexAttribArray(1)
    # glVertexAttrib3f(1, 1.0, 1.0, 1.0)  # 필요시 임의색

    glBindVertexArray(0)

    return VAO, len(indices)

def load_obj(path):
    global load_count

    path = os.path.normpath(path)
    if not os.path.isfile(path):
        print(f"[ERROR] 파일이 존재하지 않습니다: {path}")
        return None
    if not path.lower().endswith(".obj"):
        print(f"[ERROR] OBJ 파일이 아닙니다: {path}")
        return None
    
    offset_x = load_count * 2.0
    load_count += 1
    vertices = []
    normals = []
    stats = {"Total": 0, "Tri" : 0, "Quad" : 0, "Ngon" : 0}
    final_vertices = []
    final_normals = []
    indices = []
    unique = {}

    def get_or_add(vi, ni):
        key = (vi, ni if ni is not None else -1) 
        if key in unique:
            return unique[key]
        
        new_idx = len(final_vertices) // 3
        unique[key] = new_idx

        final_vertices.extend(vertices[vi])
        if ni is not None and 0 <= ni < len(normals):
            final_normals.extend(normals[ni])
        else:
            final_normals.extend([0.0,0.0,1.0])
        return new_idx
    
    try:
        f = open(path,"r",encoding="utf-8", errors = "ignore")
        for raw in f:
            line = raw.strip()
            if not line or line.startswith('#'):
                continue

            parts = line.split()
            tag = parts[0]

            if tag == 'v':
                x,y,z = map(float, parts[1:4])
                vertices.append(glm.vec3(x+offset_x,y,z))

            elif tag == 'vn':
                x,y,z = map(float, parts[1:4])
                normals.append(glm.vec3(x,y,z))

            elif tag == 'f':
                face = []
                for p in parts[1:]:
                    vn_idx = None
                    if '//' in p:
                        vi, vn = p.split('//')
                        v_idx = int(vi) - 1
                        vn_idx = int(vn) - 1 if vn else None
                    else:
                        toks = p.split('/')
                        v_idx = int(toks[0]) - 1
                        if len(toks) >= 3 and toks[2] != '':
                            vn_idx = int(toks[2]) - 1
                    face.append((v_idx, vn_idx))

                for j in range(1, len(face) - 1):
                    i0 = get_or_add(*face[0])
                    i1 = get_or_add(*face[j])
                    i2 = get_or_add(*face[j + 1])
                    indices.extend([i0, i1, i2])

                n = len(face)
                stats["Total"] += 1

                if n == 3:
                    stats["Tri"] += 1
                elif n == 4:
                    stats["Quad"] += 1
                elif n > 4:
                    stats["Ngon"] += 1
            else:
                pass
        
        file_name = os.path.basename(path)
        print(file_name)
        for key, value in stats.items():
            print(f"  {key}: {value}")
    except Exception as e:
        print(f"[ERROR] 파일 처리중 에러 발생: {e}")
        return None
    finally:
        f.close()
    return final_vertices, final_normals, indices

mesh_vaos = []
mesh_models = []
mesh_count = 0

def drop_callback(window, paths):
    global mesh_count,mesh_models,mesh_vaos
    for path in paths:
        # print("Dropped file:", path)
        if not path.lower().endswith(".obj"): #not obj file
            continue
        vertices, normals, indices = load_obj(path)
        VAO, l = prepare_vao_object(vertices, normals, indices)
        mesh_vaos.append((VAO, l))
        mesh_count += 1

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

def framebuffer_size_callback(window, width, height):
    global g_P

    glViewport(0, 0, width, height)
    g_P = glm.perspective(glm.radians(45.0), width/height, 0.1,500)

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
                    g_cam_ang += np.radians(-1.5)
                else :
                    g_cam_ang += np.radians(1.5)
            if abs(dy) > threshold:
                if dy > 0 :
                    if g_cam_height < np.radians(88):
                        g_cam_height += np.radians(1.5)
                elif dy < 0 :
                    if g_cam_height > np.radians(-88):
                        g_cam_height += np.radians(-1.5)

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

def prepare_vao_frame():
    # prepare vertex data (in main memory)
    vertices = [] 

    vertices += [0.0, -20.0, 0.0,   0.0, 1.0, 0.0]
    vertices += [0.0, 20.0, 0.0,   0.0, 1.0, 0.0]

    vertices += [0., 0., -20.,    1., 0., 0.]
    vertices += [0., 0., 20.,     1., 0., 0.]

    vertices += [20., 0., 0.,     0., 0., 1.]
    vertices += [-20., 0., 0.,    0., 0., 1.]

    for x in range(-20, 21, 1):
        if x == 0:
            continue
        vertices += [x, 0., -20.,  0.5, 0.5, 0.5]
        vertices += [x, 0.,  20.,  0.5, 0.5, 0.5]

    for z in range(-20, 21, 1):
        if z == 0:
            continue
        vertices += [-20., 0., z,  0.5, 0.5, 0.5]
        vertices += [ 20., 0., z,  0.5, 0.5, 0.5]

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

    return VAO, len(vertices)//6


def main():
    # initialize glfw
    global g_target,g_P,mesh_count,mesh_models,mesh_vaos
    if not glfwInit():
        return
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3)   # OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE)  # Do not allow legacy OpenGl API calls
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE) # for macOS

    # create a window and OpenGL context
    window = glfwCreateWindow(800, 800, '2019060546', None, None)
    if not window:
        glfwTerminate()
        return
    glfwMakeContextCurrent(window)

    # register event callbacks
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_callback)
    glfwSetMouseButtonCallback(window, button_callback)
    glfwSetScrollCallback(window, scroll_callback)
    glfwSetDropCallback(window, drop_callback)
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback)
    framebuffer_size_callback(window, 800, 800)

    # load shaders
    shader_program = load_shaders(g_vertex_shader_src, g_fragment_shader_src)
    MVP_loc = glGetUniformLocation(shader_program, 'MVP')
    view_pos_loc = glGetUniformLocation(shader_program, 'view_pos')
    light_loc = glGetUniformLocation(shader_program, 'light_pos')


    frame_shader_program = load_shaders(g_vertex_shader_src, g_frame_fragment_shader_src)
    frame_MVP_loc = glGetUniformLocation(frame_shader_program, 'MVP')
    # get uniform locations
    light_init = glm.vec3(2,2,0);

    # prepare vaos
    vao_frame, lines = prepare_vao_frame()

    # loop until the user closes the window
    while not glfwWindowShouldClose(window):
        # render

        # enable depth test (we'll see details later)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
        glEnable(GL_DEPTH_TEST)

        glUseProgram(frame_shader_program)

        # view matrix
        # rotate camera position with g_cam_ang / move camera up & down with g_cam_height]
        view_pos = glm.vec3(radius * np.cos(g_cam_height) * np.sin(g_cam_ang), 
                                radius * np.sin(g_cam_height), 
                                radius * np.cos(g_cam_ang) * np.cos(g_cam_height)) + g_target
        
        t = glfwGetTime()
        angle = t * 1.0
        R = glm.rotate(angle, glm.vec3(0,1,0))
        light_cur = glm.vec3(R*glm.vec4(light_init, 1))

        V = glm.lookAt(view_pos, 
                       g_target, 
                       glm.vec3(0,1,0))

        # current frame: P*V*I (now this is the world frame)
        I = glm.mat4()
        MVP = g_P * V * I 
        # 사실상 이거 적용후 생기는 물체는 모든 이 매트릭스의 회전, 이동이 적용됨.
        glUniformMatrix4fv(frame_MVP_loc, 1, GL_FALSE, glm.value_ptr(MVP))
        # draw current frame
        glBindVertexArray(vao_frame)
        glDrawArrays(GL_LINES, 0, lines)
        

        glUseProgram(shader_program)
        glUniformMatrix4fv(MVP_loc, 1, GL_FALSE, glm.value_ptr(MVP))
        glUniform3f(light_loc, light_cur.x, light_cur.y, light_cur.z)
        glUniform3f(view_pos_loc, view_pos.x, view_pos.y, view_pos.z)

        # 새로운거 그릴때마다 glUniformMatrixs4fv 필요
        # 예전에는 스택형식으로 자동으로 관리가 됐다면 요즘에는 직접 MVP에 지금까지 쌓인거를 할당한 다음
        # glUniformMatrix4fv(MVP_loc, 1, GL_FALSE, glm.value_ptr(MVP))
        # 이 함수에 MVP 로 넣어야 한다?

        # draw triangle w.r.t. the current frame

        for i, (vao, count) in enumerate(mesh_vaos):
            glBindVertexArray(vao)
            glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, None)


        # swap front and back buffers
        glfwSwapBuffers(window)

        # poll events
        glfwPollEvents()

    # terminate glfw
    glfwTerminate()

if __name__ == "__main__":
    main()

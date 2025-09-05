from OpenGL.GL import *
from glfw.GLFW import *
import os
import glm
import ctypes
import numpy as np

g_target = glm.vec3(0, 0, 0)  # 카메라가 바라보는 타겟 위치
g_P = glm.mat4()

load_meshes = []
mouse_x = 0
mouse_y = 0

radius = 5.
scale = 1

g_cam_ang = 0.
g_cam_height = 0.

g_triangle_x = 0
g_triangle_y = 0
g_triangle_z = 0

g_root = None
bvh_data = None
current_frame_idx = -1
last_frame_time = None
animate = False
obj_mode = False

g_vertex_shader_src = '''
#version 330 core

layout (location = 0) in vec3 vin_pos; 
layout (location = 1) in vec3 vin_color; 
layout (location = 2) in vec3 vin_normal; 

out vec4 vout_color;
out vec3 vout_surface_pos;
out vec3 vout_normal;

uniform mat4 MVP;
uniform mat4 M;

void main()
{
    // 3D points in homogeneous coordinates
    vec4 p3D_in_hcoord = vec4(vin_pos.xyz, 1.0);

    gl_Position = MVP * p3D_in_hcoord;

    vout_surface_pos = vec3(M * vec4(vin_pos, 1));
    mat3 normal_mat = mat3(transpose(inverse(M)));
    vout_normal = normalize(normal_mat * vin_normal);
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

class Joint:
    def __init__(self, parent, name, offset, start_index, channel, link_transform_from_parent, shape_transform, color):
        # hierarchy
        self.parent = parent
        self.name = name
        self.children = []
        self.offset = offset
        self.start_index = start_index
        self.channel = channel

        # transform
        self.link_transform_from_parent = link_transform_from_parent
        self.joint_transform = glm.translate((offset.x,offset.y,offset.z))
        self.global_transform = glm.mat4()

        # shape
        self.shape_transform = shape_transform
        self.color = color

    def set_joint_transform(self, joint_transform):
        self.joint_transform = joint_transform * self.joint_transform

    def update_tree_global_transform_rest(self, parent_global):
        # 레스트: parent * translate(offset) 만
        T = glm.translate(glm.mat4(1), self.offset)
        self.rest_global_transform = parent_global * T
        for ch in self.children:
            ch.update_tree_global_transform_rest(self.rest_global_transform)

    def get_global_transform(self):
        return self.global_transform
    def get_shape_transform(self):
        return self.shape_transform
    def get_color(self):
        return self.color
    
    def get_child(self, index):
        print()
        print(self.name + index)
        print(self.start_index)
        print(self.offset)
        for child in self.children:
            if not child == None:
                child.get_child(chr(ord(index)+1))

    def apply_scale(self):
        global scale
        self.offset = scale * self.offset
        self.joint_transform = glm.translate(scale * self.offset)
        for child in self.children:
            if not child == None:
                child.apply_scale()


    def apply_motion(self, frame, parent):
        T = glm.mat4(1)
        R = glm.mat4(1)

        channels_lower = [channel.lower() for channel in self.channel] # consistency

        if 'xposition' in channels_lower:
            index = self.start_index
            x = frame[index + channels_lower.index('xposition')]
            y = frame[index + channels_lower.index('yposition')]
            z = frame[index + channels_lower.index('zposition')]
            T = glm.translate(glm.mat4(1), glm.vec3(x, y, z))
        else:
            T = glm.translate(glm.mat4(1), self.offset)

        for i, channel in enumerate(channels_lower):
            val = frame[self.start_index + i]
            if 'rotation' in channel:
                angle_rad = glm.radians(val)
                if channel == 'xrotation':
                    R = R * glm.rotate(glm.mat4(1), angle_rad, glm.vec3(1, 0, 0))
                elif channel == 'yrotation':
                    R = R * glm.rotate(glm.mat4(1), angle_rad, glm.vec3(0, 1, 0))
                elif channel == 'zrotation':
                    R = R * glm.rotate(glm.mat4(1), angle_rad, glm.vec3(0, 0, 1))

        return parent * T * R


    def draw(self, vao, parent, VP, MVP_loc, M_loc, frame_idx):
        if self.name == 'END':
            return
        current = self.get_global_transform()
        if frame_idx < 0:
            # rest pose: 부모 * translate(offset)
            current = parent * glm.translate(glm.mat4(1), self.offset)
        else:
            # motion pose
            current = self.apply_motion(bvh_data['frames'][frame_idx], parent)
        draw_joint(vao, self, current, VP, MVP_loc, M_loc)
        for child in self.children:
            if not child == None:
                child.draw(vao, current, VP, MVP_loc, M_loc, frame_idx)

def draw_obj(joint, parent, VP, MVP_loc, M_loc, frame_idx):
    if joint.name == 'END':
        return

    if frame_idx < 0:
        current = parent * glm.translate(glm.mat4(1), joint.offset)
    else:
        current = joint.apply_motion(bvh_data['frames'][frame_idx], parent)

        # 모델행렬 = 현재 * 인버스(레스트)
    model = current * glm.inverse(joint.rest_global_transform)

    MVP = VP * model

    if joint.name in mesh_vaos:
        vao, count = mesh_vaos[joint.name]
        glUniformMatrix4fv(MVP_loc, 1, GL_FALSE, glm.value_ptr(MVP))
        glUniformMatrix4fv(M_loc, 1, GL_FALSE, glm.value_ptr(model))
        glBindVertexArray(vao)
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, None)

    for child in joint.children:
        draw_obj(child, current, VP, MVP_loc, M_loc, frame_idx)


def draw_joint(vao, joint, current, VP, MVP_loc, M_loc, thickness=0.05):
    color = joint.get_color()
    I = glm.mat4(1)
    
    dir_local = joint.children[0].offset
    length = glm.length(dir_local)

    # 2) 월드 기준 방향 벡터 (부모의 회전만 적용: mat3로 회전 성분 추출)
    dir_world = glm.normalize(glm.mat3(joint.global_transform) * dir_local)

    # 3) +Y축을 dir_world로 회전시키는 회전행렬
    y = glm.vec3(0, 1, 0)
    # 회전축
    axis = glm.cross(y, dir_world)
    dot  = glm.clamp(glm.dot(y, dir_world), -1.0, 1.0)
    angle = glm.acos(dot)
    rot = I if glm.length(axis) < 1e-6 else glm.rotate(I, angle, glm.normalize(axis))

    # 4) 모델행렬: (부모 포즈) → (방향 정렬) → (링크 중앙으로 올림) → (두께/길이 스케일)
    model = (
        current
        * rot
        * glm.translate(I, glm.vec3(0, length/3, 0))
        * glm.scale(I, glm.vec3(thickness, length/2, thickness))
    )
    MVP = VP * model
    #
    
    glBindVertexArray(vao)
    glUniformMatrix4fv(MVP_loc, 1, GL_FALSE, glm.value_ptr(MVP))
    glUniformMatrix4fv(M_loc, 1, GL_FALSE, glm.value_ptr(model))
    
    glDrawArrays(GL_TRIANGLES, 0, 36)

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

def cal_mat(parent_pos, child_pos):
    offset = child_pos - parent_pos
    length = glm.length(offset)
    direction = glm.normalize(offset)

    y_axis = glm.vec3(0, 1, 0)
    rot_axis = glm.cross(y_axis, direction)
    angle = np.arccos(np.clip(glm.dot(y_axis, direction), -1.0, 1.0))

    if abs(angle) < 1e-5:
        R = glm.mat4(1)  # 회전이 필요 없는 경우 단위 행렬
    else:
        R = glm.rotate(angle, rot_axis)
    S = glm.scale(glm.mat4(1), glm.vec3(0.05, length*0.8, 0.05))  # y축 방향으로 스케일
    T = glm.translate(glm.mat4(1), glm.vec3(0, 0.5, 0))  # y축 방향으로 절반 이동

    return glm.translate(glm.mat4(1), parent_pos) * R * S * T

def load_obj(path):

    path = os.path.normpath(path)
    if not os.path.isfile(path):
        print(f"[ERROR] 파일이 존재하지 않습니다: {path}")
        return None
    if not path.lower().endswith(".obj"):
        print(f"[ERROR] OBJ 파일이 아닙니다: {path}")
        return None
    
    # offset_x = load_count * 2.0
    # load_count += 1
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
                vertices.append(glm.vec3(x,y,z))

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
        
    except Exception as e:
        print(f"[ERROR] 파일 처리중 에러 발생: {e}")
        return None
    finally:
        f.close()
    return final_vertices, final_normals, indices

mesh_vaos = {}
mesh_count = 0

def load_bvh(path):
    global g_root, current_frame_idx
    current_frame_idx = -1
    path = os.path.normpath(path)
    if not os.path.isfile(path):
        print(f"[ERROR] 파일이 존재하지 않습니다: {path}")
        return None
    if not path.lower().endswith(".bvh"):
        print(f"[ERROR] BVH 파일이 아닙니다: {path}")
        return None
    
    with open(path, "r") as f:
        lines = [ln.strip() for ln in f if ln.strip()]
    index = 1
    start_index = 0
    num_joint = 0
    name_joint = []

    def parse(parent):
        nonlocal index, start_index, num_joint
        global scale
        line = lines[index]

        # NAME
        parts = line.split()
        tag = parts[0]
        name = parts[1]
        # print(name)
        index += 2

        # OFFSET
        parts = lines[index].split()
        offset = glm.vec3(float(parts[1]), float(parts[2]), float(parts[3]))
        if glm.length(offset) > 5:
            scale = 0.01
        elif glm.length(offset) < 1:
            scale = 1
        index += 1

        # CHANNELS
        parts = lines[index].split()
        offset_index = start_index
        start_index += int(parts[1])
        channels = parts[2:]
        index += 1

        cur = Joint(parent, name, offset, offset_index, channels, glm.mat4(), glm.scale((0.05,0.05,0.05)), glm.mat3())
        num_joint += 1
        name_joint.append(name)

        if lines[index].split()[0] == 'End':
            index += 2
            parts = lines[index].split()
            offset = glm.vec3(float(parts[1]), float(parts[2]), float(parts[3]))
            end = Joint(cur, 'END', offset, None, None, glm.mat4(), glm.scale((0.05,0.05,0.05)), glm.mat3())
            cur.children.append(end)
            index += 2

        while lines[index].split()[0] == 'JOINT':
            cur.children.append(parse(cur))
            # print(index)
            index += 1
        
        return cur


    g_root = parse(None)
    # g_root.get_child('0')

    index += 2

    num_frames = int(lines[index].split()[1])
    index += 1
    fps = float(lines[index].split()[2])
    index += 1
    
    file_name = os.path.basename(path)
    print(f"filename : {file_name}")
    print(f"Frames   : {num_frames}")
    print(f"FPS      : {int(1/fps)}")
    print(f"num_joint: {num_joint}")
    print(f"joints   : {name_joint}")

    frames = []
    for i in range(num_frames):
        frame_vals = list(map(float, lines[index].split()))
        frames.append(frame_vals)
        index += 1

    p_scale = 0.03
    root_pos_0 = [v*p_scale for v in frames[0][:3]] # initial root pos (scaled)

    for frame in frames:
        for i in range(3):
            frame[i] = frame[i] * p_scale - root_pos_0[i]
    
    g_root.apply_scale()
    g_root.update_tree_global_transform_rest(glm.mat4(1))

    return {
        'joints': name_joint,
        'num_frames': num_frames,
        'fps': fps,
        'frames': frames
    }


def drop_callback(window, paths):
    global mesh_count, mesh_vaos, bvh_data
    path = paths[-1]
    # print("Dropped file:", path)
    if not path.lower().endswith(".bvh"): #not bvh file
        return
    bvh_data = load_bvh(path)

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
    global current_frame_idx, last_frame_time, animate, obj_mode, bvh_data, load_meshes
    global g_cam_ang, g_cam_height, g_triangle_x, g_triangle_y, g_triangle_z, bvh_data
    # 방금 눌렸는가 검사
    if key==GLFW_KEY_ESCAPE and action==GLFW_PRESS:
        glfwSetWindowShouldClose(window, GLFW_TRUE)
    
    if action == GLFW_PRESS:
        if key == GLFW_KEY_SPACE:
            animate = not animate
            if animate:
                # current_frame_idx = 0
                last_frame_time = glfwGetTime()
        elif key == GLFW_KEY_1:
            bvh_path = os.path.join("data", "test.bvh")
            bvh_data = load_bvh(bvh_path)
            # print(bvh_data)
            obj_mode = not obj_mode
            if os.path.exists(bvh_path):
                for name in bvh_data['joints']:
                    data = load_obj(os.path.join("data", name + ".obj"))
                    if not data:
                        continue
                    vertices, normals, indices = data
                    VAO, idx_count = prepare_vao_object(vertices, normals, indices)
                    mesh_vaos[name] = (VAO, idx_count)
                    # mesh_count += 1
                # bvh_data = parse_bvh_file(bvh_path)  

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
            radius *= 1.0 + dy * 0.01
            radius = np.clip(radius, 2, 100)
                    
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

def prepare_vao_cube():
    # prepare vertex data (in main memory)
    # 36 vertices for 12 triangles
    vertices = glm.array(glm.float32,
        # position      normal
        -1 ,  1 ,  1 ,  0, 0, 1, # v0
         1 , -1 ,  1 ,  0, 0, 1, # v2
         1 ,  1 ,  1 ,  0, 0, 1, # v1

        -1 ,  1 ,  1 ,  0, 0, 1, # v0
        -1 , -1 ,  1 ,  0, 0, 1, # v3
         1 , -1 ,  1 ,  0, 0, 1, # v2

        -1 ,  1 , -1 ,  0, 0,-1, # v4
         1 ,  1 , -1 ,  0, 0,-1, # v5
         1 , -1 , -1 ,  0, 0,-1, # v6

        -1 ,  1 , -1 ,  0, 0,-1, # v4
         1 , -1 , -1 ,  0, 0,-1, # v6
        -1 , -1 , -1 ,  0, 0,-1, # v7

        -1 ,  1 ,  1 ,  0, 1, 0, # v0
         1 ,  1 ,  1 ,  0, 1, 0, # v1
         1 ,  1 , -1 ,  0, 1, 0, # v5

        -1 ,  1 ,  1 ,  0, 1, 0, # v0
         1 ,  1 , -1 ,  0, 1, 0, # v5
        -1 ,  1 , -1 ,  0, 1, 0, # v4
 
        -1 , -1 ,  1 ,  0,-1, 0, # v3
         1 , -1 , -1 ,  0,-1, 0, # v6
         1 , -1 ,  1 ,  0,-1, 0, # v2

        -1 , -1 ,  1 ,  0,-1, 0, # v3
        -1 , -1 , -1 ,  0,-1, 0, # v7
         1 , -1 , -1 ,  0,-1, 0, # v6

         1 ,  1 ,  1 ,  1, 0, 0, # v1
         1 , -1 ,  1 ,  1, 0, 0, # v2
         1 , -1 , -1 ,  1, 0, 0, # v6

         1 ,  1 ,  1 ,  1, 0, 0, # v1
         1 , -1 , -1 ,  1, 0, 0, # v6
         1 ,  1 , -1 ,  1, 0, 0, # v5

        -1 ,  1 ,  1 , -1, 0, 0, # v0
        -1 , -1 , -1 , -1, 0, 0, # v7
        -1 , -1 ,  1 , -1, 0, 0, # v3

        -1 ,  1 ,  1 , -1, 0, 0, # v0
        -1 ,  1 , -1 , -1, 0, 0, # v4
        -1 , -1 , -1 , -1, 0, 0, # v7
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

    # configure vertex normals
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6 * glm.sizeof(glm.float32), ctypes.c_void_p(3*glm.sizeof(glm.float32)))
    glEnableVertexAttribArray(2)

    return VAO

def prepare_vao_frame():
    # prepare vertex data (in main memory)
    vertices = [] 

    vertices += [0.0, -20.0, 0.0,   0.0, 1.0, 0.0]
    vertices += [0.0, 20.0, 0.0,   0.0, 1.0, 0.0]

    vertices += [0., 0., -20.,    0., 0., 1.]
    vertices += [0., 0., 20.,     0., 0., 1.]

    vertices += [20., 0., 0.,     1., 0., 0.]
    vertices += [-20., 0., 0.,    1., 0., 0.]

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
    global g_target,g_P,mesh_count,mesh_models,mesh_vaos,g_root
    global last_frame_time, current_frame_idx

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
    glfwSetKeyCallback(window, key_callback)
    glfwSetCursorPosCallback(window, cursor_callback)
    glfwSetMouseButtonCallback(window, button_callback)
    glfwSetScrollCallback(window, scroll_callback)
    glfwSetDropCallback(window, drop_callback)
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback)
    framebuffer_size_callback(window, 800, 800)

    # load shaders
    shader_program = load_shaders(g_vertex_shader_src, g_fragment_shader_src)
    MVP_loc = glGetUniformLocation(shader_program, 'MVP')
    M_loc = glGetUniformLocation(shader_program, 'M')
    view_pos_loc = glGetUniformLocation(shader_program, 'view_pos')
    light_loc = glGetUniformLocation(shader_program, 'light_pos')


    frame_shader_program = load_shaders(g_vertex_shader_src, g_frame_fragment_shader_src)
    frame_MVP_loc = glGetUniformLocation(frame_shader_program, 'MVP')
    # get uniform locations
    light_init = glm.vec3(0,2,2)

    # prepare vaos
    vao_frame, lines = prepare_vao_frame()
    vao_cube = prepare_vao_cube()

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

        t = glfwGetTime()
        angle = t * 1.0
        R = glm.rotate(angle, glm.vec3(0,1,0))
        # light_cur = glm.vec3(R*glm.vec4(light_init, 1)) 
        light_cur = light_init      

        glUseProgram(shader_program)
        glUniform3f(light_loc, light_cur.x, light_cur.y, light_cur.z)
        glUniform3f(view_pos_loc, view_pos.x, view_pos.y, view_pos.z)

        # 새로운거 그릴때마다 glUniformMatrixs4fv 필요
        # 예전에는 스택형식으로 자동으로 관리가 됐다면 요즘에는 직접 MVP에 지금까지 쌓인거를 할당한 다음
        # glUniformMatrix4fv(MVP_loc, 1, GL_FALSE, glm.value_ptr(MVP))
        # 이 함수에 MVP 로 넣어야 한다?

        # draw triangle w.r.t. the current frame

        glUseProgram(shader_program)
        if not g_root == None:
            # g_root.update_tree_global_transform()
            # frame_idx = -1
            if animate:
                current_time = glfwGetTime()
                if current_time - last_frame_time >= bvh_data['fps']:
                    current_frame_idx = (current_frame_idx + 1) % bvh_data['num_frames']
                    last_frame_time = current_time
                # frame_idx = current_frame_idx
                
            if obj_mode:
                draw_obj(g_root, glm.mat4(1), g_P * V, MVP_loc, M_loc, current_frame_idx)
            else:
                g_root.draw(vao_cube, glm.mat4(1), g_P * V, MVP_loc, M_loc, current_frame_idx)



        # swap front and back buffers
        glfwSwapBuffers(window)

        # poll events
        glfwPollEvents()

    # terminate glfw
    glfwTerminate()

if __name__ == "__main__":
    main()

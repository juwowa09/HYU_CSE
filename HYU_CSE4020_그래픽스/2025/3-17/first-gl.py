from glfw.GLFW import *
from OpenGL.GL import *

def key_callback(window, key, scancode, action, mods):
    if key == GLFW_KEY_ESCAPE and action == GLFW_PRESS:
        glfwSetWindowShouldClose(window, GLFW_TRUE)
    elif key == GLFW_KEY_A:
        if action == GLFW_PRESS:
            print('press a')
        elif action == GLFW_RELEASE:
            print('release a')
        elif action == GLFW_REPEAT:
            print('repeat a')
    elif key == GLFW_KEY_SPACE and action == GLFW_PRESS:
        x, y = glfwGetCursorPos(window)
        print('press space: (%d, %d)' % (x, y))

def cursor_callback(window, xpos, ypos):
    print('mouse cursor moving: (%d, %d)' % (xpos, ypos))

def button_callback(window, button, action, mod):
    if button == GLFW_MOUSE_BUTTON_LEFT:
        if action == GLFW_PRESS:
            print('press left btn: (%d, %d)' % glfwGetCursorPos(window))
        elif action == GLFW_RELEASE:
            print('release left btn: (%d, %d)' % glfwGetCursorPos(window))

def scroll_callback(window, xoffset, yoffset):
    print('mouse wheel scroll: %d, %d' % (xoffset, yoffset))

def main():
    # GLFW 초기화
    if not glfwInit():
        return
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3)  # OpenGL 3.3 요청
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE)
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE)  # Forward Compatible 모드

    # 창 생성
    window = glfwCreateWindow(800, 800, '1-first-gl-program', None, None)
    if not window:
        glfwTerminate()
        return
    
    glfwMakeContextCurrent(window)

    # 콜백 함수 등록
    glfwSetKeyCallback(window, key_callback)
    glfwSetCursorPosCallback(window, cursor_callback)
    glfwSetMouseButtonCallback(window, button_callback)
    glfwSetScrollCallback(window, scroll_callback)

    # 이벤트 루프
    while not glfwWindowShouldClose(window):
        glfwSwapBuffers(window)  # 더블 버퍼링: 프론트/백 버퍼 교체
    
        glfwPollEvents()         # 입력 이벤트 처리

    # 루프 종료 후 GLFW 종료
    glfwTerminate()

if __name__ == "__main__":
    main()
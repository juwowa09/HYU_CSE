import OpenGL, glfw, numpy as np

M = np.arange(25)
M += 2
print (M)
M = M.reshape(5,5)
print()
print (M)
M[1:4,1:4] = 0
print()
print (M)
M = M @ M
print()
print(M)
v = M[0,:]
v = v*v
x = np.sqrt(np.sum(v))
print()
print(x)

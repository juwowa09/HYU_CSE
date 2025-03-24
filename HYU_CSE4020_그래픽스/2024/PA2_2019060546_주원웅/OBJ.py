# OBJ parser and OpenGL renderer implemented by Taesoo Kwon.
import numpy as np
import pdb
from OpenGL.GL import * # OBJrender class only.
class OBJparser:
    def __init__(self, filename, scale=None):
        """Loads a Wavefront OBJ file. """
        self.vertices = []
        self.normals = []
        self.texcoords = []
        self.faces = []
        self.normalIndices = []

        material = None
        for line in open(filename, "r"):
            if line.startswith('#'): continue
            values = line.split()
            if not values: continue
            if values[0] == 'v':
                v = list(map(float, values[1:4]))
                if scale:
                    v = [v[0]*scale, v[1]*scale, v[2]*scale]
                self.vertices.append(v)
            elif values[0] == 'vn':
                v = np.array( list(map(float,values[1:4])),dtype=np.float32)
                v=v*(1/np.linalg.norm(v))
                self.normals.append(v)
            elif values[0] == 'vt':
                self.texcoords.append(list(map(float, values[1:3])))
            elif values[0] in ('usemtl', 'usemat'):
                material = values[1]
            elif values[0] == 'mtllib':
                self.mtllib = values[1]
            elif values[0] == 'f':
                face = []
                texcoords = []
                norms = []
                for v in values[1:]:
                    w = v.split('/')
                    face.append(int(w[0]))
                    if len(w) >= 2 and len(w[1]) > 0:
                        texcoords.append(int(w[1]))
                    else:
                        texcoords.append(0)
                    if len(w) >= 3 and len(w[2]) > 0:
                        norms.append(int(w[2]))
                    else:
                        norms.append(0)
                #self.faces.append((face, norms, texcoords, material))
                if len(face)==4 :
                    # triangulate
                    self.faces.extend([face[0], face[1], face[2]])
                    self.normalIndices.extend([norms[0], norms[1], norms[2]])
                    self.faces.extend([face[0], face[2], face[3]])
                    self.normalIndices.extend([norms[0], norms[2], norms[3]])
                elif len(face)>4:
                    # ignore this polygon for now
                    print('warning! ignoring face', face) 
                else:
                    self.faces.extend(face)
                    self.normalIndices.extend(norms)

class OBJrenderer:
    def __init__(self, filename, scale=None):
        
        geom = OBJparser(filename, scale)

        vertex_data = np.array(geom.vertices, dtype=np.float32)
        self.bbmax=np.amax(vertex_data, axis=0)
        self.bbmin=np.amin(vertex_data, axis=0)
        index_data = np.column_stack((geom.faces, geom.normalIndices)).astype(np.int32)
        normal_data = np.array(geom.normals, dtype=np.float32)

        indices, new_data=self.rearrangeData(index_data, [vertex_data, normal_data])
        # new_data[0]: vertex positions
        # new_data[1]: normals 

        index_data=np.array(indices).astype(np.int32).flatten()
        # vx, vy, vz, nx, ny,vz, vx2, vy2, vz2, nx2,ny2,nz2,...
        vertex_data=np.column_stack((new_data[0], new_data[1])).astype(np.float32).flatten()

        self.index_data=index_data
        self.vertex_data=vertex_data

    def rearrangeData(self,index_data, data):

        # assigns a new index when normal index and vertex index differ.
        indices=[]
        dictII={}
        mapII=[]
        for i in range(index_data.shape[0]):
            ii= tuple(index_data[i][:].tolist())
            if ii in dictII:
                indices.append(dictII[ii])
            else:
                newIndex=len(mapII)
                mapII.append(ii)
                dictII[ii]=newIndex
                assert(dictII[ii]==newIndex)
                indices.append(newIndex)

        new_data=[]
        nv=len(mapII) # new number of vertices
        for i in range(len(data)):
            new_data.append(np.zeros((nv, data[i].shape[1]), dtype=np.float32))
            
        for i in range(len(mapII)):
            ii=mapII[i]
            for j in range(len(data)):
                new_data[j][i,:]=data[j][ii[j]-1,:]
                
        return indices, new_data


    def render(self):
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);


        varr=self.vertex_data
        index_data=self.index_data
        glVertexPointer(3, GL_FLOAT, 6*4, ctypes.c_void_p(varr.ctypes.data ))
        glNormalPointer(GL_FLOAT, 6*4, ctypes.c_void_p(varr.ctypes.data+3*4))
        glDrawElements(GL_TRIANGLES, index_data.shape[0], GL_UNSIGNED_INT, index_data);
        glDisableClientState(GL_NORMAL_ARRAY)
        glDisableClientState(GL_VERTEX_ARRAY);

# implmented by Taesoo Kwon
import numpy as np
import pdb

def normalize(v):
    norm = np.linalg.norm(v)
    if norm == 0: 
       return v
    return v / norm
# check if a include b
def intervalInclude(a,b):
    EPS=np.finfo(float).eps
    if a[0]<b[0]+EPS and b[1]<a[1]+EPS:
        return True
    return False
def intersectInterval(a, b):
    if intervalInclude(a,b):
        return b
    elif intervalInclude(b,a):
        return a
    elif b[1]<a[0]:
        return (0,0)
    elif a[1]<b[0]:
        return (0,0)
    elif a[0]<b[0]:
        return (b[0], a[1])
    return (a[0], b[1])


class Ray:
    def __init__(self, origin, direction):
        self.origin=origin 
        self.direction=direction 

    def getPoint(self,t):
        return self.origin + (self.direction * t);
    def intersectsPlane(self, plane):
        denom = np.dot(plane.normal,self.direction)
        if np.abs(denom) < np.finfo(float).eps:
            # Parallel
            return (False, 0);
        else:
            nom = np.dot(plane.normal, self.origin)+plane.d
            t = -(nom/denom);
            return (t >= 0, t);
    def intersectsPlanes(self, planes):
        ret=(False, -1e9)
        overlap=(-1e9,1e9)
        for plane in planes:
            planeRes=self.intersectsPlane(plane)
            if planeRes[0]:
                if plane.distance(self.origin)>0:
                    overlap=intersectInterval(overlap, (planeRes[1],1e9))
                else:
                    overlap=intersectInterval(overlap, (-1e9, planeRes[1]))
            else:
                if plane.distance(self.origin)>0:
                    return (False,0)

        if overlap[1]>overlap[0]: 
            return (True, overlap[0]) # first intersection point
        return (False,0)

#Mainly for picking objects.
#
#      A plane is defined in 3D space by the equation
#      Ax + By + Cz + D = 0
#      	(normal=(A,B,C))
class Plane:
    def __init__(self, normal, point):
        self.setPlane(normal, point)

    def setPlane(self, vNormal, vPoint):
        self.normal=normalize(vNormal)
        self.d=np.dot(-vNormal, vPoint)

    def distance(self, vPoint):
        return np.dot(self.normal,vPoint)+self.d


        



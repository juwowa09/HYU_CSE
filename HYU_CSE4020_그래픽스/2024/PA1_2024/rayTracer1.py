import sys
import xml.etree.ElementTree as ET
import numpy as np
from PIL import Image

FARAWAY = sys.maxsize

class Color:
    def __init__(self, R, G, B):
        self.color = np.array([R, G, B]).astype(np.float64)

    def gammaCorrect(self, gamma):
        inverseGamma = 1.0 / gamma;
        self.color = np.power(self.color, inverseGamma)

    def toUINT8(self):
        return (np.clip(self.color, 0, 1) * 255).astype(np.uint8)

class Shader:
    def __init__(self, type):
        self.t = type

class ShaderPhong(Shader):
    def __init__(self, diffuse, specular, exponent):
        self.d = diffuse
        self.s = specular
        self.e = exponent

class ShaderLambertian(Shader):
    def __init__(self, diffuse):
        self.d = diffuse

class Sphere:
    def __init__(self, center, radius, shader):
        self.c = center
        self.r = radius
        self.s = shader

class Box:
    def __init__(self, minPt, maxPt, shader, normals):
        self.minPt = minPt
        self.maxPt = maxPt
        self.s = shader
        self.n = normals

class View:
    def __init__(self, viewPoint, viewDir, viewUp, viewProjNormal, viewWidth, viewHeight, projDistance, intensity):
        self.viewPoint = viewPoint
        self.viewDir = viewDir
        self.viewUp = viewUp
        self.viewProjNormal = viewProjNormal
        self.viewWidth = viewWidth
        self.viewHeight = viewHeight
        self.projDistance = projDistance
        self.intensity = intensity

class Light:
    def __init__(self, position, intensity):
        self.position = position
        self.intensity = intensity

def raytrace(list, ray, viewPoint):
    global FARAWAY
    m = FARAWAY

    # idx is the index of the closest sphere or box
    idx = -1
    cnt = 0

    for i in list:
        if i.__class__.__name__ == 'Sphere':

            a = np.sum(ray * ray)
            b = np.sum((viewPoint - i.c) * ray)
            c = np.sum((viewPoint - i.c) ** 2) - i.r ** 2

            if b ** 2 - a * c >= 0:
                if -b + np.sqrt(b ** 2 - a * c) >= 0:
                    if m >= (-b + np.sqrt(b ** 2 - a * c)) / a:
                        m = (-b + np.sqrt(b ** 2 - a * c)) / a
                        idx = cnt

                if -b - np.sqrt(b ** 2 - a * c) >= 0:
                    if m >= (-b - np.sqrt(b ** 2 - a * c)) / a:
                        m = (-b - np.sqrt(b ** 2 - a * c)) / a
                        idx = cnt

        elif i.__class__.__name__ == 'Box':
            result = 1

            txmin = (i.minPt[0]-viewPoint[0])/ray[0]
            txmax = (i.maxPt[0]-viewPoint[0])/ray[0]

            if txmin > txmax:
                txmin, txmax = txmax, txmin

            tymin = (i.minPt[1]-viewPoint[1])/ray[1]
            tymax = (i.maxPt[1]-viewPoint[1])/ray[1]

            if tymin > tymax:
                tymin, tymax = tymax, tymin

            if txmin > tymax or tymin > txmax:
                result = 0

            if tymin > txmin:
                txmin = tymin
            if tymax < txmax:
                txmax = tymax

            tzmin = (i.minPt[2]-viewPoint[2])/ray[2]
            tzmax = (i.maxPt[2]-viewPoint[2])/ray[2]

            if tzmin > tzmax:
                tzmin, tzmax = tzmax, tzmin

            if txmin > tzmax or tzmin > txmax:
                result = 0

            if tzmin >= txmin:
                txmin = tzmin
            if tzmax < txmax:
                txmax = tzmax

            if result == 1:
                if m >= txmin:
                    m = txmin
                    idx = cnt

        cnt = cnt + 1

    # return gamma and index of closest figure in list
    return [m, idx]

def shade(m, ray, view, list, idx, light):
    if idx == -1:
        # No intersection point
        return np.array([0, 0, 0])
    else:
        x = 0
        y = 0
        z = 0
        n = np.array([0, 0, 0])
        v = -m*ray

        if list[idx].__class__.__name__ == 'Sphere':
            n = view.viewPoint + m*ray - list[idx].c

            # The difference between length of the radius and the vector length sphere's center to intersection point
            if(abs(np.sqrt(np.sum(n*n)) - list[idx].r)>0.000001):
                print('check', abs(np.sqrt(np.sum(n*n)) - list[idx].r))

            n = n / np.sqrt(np.sum(n * n))

        elif list[idx].__class__.__name__ == 'Box':
            point_i = view.viewPoint + m*ray
            diff = FARAWAY
            i = -1
            cnt = 0

            for normal in list[idx].n:
                if abs(np.sum(normal[0:3] * point_i)-normal[3]) < diff:
                    diff = abs(np.sum(normal[0:3] * point_i)-normal[3])
                    i = cnt
                cnt = cnt + 1
            n = list[idx].n[i][0:3]
            n = n / np.sqrt(np.sum(n * n))

        for i in light:
            # sum of the reflected light
            l_i = v + i.position - view.viewPoint
            l_i = l_i / np.sqrt(np.sum(l_i * l_i))
            check = raytrace(list, -l_i, i.position)

            if check[1] == idx:
                if list[idx].s.__class__.__name__ == 'ShaderPhong':
                    v_unit = v / np.sqrt(np.sum(v**2))
                    h = v_unit + l_i
                    h = h / np.sqrt(np.sum(h*h))
                    x = x + list[idx].s.d[0]*max(0,np.dot(n,l_i))*i.intensity[0] + list[idx].s.s[0] * i.intensity[0] * pow(max(0, np.dot(n, h)),list[idx].s.e[0])
                    y = y + list[idx].s.d[1]*max(0,np.dot(n,l_i))*i.intensity[1] + list[idx].s.s[1] * i.intensity[1] * pow(max(0, np.dot(n, h)),list[idx].s.e[0])
                    z = z + list[idx].s.d[2]*max(0,np.dot(n,l_i))*i.intensity[2] + list[idx].s.s[2] * i.intensity[2] * pow(max(0, np.dot(n, h)),list[idx].s.e[0])

                elif list[idx].s.__class__.__name__ == "ShaderLambertian":
                    x = x + list[idx].s.d[0] * i.intensity[0] * max(0, np.dot(l_i, n))
                    y = y + list[idx].s.d[1] * i.intensity[1] * max(0, np.dot(l_i, n))
                    z = z + list[idx].s.d[2] * i.intensity[2] * max(0, np.dot(l_i, n))
        
        res = Color(x, y, z)
        res.gammaCorrect(2.2)
        return res.toUINT8()


def getNormal(x, y, z):
    dir = np.cross((y-x), (z-x))
    d = np.sum(dir*z)
    return np.array([dir[0], dir[1], dir[2], d])

def main():
    tree = ET.parse(sys.argv[1])
    root = tree.getroot()

    viewPoint = np.array([0, 0, 0]).astype(np.float64)
    viewDir = np.array([0, 0, -1]).astype(np.float64)
    viewUp = np.array([0, 1, 0]).astype(np.float64)
    viewProjNormal = -1 * viewDir
    viewWidth = 1.0
    viewHeight = 1.0
    projDistance = 1.0
    intensity = np.array([1, 1, 1]).astype(np.float64)  # how bright the light is.

    imgSize = np.array(root.findtext('image').split()).astype(np.int64)

    # list of the spheres and boxes
    list = []
    light = []

    for c in root.findall('camera'):
        viewPoint = np.array(c.findtext('viewPoint').split()).astype(np.float64)
        viewDir = np.array(c.findtext('viewDir').split()).astype(np.float64)
        if (c.findtext('projNormal')):
            viewProjNormal = np.array(c.findtext('projNormal').split()).astype(np.float64)
        viewUp = np.array(c.findtext('viewUp').split()).astype(np.float64)
        if (c.findtext('projDistance')):
            projDistance = np.array(c.findtext('projDistance').split()).astype(np.float64)
        viewWidth = np.array(c.findtext('viewWidth').split()).astype(np.float64)
        viewHeight = np.array(c.findtext('viewHeight').split()).astype(np.float64)

    view = View(viewPoint, viewDir, viewUp, viewProjNormal, viewWidth, viewHeight, projDistance, intensity)

    for c in root.findall('surface'):
        type_c = c.get('type')
        if type_c == 'Sphere':
            center_c = np.array(c.findtext('center').split()).astype(np.float64)
            radius_c = np.array(c.findtext('radius')).astype(np.float64)
            ref = ''
            for child in c:
                if child.tag == 'shader':
                    ref = child.get('ref')
            for d in root.findall('shader'):
                if d.get('name') == ref:
                    diffuse_d = np.array(d.findtext('diffuseColor').split()).astype(np.float64)
                    type_d = d.get('type')
                    if type_d == 'Lambertian':
                        shader = ShaderLambertian(diffuse_d)
                        list.append(Sphere(center_c, radius_c, shader))
                    elif type_d == 'Phong':
                        exponent_d = np.array(d.findtext('exponent').split()).astype(np.float64)
                        specular_d = np.array(d.findtext('specularColor').split()).astype(np.float64)
                        shader = ShaderPhong(diffuse_d, specular_d, exponent_d)
                        list.append(Sphere(center_c, radius_c, shader))
        elif type_c == 'Box':
            minPt_c = np.array(c.findtext('minPt').split()).astype(np.float64)
            maxPt_c = np.array(c.findtext('maxPt').split()).astype(np.float64)

            normals = []

            point_a = np.array([minPt_c[0], minPt_c[1], maxPt_c[2]])
            point_b = np.array([minPt_c[0], maxPt_c[1], minPt_c[2]])
            point_c = np.array([maxPt_c[0], minPt_c[1], minPt_c[2]])
            point_d = np.array([minPt_c[0], maxPt_c[1], maxPt_c[2]])
            point_e = np.array([maxPt_c[0], minPt_c[1], maxPt_c[2]])
            point_f = np.array([maxPt_c[0], maxPt_c[1], minPt_c[2]])

            normals.append(getNormal(point_a, point_c, point_e))
            normals.append(getNormal(point_b, point_c, point_f))
            normals.append(getNormal(point_a, point_b, point_d))
            normals.append(getNormal(point_a, point_e, point_d))
            normals.append(getNormal(point_e, point_c, point_f))
            normals.append(getNormal(point_d, point_f, point_b))

            ref = ''
            for child in c:
                if child.tag == 'shader':
                    ref = child.get('ref')
            for d in root.findall('shader'):
                if d.get('name') == ref:
                    diffuse_d = np.array(d.findtext('diffuseColor').split()).astype(np.float64)
                    type_d = d.get('type')
                    if type_d == 'Lambertian':
                        shader = ShaderLambertian(diffuse_d)
                        list.append(Box(minPt_c, maxPt_c, shader, normals))
                    elif type_d == 'Phong':
                        exponent_d = np.array(d.findtext('exponent').split()).astype(np.float64)
                        specular_d = np.array(d.findtext('specularColor').split()).astype(np.float64)
                        shader = ShaderPhong(diffuse_d, specular_d, exponent_d)
                        list.append(Box(minPt_c, maxPt_c, shader, normals))

    for c in root.findall('light'):
        position_c = np.array(c.findtext('position').split()).astype(np.float64)
        intensity_c = np.array(c.findtext('intensity').split()).astype(np.float64)
        light.append(Light(position_c, intensity_c))

    # Create an empty image
    channels = 3
    img = np.zeros((imgSize[1], imgSize[0], channels), dtype=np.uint8)
    img[:, :] = 0

    # pixel's width and height
    pixel_x = view.viewWidth / imgSize[0]
    pixel_y = view.viewHeight / imgSize[1]

    w = view.viewDir
    u = np.cross(w, view.viewUp)
    v = np.cross(w, u)

    # w,u,v unit vector
    w_unit = w / np.sqrt(np.sum(w * w))
    u_unit = u / np.sqrt(np.sum(u * u))
    v_unit = v / np.sqrt(np.sum(v * v))

    # the length between the view point and view plane's center ( It should be equal to projDistance )
    # projCenter = view.viewPoint + w_unit * view.projDistance
    # d = view.viewPoint-projCenter
    # print(np.sqrt(np.sum(d*d)), ' == projDistance( ', view.projDistance, ' )')

    # the point of the pixel[0,0]
    start = w_unit * view.projDistance - u_unit * pixel_x * ((imgSize[0]/2) + 1/2) - v_unit * pixel_y * ((imgSize[1]/2) + 1/2)
    # start_p = start + view.viewPoint
    # end_p = view.viewPoint+start + u_unit*imgSize[0]*pixel_x + v_unit*imgSize[1]*pixel_y
    # print(np.sqrt(np.sum((start_p-projCenter)*(start_p-projCenter))), '==', np.sqrt((view.viewHeight/2)**2+(view.viewWidth/2)**2), '==', np.sqrt(np.sum((end_p-projCenter)*(end_p-projCenter))))
    # print(view.viewPoint[0], 'x + ', view.viewPoint[1], 'y + ', view.viewPoint[2],  'z = ', np.sum(start_p*view.viewProjNormal))

    for x in np.arange(imgSize[0]):
        for y in np.arange(imgSize[1]):
            ray = start + u_unit * x * pixel_x + pixel_y * y * v_unit
            tmp = raytrace(list, ray, view.viewPoint)
            img[y][x] = shade(tmp[0], ray, view, list, tmp[1], light)

    rawimg = Image.fromarray(img, 'RGB')
    rawimg.save(sys.argv[1] + '.png')


if __name__ == "__main__":
    main()

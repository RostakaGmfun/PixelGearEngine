'''
Created on Jun 13, 2014

@author: RostakaGmfun
@version: 0.09

file format
    PXGSCN - null terminated
    0x10 - OBJECT CHUNK - string, null terminated
        0x11 - OBJECT TYPE CHUNK  - byte:
            0x30 - mesh
            0x31 - point light
            0x32 - directional light
            0x33 - spot light
            0x34 - logical object(just for the transformation)
        0x12 - MATERIAL CHUNK
            3 floats ambient
            3 floats diffuse
            3 floats specular
            1 float shininess
            1 float alpha
            if texture exists - null-terminated path (relative to scene file)
            else - null (0x00 byte)
        0x13 - ANIMATION CHUNK - 1 int - number of keyframes
            0x14 - ANIMATION KEY - (t,r,s) -  floats + time (int)
        0x15 - TRANSFORMATION CHUNK (t,r,s) - 9 floats
        0x17 - MESH DATA CHUNK
            0x18 - VERTEX LAYOUT CHUNK
                0x01 - PXG_VERTEX3D
                0x02 - PXG_VERTEX3DT
                0x03 - PXG_VERTEX3DTN
            0x19 - VERTEX DATA START - 1 int - number of vertices 
            ... - data in corresponding vertex layout
            0x20 - VERTEX DATA END
        0x21 - MESH DATA END
    0x16 - OBJECT END CHUNK - nothing
'''

bl_info = {
    "name": "PXG Scene Export",
    "category": "Import/Export",
}

#Global Chunks:
IDENT = "PXGSCN\0"
OBJECT_CHUNK = 0x10
OBJECT_TYPE = 0x11
MATERIAL_CHUNK = 0x12
ANIMATION_CHUNK = 0x13
ANIMATION_KEY = 0x14
TRANSFORMATION_CHUNK = 0x15
MESH_DATA = 0x17
VERTEX_LAYOUT = 0x18
VERTEX_DATA_START = 0x19
VERTEX_DATA_END = 0x20
END_CHUNK = 0x16
MESH_DATA_END = 0x21

#Vertex Layouts:
PXG_VERTEX3D = 0x01
PXG_VERTEX3DT = 0x02
PXG_VERTEX3DTN = 0x03

#Object Types:
OBJECT_MESH = 0x30
OBJECT_POINT_LIGHT = 0x31
OBJECT_DIR_LIGHT = 0x32
OBJECT_SPOT_LIGHT = 0x33
OBJECT_LOGIC = 0x34

import bpy
from bpy.types import Material
from mathutils import Vector, Color
import struct
from bpy_extras.io_utils import ExportHelper

def WriteByte(b):
    return struct.pack('B',b)

def WriteVector(v):
    return struct.pack('fff',v.x,v.z,v.y)

def WriteColor(c):
    return struct.pack('fff', 1, 0, 1)

def WriteString(s):
    return bytes(s,'ascii')

def WriteInt(i):
    return struct.pack('i', i)

def WriteFloat(f):
    return struct.pack('f', f)

def PrintVector(v):
    return str(v.x)+"    "+str(v.z)+"    "+str(v.y)

def PrintString(s):
    return s

class pxgKeyframe():
    """Represents single keyframe"""
    pos = Vector((0,0,0))
    rot = Vector((0,0,0))
    scale = Vector((0,0,0))
    time = 0
    
    def _init(self, pos,rot,scale, t):
        self.pos = pos
        self.rot = rot
        self.scale = scale
        self.time = t

class pxgHeader():
    """Represents a header data for a file"""
    def __init__(self):
        return
    def writeOut(self,f):
        f.write(WriteString(IDENT))
        return
    
class pxgVertex3d():
    """Represents PXG_VERTEX3D vertex type"""
    pos = Vector((0,0,0))
    
    def __init__(self,pos):
        self.pos = pos
    
class pxgVertex3dT():
    """Represents PXG_VERTEX3DT vertex type"""
    pos = Vector((0,0,0))
    tc = Vector((0,0,0))
    
    def __init__(self,pos, tc):
        self.pos = pos
        self.tc = tc
        
class pxgVertex3dTN():
    """Represents PXG_VERTEX3D vertex type"""
    pos = Vector((0,0,0))
    tc = Vector((0,0,0))
    normal = Vector((0,0,0))
    
    def __init__(self,pos, tc, normal):
        self.pos = pos
        self.tc = tc
        self.normal = normal

class PxgObject():
    """Represents an object"""
    name = "unknown"
    type = "none"
    pos = Vector((0,0,0))
    rot = Vector((0,0,0))
    scale = Vector((0,0,0))
    keyframes = []
    
    def __init__(self,name,t,pos,rot,scale):
        self.name = name
        self.type = t
        self.pos = pos
        self.rot = rot
        self.scale = scale
        print(name)
        
    def addKeyframe(self,t,r,s,time):
        self.keyframes.append(pxgKeyframe(t,r,s,time))
        return
    
    def writeMeshData(self, f):
        """DO NOT call alone, call writeOut()"""
        if bpy.context.scene.objects.active is None:
            return
        if bpy.context.scene.objects.active.type == 'MESH':
            f.write(WriteByte(MESH_DATA))
            f.write(WriteByte(VERTEX_LAYOUT))
            f.write(WriteByte(PXG_VERTEX3D))
            f.write(WriteByte(VERTEX_DATA_START))
            f.write(WriteInt(len(bpy.context.scene.objects.active.data.vertices)))
            #vertices = []
            for v in bpy.context.scene.objects.active.data.vertices:
                vv = Vector((v.co[0],v.co[2],v.co[1])) #swap y and z axes
                f.write(WriteVector(vv))
            
            f.write(WriteByte(VERTEX_DATA_END))
            f.write(WriteByte(MESH_DATA_END))
        return
        
    def writeMaterial(self, f):
        """DO NOT call alone, call writeOut()"""
        f.write(WriteByte(MATERIAL_CHUNK))
        material = False
        ob =  bpy.context.scene.objects.active
        if len(ob.material_slots) > 0:
            if ob.material_slots[0] is not None:
                mat = ob.material_slots[0].material
                ambient = Color((0,0,0))
                if bpy.context.scene.world:
                    ambient = bpy.context.scene.world.ambient_color
                
                f.write(WriteColor(ambient*mat.ambient))
                f.write(WriteColor(Color(mat.diffuse_color)))
                f.write(WriteColor(Color(mat.specular_color)))
                f.write(WriteFloat(1000)) #shininess. TODO: get shininess from Material
                f.write(WriteFloat(mat.alpha))
                if len(mat.texture_slots) > 0:
                    if mat.texture_slots[0] is not None:
                        tx = mat.texture_slots[0].texture
                        if tx.type is 'IMAGE':
                            f.write(WriteString(tx.image.filepath))
                f.write(WriteByte(0x00)) #null-termination  
                material = True       
        if not material:
            print("Asd")
            ambient = Color((0,0,0))
            if bpy.context.scene.world:
                ambient = bpy.context.scene.world.ambient_color
            f.write(WriteColor(ambient))
            f.write(WriteColor(Color((0.5,0.5,0.5))))
            f.write(WriteColor(Color((1,1,1))))
            f.write(WriteFloat(1000)) #shininess
            f.write(WriteFloat(1)) #alpha
            f.write(WriteByte(0x00)) #no material - no texture
            
    
    def writeOut(self,f):
        f.write(WriteByte(OBJECT_CHUNK))
        f.write(WriteString(self.name+"\0"))
        f.write(WriteByte(OBJECT_TYPE))
        f.write(WriteByte(OBJECT_MESH))
        self.writeMaterial(f)
        f.write(WriteByte(ANIMATION_CHUNK))
        
        sce = bpy.context.scene
        ob = bpy.context.scene.objects.active
        
        fnum = 0
        if ob.animation_data is not None:
            fnum = len(ob.animation_data.action.fcurves)
        f.write(WriteInt(0))
        print("Number of frames: %s"% str(fnum))
        
        if ob.animation_data is not None:
            for fc in ob.animation_data.action.fcurves:
                #print("a")
                pass
        f.write(WriteByte(TRANSFORMATION_CHUNK))
        f.write(WriteVector(self.pos))
        f.write(WriteVector(self.rot))
        f.write(WriteVector(self.scale))
        self.writeMeshData(f)
        
            
            
        
        f.write(WriteByte(END_CHUNK))
        return
    

class PxgExportPanel(bpy.types.Panel, ExportHelper):
    """PXG Exporter Options Panel"""
    bl_label = "PXG Exporter Panel" 
    bl_idname = "PXG_EXPORT"
    bl_space_type = 'PROPERTIES'
    bl_region_type = 'WINDOW'
    bl_context = "scene"   
    #bpy.types.Scene.scn_property = bpy.props.FloatProperty(name = "SceneProperty")

    def draw(self, context):
        layout = self.layout
        #row.label(text="Hello world!", icon='WORLD_DATA')

        #row = layout.row()
        #row.label(text="Active object is: " + obj.name)
        #row = layout.row()
        #row.prop(obj, "name")

        row = layout.row()
        row.operator("scene.pxg_export")



class PxgExport(bpy.types.Operator, ExportHelper):
    """PXG Scene Exporter"""      # blender will use this as a tooltip for menu items and buttons.
    bl_idname = "scene.pxg_export"        # unique identifier for buttons and menu items to reference.
    bl_label = "Export Scene to PXG"         # display name in the interface.
    bl_options = {'REGISTER', 'UNDO'}  # enable undo for the operator.
    
    filename_ext = ".pxgS"

    def execute(self, context):        # execute() is called by blender when running the operator.
        
        scn = bpy.context.scene
        out = open(self.filepath, "wb")
        hdr = pxgHeader()
        hdr.writeOut(out)
        for ob in scn.objects:
            t = "unknown"
            if ob.get('pxg_type') is not None:
                t = ob["pxg_type"]
            o = PxgObject(ob.name,t,ob.location,ob.rotation_euler,ob.scale)
            bpy.context.scene.objects.active = ob
            o.writeOut(out)
        

        return {'FINISHED'}            # this lets blender know the operator finished successfully.

def register():
    try:
        unregister()
    except RuntimeError:
        pass
    bpy.utils.register_class(PxgExport)
    bpy.utils.register_class(PxgExportPanel) 


def unregister():
    bpy.utils.unregister_class(PxgExport)
    bpy.utils.unregister_class(PxgExportPanel)


# This allows you to run the script directly from blenders text editor
# to test the addon without having to install it.
if __name__ == "__main__":
    register()
